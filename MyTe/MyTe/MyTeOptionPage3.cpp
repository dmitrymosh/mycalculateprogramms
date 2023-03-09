// MyTeOptionPage3.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeOptionPage3.h"


// CMyTeOptionPage3 dialog

IMPLEMENT_DYNAMIC(CMyTeOptionPage3, CPropertyPage)

CMyTeOptionPage3::CMyTeOptionPage3()
	: CPropertyPage(CMyTeOptionPage3::IDD)
{

}

CMyTeOptionPage3::~CMyTeOptionPage3()
{
}

void CMyTeOptionPage3::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    CWnd* hListBox = GetDlgItem(IDC_BAND_FILES);

    LPTSTR lpszFile;
    TCHAR tchBuffer[MAX_PATH + 1];
    lpszFile = tchBuffer;
    if (pDX->m_bSaveAndValidate)
    {
	theApp.BandArray.clear();
	theApp.BandCount = (ULONG32)hListBox->SendMessage(LB_GETCOUNT, 0, 0);
	theApp.BandArray.resize(theApp.BandCount);
	for (ULONG32 i = 0; i < theApp.BandCount; i++)
	{
	    hListBox->SendMessage(LB_GETTEXT, i, (LPARAM)lpszFile);
	    theApp.BandArray[i].LoadFromFile(lpszFile);
	}
	CString A;
	DDX_Text(pDX, IDC_REDDEN, A);
	theApp.Redden.FilePath = A;
	theApp.Redden.LoadFromFile(theApp.Redden.FilePath);
    }
    else
    {
	ULONG32 ccount = (ULONG32)hListBox->SendMessage(LB_GETCOUNT, 0, 0);
	while (ccount > 0) {
	    ccount = (ULONG32)hListBox->SendMessage(LB_DELETESTRING, 0, 0);
	}

	//заполняем список файлов в диалоге
	int count = theApp.BandCount;
	if (count != 0)
	{
	    for (int i = 0; i < count; i++)
	    {
		int pos = (int)hListBox->SendMessage(LB_ADDSTRING, 0, (LPARAM)theApp.BandArray[i].FilePath.c_str());
		hListBox->SendMessage(LB_SETITEMDATA, pos, (LPARAM)i);
	    }
	}
	CString A = theApp.Redden.FilePath.c_str();
	DDX_Text(pDX, IDC_REDDEN, A);
    }


    //theApp.Redden.ConvertLnToFlux();
    //theApp.Redden.NormMax();
}


BEGIN_MESSAGE_MAP(CMyTeOptionPage3, CPropertyPage)
	ON_BN_CLICKED(IDC_ADD_BAND, &CMyTeOptionPage3::OnBnClickedAddBand)
	ON_BN_CLICKED(IDC_REMOVE_BAND, &CMyTeOptionPage3::OnBnClickedRemoveBand)
	ON_BN_CLICKED(IDC_ADD_REDDEN, &CMyTeOptionPage3::OnBnClickedAddRedden)
END_MESSAGE_MAP()


// CMyTeOptionPage3 message handlers

void CMyTeOptionPage3::OnBnClickedAddBand()
{
    CFileDialog dlgFile(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, NULL, 0);
    //CFileDialog dlgFile(true);
    CString strFilter;
    CString strDefault;
    CString fileName;
    //CString fileName;
    // append the "*.*" all files filter
    CString allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
    strFilter += allFilter;
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;

    //dlgFile.m_ofn.lpstrTitle = title;
    dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(MAX_PATH * 5000);

    dlgFile.m_ofn.nMaxFile = MAX_PATH * 5000;
    INT_PTR nResult = dlgFile.DoModal();
    if (nResult == IDOK)
    {
	POSITION fpos = dlgFile.GetStartPosition();

	CWnd* hwndList = GetDlgItem(IDC_BAND_FILES);
	CString data;
	int i = 0;
	do
	{
	    data = dlgFile.GetNextPathName(fpos);
	    //data=dlgFile.GetFileName();
	    int pos = (int)hwndList->SendMessage(LB_ADDSTRING, 0, (LPARAM)data.GetString());
	    hwndList->SendMessage(LB_SETITEMDATA, pos, (LPARAM)i);
	    i++;
	} while (fpos != NULL);
	hwndList->SetFocus();
    }
    fileName.ReleaseBuffer();
}

void CMyTeOptionPage3::OnBnClickedRemoveBand()
{
	LPTSTR lpszFileToDelete; 
	int nSelItems; 
	int nSelItemsInBuffer; 
	TCHAR tchBuffer[MAX_PATH+1];      
	int nBuffer[8192]; 

	lpszFileToDelete = tchBuffer; 
	CWnd* hListBox = GetDlgItem(IDC_BAND_FILES);   
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

void CMyTeOptionPage3::OnBnClickedAddRedden()
{
	CFileDialog dlgFile(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , NULL, NULL, 0);
	//CFileDialog dlgFile(true);
	CString strFilter;
	CString strDefault;	
	CString fileName;

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	INT_PTR nResult = dlgFile.DoModal();
	if(nResult==IDOK)
	{
		CWnd* hEditBox = GetDlgItem(IDC_REDDEN);  	
		POSITION fpos=dlgFile.GetStartPosition();
		CString data(_T("Error"));		
		data=dlgFile.GetNextPathName(fpos);	
		
		SETTEXTEX opt;
		opt.flags=ST_KEEPUNDO;
		opt.codepage=1200;
		hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 
	}
}
