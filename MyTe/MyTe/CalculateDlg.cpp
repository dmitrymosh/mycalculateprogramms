// CalculateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTe.h"
#include "CalculateDlg.h"



// CCalculateDlg dialog

IMPLEMENT_DYNAMIC(CCalculateDlg, CDialog)

CCalculateDlg::CCalculateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalculateDlg::IDD, pParent)
{
	NeedFolder=false;
}

CCalculateDlg::~CCalculateDlg()
{
}

void CCalculateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //The flag(m_bSaveAndValidate) is nonzero if the 
    //CDataExchange object is being used to move data from the 
    //dialog controls to dialog-class 
    //data members after the user edits the controls. 
    //The flag is zero if the object is being used to initialize 
    //dialog controls from dialog-class data members.
    CWnd* hListBox = GetDlgItem(IDC_INPUT_FILES);
    CWnd* hListBox2 = GetDlgItem(IDC_INPUT_FILES2);
    CWnd* hListBox3 = GetDlgItem(IDC_INPUT_FILES3);
    CWnd* hRadioBtn1 = GetDlgItem(IDC_REWRITE);
    CWnd* hRadioBtn2 = GetDlgItem(IDC_APPEND);
    LPTSTR lpszFile;
    TCHAR tchBuffer[MAX_PATH + 1];
    lpszFile = tchBuffer;
    if (pDX->m_bSaveAndValidate)
    {
	//получаем список файлов из диалога
	int nItems = hListBox->SendMessage(LB_GETCOUNT, 0, 0);
	if (theApp.FileList.Files.size() != 0) theApp.FileList.Files.clear();
	for (int i = 0; i < nItems; i++)
	{
	    hListBox->SendMessage(LB_GETTEXT, i, (LPARAM)lpszFile);
	    theApp.FileList.Files.push_back(lpszFile);
	}

	nItems = hListBox2->SendMessage(LB_GETCOUNT, 0, 0);
	if (theApp.FileList.ExtintFile.size() != 0) theApp.FileList.ExtintFile.clear();
	for (int i = 0; i < nItems; i++)
	{
	    hListBox2->SendMessage(LB_GETTEXT, i, (LPARAM)lpszFile);
	    theApp.FileList.ExtintFile.push_back(lpszFile);
	}

	nItems = hListBox3->SendMessage(LB_GETCOUNT, 0, 0);
	if (theApp.FileList.AdvFilterFile.size() != 0) theApp.FileList.AdvFilterFile.clear();
	for (int i = 0; i < nItems; i++)
	{
	    hListBox3->SendMessage(LB_GETTEXT, i, (LPARAM)lpszFile);
	    theApp.FileList.AdvFilterFile.push_back(lpszFile);
	}

	//запоминаем настройку чекбоксов
	theApp.FileList.Over = hRadioBtn1->SendMessage(BM_GETCHECK, 0, 0);
	CString OutFile;
	CString Vega;
	DDX_Text(pDX, IDC_OUTPUT_FILE, OutFile);
	DDX_Text(pDX, IDC_VEGA_FILE, Vega);
	theApp.FileList.OutFile = OutFile;
	theApp.FileList.VegaFile = Vega;
    }
    else
    {
	//заполняем список файлов в диалоге
	size_t count = theApp.FileList.Files.size();
	if (count != 0)
	{
	    for (size_t i = 0; i < count; i++)
	    {
		int pos = (int)hListBox->SendMessage(LB_ADDSTRING, 0, (LPARAM)theApp.FileList.Files[i].c_str());
		hListBox->SendMessage(LB_SETITEMDATA, pos, (LPARAM)i);
	    }
	}
	count = theApp.FileList.ExtintFile.size();
	if (count != 0)
	{
	    for (size_t i = 0; i < count; i++)
	    {
		int pos = (int)hListBox2->SendMessage(LB_ADDSTRING, 0, (LPARAM)theApp.FileList.ExtintFile[i].c_str());
		hListBox2->SendMessage(LB_SETITEMDATA, pos, (LPARAM)i);
	    }
	}
	count = theApp.FileList.AdvFilterFile.size();
	if (count != 0)
	{
	    for (size_t i = 0; i < count; i++)
	    {
		int pos = (int)hListBox3->SendMessage(LB_ADDSTRING, 0, (LPARAM)theApp.FileList.AdvFilterFile[i].c_str());
		hListBox3->SendMessage(LB_SETITEMDATA, pos, (LPARAM)i);
	    }
	}
	//настройка чекбоксов
	if (theApp.FileList.Over == 1) hRadioBtn1->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	else hRadioBtn2->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	CString OutFile = theApp.FileList.OutFile.c_str();
	CString Vega = theApp.FileList.VegaFile.c_str();
	DDX_Text(pDX, IDC_OUTPUT_FILE, OutFile);
	DDX_Text(pDX, IDC_VEGA_FILE, Vega);
    }

}


