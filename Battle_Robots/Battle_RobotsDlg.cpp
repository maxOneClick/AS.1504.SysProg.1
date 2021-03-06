
// Battle_RobotsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Battle_Robots.h"
#include "Battle_RobotsDlg.h"
#include "ResultRB.h"
#include "SettingsBR.h"
#include "afxdialogex.h"
#include "Options.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CBattleRobotsDlg




CBattleRobotsDlg::CBattleRobotsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BATTLE_ROBOTS_DIALOG, pParent)
	, robot_size_slider(0)
	, showNames(TRUE)
	, tickTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	e_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ENERGY);
	r_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_REMONT); 
	g_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_GRAVE);

}

void CBattleRobotsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBattleRobotsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CBattleRobotsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CBattleRobotsDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CBattleRobotsDlg::OnBnClickedButton4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CBattleRobotsDlg::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CBattleRobotsDlg::OnNMCustomdrawList1)
	ON_BN_CLICKED(IDC_CHECK1, &CBattleRobotsDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBattleRobotsDlg::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CBattleRobotsDlg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
	{
		if (pLVCD->iSubItem >= 0)
		{
			pLVCD->clrTextBk = RGB(255, 0, 0);
		}
	}
	break;
	}
}

// Обработчики сообщений CBattleRobotsDlg

BOOL CBattleRobotsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	rbCore.initCore();
	sliderCtrl=11;
	CString Ns;
	Ns.Format("%d of %d", rbCore.steps, rbCore.settings.N);
	GetDlgItem(IDC_STATIC)->SetWindowTextA(Ns);
	Ns.Format("%d of %d", roundNum, 5);
	GetDlgItem(IDC_STATIC_R)->SetWindowTextA(Ns);

	CFont font;
	font.CreateFont(24, 24, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 0, 0, 0, 0, 0, "Tahoma");
	GetDlgItem(IDC_STATIC)->SetFont(&font);
	GetDlgItem(IDC_STATIC_R)->SetFont(&font);






	return TRUE;
}



void CBattleRobotsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CBattleRobotsDlg::OnPaint()
{

	UpdateData();
	double FIELD_SIZE = 600;
	const int PADDING = 5;

	double cell_size_w = FIELD_SIZE / rbCore.settings.W;
	double cell_size_h = FIELD_SIZE / rbCore.settings.H;

	int space = 0;
	int robot_size = sliderCtrl;

	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	CPen myPen(PS_SOLID, 1, RGB(230, 230, 230));
	dc.SelectObject(myPen);

	dc.Rectangle(PADDING*50, PADDING, PADDING*50 + FIELD_SIZE, PADDING + FIELD_SIZE);

	if (rbCore.settings.W < 100 && rbCore.settings.H < 100) {
		for (int i = PADDING; i < FIELD_SIZE + PADDING; i += cell_size_h) {
			dc.MoveTo(PADDING*50 , i);
			dc.LineTo(PADDING*50 + FIELD_SIZE, i);
		}

		for (int i = PADDING*50; i < FIELD_SIZE + PADDING*50; i += cell_size_w) {
			dc.MoveTo(i, PADDING);
			dc.LineTo(i, PADDING + FIELD_SIZE);
		}
		space = 1;
	}



	SetBkMode(dc, TRANSPARENT);

	for (auto it = rbCore.robots.begin(); it != rbCore.robots.end(); it++) {
		CBrush myBrush1;

		int color = 255 * it->second.E / rbCore.settings.E_max;
		double compress = 0.5 + 0.5 * (it->second.E / rbCore.settings.E_max);
		color = color > 0 ? color : 0;
		compress = compress > 0.5 ? compress : 0.5;
		int rs = robot_size * compress;

		if (it->first.Left(9) == "BaseRobot")
			myBrush1.CreateSolidBrush((RGB(0, 0, color)));
		else
			myBrush1.CreateSolidBrush((RGB(color, 0, 0)));

		dc.SelectObject(myBrush1);
		int x = it->second.X;
		int y = it->second.Y;
		dc.Ellipse(x*cell_size_w + PADDING*50 + space + (cell_size_w / 2) - (rs / 2), y*cell_size_h + PADDING + space + (cell_size_h / 2) - (rs / 2), x*cell_size_w + PADDING*50 + (cell_size_w / 2) + (rs / 2), y*cell_size_h + PADDING + (cell_size_h / 2) + (rs / 2));

		if (showNames && it->first.Left(9) != "BaseRobot" && it->second.E > 0)
			dc.TextOutA(x*cell_size_w + PADDING*50 + space, y*cell_size_h + PADDING + space, it->first);

		if (it->second.E <= 0)
		{
			int nX = space + (cell_size_w / 2) - (robot_size / 2);
			int nY = (cell_size_h / 2) + (robot_size / 2);
			int normX = x * cell_size_w + PADDING*50;
			int normY = y * cell_size_h + PADDING;
			DrawIconEx(dc, normX + nX, normY - nY, g_hIcon, 0.85 * robot_size, 0.85 * robot_size, 0, NULL, DI_NORMAL | DI_COMPAT);
		}

	}


	for (size_t i = 0; i < rbCore.chargePoints.size(); i++) {
		int x = rbCore.chargePoints[i].X;
		int y = rbCore.chargePoints[i].Y;
		int nX = space + (cell_size_w / 2) - (robot_size / 2);
		int nY = (cell_size_h / 2) + (robot_size / 2);
		int normX = x * cell_size_w + PADDING*50;
		int normY = y * cell_size_h + PADDING;
		DrawIconEx(dc, normX + nX, normY - nY, e_hIcon, 0.85 * robot_size, 0.85 * robot_size, 0, NULL, DI_NORMAL | DI_COMPAT);
	}

	for (size_t i = 0; i < rbCore.remontPoints.size(); i++) {
		int x = rbCore.remontPoints[i].X;
		int y = rbCore.remontPoints[i].Y;
		int nX = space + (cell_size_w / 2) - (robot_size / 2);
		int nY = (cell_size_h / 2) + (robot_size / 2);
		int normX = x * cell_size_w + PADDING*50;
		int normY = y * cell_size_h + PADDING;
		DrawIconEx(dc, normX + nX, normY - nY, r_hIcon, 0.85 * robot_size, 0.85 * robot_size, 0, NULL, DI_NORMAL | DI_COMPAT);
	}

}

