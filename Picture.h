#pragma once
#include "afxwin.h"
#include "Particle.h"
#include <iostream>
#include <vector>

class CPicture : public CStatic
{
public:
	CStatic* Owner;
	int Width, Height;

	CPicture(void);
	~CPicture(void);

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);

private:
	CD2DSolidColorBrush *debugBrush;
	CD2DTextFormat *debugFontBig, *debugFontSmall;
	CRect picRect, debugRect1, debugRect2;
	D2D1::ColorF *bgcf;
};

