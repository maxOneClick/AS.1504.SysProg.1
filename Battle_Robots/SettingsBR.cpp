// SettingsBR.cpp: файл реализации
//

#include "stdafx.h"
#include "Battle_Robots.h"
#include "SettingsBR.h"
#include "afxdialogex.h"


// Диалоговое окно SettingsBR

IMPLEMENT_DYNAMIC(SettingsBR, CDialog)

SettingsBR::SettingsBR(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SETTINGS, pParent)
{

}

void SettingsBR::updParams(CBattleRobotsDlg * rbDlg) {
	this->rbDlg = rbDlg;
	W = rbDlg->rbCore.settings.W;
	H = rbDlg->rbCore.settings.H;
	N = rbDlg->rbCore.settings.N;
	T = rbDlg->rbCore.settings.T;
	E_max = rbDlg->rbCore.settings.E_max;
	L_max = rbDlg->rbCore.settings.L_max;
	V_max = rbDlg->rbCore.settings.V_max;
	R_max = rbDlg->rbCore.settings.R_max;
	dL = rbDlg->rbCore.settings.dL;
	dE_S = rbDlg->rbCore.settings.dE_S;
	dE_V = rbDlg->rbCore.settings.dE_V;
	dE_A = rbDlg->rbCore.settings.dE_A;
	dE_P = rbDlg->rbCore.settings.dE_P;
	dE = rbDlg->rbCore.settings.dE;
	N_E = rbDlg->rbCore.settings.N_E;
	N_L = rbDlg->rbCore.settings.N_L;
	RND_min = rbDlg->rbCore.settings.RND_min;
	RND_max = rbDlg->rbCore.settings.RND_max;
	K = rbDlg->rbCore.settings.K;
	robotsFilePath = rbDlg->rbCore.robotsFilePath;
}



SettingsBR::~SettingsBR()
{
}

void SettingsBR::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_EDIT2, W);
		DDX_Text(pDX, IDC_EDIT3, H);
		DDX_Text(pDX, IDC_EDIT4, N);
		DDX_Text(pDX, IDC_EDIT5, T);
		DDX_Text(pDX, IDC_EDIT6, E_max);
		DDX_Text(pDX, IDC_EDIT7, L_max);
		DDX_Text(pDX, IDC_EDIT8, V_max);
		DDX_Text(pDX, IDC_EDIT9, R_max);
		DDX_Text(pDX, IDC_EDIT10, dL);
		DDX_Text(pDX, IDC_EDIT11, dE_S);
		DDX_Text(pDX, IDC_EDIT12, dE_V);
		DDX_Text(pDX, IDC_EDIT13, dE_A);
		DDX_Text(pDX, IDC_EDIT14, dE_P);
		DDX_Text(pDX, IDC_EDIT15, dE);
		DDX_Text(pDX, IDC_EDIT16, N_E);
		DDX_Text(pDX, IDC_EDIT17, N_L);
		DDX_Text(pDX, IDC_EDIT18, RND_min);
		DDX_Text(pDX, IDC_EDIT19, RND_max);
		DDX_Text(pDX, IDC_EDIT20, K);
		DDX_Text(pDX, IDC_MFCEDITBROWSE1, robotsFilePath);

}


BEGIN_MESSAGE_MAP(SettingsBR, CDialog)
	ON_BN_CLICKED(IDOK, &SettingsBR::OnBnClickedOk)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// Обработчики сообщений SettingsBR


void SettingsBR::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	rbDlg->rbCore.settings.W = W;
	rbDlg->rbCore.settings.H = H;
	rbDlg->rbCore.settings.N = N;
	rbDlg->rbCore.settings.T = T;
	rbDlg->rbCore.settings.E_max = E_max;
	rbDlg->rbCore.settings.L_max = L_max;
	rbDlg->rbCore.settings.V_max = V_max;
	rbDlg->rbCore.settings.R_max = R_max;
	rbDlg->rbCore.settings.dL = dL;
	rbDlg->rbCore.settings.dE_S = dE_S;
	rbDlg->rbCore.settings.dE_V = dE_V;
	rbDlg->rbCore.settings.dE_A = dE_A;
	rbDlg->rbCore.settings.dE_P = dE_P;
	rbDlg->rbCore.settings.dE = dE;
	rbDlg->rbCore.settings.N_E = N_E;
	rbDlg->rbCore.settings.N_L = N_L;
	rbDlg->rbCore.settings.RND_min = RND_min;
	rbDlg->rbCore.settings.RND_max = RND_max;
	rbDlg->rbCore.settings.K = K;
	if (rbDlg->rbCore.robotsFilePath != robotsFilePath) {
		rbDlg->rbCore.robotsFilePath = robotsFilePath;
	}
	rbDlg->rbCore.initCore();

	rbDlg->Invalidate(TRUE);
	rbDlg->UpdateWindow();
	CDialog::OnOK();
}

void SettingsBR::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}