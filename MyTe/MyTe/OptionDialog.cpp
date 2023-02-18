// OptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeMath.h"
#include "OptionDialog.h"
#include <Windows.h>
#include "MyTeSheet.h"


// COptionDialog dialog

IMPLEMENT_DYNAMIC(COptionDialog, CPropertyPage)
BEGIN_MESSAGE_MAP(COptionDialog, CPropertyPage)
	//	ON_BN_CLICKED(IDC_VEGA_BUTTON, &COptionDialog::OnBnClickedVegaButton)


END_MESSAGE_MAP()

COptionDialog::COptionDialog()
	: CPropertyPage(COptionDialog::IDD)
{
	
}

COptionDialog::~COptionDialog()
{
}

void COptionDialog::DoDataExchange(CDataExchange* pDX)
{	
	CDialog::DoDataExchange(pDX);	
	
	//DDX_Text(pDX, IDC_VEGA,(double)theApp.Options.Vega);
	//DDX_Text(pDX, IDC_VEGA_FILE,theApp.Options.VegaFile);
	CMyTeSheet* temp = (CMyTeSheet*) GetParent();
	
	DDX_Text(pDX, IDC_GRAPH_BEG,(double)temp->TempOptions.GraphBeg);
	DDX_Text(pDX, IDC_GRAPH_END,(double)temp->TempOptions.GraphEnd);
}




// COptionDialog message handlers



