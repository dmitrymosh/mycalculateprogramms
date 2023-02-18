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



#ifndef _MYTE_STDAFX
#define _MYTE_STDAFX
struct SData_option
{
	double Vega; //Значение Веги
	CString VegaFile;//путь к файлу Веги
	double* Lambda_Beg;//описание диапазонов. массив с Лямбдой начала диапазона
	double* Lambda_End;//описание диапазонов. массив с Лямбдой конца диапазона
	double* VegaFlux;//описание диапазонов. массив со значением Веги в этом диапазоне
	UINT NumberBand;//описание диапазонов. количество диапазонов
	double StepLmkm;//Выходные данные. размер шага
	double LmkmBeg;//Выходные данные. начало диапазона
	double LmkmEnd;//Выходные данные. конец диапазона
	//ULONG32 OutCount;//Выходные данные. размер массивов
	double GraphBeg;//Построение графиков.начало диапазона 
	double GraphEnd;//Построение графиков.конец диапазона
	double Coeff;//
};

class DocDataType
{
public:
	double *Lambda;//Лямбда, входные данные(X) массив с размером Count
	double *Flux;	//Светимость, входные данные(Y) массив с размером Count
	double *ErrFlux;//Ошибка входные данные массив с размером Count
	double *NormErrFlux;//Нормальная ошибка, входные данные массив с размером Count
	UINT Count;//размер входных массивов
	UINT  NumberBand;
	double *SummErrFlux;//сумма квадратов ошибок по диапазонам, массив с размером options->NumberBand
	double *SummNormErrFlux;//сумма нормальных ошибок , массив с размером options->NumberBand
	double *SummFlux;//сумма светимости , массив с размером options->NumberBand
	double *LambdaOut;//Лямбда, выходные данные массив с размером options->OutCount; 
	double *FluxOutW;//Светимость , выходные данные массив с размером options->OutCount;
	double *ErrOut;//ошибка,  выходные данные массив с размером options->OutCount;
	double *FotonCount;//Количество фотонов, выходной массив с размером options->NumberBand
	double *EmFluxWsm2A_7;//Поток в полосах в W/cm2/A, выходной массив с размером options->NumberBand
	double LmBeg;
	double LmEnd;
	double LmStep;
	UINT OutCount;
	DocDataType (void);
	void Init (UINT strcount,UINT NumberBand);
	void InitOut (UINT _OutCount);
	~DocDataType ();
	void operator=(DocDataType &Source); 
	double GetFlux(DocDataType* data,double lambda);
};
typedef DocDataType DDT;
struct FileListType//список файлов для вычислений
{
	CString* Files;//список файлов для вычислений
	UINT Count;
	CString VegaFile;//имя файла Веги
	CString OutFile;//имя файла для вывода отчета
	int Over;//признак перезаписи или добавления файла отчета
};

class st_GraphWindow
{
public:
	double MinX;	//минимальное и максимальное значение Lambda и Flux
	double MinY;
	double MaxX;
	double MaxY;
	double SelectMinX;	//минимальное и максимальное значение Lambda и Flux выбранного участка
	double SelectMinY;
	double SelectMaxX;
	double SelectMaxY;
	CRect Window;	//Окно на экране

};


/*
struct SBand//кривые реакций
{
	double* Lambda;
	double* Flux;
	double Count;
	CString fname;
};*/
#include "MyTeBand.h"
#include "MyTeMath.h"
#include "DataView.h"
#endif