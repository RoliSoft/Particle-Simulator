#include "stdafx.h"
#include "Picture.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"

CPicture::CPicture(void)
{
	using namespace D2D1;

	EnableD2DSupport();

	auto d = GetRenderTarget();
	
	this->Width          = 0;
	this->Height         = 0;
	this->debugBrush     = new CD2DSolidColorBrush(d, ColorF(ColorF::RoyalBlue));
	this->debugFontBig   = new CD2DTextFormat(d, _T("Verdana"), 14, DWRITE_FONT_WEIGHT_BOLD);
	this->debugFontSmall = new CD2DTextFormat(d, _T("Tahoma"), 8);
	this->bgcf           = new ColorF(ColorF::White);
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

	auto d = (CHwndRenderTarget*)lParam;

	if (this->Width == 0 && this->Height == 0)
	{
		auto s = d->GetPixelSize();

		this->Width      = s.width;
		this->Height     = s.height;
		this->picRect    = CRect(0, 0, s.width, s.height);
		this->debugRect1 = CRect(5, 5, s.width - 5, s.height - 5);
		this->debugRect2 = CRect(5, 45, s.width - 5, s.height - 45);
	}

	d->Clear(*this->bgcf);

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
		d->DrawText(str1, this->debugRect1, this->debugBrush, this->debugFontBig);
		d->DrawText(str2, this->debugRect2, this->debugBrush, this->debugFontSmall);
	}

	return TRUE;
}