BEGIN_MESSAGE_MAP(CCalculateDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_FILE, &CCalculateDlg::OnBnClickedAddFile)
	ON_BN_CLICKED(IDC_REMOVE_FILE, &CCalculateDlg::OnBnClickedRemoveFile)
	ON_BN_CLICKED(IDC_BROWSE_FILE, &CCalculateDlg::OnBnClickedBrowseFile)
	ON_BN_CLICKED(IDC_ADD_FOLDER, &CCalculateDlg::OnBnClickedAddFolder)
	ON_BN_CLICKED(IDC_BROWSE_FILE_VEGA, &CCalculateDlg::OnBnClickedBrowseFileVega)
	ON_BN_CLICKED(IDC_ADD_FILE2, &CCalculateDlg::OnBnClickedAddFile2)
	ON_BN_CLICKED(IDC_REMOVE_FILE2, &CCalculateDlg::OnBnClickedRemoveFile2)
    ON_BN_CLICKED(IDC_ADD_FILE3, &CCalculateDlg::OnBnClickedAddFile3)
    ON_BN_CLICKED(IDC_REMOVE_FILE3, &CCalculateDlg::OnBnClickedRemoveFile3)
END_MESSAGE_MAP()


// CCalculateDlg message handlers

void CCalculateDlg::OnBnClickedAddFile()
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
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(MAX_PATH*5000);
	
	dlgFile.m_ofn.nMaxFile = MAX_PATH*5000;
	INT_PTR nResult = dlgFile.DoModal();
	if(nResult==IDOK)
	{
		POSITION fpos=dlgFile.GetStartPosition();	

		CWnd* hwndList = GetDlgItem(IDC_INPUT_FILES);  
		CString data;
		int i=0;
		do
		{		
			data=dlgFile.GetNextPathName(fpos);
			//data=dlgFile.GetFileName();
			int pos = (int)hwndList->SendMessage(LB_ADDSTRING, 0, (LPARAM) data.GetString()); 
			hwndList->SendMessage(LB_SETITEMDATA, pos, (LPARAM) i); 
			i++;
		} while (fpos!=NULL);
		hwndList->SetFocus(); 
	}
	fileName.ReleaseBuffer();
}

void CCalculateDlg::OnBnClickedRemoveFile()
{
	LPTSTR lpszFileToDelete; 
    int nSelItems; 
    int nSelItemsInBuffer; 
    TCHAR tchBuffer[MAX_PATH+1];      
    int nBuffer[8192]; 

	lpszFileToDelete = tchBuffer; 
	CWnd* hListBox = GetDlgItem(IDC_INPUT_FILES);   
    nSelItems =hListBox->SendMessage(LB_GETSELCOUNT, 0, 0); 
    nSelItemsInBuffer = hListBox->SendMessage(LB_GETSELITEMS, 512, (LPARAM) nBuffer); 
    if (nSelItems <=nSelItemsInBuffer) 
    { 
         for (int i = nSelItemsInBuffer - 1; i >= 0; i--) 
        {             
            hListBox->SendMessage(LB_DELETESTRING, nBuffer[i], 0); 
        } 
        hListBox->SendMessage(LB_SETCARETINDEX, 0, 0); 
    } 
}

void CCalculateDlg::OnBnClickedBrowseFile()
{
	CWnd* hEditBox = GetDlgItem(IDC_OUTPUT_FILE); 
	CString data(_T("Error"));
	if (NeedFolder==false)
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
			CWnd* hEditBox = GetDlgItem(IDC_OUTPUT_FILE);  	
			POSITION fpos=dlgFile.GetStartPosition();
			
			if(!NeedFolder)
			{
				data=dlgFile.GetNextPathName(fpos);	 
			}else 
			{		

				data=dlgFile.GetNextPathName(fpos);	 
				ULONG32 NewLen=0;			
				for(int i=data.GetLength()-1;i>=0;i--)
				{
					if(data[i]=='\\') 
					{
						NewLen=i;
						break;
					}
				}
				data.Truncate(NewLen);
			}
		}
	}else
	{
		//LPITEMIDLIST p;
		BROWSEINFO inf;
		inf.hwndOwner = AfxGetApp()->m_pMainWnd->m_hWnd;
		inf.pidlRoot = NULL;
		inf.lpszTitle = _T("Выберите папку для сохрания ...");
		inf.lpfn = NULL;
		inf.lParam = NULL;
		inf.pszDisplayName = NULL;
		inf.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_NEWDIALOGSTYLE|BIF_STATUSTEXT|BIF_USENEWUI ;
		inf.iImage = NULL;
		//InitCommonControls();
		//p = SHBrowseForFolder(&inf);
		//data=inf.pszDisplayName;

		//BROWSEINFO bi = { 0 };
		//bi.lpszTitle = _T("Выберите папку для сохрания ...");
		LPITEMIDLIST pidl = SHBrowseForFolder ( &inf );
		if ( pidl != 0 )
		{
			// get the name of the folder
			TCHAR path[MAX_PATH];
			if ( SHGetPathFromIDList ( pidl, path ) )
			{
				data.SetString(path);
			}

			// free memory used
			IMalloc * imalloc = 0;
			if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
			{
				imalloc->Free ( pidl );
				imalloc->Release ( );
			}
		}

	}
	SETTEXTEX opt;
	opt.flags=ST_KEEPUNDO;
	opt.codepage=1200;
	hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 
	
}

