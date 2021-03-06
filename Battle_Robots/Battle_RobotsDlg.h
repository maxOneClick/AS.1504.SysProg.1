
// Battle_RobotsDlg.h: файл заголовка
//

#pragma once
#include "Core.h"
#include "afxcmn.h"
#include "Options.h"
#include "afxwin.h"

// Диалоговое окно CBattleRobotsDlg
class CBattleRobotsDlg : public CDialogEx
{
// Создание
public:
	CBattleRobotsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BATTLE_ROBOTS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	HICON e_hIcon;
	HICON r_hIcon;
	HICON g_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Core rbCore;
	int robot_size_slider;
	bool gameGoing = false;
	bool isFirst = true;
	bool pause = true;
	
	int roundNum = 1;
	//int tickTimee;	
	afx_msg void OnNcDestroy();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnBnClickedButton2(); //start
	afx_msg void OnBnClickedButton3(); //settings
	afx_msg void OnBnClickedButton5(); //stop
	afx_msg void OnBnClickedButton4(); //exit
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	bool showNames;
	int sliderCtrl;
	CSliderCtrl sliderTickTime;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnNMCustomdrawList4(NMHDR *pNMHDR, LRESULT *pResult);
	int tickTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
};
