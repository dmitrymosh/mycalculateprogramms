// MyTeSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeSheet.h"
//#include "MyTeOptionPage1.h"
//#include "OptionDialog.h"

// CMyTeSheet

IMPLEMENT_DYNAMIC(CMyTeSheet, CPropertySheet)

CMyTeSheet::CMyTeSheet(ULONG32 nIDCaption, CWnd* pParentWnd, ULONG32 iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
		AddPages();
}

CMyTeSheet::CMyTeSheet(LPCTSTR pszCaption, CWnd* pParentWnd, ULONG32 iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
		AddPages();
}

CMyTeSheet::~CMyTeSheet()
{
}


BEGIN_MESSAGE_MAP(CMyTeSheet, CPropertySheet)
END_MESSAGE_MAP()


// CMyTeSheet message handlers

void CMyTeSheet::AddPages(void)
{	
	AddPage(&PageMain);
	AddPage(&Page1);
	AddPage(&Page2);	
	AddPage(&Page3);
	
}

void CMyTeSheet::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
	
	//не работает !?
	CPropertySheet::DoDataExchange(pDX);
}
/*
struct SData_option
{
	double Vega; //Значение Веги
	CString VegaFile;//путь к файлу Веги
	double* Lambda_Beg;//описание диапазонов. массив с Лямбдой начала диапазона
	double* Lambda_End;//описание диапазонов. массив с Лямбдой конца диапазона
	double* VegaFlux;//описание диапазонов. массив со значением Веги в этом диапазоне
	ULONG32 NumberBand;//описание диапазонов. количество диапазонов
	double StepLmkm;//Выходные данные. размер шага
	double LmkmBeg;//Выходные данные. начало диапазона
	double LmkmEnd;//Выходные данные. конец диапазона
	ULONG32 OutCount;//Выходные данные. размер массивов
	double GraphBeg;//Построение графиков.начало диапазона 
	double GraphEnd;//Построение графиков.конец диапазона
};*/