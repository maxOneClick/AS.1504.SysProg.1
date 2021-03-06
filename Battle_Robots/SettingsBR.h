#pragma once
#include "Battle_RobotsDlg.h"
#include "Options.h"


// Диалоговое окно SettingsBR

class SettingsBR : public CDialog
{
	DECLARE_DYNAMIC(SettingsBR)

public:
	SettingsBR(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~SettingsBR();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CBattleRobotsDlg * rbDlg = NULL;
	void updParams(CBattleRobotsDlg*);
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedOk();
	int W;
	int H;
	int N;
	int T;
	int E_max;
	int L_max;
	int V_max;
	int R_max;
	int dL;
	int dE_S;
	int dE_V;
	int dE_A;
	int dE_P;
	int dE;
	int N_E;
	int N_L;
	double RND_min;
	double RND_max;
	int K;
	CString robotsFilePath;
};
