#include "stdafx.h"
#include "Picture.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"

CPicture::CPicture(void)
{
	using namespace D2D1;

	EnableD2DSupport();

	auto d = GetRenderTarget();
	
	Width          = 0;
	Height         = 0;
	debugBrush     = new CD2DSolidColorBrush(d, ColorF(ColorF::RoyalBlue));
	debugFontBig   = new CD2DTextFormat(d, _T("Verdana"), 14, DWRITE_FONT_WEIGHT_BOLD);
	debugFontSmall = new CD2DTextFormat(d, _T("Tahoma"), 8);
	bgcf           = new ColorF(ColorF::White);
}

CPicture::~CPicture(void)
{
	delete debugBrush, debugFontBig, debugFontSmall, bgcf;
}

BEGIN_MESSAGE_MAP(CPicture, CStatic)
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CPicture::OnDraw2D)
END_MESSAGE_MAP()

LRESULT CPicture::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
	using namespace D2D1;

	auto d = (CHwndRenderTarget*)lParam;

	if (Width == 0 && Height == 0)
	{
		auto s = d->GetPixelSize();

		Width      = s.width;
		Height     = s.height;
		picRect    = CRect(0, 0, s.width, s.height);
		debugRect1 = CRect(5, 5, s.width - 5, s.height - 5);
		debugRect2 = CRect(5, 45, s.width - 5, s.height - 45);
	}

	d->Clear(*bgcf);

	bool debug = Owner->DebugCheck.GetCheck() == BST_CHECKED;
	bool trace = Owner->TraceCheck.GetCheck() == BST_CHECKED;

	CString str1, str2;

	if (debug)
	{
		str1.AppendFormat(_T("%i particles\r\n%u generation"), Owner->Particles->size(), Owner->Generation);
	}

	for (unsigned int i = 0; i < Owner->Particles->size(); i++)
	{
		if (debug)
		{
			str2.AppendFormat(_T("#%u: m = %.5f 0.03px\r\n       v = [ %.5f %.5f ] px/gen\r\n       a = [ %.5f %.5f ] px/gen^2\r\n"), i + 1, (*Owner->Particles)[i]->Mass, (*Owner->Particles)[i]->Velocity.X, (*Owner->Particles)[i]->Velocity.Y, (*Owner->Particles)[i]->Acceleration.X, (*Owner->Particles)[i]->Acceleration.Y);
		}

		(*Owner->Particles)[i]->Draw(d, trace);
	}

	if (debug)
	{
		d->DrawText(str1, debugRect1, debugBrush, debugFontBig);
		d->DrawText(str2, debugRect2, debugBrush, debugFontSmall);
	}

	ReleaseSemaphore(Owner->RenderMutex, 1, NULL);

	return TRUE;
}
