// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif


#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>            // MFC socket extensions
#include <afxdlgs.h>

#include <string>
#include <iostream>



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define E_CONST -1.0857362047581295691278222972915
#include <math.h>
#include <vector>
#include <string>
using namespace std;
//#include "DocDataType.h"
#include "MyFile.h" 

typedef vector <wstring> PathsArray;
//typedef vector < vector<double>> VectorArray;

#ifndef _MYTE_STDAFX
#define _MYTE_STDAFX
struct SData_option
{
	double Vega; //�������� ����
	CString VegaFile;//���� � ����� ����
	double* Lambda_Beg;//�������� ����������. ������ � ������� ������ ���������
	double* Lambda_End;//�������� ����������. ������ � ������� ����� ���������
	double* VegaFlux;//�������� ����������. ������ �� ��������� ���� � ���� ���������
	UINT NumberBand;//�������� ����������. ���������� ����������
	double StepLmkm;//�������� ������. ������ ����
	double LmkmBeg;//�������� ������. ������ ���������
	double LmkmEnd;//�������� ������. ����� ���������
	//ULONG32 OutCount;//�������� ������. ������ ��������
	double GraphBeg;//���������� ��������.������ ��������� 
	double GraphEnd;//���������� ��������.����� ���������
	double Coeff;//
};

struct FileListType//������ ������ ��� ����������
{
	PathsArray Files;//������ ������ ��� ����������
	UINT Count;
	wstring VegaFile;//��� ����� ����
	wstring OutFile;//��� ����� ��� ������ ������. ����� ���� ������ �����. ����� ��� ����� ����� ������������� �����������
	PathsArray ExtintFile; 
	int Over;//������� ���������� ��� ���������� ����� ������
};

class st_GraphWindow
{
public:
	double MinX;	//����������� � ������������ �������� Lambda � Flux
	double MinY;
	double MaxX;
	double MaxY;
	double SelectMinX;	//����������� � ������������ �������� Lambda � Flux ���������� �������
	double SelectMinY;
	double SelectMaxX;
	double SelectMaxY;
	CRect Window;	//���� �� ������

};


/*
struct SBand//������ �������
{
	double* Lambda;
	double* Flux;
	double Count;
	CString fname;
};*/
#include "MyFile.h"
#include "MyTeBand.h"
#include "DocDataType.h"
#include "MyTeMath.h"
#include "DataView.h"
#endif