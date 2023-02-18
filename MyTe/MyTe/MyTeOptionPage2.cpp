// MyTeOptionPage2.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeOptionPage2.h"
#include "MyTeSheet.h"


// CMyTeOptionPage2 dialog

IMPLEMENT_DYNAMIC(CMyTeOptionPage2, CPropertyPage)

CMyTeOptionPage2::CMyTeOptionPage2()
	: CPropertyPage(CMyTeOptionPage2::IDD)
{

}

CMyTeOptionPage2::~CMyTeOptionPage2()
{
}

void CMyTeOptionPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	CMyTeSheet* temp = (CMyTeSheet*) GetParent(); 
	DDX_Text(pDX, IDC_STEP_LMKM,(double)temp->TempOptions.StepLmkm);
	DDX_Text(pDX, IDC_BEG_LMKM,(double)temp->TempOptions.LmkmBeg);
	DDX_Text(pDX, IDC_END_LMKM,(double)temp->TempOptions.LmkmEnd);
}


BEGIN_MESSAGE_MAP(CMyTeOptionPage2, CPropertyPage)
END_MESSAGE_MAP()


// CMyTeOptionPage2 message handlers
