#pragma once
#include "afxwin.h"
#include "picture.h"
#include "Particle.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

class CParticleSimDlg : public CDialog
{
public:
	CParticleSimDlg(CWnd* pParent = NULL);
	~CParticleSimDlg(void);

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
	CButton GravityCheck, CollisionCheck, CoreOnlyCheck, MergeCheck, BounceCheck, SpeedLimitCheck, TraceCheck, DebugCheck;
	bool Simulating;
	unsigned long long Generation;
	std::vector<Particle*> *Particles;
	std::queue<Particle*> *Queue;
	HANDLE RenderMutex;

	static UINT SpinThd(LPVOID pParam);

	afx_msg void OnBnClickedExitbutton();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedCollisioncheck();
};