HCURSOR CBattleRobotsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBattleRobotsDlg::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
}

void CBattleRobotsDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!gameGoing) {
		SettingsBR * sd = new SettingsBR;
		sd->updParams(this);
		sd->DoModal();
	}
}


void CBattleRobotsDlg::OnBnClickedButton5()
{
	// TODO: добавьте свой код обработчика уведомлений

	KillTimer(1);
	gameGoing = false;
	pause = true;
	CString Ns;
	Ns.Format("%d of %d", rbCore.steps, rbCore.settings.N);
	GetDlgItem(IDC_STATIC)->SetWindowTextA(Ns);
	rbCore.initCore();
	UpdateWindow();
	Invalidate(TRUE);
}


void CBattleRobotsDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	exit(0);
}


void CBattleRobotsDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	Invalidate(TRUE);
	UpdateWindow();
	*pResult = 0;
}


void CBattleRobotsDlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	Invalidate(TRUE);
	UpdateWindow();
}


void CBattleRobotsDlg::OnLbnSelchangeList2()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void CBattleRobotsDlg::OnNMCustomdrawList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CBattleRobotsDlg::OnBnClickedButton2()
{
	 //TODO: добавьте свой код обработчика уведомлений
		if (!gameGoing) {
			gameGoing = true;
				isFirst = false;
		}
		if (pause) {
			SetTimer(1, tickTime, NULL);
			pause = false;
		}
		else
		{
			KillTimer(1);
			pause = true;
			OnTimer(0);
		}

}


void CBattleRobotsDlg::OnTimer(UINT_PTR nIDEvent)
{
	rbCore.battleStep();
	CString Ns;
	Ns.Format("%d of %d", rbCore.steps, rbCore.settings.N);
	GetDlgItem(IDC_STATIC)->SetWindowTextA(Ns);
	Invalidate(TRUE);
	UpdateWindow();
	CDialogEx::OnTimer(nIDEvent);
	if (rbCore.steps == rbCore.settings.N) {
		KillTimer(1);
		GetDlgItem(IDC_BUTTON3)->SetWindowTextA("Settings");
		gameGoing = false;
		pause = true;
		roundNum++;

		if (roundNum <= 5) {
			rbCore.loadSettings(roundNum);
			Ns.Format("%d of %d", roundNum, 5);
	    	GetDlgItem(IDC_STATIC_R)->SetWindowTextA(Ns);
		}

		rbCore.winners.clear();

		for (auto it = rbCore.robots.begin(); it != rbCore.robots.end(); ++it) {
			Result res;
			res.name = it->first;
			res.kills = it->second.kills;
			res.energy = it->second.E > 0 ? it->second.E : 0;
			res.steps = it->second.prevAction.step;

			res.score = res.energy + res.kills * rbCore.settings.K;
			rbCore.results.push_back(res);

			if (it->second.E > 0) {
				Robot robot = it->second;

				rbCore.winners.push_back(robot);

			}
		}
		bool kkkk = false;
		if (roundNum < 7) {
			CString fname;
			fname.Format("Reports\\Round %d dll.txt", roundNum - 1);
			ofstream f(fname);
			ofstream fnew;
			map<CString, int> bufMap;

			for (auto it = rbCore.robots.begin(); it != rbCore.robots.end(); ++it) {
				CString name = it->first;
				if (name.Right(1) == ")") {
					name = name.Left(name.Find("(", 1));
				}
				bufMap[name] = 1;
			}

			for (size_t i = 0; i < rbCore.winners.size(); i++) {
				CString name = rbCore.winners[i].name;
				if (name.Right(1) == ")") {
					name = name.Left(name.Find("(", 1));
				}
				bufMap[name]++;
			}

			for (auto it = bufMap.begin(); it != bufMap.end(); ++it) {
				CString dllstr = rbCore.robots[it->first].fname;
				int k = it->second;
				if (it->first == "BaseRobot") {
					dllstr = "BaseRobot.dll";
					k--;
				}
				f << dllstr << " " << it->first << " " << k << endl;
				if (roundNum - 1 == 5) {
					if (kkkk == false) {
						fnew.open("robots.txt");
					}
					kkkk = true;
					if (dllstr != "BaseRobot.dll") {
						fnew << " " << dllstr << " " << it->first << " " << k;
					}
				}
			}
			fnew.close();
			f.close();
		}
		ResultRB * sd = new ResultRB;
		sd->Create(IDD_RESULT);
		sd->initResults(rbCore.results, rbCore.settings, roundNum);
		sd->ShowWindow(SW_SHOW);
		rbCore.initCore();
		Invalidate(TRUE);
		UpdateWindow();
	

	}
}


