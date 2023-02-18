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
	
	//�� �������� !?
	CPropertySheet::DoDataExchange(pDX);
}
/*
struct SData_option
{
	double Vega; //�������� ����
	CString VegaFile;//���� � ����� ����
	double* Lambda_Beg;//�������� ����������. ������ � ������� ������ ���������
	double* Lambda_End;//�������� ����������. ������ � ������� ����� ���������
	double* VegaFlux;//�������� ����������. ������ �� ��������� ���� � ���� ���������
	ULONG32 NumberBand;//�������� ����������. ���������� ����������
	double StepLmkm;//�������� ������. ������ ����
	double LmkmBeg;//�������� ������. ������ ���������
	double LmkmEnd;//�������� ������. ����� ���������
	ULONG32 OutCount;//�������� ������. ������ ��������
	double GraphBeg;//���������� ��������.������ ��������� 
	double GraphEnd;//���������� ��������.����� ���������
};*/