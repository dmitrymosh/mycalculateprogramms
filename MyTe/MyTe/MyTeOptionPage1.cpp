// MyTeOptionPage1.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeOptionPage1.h"
#include "MyTeSheet.h"
// CMyTeOptionPage1 dialog

IMPLEMENT_DYNAMIC(CMyTeOptionPage1, CPropertyPage)
BEGIN_MESSAGE_MAP(CMyTeOptionPage1, CPropertyPage)
	//	ON_BN_CLICKED(IDC_VEGA_BUTTON, &COptionDialog::OnBnClickedVegaButton)

	ON_BN_CLICKED(IDC_ADD_BAND, &CMyTeOptionPage1::OnBnClickedAddBand)
	ON_BN_CLICKED(IDC_REMOVE_BAND, &CMyTeOptionPage1::OnBnClickedRemoveBand)

END_MESSAGE_MAP()

CMyTeOptionPage1::CMyTeOptionPage1()
	: CPropertyPage(CMyTeOptionPage1::IDD)
{

}

CMyTeOptionPage1::~CMyTeOptionPage1()
{
}

void CMyTeOptionPage1::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);
	CMyTeSheet* temp = (CMyTeSheet*) GetParent();  	
	CWnd* hListBox = GetDlgItem(IDC_BAND_LIST); 
	LPTSTR lpszFile;
	TCHAR tchBuffer[MAX_PATH +1];
	lpszFile = tchBuffer; 
	if(pDX->m_bSaveAndValidate) 
	{		
		//получаем список файлов из диалога
		int nItems = (int)hListBox->SendMessage(LB_GETCOUNT, 0, 0); 	 
		if(temp->TempOptions.NumberBand!=0)
		{
			delete[] temp->TempOptions.Lambda_Beg;
			delete[] temp->TempOptions.Lambda_End;
			delete[] temp->TempOptions.VegaFlux;
		}
		temp->TempOptions.NumberBand=nItems;
		temp->TempOptions.Lambda_Beg=new double[temp->TempOptions.NumberBand];
		temp->TempOptions.Lambda_End=new double[temp->TempOptions.NumberBand];	
		temp->TempOptions.VegaFlux=new double[temp->TempOptions.NumberBand];
		//wchar_t temp[200];
		for(int i=0;i<nItems;i++)
		{
			hListBox->SendMessage(LB_GETTEXT,i, (LPARAM) lpszFile); 			
			swscanf_s(lpszFile,_T("%lf  %lf  %lf"),&temp->TempOptions.Lambda_Beg[i],
				&temp->TempOptions.Lambda_End[i],&temp->TempOptions.VegaFlux[i]);		

		}		
	}
	else
	{ 
		//очищаем список
		hListBox->SendMessage(LB_RESETCONTENT,0, 0); 
		//заполняем список в диалоге
		CString bb;
		int count=temp->TempOptions.NumberBand;
		if(count!=0)
		{
			wchar_t buf[100];
			for(int i=0;i<count;i++)
			{
				swprintf( buf, 100, L"%10.5f", temp->TempOptions.Lambda_Beg[i]);	
				bb=buf;
				swprintf( buf, 100, L"%10.5f", temp->TempOptions.Lambda_End[i]);	
				bb.Append(_T("  "));
				bb.Append(buf);
				swprintf( buf, 100, L"%10.5f", temp->TempOptions.VegaFlux[i]);	
				bb.Append(_T("  "));
				bb.Append(buf);
				int pos = (int)hListBox->SendMessage(LB_ADDSTRING, 0, (LPARAM) bb.GetString()); 
				hListBox->SendMessage(LB_SETITEMDATA, pos, (LPARAM) i); 
			}
		}				
	}
}

// CMyTeOptionPage1 message handlers
void CMyTeOptionPage1::OnBnClickedAddBand()
{
	CWnd* hEditBoxLMin = GetDlgItem(IDC_LAMBDA_MIN);
	CWnd* hEditBoxLMax = GetDlgItem(IDC_LAMBDA_MAX);
	CWnd* hEditBoxV = GetDlgItem(IDC_VEGA_FLUX);
	CWnd* hListBox = GetDlgItem(IDC_BAND_LIST); 
	CString dataLMin("                    ");
	CString dataLMax("                    ");
	CString dataV("                    ");
	CString bb;	
	double LMax;
	double LMin;
	double V;
	hEditBoxLMin->SendMessage(WM_GETTEXT, 20,  (LPARAM)dataLMax.GetBuffer()); 
	hEditBoxLMax->SendMessage(WM_GETTEXT, 20,  (LPARAM)dataLMin.GetBuffer()); 
	hEditBoxV->SendMessage(WM_GETTEXT, 20,  (LPARAM)dataV.GetBuffer());
	if((dataLMax=="")||(dataLMax=="")||(dataV=="")) return;

	swscanf_s(dataLMax.GetBuffer(),_T("%lf"),&LMax);
	swscanf_s(dataLMin.GetBuffer(),_T("%lf"),&LMin);
	swscanf_s(dataV.GetBuffer(),_T("%lf"),&V);

	wchar_t buf[100];
	swprintf( buf, 100, L"%15.10f", LMax);	
	bb=buf;
	swprintf( buf, 100, L"%15.10f", LMin);	
	bb.Append(_T("  "));
	bb.Append(buf);
	swprintf( buf, 100, L"%15.10f", V);	
	bb.Append(_T("  "));
	bb.Append(buf);

	int pos = (int)hListBox->SendMessage(LB_ADDSTRING, 0, (LPARAM) bb.GetString()); 
	hListBox->SendMessage(LB_SETITEMDATA, pos, 3);	
}

void CMyTeOptionPage1::OnBnClickedRemoveBand()
{
	LPTSTR lpszFileToDelete; 
	int nSelItems; 
	int nSelItemsInBuffer; 
	TCHAR tchBuffer[MAX_PATH+1];      
	int nBuffer[8192]; 

	lpszFileToDelete = tchBuffer; 
	CWnd* hListBox = GetDlgItem(IDC_BAND_LIST);   
	nSelItems = (int)hListBox->SendMessage(LB_GETSELCOUNT, 0, 0); 
	nSelItemsInBuffer = (int)hListBox->SendMessage(LB_GETSELITEMS, 512, (LPARAM) nBuffer); 
	if (nSelItems <=nSelItemsInBuffer) 
	{ 
		for (int i = nSelItemsInBuffer - 1; i >= 0; i--) 
		{             
			hListBox->SendMessage(LB_DELETESTRING, nBuffer[i], 0); 
		} 
		hListBox->SendMessage(LB_SETCARETINDEX, 0, 0); 
	} 
}

