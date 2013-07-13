#include "stdafx.h"
#include "Picture.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"

CPicture::CPicture(void)
{
	EnableD2DSupport();
}

CPicture::~CPicture(void)
{

}

BEGIN_MESSAGE_MAP(CPicture, CStatic)
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CPicture::OnDraw2D)
END_MESSAGE_MAP()

LRESULT CPicture::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
	using namespace D2D1;

	CHwndRenderTarget* d = (CHwndRenderTarget*)lParam;
	CRect rect;

	GetWindowRect(&rect);
	ScreenToClient(&rect);

	this->Width  = rect.Width();
	this->Height = rect.Height();

	d->Clear(ColorF(ColorF::White));

	bool debug = ((CParticleSimDlg*)Owner)->DebugCheck.GetCheck() == BST_CHECKED;
	bool trace = ((CParticleSimDlg*)Owner)->TraceCheck.GetCheck() == BST_CHECKED;

	CString str1, str2;

	if (debug)
	{
		str1.AppendFormat(_T("%i particles\r\n%u generation"), ((CParticleSimDlg*)Owner)->Particles->size(), ((CParticleSimDlg*)Owner)->Generation);
	}

	for (unsigned int i = 0; i < ((CParticleSimDlg*)Owner)->Particles->size(); i++)
	{
		if (debug)
		{
			str2.AppendFormat(_T("#%u: m = %.5f 0.03px\r\n       v = [ %.5f %.5f ] px/gen\r\n       a = [ %.5f %.5f ] px/gen^2\r\n"), i + 1, (*((CParticleSimDlg*)Owner)->Particles)[i]->Mass, (*((CParticleSimDlg*)Owner)->Particles)[i]->Velocity.X, (*((CParticleSimDlg*)Owner)->Particles)[i]->Velocity.Y, (*((CParticleSimDlg*)Owner)->Particles)[i]->Acceleration.X, (*((CParticleSimDlg*)Owner)->Particles)[i]->Acceleration.Y);
		}

		(*((CParticleSimDlg*)Owner)->Particles)[i]->Draw(d, trace);
	}

	if (debug)
	{
		auto brush = CD2DSolidColorBrush(d, ColorF(ColorF::RoyalBlue));
		auto font1 = CD2DTextFormat(d, _T("Verdana"), 14, DWRITE_FONT_WEIGHT_BOLD);
		auto font2 = CD2DTextFormat(d, _T("Tahoma"), 8);

		CRect rect1(5, 5, this->Width - 5, this->Height - 5);
		CRect rect2(5, 45, this->Width - 5, this->Height - 45);

		d->DrawText(str1, rect1, &brush, &font1);
		d->DrawText(str2, rect2, &brush, &font2);
	}

	return TRUE;
}
