// find.h : главный файл заголовка для приложения find
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CfindApp:
// О реализации данного класса см. find.cpp
//

class CfindApp : public CWinApp
{
public:
	CfindApp();


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFindstr1();
	afx_msg void OnFindstr2();
	CString ReferenceFile;
	CString SourceFile;
	CString DestFile;
};

extern CfindApp theApp;