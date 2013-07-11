#include "stdafx.h"
#include "Picture.h"
#include "ParticleSim.h"
#include "ParticleSimDlg.h"

CPicture::CPicture(void)
{

}

CPicture::~CPicture(void)
{

}

BEGIN_MESSAGE_MAP(CPicture, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CPicture::OnPaint()
{
	using namespace Gdiplus;

	CPaintDC dc(this);
	CRect rect;

	GetWindowRect(&rect);
	ScreenToClient(&rect);

	CDC cdc;
	CBitmap bmp;
	cdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	auto old = cdc.SelectObject(&bmp);

	Graphics g(cdc);
	g.Clear(Color(255, 255, 255, 255));

	g.SetPageUnit(UnitPixel);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.SetCompositingQuality(CompositingQualityHighQuality);

	CString str1 = _T("");
	CString str2 = _T("");
	str1.AppendFormat(_T("%i particles\r\n%u generation"), ((CParticleSimDlg*)Owner)->Particles->size(), ((CParticleSimDlg*)Owner)->Generation);

	for (unsigned int i = 0; i < ((CParticleSimDlg*)Owner)->Particles->size(); i++)
	{
		str2.AppendFormat(_T("#%u: m = %.5f 0.03px\r\n       v = [ %.5f %.5f ] px/gen\r\n       a = [ %.5f %.5f ] px/gen^2\r\n"), i + 1, (*((CParticleSimDlg*)Owner)->Particles)[i]->Mass, (*((CParticleSimDlg*)Owner)->Particles)[i]->Velocity.X, (*((CParticleSimDlg*)Owner)->Particles)[i]->Velocity.Y, (*((CParticleSimDlg*)Owner)->Particles)[i]->Acceleration.X, (*((CParticleSimDlg*)Owner)->Particles)[i]->Acceleration.Y);
		(*((CParticleSimDlg*)Owner)->Particles)[i]->Draw(&g);
	}

	SolidBrush brush(Color(255, 0, 0, 255));
	FontFamily fontFamily(_T("Verdana"));
	Gdiplus::Font font1(&fontFamily, 14, FontStyleBold, UnitPixel);
	Gdiplus::Font font2(&fontFamily, 12, FontStyleRegular, UnitPixel);
	PointF point1(5, 5);
	PointF point2(5, 45);
	g.DrawString(str1, -1, &font1, point1, &brush);
	g.DrawString(str2, -1, &font2, point2, &brush);

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &cdc, 0, 0, SRCCOPY);
	cdc.SelectObject(old);
}
