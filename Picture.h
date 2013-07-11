#pragma once
#include "afxwin.h"
#include "Particle.h"
#include <iostream>
#include <vector>

class CPicture : public CStatic
{
public:
	CStatic* Owner;

	CPicture(void);
	~CPicture(void);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
};

