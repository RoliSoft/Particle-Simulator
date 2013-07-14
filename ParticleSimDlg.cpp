#include "stdafx.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CParticleSimDlg::CParticleSimDlg(CWnd* pParent /*=NULL*/) : CDialog(CParticleSimDlg::IDD, pParent)
	, Gravity(TRUE)
	, Collision(TRUE)
	, Merge(TRUE)
	, CoreOnly(TRUE)
	, Bounce(TRUE)
	, Trace(TRUE)
	, Debug(TRUE)
	, Generation(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	MainPict.Owner = (CStatic*)this;
	srand(time(NULL));
}

CParticleSimDlg::~CParticleSimDlg()
{

}

void CParticleSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, MainPict);
	DDX_Control(pDX, IDC_GRAVITYCHECK, GravityCheck);
	DDX_Control(pDX, IDC_COLLISIONCHECK, CollisionCheck);
	DDX_Control(pDX, IDC_MERGECHECK, MergeCheck);
	DDX_Control(pDX, IDC_COREONLYCHECK, CoreOnlyCheck);
	DDX_Control(pDX, IDC_BOUNCECHECK, BounceCheck);
	DDX_Control(pDX, IDC_DEBUGCHECK, DebugCheck);
	DDX_Control(pDX, IDC_TRACECHECK, TraceCheck);
	DDX_Check(pDX, IDC_GRAVITYCHECK, Gravity);
	DDX_Check(pDX, IDC_COLLISIONCHECK, Collision);
	DDX_Check(pDX, IDC_MERGECHECK, Merge);
	DDX_Check(pDX, IDC_COREONLYCHECK, CoreOnly);
	DDX_Check(pDX, IDC_BOUNCECHECK, Bounce);
	DDX_Check(pDX, IDC_TRACECHECK, Trace);
	DDX_Check(pDX, IDC_DEBUGCHECK, Debug);
}

BEGIN_MESSAGE_MAP(CParticleSimDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXITBUTTON, &CParticleSimDlg::OnBnClickedExitbutton)
	ON_STN_CLICKED(IDC_PICTURE, &CParticleSimDlg::OnStnClickedPicture)
	ON_BN_CLICKED(IDC_COLLISIONCHECK, &CParticleSimDlg::OnBnClickedCollisioncheck)
END_MESSAGE_MAP()

