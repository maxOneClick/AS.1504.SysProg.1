// dllmain.cpp: определяет процедуры инициализации для DLL.
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
	// Удалите этот код, если вы используете lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Инициализация BaseRobot.DLL!\n");
		
		// Одноразовая инициализация DLL расширений
		if (!AfxInitExtensionModule(BaseRobotDLL, hInstance))
			return 0;

		// Вставить DLL в цепочку ресурсов
		// ПРИМЕЧАНИЕ. Если выполняется неявная привязка данной DLL расширения с помощью
		//  обычной MFC DLL (например, элемент ActiveX),
		//  а не приложения MFC, то следует
		//  удалить данную строку из DllMain и поместить ее в отдельную
		//  функцию, экспортированную из данной DLL расширения.  Обычная DLL-библиотека,
		//  использующая эту DLL расширения, должна затем явным образом вызвать данную
		//  функцию для инициализации DLL расширения.  В противном случае
		//  объект CDynLinkLibrary не будет прикреплен к
		//  цепочке ресурсов обычной DLL, что приведет к серьезным проблемам
		//  .

		new CDynLinkLibrary(BaseRobotDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Завершение работы BaseRobot.DLL!\n");

		// Завершить работу библиотеки, прежде чем будут вызваны деструкторы
		AfxTermExtensionModule(BaseRobotDLL);
	}
	return 1;   // ОК
}
