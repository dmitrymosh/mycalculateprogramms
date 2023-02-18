// FindStr.cpp: файл реализации
//

#include "stdafx.h"
#include "find.h"
#include "FindStr.h"


// диалоговое окно CFindStr

IMPLEMENT_DYNAMIC(CFindStr, CDialog)

CFindStr::CFindStr(CWnd* pParent /*=NULL*/)
	: CDialog(CFindStr::IDD, pParent)
	, v_ReferenceFile(_T(""))
	, v_SourceFile(_T(""))
	, v_DestFile(_T(""))
{

}

CFindStr::~CFindStr()
{
}

void CFindStr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ReferenceFile);
	DDX_Text(pDX, IDC_EDIT1, v_ReferenceFile);
	DDX_Control(pDX, IDC_EDIT2, c_SourceFile);
	DDX_Text(pDX, IDC_EDIT2, v_SourceFile);
	DDX_Control(pDX, IDC_EDIT3, c_DestFile);
	DDX_Text(pDX, IDC_EDIT3, v_DestFile);
}


BEGIN_MESSAGE_MAP(CFindStr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFindStr::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFindStr::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFindStr::OnBnClickedButton3)
END_MESSAGE_MAP()


// обработчики сообщений CFindStr

void CFindStr::OnBnClickedButton1()
{
	CFileDialog dlgFile(true);
	CString allFilter;
	CString strFilter;
	CString strDefault;	
	CString fileName;

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
	
	POSITION fpos=dlgFile.GetStartPosition();	
	CString data;				
	ReferenceFile.SetWindowText(dlgFile.GetNextPathName(fpos));	
	
}

void CFindStr::OnBnClickedButton2()
{
	CFileDialog dlgFile(true);
	CString allFilter;
	CString strFilter;
	CString strDefault;	
	CString fileName;

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

	POSITION fpos=dlgFile.GetStartPosition();	
	CString data;				
	c_SourceFile.SetWindowText(dlgFile.GetNextPathName(fpos));	
}

void CFindStr::OnBnClickedButton3()
{
	CFileDialog dlgFile(true);
	CString allFilter;
	CString strFilter;
	CString strDefault;	
	CString fileName;

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

	POSITION fpos=dlgFile.GetStartPosition();	
	CString data;				
	c_DestFile.SetWindowText(dlgFile.GetNextPathName(fpos));	
}