UINT CParticleSimDlg::SpinThd(LPVOID pParam)
{
	using namespace std;

	auto that = (CParticleSimDlg*)pParam;

	while (that->Simulating)
	{
		while (!that->Queue->empty())
		{
			that->Particles->push_back(that->Queue->front());
			that->Queue->pop();
		}

		if (that->Collision)
		{
			auto del = new unordered_set<Particle*>();

			for (auto p1 : *that->Particles)
			{
				for (auto p2 : *that->Particles)
				{
					if (p1 == p2) continue;

					bool collides = false;

					if (that->CoreOnly)
					{
						collides = ((p2->Location.X - p1->Location.X) * (p2->Location.X - p1->Location.X)) + ((p2->Location.Y - p1->Location.Y) * (p2->Location.Y - p1->Location.Y))
							    <= 4;
					}
					else
					{
						collides = ((p2->Location.X - p1->Location.X) * (p2->Location.X - p1->Location.X)) + ((p2->Location.Y - p1->Location.Y) * (p2->Location.Y - p1->Location.Y))
							    <= ((p1->Size + p2->Size) * (p1->Size + p2->Size));
					}

					if (collides)
					{
						//if (find(del->begin(), del->end(), p1) != del->end()
						// || find(del->begin(), del->end(), p2) != del->end())
						if (del->count(p1) > 0 || del->count(p2) > 0)
						{
							continue;
						}

						del->emplace(p1);
						del->emplace(p2);

						if (that->Merge)
						{
							auto part = new Particle();
							auto sum  = p1->Size + p2->Size;

							part->Size     = max(p1->Size, p2->Size) + (0.5 * min(p1->Size, p2->Size));
							part->Mass     = max(p1->Mass, p2->Mass) + (0.5 * min(p1->Mass, p2->Mass));
							part->Color    = RGB(GetRValue(p1->Color) * (p1->Size / sum) + GetRValue(p2->Color) * (p2->Size / sum), GetGValue(p1->Color) * (p1->Size / sum) + GetGValue(p2->Color) * (p2->Size / sum), GetBValue(p1->Color) * (p1->Size / sum) + GetBValue(p2->Color) * (p2->Size / sum));
							part->Location = p1->Location * (p1->Size / sum) + p2->Location * (p2->Size / sum);
							part->Velocity = ((p1->Velocity * p1->Mass) + (p2->Velocity * p2->Mass)) / (p1->Mass + p2->Mass);

							that->Queue->push(part);
						}
						else
						{
							p1->Velocity.X *= -1;
							p1->Velocity.Y *= -1;
							p2->Velocity.X *= -1;
							p2->Velocity.Y *= -1;
						}
					}
				}
			}

			if (that->Merge)
			{
				for (auto p : *del)
				{
					auto iter = find(that->Particles->begin(), that->Particles->end(), p);

					if (iter != that->Particles->end())
					{
						that->Particles->erase(iter);
					}
				}
			}

			delete del;
		}

		if (that->Gravity)
		{
			for (auto p1 : *that->Particles)
			{
				auto grav = new Vector();

				for (auto p2 : *that->Particles)
				{
					if (p1 == p2) continue;

					auto dist = p2->Location - p1->Location;
					auto magn = sqrt((dist.X * dist.X) + (dist.Y * dist.Y) + (dist.Z * dist.Z));
					auto efct = (6.67384e-3 * ((p1->Mass * p2->Mass) / (magn * magn * magn))) / p1->Mass;

					dist  *= efct;
					*grav += dist;
				}

				p1->Acceleration = *grav;

				delete grav;
			}
		}

		if (that->Bounce)
		{
			for (auto p : *that->Particles)
			{
				if ((p->Location.X <= p->Size && p->Velocity.X < 0)
				 || (p->Location.X >= (that->MainPict.Width - p->Size) && p->Velocity.X > 0))
				{
					p->Velocity.X *= -1;
				}
				if ((p->Location.Y <= p->Size && p->Velocity.Y < 0)
				 || (p->Location.Y >= (that->MainPict.Height - p->Size) && p->Velocity.Y > 0))
				{
					p->Velocity.Y *= -1;
				}
			}
		}

		for (auto p : *that->Particles)
		{
			p->Update(that->Trace);
		}

		that->Generation++;

		that->MainPict.Invalidate();
		Sleep(10);
	}

	return EXIT_SUCCESS;
}

BOOL CParticleSimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	Particles = new std::vector<Particle*>();
	Queue = new std::queue<Particle*>();
	Simulating = true;

	AfxBeginThread(&CParticleSimDlg::SpinThd, this);

	return TRUE;
}

void CParticleSimDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CParticleSimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CParticleSimDlg::OnBnClickedExitbutton()
{
	this->OnCancel();
}

void CParticleSimDlg::OnCancel()
{
	Simulating = false;
	CDialog::OnCancel();
}

void CParticleSimDlg::OnStnClickedPicture()
{
	POINT point;
	GetCursorPos(&point);
	MainPict.ScreenToClient(&point);

	Queue->push(new Particle(RGB(rand() % 255, rand() % 255, rand() % 255), 0, 100 + (rand() % 900) + (1.0 / (rand() % 1000) * 100), Vector(point.x, point.y, 0), Vector(1.0 / (rand() % 1000) * 50 * (rand() % 2 == 1 ? -1 : 1), 1.0 / (rand() % 1000) * 50 * (rand() % 2 == 1 ? -1 : 1), 0), Vector(1.0 / (rand() % 100000) * 10 * (rand() % 2 == 1 ? -1 : 1), 1.0 / (rand() % 100000) * 10 * (rand() % 2 == 1 ? -1 : 1), 0)));
}

void CParticleSimDlg::OnBnClickedCollisioncheck()
{
	bool checked = CollisionCheck.GetCheck() == BST_CHECKED;

	MergeCheck.EnableWindow(checked);
	CoreOnlyCheck.EnableWindow(checked);
}