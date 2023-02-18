// MyTeRespSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeRespSelectDlg.h"


// CMyTeRespSelectDlg dialog

IMPLEMENT_DYNAMIC(CMyTeRespSelectDlg, CDialog)

CMyTeRespSelectDlg::CMyTeRespSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyTeRespSelectDlg::IDD, pParent)
{

}

CMyTeRespSelectDlg::~CMyTeRespSelectDlg()
{
}

void CMyTeRespSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CWnd* hListBox = GetDlgItem(IDC_RESPONCE_LIST); 

	LPTSTR lpszFile;
	TCHAR tchBuffer[MAX_PATH +1];
	lpszFile = tchBuffer; 

	//LPTSTR lpszFileToDelete; 
	//int nSelItems; 
	int nSelItemsInBuffer; 	    
	int nBuffer[8192]; 

	if(pDX->m_bSaveAndValidate) 
	{							  
		BandCount = (int)hListBox->SendMessage(LB_GETSELCOUNT, 0, 0); 
		nSelItemsInBuffer = (int)hListBox->SendMessage(LB_GETSELITEMS, 512, (LPARAM) nBuffer); 
		BandNameArray = new CString[BandCount];
		if (BandCount <= nSelItemsInBuffer) 
		{ 
			for (int i = nSelItemsInBuffer - 1; i >= 0; i--) 
			{             
				hListBox->SendMessage(LB_GETTEXT,nBuffer[i], (LPARAM) lpszFile); 
				BandNameArray[i]=lpszFile;
				//hListBox->SendMessage(LB_DELETESTRING, nBuffer[i], 0); 
			} 
			hListBox->SendMessage(LB_SETCARETINDEX, 0, 0); 
		} 
		//получаем список файлов из диалога					
		
	}
	else
	{
		//заполняем список файлов в диалоге
		int count=theApp.BandCount;
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				int pos = (int)hListBox->SendMessage(LB_ADDSTRING, 0, (LPARAM) theApp.BandArray[i].FName.GetString()); 
				hListBox->SendMessage(LB_SETITEMDATA, pos, (LPARAM) i); 
			}
		}		
	}		
}


BEGIN_MESSAGE_MAP(CMyTeRespSelectDlg, CDialog)
END_MESSAGE_MAP()


// CMyTeRespSelectDlg message handlers
