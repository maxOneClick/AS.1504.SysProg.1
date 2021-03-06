// ResultRB.cpp: файл реализации
//

#include "stdafx.h"
#include "Battle_Robots.h"
#include "ResultRB.h"
#include "SettingsBR.h"
#include "afxdialogex.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>


// Диалоговое окно ResultRB

IMPLEMENT_DYNAMIC(ResultRB, CDialogEx)

ResultRB::ResultRB(CWnd* pParent )
	: CDialogEx(IDD_RESULT, pParent)
{

}

ResultRB::~ResultRB()
{
}

void ResultRB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listCtrl);

}


BEGIN_MESSAGE_MAP(ResultRB, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &ResultRB::OnNMCustomdrawList1)
ON_WM_NCDESTROY()
ON_WM_PAINT()
END_MESSAGE_MAP()

void ResultRB::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}

bool compare(Result res1, Result res2) {
	return (res1.score == res2.score) ? res1.steps > res2.steps : res1.score > res2.score;
}

void ResultRB::AddData(CListCtrl &ctrl, int row, int col, const char *str)
{
	LVITEM lv;
	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = (LPSTR)str;
	lv.mask = LVIF_TEXT;
	if (col == 0)
		ctrl.InsertItem(&lv);
	else
		ctrl.SetItem(&lv);
}

void ResultRB::initResults(vector<Result> results, Settings settings, int roundNum) {
	std::sort(results.begin(), results.end(), compare);
	this->results = results;
	this->settings = settings;
	
	listCtrl.InsertColumn(0, "№");
	listCtrl.SetColumnWidth(0, 35);

	listCtrl.InsertColumn(1, "Имя");
	listCtrl.SetColumnWidth(1, 80);

	listCtrl.InsertColumn(2, "Энергия");
	listCtrl.SetColumnWidth(2, 80);

	listCtrl.InsertColumn(3, "Убийства");
	listCtrl.SetColumnWidth(3, 80);

	listCtrl.InsertColumn(4, "Шаги");
	listCtrl.SetColumnWidth(4, 80);

	listCtrl.InsertColumn(5, "Очки");
	listCtrl.SetColumnWidth(5, 65);

	listCtrl.InsertColumn(6, "Баллы");
	listCtrl.SetColumnWidth(6, 65);

	CString s;
	s.Format("Reports\\Round %d Result.txt", --roundNum);
	ofstream out(s);

	out << setw(3) << "№";
	out << setw(24) << "Имя";
	out << setw(10) << "Энергия";
	out << setw(12) << "Убийства";
	out << setw(8) << "Шаги";
	out << setw(8) << "Очки";
	out << setw(10) << "Баллы\n\n";

	int nPlace = 0;

	for (size_t i = 0; i < results.size(); i++)
	{
		CString s;
		s.Format("%d", i + 1);
		AddData(listCtrl, i, 0, s);

		AddData(listCtrl, i, 1, results[i].name);

		s.Format("%d", (int)results[i].energy);
		AddData(listCtrl, i, 2, s);

		s.Format("%g", results[i].kills);
		AddData(listCtrl, i, 3, s);

		s.Format("%d", results[i].steps);
		AddData(listCtrl, i, 4, s);

		s.Format("%d", results[i].score);
		AddData(listCtrl, i, 5, s);

		out << setw(3) << i + 1;
		out << setw(24) << results[i].name;
		out << setw(10) << (int)results[i].energy;
		out << setw(12) << results[i].kills;
		out << setw(8) << results[i].steps;
		out << setw(8) << results[i].score;

		if (i != 0 && (results[i].score != results[i - 1].score || results[i].steps != results[i - 1].steps))
			nPlace = i;

		if (nPlace == 0)
		{
			s.Format("%d", 5);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 5 << "\n";
		}


		if (nPlace >= 1 && nPlace <= 2)
		{
			s.Format("%d", 4);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 4 << "\n";
		}


		if (nPlace >= 3 && nPlace <= 9)
		{
			s.Format("%d", 3);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 3 << "\n";
		}


		if (nPlace >= 10 && nPlace <= 19)
		{
			s.Format("%d", 2);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 2 << "\n";
		}


		if (nPlace >= 20 && nPlace <= 29)
		{
			s.Format("%d", 1);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 1 << "\n";
		}


		if (nPlace > 29)
		{
			s.Format("%d", 0);
			AddData(listCtrl, i, 6, s);
			out << setw(8) << 0 << "\n";
		}

	}
	Invalidate(TRUE);
	UpdateWindow();
}


void ResultRB::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
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
		if (pLVCD->nmcd.dwItemSpec == 0)
		{
		}
	}
	break;
	}
}


void ResultRB::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
	delete this;
}


void ResultRB::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
}
