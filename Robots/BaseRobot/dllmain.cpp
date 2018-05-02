// dllmain.cpp: ���������� ��������� ������������� ��� DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE BaseRobotDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ������� ���� ���, ���� �� ����������� lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("������������� BaseRobot.DLL!\n");
		
		// ����������� ������������� DLL ����������
		if (!AfxInitExtensionModule(BaseRobotDLL, hInstance))
			return 0;

		// �������� DLL � ������� ��������
		// ����������. ���� ����������� ������� �������� ������ DLL ���������� � �������
		//  ������� MFC DLL (��������, ������� ActiveX),
		//  � �� ���������� MFC, �� �������
		//  ������� ������ ������ �� DllMain � ��������� �� � ���������
		//  �������, ���������������� �� ������ DLL ����������.  ������� DLL-����������,
		//  ������������ ��� DLL ����������, ������ ����� ����� ������� ������� ������
		//  ������� ��� ������������� DLL ����������.  � ��������� ������
		//  ������ CDynLinkLibrary �� ����� ���������� �
		//  ������� �������� ������� DLL, ��� �������� � ��������� ���������
		//  .

		new CDynLinkLibrary(BaseRobotDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("���������� ������ BaseRobot.DLL!\n");

		// ��������� ������ ����������, ������ ��� ����� ������� �����������
		AfxTermExtensionModule(BaseRobotDLL);
	}
	return 1;   // ��
}
