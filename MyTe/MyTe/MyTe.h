// MyTe.h : main header file for the MyTe application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"
//#include "MyTeBand.h"

// CMyTeApp:
// See MyTe.cpp for the implementation of this class
//

class CMyTeApp : public CWinApp
{
public:
	CMyTeApp();
	SData_option Options;
	CMyTeBand* BandArray;//массив кривых реакций
	CMyTeBand Redden;//кривая покраснений
	CMyTeBand EXTINT;//кривая атмосферной экстинкции
	ULONG32 BandCount;//количество кривых реакций в массиве
	//SData_option TempOptions;
	FileListType FileList;//список файлов для вычислений
	//PathsArray ExtintList;//список файлов атмосферной экстинкции
	CMainFrame* m_nWnd;
	
// Overrides
public:
	virtual BOOL InitInstance();
	bool LoadRowFile(CString fname,double* &DataBuffer,ULONG& count,ULONG& position,CString* Filename);
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnActionOption();	
	virtual void OnFinalRelease();
	virtual int ExitInstance();
	//afx_msg void OnActionTest();
	
	afx_msg void OnActionEnergyDistr();
	afx_msg void OnActionMagn();
	afx_msg void OnActionAny();
	afx_msg void OnActionNorm5500();
	afx_msg void OnSaveData();
	afx_msg void OnActionAid();
	afx_msg void OnActionAid1();
	afx_msg void OnConvert();
	afx_msg void OnConvert_1();
	afx_msg void OnConvert_2();
	afx_msg void OnRemoveMinus();
	void OnFileOpen(void);
	afx_msg void OnActionBands();
	afx_msg void OnMenuOH_8();
	afx_msg void OnActionTesting();
	//afx_msg void OnActionAid();
	//afx_msg void OnActionTest();
};

// The one and only CMyTeApp object


extern CMyTeApp theApp;