void CCalculateDlg::OnBnClickedAddFolder()
{

	//BrowseFolderDlg dlgFolder;
	//INT_PTR nResult = dlgFolder.DoModal();

	/*
	BrowseFolderDlg dlgFile(true);
	//CFileDialog dlgFile(true);
	CString strFilter;
	CString strDefault;	
	CString fileName;


	INT_PTR nResult = dlgFile.DoModal();
	if(nResult==IDOK)
	{
		CWnd* hEditBox = GetDlgItem(IDC_OUTPUT_FILE);  	

		CString data=dlgFile.GetFolderPath();	 
		SETTEXTEX opt;
		opt.flags=ST_KEEPUNDO;
		opt.codepage=1200;
		hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 	
	}*/
}

INT_PTR CCalculateDlg::DoModal(bool NeedFolder)
{
	//this->NeedFolder=NeedFolder;
	return CDialog::DoModal();
}

void CCalculateDlg::OnBnClickedBrowseFileVega()
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
		CWnd* hEditBox = GetDlgItem(IDC_VEGA_FILE);  	
		POSITION fpos=dlgFile.GetStartPosition();
		CString data(_T("Error"));
		if(!NeedFolder)
		{
			data=dlgFile.GetNextPathName(fpos);	 
		}else 
		{		

			data=dlgFile.GetNextPathName(fpos);	 
			ULONG32 NewLen=0;			
			for(int i=data.GetLength()-1;i>=0;i--)
			{
				if(data[i]=='\\') 
				{
					NewLen=i;
					break;
				}
			}
			data.Truncate(NewLen);
		}
		SETTEXTEX opt;
		opt.flags=ST_KEEPUNDO;
		opt.codepage=1200;
		hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 
	}
}
void CCalculateDlg::OnBnClickedAddFile2()
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
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(MAX_PATH);

	dlgFile.m_ofn.nMaxFile = MAX_PATH;
	INT_PTR nResult = dlgFile.DoModal();
	if (nResult == IDOK)
	{
		POSITION fpos = dlgFile.GetStartPosition();

		CWnd* hwndList = GetDlgItem(IDC_INPUT_FILES2);
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



void CCalculateDlg::OnBnClickedRemoveFile2()
{
	int nSelItems;
	int nSelItemsInBuffer;
	TCHAR tchBuffer[MAX_PATH + 1];
	int nBuffer[8192];

	CWnd* hListBox = GetDlgItem(IDC_INPUT_FILES2);
	nSelItems = hListBox->SendMessage(LB_GETSELCOUNT, 0, 0);
	nSelItemsInBuffer = hListBox->SendMessage(LB_GETSELITEMS, 512, (LPARAM)nBuffer);
	if (nSelItems <= nSelItemsInBuffer)
	{
		for (int i = nSelItemsInBuffer - 1; i >= 0; i--)
		{
			hListBox->SendMessage(LB_DELETESTRING, nBuffer[i], 0);
		}
		hListBox->SendMessage(LB_SETCARETINDEX, 0, 0);
	}
}

void CCalculateDlg::OnBnClickedAddFile3()
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
    dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(MAX_PATH);

    dlgFile.m_ofn.nMaxFile = MAX_PATH;
    INT_PTR nResult = dlgFile.DoModal();
    if (nResult == IDOK)
    {
	POSITION fpos = dlgFile.GetStartPosition();

	CWnd* hwndList = GetDlgItem(IDC_INPUT_FILES3);
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

void CCalculateDlg::OnBnClickedRemoveFile3()
{
    int nSelItems;
    int nSelItemsInBuffer;
    TCHAR tchBuffer[MAX_PATH + 1];
    int nBuffer[8192];

    CWnd* hListBox = GetDlgItem(IDC_INPUT_FILES3);
    nSelItems = hListBox->SendMessage(LB_GETSELCOUNT, 0, 0);
    nSelItemsInBuffer = hListBox->SendMessage(LB_GETSELITEMS, 512, (LPARAM)nBuffer);
    if (nSelItems <= nSelItemsInBuffer)
    {
	for (int i = nSelItemsInBuffer - 1; i >= 0; i--)
	{
	    hListBox->SendMessage(LB_DELETESTRING, nBuffer[i], 0);
	}
	hListBox->SendMessage(LB_SETCARETINDEX, 0, 0);
    }
}

