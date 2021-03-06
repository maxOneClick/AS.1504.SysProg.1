#pragma once
#include "Core.h"
#include "afxcmn.h"

// Диалоговое окно ResultRB

class ResultRB : public CDialogEx
{
	DECLARE_DYNAMIC(ResultRB)

public:
	ResultRB(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ResultRB();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	vector<Result> results;
	Settings settings;
	void initResults(vector<Result>, Settings, int);
	void AddData(CListCtrl &ctrl, int row, int col, const char *str);
	//CListCtrl listCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl listCtrl;
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
};
