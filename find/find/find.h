// find.h : ������� ���� ��������� ��� ���������� find
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// CfindApp:
// � ���������� ������� ������ ��. find.cpp
//

class CfindApp : public CWinApp
{
public:
	CfindApp();


// ���������������
public:
	virtual BOOL InitInstance();

// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFindstr1();
	afx_msg void OnFindstr2();
	CString ReferenceFile;
	CString SourceFile;
	CString DestFile;
};

extern CfindApp theApp;