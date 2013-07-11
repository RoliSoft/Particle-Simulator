#include "stdafx.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CParticleSimDlg::CParticleSimDlg(CWnd* pParent /*=NULL*/) : CDialog(CParticleSimDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	MainPict.Owner = (CStatic*)this;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&GdiplusToken, &gdiplusStartupInput, NULL);
	Generation = 0;
	srand(time(NULL));
}

CParticleSimDlg::~CParticleSimDlg()
{
	Gdiplus::GdiplusShutdown(GdiplusToken);
}

void CParticleSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, MainPict);
	DDX_Control(pDX, IDC_GRAVITYCHECK, GravityCheck);
	DDX_Control(pDX, IDC_COLLISIONCHECK, CollisionCheck);
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
	auto that = (CParticleSimDlg*)pParam;

	while (true)
	{
		if (that->CollisionCheck.GetCheck() == BST_CHECKED)
		{
			auto del = new std::vector<Particle*>();
			auto cnt = that->Particles->size();

			for (unsigned int i = 0; i < cnt; i++)
			{
				for (unsigned int j = 0; j < cnt; j++)
				{
					if (i == j) continue;

					bool collides = false;

					if (that->CoreOnlyCheck.GetCheck() == BST_CHECKED)
					{
						collides = (((*that->Particles)[j]->Location.X - (*that->Particles)[i]->Location.X) * ((*that->Particles)[j]->Location.X - (*that->Particles)[i]->Location.X)) + (((*that->Particles)[j]->Location.Y - (*that->Particles)[i]->Location.Y) * ((*that->Particles)[j]->Location.Y - (*that->Particles)[i]->Location.Y))
								<= 4;
					}
					else
					{
						collides = (((*that->Particles)[j]->Location.X - (*that->Particles)[i]->Location.X) * ((*that->Particles)[j]->Location.X - (*that->Particles)[i]->Location.X)) + (((*that->Particles)[j]->Location.Y - (*that->Particles)[i]->Location.Y) * ((*that->Particles)[j]->Location.Y - (*that->Particles)[i]->Location.Y))
								<= (((*that->Particles)[i]->Size + (*that->Particles)[j]->Size) * ((*that->Particles)[i]->Size + (*that->Particles)[j]->Size));
					}

					if (collides)
					{
						if (std::find(del->begin(), del->end(), (*that->Particles)[i]) != del->end()
							|| std::find(del->begin(), del->end(), (*that->Particles)[j]) != del->end())
						{
							continue;
						}

						del->push_back((*that->Particles)[i]);
						del->push_back((*that->Particles)[j]);

						auto part = new Particle();
						auto sum = (*that->Particles)[i]->Size + (*that->Particles)[j]->Size;

						part->Size = max((*that->Particles)[i]->Size, (*that->Particles)[j]->Size) + (0.5 * min((*that->Particles)[i]->Size, (*that->Particles)[j]->Size));
						part->Mass = max((*that->Particles)[i]->Mass, (*that->Particles)[j]->Mass) + (0.5 * min((*that->Particles)[i]->Mass, (*that->Particles)[j]->Mass));
						part->Color = RGB(GetRValue((*that->Particles)[i]->Color) * ((*that->Particles)[i]->Size / sum) + GetRValue((*that->Particles)[j]->Color) * ((*that->Particles)[j]->Size / sum), GetGValue((*that->Particles)[i]->Color) * ((*that->Particles)[i]->Size / sum) + GetGValue((*that->Particles)[j]->Color) * ((*that->Particles)[j]->Size / sum), GetBValue((*that->Particles)[i]->Color) * ((*that->Particles)[i]->Size / sum) + GetBValue((*that->Particles)[j]->Color) * ((*that->Particles)[j]->Size / sum));
						part->Location = (*that->Particles)[i]->Location * ((*that->Particles)[i]->Size / sum) + (*that->Particles)[j]->Location * ((*that->Particles)[j]->Size / sum);
						part->Velocity = (((*that->Particles)[i]->Velocity * (*that->Particles)[i]->Mass) + ((*that->Particles)[j]->Velocity * (*that->Particles)[j]->Mass)) / ((*that->Particles)[i]->Mass + (*that->Particles)[j]->Mass);

						that->Particles->push_back(part);
					}
				}
			}

			for (unsigned int i = 0; i < del->size(); i++)
			{
				auto iter = std::find(that->Particles->begin(), that->Particles->end(), (*del)[i]);

				if (iter != that->Particles->end())
				{
					that->Particles->erase(iter);
				}
			}

			delete del;
		}

		if (that->GravityCheck.GetCheck() == BST_CHECKED)
		{
			for (unsigned int i = 0; i < that->Particles->size(); i++)
			{
				auto grav = new Vector();

				for (unsigned int j = 0; j < that->Particles->size(); j++)
				{
					if (i == j) continue;

					auto dist = (*that->Particles)[j]->Location - (*that->Particles)[i]->Location;
					auto magn = sqrt((dist.X * dist.X) + (dist.Y * dist.Y) + (dist.Z * dist.Z));
					auto efct = (6.67384e-3 * (((*that->Particles)[i]->Mass * (*that->Particles)[j]->Mass) / (magn * magn * magn))) / (*that->Particles)[i]->Mass;

					dist *= efct;
					*grav += dist;
				}

				(*that->Particles)[i]->Acceleration = *grav;

				delete grav;
			}
		}

		if (that->BounceCheck.GetCheck() == BST_CHECKED)
		{
			for (unsigned int i = 0; i < that->Particles->size(); i++)
			{
				if (((*that->Particles)[i]->Location.X <= (*that->Particles)[i]->Size && (*that->Particles)[i]->Velocity.X < 0)
				 || ((*that->Particles)[i]->Location.X >= (that->MainPict.Width - (*that->Particles)[i]->Size - 3) && (*that->Particles)[i]->Velocity.X > 0))
				{
					(*that->Particles)[i]->Velocity.X *= -1;
				}
				if (((*that->Particles)[i]->Location.Y <= (*that->Particles)[i]->Size && (*that->Particles)[i]->Velocity.Y < 0)
				 || ((*that->Particles)[i]->Location.Y >= (that->MainPict.Height - (*that->Particles)[i]->Size - 3) && (*that->Particles)[i]->Velocity.Y > 0))
				{
					(*that->Particles)[i]->Velocity.Y *= -1;
				}
			}
		}

		bool trace = that->TraceCheck.GetCheck() == BST_CHECKED;

		for (unsigned int i = 0; i < that->Particles->size(); i++)
		{
			(*that->Particles)[i]->Update(trace);
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
	BounceCheck.SetCheck(BST_CHECKED);
	//TraceCheck.SetCheck(BST_CHECKED);
	DebugCheck.SetCheck(BST_CHECKED);

	Particles = new std::vector<Particle*>();

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

void CParticleSimDlg::OnStnClickedPicture()
{
	POINT point;
	GetCursorPos(&point);
	MainPict.ScreenToClient(&point);

	Particles->push_back(new Particle(RGB(rand() % 255, rand() % 255, rand() % 255), 0, 100 + (rand() % 900) + (1.0 / (rand() % 1000) * 100), Vector(point.x, point.y, 0), Vector(1.0 / (rand() % 1000) * 50 * (rand() % 2 == 1 ? -1 : 1), 1.0 / (rand() % 1000) * 50 * (rand() % 2 == 1 ? -1 : 1), 0), Vector(1.0 / (rand() % 100000) * 10 * (rand() % 2 == 1 ? -1 : 1), 1.0 / (rand() % 100000) * 10 * (rand() % 2 == 1 ? -1 : 1), 0)));
}

void CParticleSimDlg::OnBnClickedCollisioncheck()
{
	CoreOnlyCheck.EnableWindow(CollisionCheck.GetCheck() == BST_CHECKED);
}
