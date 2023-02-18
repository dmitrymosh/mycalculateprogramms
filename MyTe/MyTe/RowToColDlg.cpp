// RowToColDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MyTe.h"
#include "RowToColDlg.h"


// диалоговое окно CRowToColDlg

IMPLEMENT_DYNAMIC(CRowToColDlg, CDialog)

CRowToColDlg::CRowToColDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRowToColDlg::IDD, pParent)
{

	EnableAutomation();

}

CRowToColDlg::~CRowToColDlg()
{
}

void CRowToColDlg::OnFinalRelease()
{
	// Когда будет освобождена последняя ссылка на объект автоматизации,
	// вызывается OnFinalRelease. Базовый класс автоматически
	// удалит объект. Перед вызовом базового класса добавьте
	// дополнительную очистку, необходимую вашему объекту.

	CDialog::OnFinalRelease();
}

void CRowToColDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_SOURCE2,SourceFileFlux);
	DDX_Text(pDX, IDC_EDIT_FILE_SOURCE,SourceFileLambda);
	DDX_Text(pDX, IDC_EDIT_FOLDER_DEST,DestFolder);
}


BEGIN_MESSAGE_MAP(CRowToColDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_FILE_SOURCE, &CRowToColDlg::OnBnClickedBrowseFileSource)
	ON_BN_CLICKED(IDC_BROWSE_FILE_SOURCE2, &CRowToColDlg::OnBnClickedBrowseFileSource2)
	ON_BN_CLICKED(IDC_BROWSE_FOLDER_DEST, &CRowToColDlg::OnBnClickedBrowseFolderDest)
	ON_BN_CLICKED(IDOK, &CRowToColDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRowToColDlg, CDialog)
END_DISPATCH_MAP()

// Примечание: мы добавили поддержку для IID_IRowToColDlg, чтобы обеспечить безопасную с точки зрения типов привязку
//  из VBA. Этот IID должен соответствовать GUID, связанному с 
//  disp-интерфейсом в файле .IDL.

// {51C76AF2-9588-45FC-83C8-143E29B9D7DD}
static const IID IID_IRowToColDlg =
{ 0x51C76AF2, 0x9588, 0x45FC, { 0x83, 0xC8, 0x14, 0x3E, 0x29, 0xB9, 0xD7, 0xDD } };

BEGIN_INTERFACE_MAP(CRowToColDlg, CDialog)
	INTERFACE_PART(CRowToColDlg, IID_IRowToColDlg, Dispatch)
END_INTERFACE_MAP()


// обработчики сообщений CRowToColDlg

void CRowToColDlg::OnBnClickedBrowseFileSource()
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
		CWnd* hEditBox = GetDlgItem(IDC_EDIT_FILE_SOURCE);  	
		POSITION fpos=dlgFile.GetStartPosition();
		CString data(_T("Error"));		
		data=dlgFile.GetNextPathName(fpos);	

		SETTEXTEX opt;
		opt.flags=ST_KEEPUNDO;
		opt.codepage=1200;
		hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 
	}
}
void CRowToColDlg::OnBnClickedBrowseFileSource2()
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
		CWnd* hEditBox = GetDlgItem(IDC_EDIT_FILE_SOURCE2);  	
		POSITION fpos=dlgFile.GetStartPosition();
		CString data(_T("Error"));		
		data=dlgFile.GetNextPathName(fpos);	

		SETTEXTEX opt;
		opt.flags=ST_KEEPUNDO;
		opt.codepage=1200;
		hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)data.GetString()); 
	}
}

void CRowToColDlg::OnBnClickedBrowseFolderDest()
{
	// TODO: добавьте свой код обработчика уведомлений
	LPITEMIDLIST p;
	BROWSEINFO inf;
	inf.hwndOwner = AfxGetApp()->m_pMainWnd->m_hWnd;
	inf.pidlRoot = NULL;
	inf.lpszTitle = _T("Выберите папку для сохрания ...");
	inf.lpfn = NULL;
	inf.lParam = NULL;
	inf.pszDisplayName = NULL;
	inf.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_NEWDIALOGSTYLE|BIF_STATUSTEXT|BIF_USENEWUI ;
	inf.iImage = NULL;
	InitCommonControls();
	p = SHBrowseForFolder(&inf);

	CWnd* hEditBox = GetDlgItem(IDC_EDIT_FOLDER_DEST);  	
	wchar_t* sz=new wchar_t[MAX_PATH];
	SHGetPathFromIDList(p, sz);	
	SETTEXTEX opt;
	opt.flags=ST_KEEPUNDO;
	opt.codepage=1200;
	hEditBox->SendMessage(WM_SETTEXT, (WPARAM)&opt,  (LPARAM)sz); 
	delete[] sz;
	
}

void CRowToColDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	OnOK();
}
