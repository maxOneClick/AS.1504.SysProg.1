
// Battle_Robots.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CBattleRobotsApp:
// Сведения о реализации этого класса: Battle_Robots.cpp
//

class CBattleRobotsApp : public CWinApp
{
public:
	CBattleRobotsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CBattleRobotsApp theApp;
