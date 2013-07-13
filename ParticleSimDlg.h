#pragma once
#include "afxwin.h"
#include "picture.h"
#include "Particle.h"
#include <iostream>
#include <vector>
#include <algorithm>

class CParticleSimDlg : public CDialog
{
public:
	CParticleSimDlg(CWnd* pParent = NULL);
	~CParticleSimDlg();

	enum { IDD = IDD_PARTICLESIM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	CPicture MainPict;
	CButton GravityCheck;
	CButton CollisionCheck;
	CButton CoreOnlyCheck;
	CButton BounceCheck;
	CButton DebugCheck;
	CButton TraceCheck;
	std::vector<Particle*>* Particles;
	ULONG_PTR GdiplusToken;
	unsigned long long Generation;
	bool Simulating;

	static UINT SpinThd(LPVOID pParam);
	afx_msg void OnBnClickedExitbutton();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedCollisioncheck();
};
