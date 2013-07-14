#include "stdafx.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CParticleSimDlg::CParticleSimDlg(CWnd* pParent /*=NULL*/) : CDialog(CParticleSimDlg::IDD, pParent)
	, Generation(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	MainPict.Owner = (CStatic*)this;
	srand(time(NULL));
}

CParticleSimDlg::~CParticleSimDlg(void)
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
		bool gravity   = that->GravityCheck.GetCheck()   == BST_CHECKED,
			 collision = that->CollisionCheck.GetCheck() == BST_CHECKED,
			 merge     = that->MergeCheck.GetCheck()     == BST_CHECKED,
			 coreOnly  = that->CoreOnlyCheck.GetCheck()  == BST_CHECKED,
			 bounce    = that->BounceCheck.GetCheck()    == BST_CHECKED,
			 trace     = that->TraceCheck.GetCheck()     == BST_CHECKED,
			 debug     = that->DebugCheck.GetCheck()     == BST_CHECKED;

		while (!that->Queue->empty())
		{
			that->Particles->push_back(that->Queue->front());
			that->Queue->pop();
		}

		if (collision)
		{
			auto del = new unordered_set<Particle*>();

			for (auto p1 : *that->Particles)
			{
				for (auto p2 : *that->Particles)
				{
					if (p1 == p2) continue;

					bool collides = false;

					auto dt = (p2->Location + p2->Velocity + p2->Acceleration) - (p1->Location + p1->Velocity + p1->Acceleration);
					auto it = coreOnly
						    ? ceil(0.25 * p1->Size)
							: (p1->Size + p2->Size) * (p1->Size + p2->Size);

					if ((dt.X * dt.X) + (dt.Y * dt.Y) <= it)
					{
						if (del->count(p1) > 0 || del->count(p2) > 0)
						{
							continue;
						}

						del->emplace(p1);
						del->emplace(p2);

						if (merge)
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
							//p1->Velocity = -p1->Velocity;
							//p2->Velocity = -p2->Velocity;

							auto s1 = (p1->Velocity * (p1->Mass - p2->Mass) + p2->Velocity * p2->Mass * 2) / (p1->Mass + p2->Mass);
							auto s2 = (p2->Velocity * (p2->Mass - p1->Mass) + p1->Velocity * p1->Mass * 2) / (p1->Mass + p2->Mass);
							auto uv = (p2->Location - p1->Location) / p1->Location.DistanceTo(p2->Location);
							p1->Velocity = (-uv) / (-uv).Magnitude() * s1;
							p2->Velocity = uv / uv.Magnitude() * s2;

							//p1->Velocity = (p1->Velocity * (p1->Mass - p2->Mass) + p2->Velocity * p2->Mass * 2) / (p1->Mass + p2->Mass);
							//p2->Velocity = (p2->Velocity * (p2->Mass - p1->Mass) + p1->Velocity * p1->Mass * 2) / (p1->Mass + p2->Mass);

							//auto s1 = sqrt(p1->Velocity.X * p1->Velocity.X + p1->Velocity.Y * p1->Velocity.Y);
							//auto s2 = sqrt(p2->Velocity.X * p2->Velocity.X + p2->Velocity.Y * p2->Velocity.Y);
							//auto cf = (p1->Velocity.X * p2->Velocity.X + p1->Velocity.Y * p2->Velocity.Y) / (s1 * s2);
							//auto fi = cos(pow(cf, -1));
							//p1->Velocity = p1->Velocity * (sqrt(p1->Mass * p1->Mass + p2->Mass * p2->Mass + 2 * p1->Mass * p2->Mass * cf) / (p1->Mass + p2->Mass));
							//p2->Velocity = p1->Velocity * ((p1->Mass * 2) / (p1->Mass + p2->Mass)) * sin(fi / 2);
						}
					}
				}
			}

			if (merge)
			{
				for (auto p : *del)
				{
					auto iter = find(that->Particles->begin(), that->Particles->end(), p);

					if (iter != that->Particles->end())
					{
						that->Particles->erase(iter);
						delete p;
					}
				}
			}

			delete del;
		}

		if (gravity)
		{
			for (auto p1 : *that->Particles)
			{
				Vector grav;

				for (auto p2 : *that->Particles)
				{
					if (p1 == p2) continue;

					auto dist = p2->Location - p1->Location;
					auto efct = (6.67384e-3 * ((p1->Mass * p2->Mass) / pow(dist.Magnitude(), 3))) / p1->Mass;

					dist *= efct;
					grav += dist;
				}

				p1->Acceleration = grav;
			}
		}

		if (bounce)
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
			p->Update(trace);
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

	GravityCheck.SetCheck(BST_CHECKED);
	CollisionCheck.SetCheck(BST_CHECKED);
	CoreOnlyCheck.SetCheck(BST_CHECKED);
	MergeCheck.SetCheck(BST_CHECKED);
	BounceCheck.SetCheck(BST_CHECKED);
	TraceCheck.SetCheck(BST_CHECKED);
	DebugCheck.SetCheck(BST_CHECKED);

	Particles  = new std::vector<Particle*>();
	Queue      = new std::queue<Particle*>();
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
	OnCancel();
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