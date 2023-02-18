 // MyTe.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyTe.h"
#include "MainFrm.h"

#include "MyTeSheet.h"
#include "ChildFrm.h"
#include "MyTeDoc.h"
#include "MyTeView.h"
#include "OptionDialog.h"
#include "MyTeMath.h"
#include "CalculateDlg.h"
#include "RowToColDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTeApp

BEGIN_MESSAGE_MAP(CMyTeApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMyTeApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMyTeApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEDATA, &CMyTeApp::OnSaveData)
	
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)

	ON_COMMAND(ID_ACTION_OPTION, &CMyTeApp::OnActionOption)
	ON_COMMAND(ID_ACTION_OPTION1, &CMyTeApp::OnActionOption)
	ON_COMMAND(ID_ENERGY_DISTR, &CMyTeApp::OnActionEnergyDistr)
	ON_COMMAND(ID_MAGNITUDE, &CMyTeApp::OnActionMagn) 
	ON_COMMAND(ID_ENERGY_DISTR1, &CMyTeApp::OnActionEnergyDistr)
	ON_COMMAND(ID_MAGNITUDE1, &CMyTeApp::OnActionMagn) 
	ON_COMMAND(ID_ACTION_ANY, &CMyTeApp::OnActionAny)
	ON_COMMAND(ID_ACTION_ANY1, &CMyTeApp::OnActionAny) 
	ON_COMMAND(ID_ACTION_NORM5500, &CMyTeApp::OnActionNorm5500)
	ON_COMMAND(ID_ACTION_BANDS, &CMyTeApp::OnActionBands)
	ON_COMMAND(ID_ACTION_AID, &CMyTeApp::OnActionAid)
	ON_COMMAND(ID_ACTION_AID1, &CMyTeApp::OnActionAid)
	ON_COMMAND(ID_CONVERT_M, &CMyTeApp::OnConvert)
	ON_COMMAND(ID_CONVERT_MT, &CMyTeApp::OnConvert)
	ON_COMMAND(ID_ACTION_REMOVE_MINUS_MANY, &CMyTeApp::OnRemoveMinus)
	//ON_COMMAND(ID_ACTION_TEST, &CMyTeApp::OnActionTest)
	ON_COMMAND(ID_Menu32892, &CMyTeApp::OnMenuOH_8)
	ON_COMMAND(ID_ACTION_TESTING, &CMyTeApp::OnActionTesting)
END_MESSAGE_MAP()


// CMyTeApp construction

CMyTeApp::CMyTeApp()
{
	EnableHtmlHelp();
	BandCount=0;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMyTeApp object

CMyTeApp theApp;


// CMyTeApp initialization

BOOL CMyTeApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//Загрузка настроек из реестра
	SetRegistryKey(_T("MOSH"));
	const TCHAR* pszKey = _T("Settings");				
	swscanf_s(GetProfileString(pszKey, _T("VegaClass")),_T("%lf"),&Options.Vega);
	swscanf_s(GetProfileString(pszKey, _T("NumberBand")),_T("%d"),&Options.NumberBand);
	Options.VegaFile=GetProfileString(pszKey, _T("VegaFile"));
	Options.Lambda_Beg=new double[Options.NumberBand];
	Options.Lambda_End=new double[Options.NumberBand];	
	Options.VegaFlux=new double[Options.NumberBand];
	wchar_t buf[5];
	CString subKey;
	for(ULONG32 i=0;i<Options.NumberBand;i++)
	{
		swprintf(buf, 5, L"%d", i);	
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MIN"));
		swscanf_s(GetProfileString(subKey, buf),_T("%lf"),&Options.Lambda_Beg[i]);//добавить массив
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MAX"));		
		swscanf_s(GetProfileString(subKey, buf),_T("%lf"),&Options.Lambda_End[i]);
		subKey=pszKey;
		subKey.Append(_T("\\VegaFlux"));
		swscanf_s(GetProfileString(subKey, buf),_T("%lf"),&Options.VegaFlux[i]);
	}	
	swscanf_s(GetProfileString(pszKey, _T("StepLmkm")),_T("%lf"),&Options.StepLmkm);
	swscanf_s(GetProfileString(pszKey, _T("LmkmBeg")),_T("%lf"),&Options.LmkmBeg);
	swscanf_s(GetProfileString(pszKey, _T("LmkmEnd")),_T("%lf"),&Options.LmkmEnd);
	swscanf_s(GetProfileString(pszKey, _T("GraphBeg")),_T("%lf"),&Options.GraphBeg);
	swscanf_s(GetProfileString(pszKey, _T("GraphEnd")),_T("%lf"),&Options.GraphEnd);
	
	swscanf_s(GetProfileString(pszKey, _T("BandCount")),_T("%d"),&BandCount);	
	BandArray=new CMyTeBand[BandCount];
	for(ULONG32 i=0;i<BandCount;i++)
	{
		CString b;
		subKey=pszKey;
		subKey.Append(_T("\\Band"));
		b.AppendFormat(_T("%d"),i);
		theApp.BandArray[i].LoadFromFile(GetProfileString(subKey, b));
	}
	for(ULONG32 i=0;i<Options.NumberBand;i++)
	{
		swprintf(buf, 5, L"%d", i);	
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MIN"));
		swscanf_s(GetProfileString(subKey, buf),_T("%lf"),&Options.Lambda_Beg[i]);//добавить массив
	}
	Redden.FName=GetProfileString(pszKey, _T("ReddenFile"));
	if(Redden.FName!="")
	{
		Redden.LoadFromFile(Redden.FName);	
		//Redden.ConvertLnToFlux();
		//Redden.NormMax();
	}
	//double stepcount=((Options.LmkmEnd-Options.LmkmBeg)/Options.StepLmkm)+1;
	//Options.OutCount=0;
	FileList.Over=true;
	//Options.Coeff=1.89e-11;
	Options.Coeff=1.0;
	/*
	Options.NumberBand=3;

	Options.Lambda_Beg[0]=2.464;
	Options.Lambda_End[0]=2.566;
	Options.Lambda_Beg[1]=2.7;
	Options.Lambda_End[1]=2.9;
	Options.Lambda_Beg[2]=2.806;
	Options.Lambda_End[2]=2.914;		
	
	Options.VegaFlux[0]=-5.93510;
	Options.VegaFlux[1]=-5.497617;
	Options.VegaFlux[2]=-5.410;
	Options.StepLmkm=0.025;//шаг
	Options.LmkmBeg=2.36;
	Options.LmkmEnd=4.11;
	*/
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MyTeTYPE,
		RUNTIME_CLASS(CMyTeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMyTeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_nWnd=pMainFrame;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if(cmdInfo.m_nShellCommand!=cmdInfo.FileNew)
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMyTeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMyTeApp message handlers


void CMyTeApp::OnActionOption()
{	
	CMyTeSheet Sheet(_T("Настройки"));
	//в диалоги

	Sheet.TempOptions.GraphBeg=theApp.Options.GraphBeg;
	Sheet.TempOptions.GraphEnd=theApp.Options.GraphEnd;
	Sheet.TempOptions.NumberBand=theApp.Options.NumberBand;
	Sheet.TempOptions.StepLmkm=theApp.Options.StepLmkm;
	Sheet.TempOptions.LmkmBeg=theApp.Options.LmkmBeg;
	Sheet.TempOptions.LmkmEnd=theApp.Options.LmkmEnd;
	//Sheet.TempOptions.OutCount=theApp.Options.OutCount;		
	Sheet.TempOptions.Lambda_Beg=new double[Sheet.TempOptions.NumberBand];
	Sheet.TempOptions.Lambda_End=new double[Sheet.TempOptions.NumberBand];	
	Sheet.TempOptions.VegaFlux=new double[Sheet.TempOptions.NumberBand];	
	for(UINT i=0;i<Sheet.TempOptions.NumberBand;i++)
	{
		Sheet.TempOptions.Lambda_Beg[i]=theApp.Options.Lambda_Beg[i];
		Sheet.TempOptions.Lambda_End[i]=theApp.Options.Lambda_End[i];
		Sheet.TempOptions.VegaFlux[i]=theApp.Options.VegaFlux[i];
	}		
	
	if(Sheet.DoModal()==IDOK)
	{	
		//из диалогов	
		if(theApp.Options.NumberBand!=0)
		{
			delete[] theApp.Options.Lambda_Beg;
			delete[] theApp.Options.Lambda_End;
			delete[] theApp.Options.VegaFlux;
		}
		theApp.Options.GraphBeg=Sheet.TempOptions.GraphBeg;
		theApp.Options.GraphEnd=Sheet.TempOptions.GraphEnd;
		theApp.Options.NumberBand=Sheet.TempOptions.NumberBand;
		theApp.Options.StepLmkm=Sheet.TempOptions.StepLmkm;
		theApp.Options.LmkmBeg=Sheet.TempOptions.LmkmBeg;
		theApp.Options.LmkmEnd=Sheet.TempOptions.LmkmEnd;
		//theApp.Options.OutCount=Sheet.TempOptions.OutCount;		
		theApp.Options.Lambda_Beg=new double[theApp.Options.NumberBand];
		theApp.Options.Lambda_End=new double[theApp.Options.NumberBand];	
		theApp.Options.VegaFlux=new double[theApp.Options.NumberBand];	
		for(UINT i=0;i<theApp.Options.NumberBand;i++)
		{

			theApp.Options.Lambda_Beg[i]=Sheet.TempOptions.Lambda_Beg[i];
			theApp.Options.Lambda_End[i]=Sheet.TempOptions.Lambda_End[i];
			theApp.Options.VegaFlux[i]=Sheet.TempOptions.VegaFlux[i];
		}		
	}


	
	UINT count = GetOpenDocumentCount();
	if(count==0) return;
	POSITION pos=GetFirstDocTemplatePosition();
	CMultiDocTemplate* dtempl=(CMultiDocTemplate*)GetNextDocTemplate(pos);
	POSITION pos1=dtempl->GetFirstDocPosition();
	for(int i=0;i<count;i++)
	{	
		CDocument* doc=dtempl->GetNextDoc(pos1);
		dtempl->InitialUpdateFrame(dtempl->GetRoutingFrame_(),doc);
	}	


}


void CMyTeApp::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class
	

	CWinApp::OnFinalRelease();
}

int CMyTeApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//Сохранение настроек в реестр
	const TCHAR* pszKey = _T("Settings"); 		
	wchar_t buf[100];
	// Write the information to the registry.
	swprintf( buf, 100, L"%10.16f", Options.Vega);		
	WriteProfileString(pszKey, _T("VegaClass"), buf);
	swprintf( buf, 100, L"%d", Options.NumberBand);		
	WriteProfileString(pszKey, _T("NumberBand"), buf);
	wchar_t buf1[5];
	CString subKey;
	for(ULONG32 i=0;i<Options.NumberBand;i++)
	{
		swprintf(buf1, 5, L"%d", i);
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MIN"));
		swprintf( buf, 100, L"%10.16f", Options.Lambda_Beg[i]);		
		WriteProfileString(subKey,buf1, buf);
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MAX"));
		swprintf( buf, 100, L"%10.16f", Options.Lambda_End[i]);		
		WriteProfileString(subKey,buf1, buf);	
		subKey=pszKey;
		subKey.Append(_T("\\VegaFlux"));
		swprintf( buf, 100, L"%10.16f", Options.VegaFlux[i]);		
		WriteProfileString(subKey,buf1, buf);	
	}
		
	CString b;
	b.AppendFormat(_T("%d"),BandCount);
	WriteProfileString(pszKey, _T("BandCount"), b);

	for(ULONG32 i=0;i<BandCount;i++)
	{
		//swprintf(buf1, 5, L"%d", i);
		CString KeyName;
		KeyName.AppendFormat(_T("%d"),i);
		subKey=pszKey;
		subKey.Append(_T("\\Band"));
		//swprintf( buf, 100, L"%10.16f", BandArray[i].fname);
		WriteProfileString(subKey,KeyName, BandArray[i].FName.GetString());	
	}
	WriteProfileString(pszKey, _T("VegaFile"), Options.VegaFile);	
	WriteProfileString(pszKey, _T("ReddenFile"), Redden.FName);	
	swprintf( buf, 100, L"%10.16f", Options.StepLmkm);		
	WriteProfileString(pszKey, _T("StepLmkm"), buf);
	swprintf( buf, 100, L"%10.16f", Options.LmkmBeg);
	WriteProfileString(pszKey, _T("LmkmBeg"), buf);
	swprintf( buf, 100, L"%10.16f", Options.LmkmEnd);		
	WriteProfileString(pszKey, _T("LmkmEnd"), buf);
	swprintf( buf, 100, L"%10.16f", Options.GraphBeg);		
	WriteProfileString(pszKey, _T("GraphBeg"), buf);
	swprintf( buf, 100, L"%10.16f", Options.GraphEnd);		
	WriteProfileString(pszKey, _T("GraphEnd"), buf);
	return CWinApp::ExitInstance();
}

//void CMyTeApp::OnActionTest()
//{
//	CMyTeMath::ProcessCatalog(_T("D:\\Mosh_Kom\\Catalog 2MASS\\catalog.cat"),
	//CMyTeMath::ProcessCatalog(_T("D:\\Mosh_Kom\\Комета\\TestCat.txt"),
		//_T("D:\\Mosh_Kom\\Комета\\cat.cat"),_T("D:\\Mosh_Kom\\Комета\\Mo_am_J-K.txt"),
		//_T("D:\\Mosh_Kom\\Комета\\cat.cat"),_T("D:\\Mosh_Kom\\Комета\\8-12_J-K.txt"),
//								Options,BandArray[0]);
//}



void CMyTeApp::OnActionEnergyDistr()
{
	CCalculateDlg calcdlg;
	calcdlg.NeedFolder=true;
	if(calcdlg.DoModal(true)==IDOK)	
	{
		CString OutfName;
		bool b=FileList.Over;
		for(ULONG32 i=0;i<FileList.Count;i++)
		{	
			m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
			CMyTeDoc* Doc;
			Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
			ULONG32 pos=0;	
			OutfName=FileList.Files[i];		
			for(int j=OutfName.GetLength()-1;j>=0;j--)
			{
				if(OutfName.GetBuffer()[j]=='\\') 
				{
					pos=j;				
					break;
				}
			}
			CString fileName=OutfName;
			for(int k=pos;k<OutfName.GetLength();k++)
			{
				fileName.GetBuffer()[k-pos]=OutfName.GetBuffer()[k];
			}
			fileName.Truncate(fileName.GetLength()-pos);
			CString ResultFName=FileList.OutFile;
			//ResultFName.Append(_T("\\"));
			ResultFName.Append(fileName);
//			CMyTeMath::Energy_Distribution1_1(&Doc->Data,Options);
			CMyTeMath::Energy_Distribution1_2(&Doc->Data,Options);
			CMyTeMath::SaveChangedFile(ResultFName,&Doc->Data,Options,b);

			//if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
			//this->PumpMessage();
		}	
		m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
	}
}
void CMyTeApp::OnActionMagn()
{
	CCalculateDlg calcdlg;
	calcdlg.DoModal();	
	
	bool b=FileList.Over;
	for(ULONG32 i=0;i<FileList.Count;i++)
	{	
		m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
		CMyTeDoc* Doc;
		Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
		CMyTeMath::MagnK(&Doc->Data,Options);
		CMyTeMath::WriteResult(FileList.OutFile,FileList.Files[i],&Doc->Data,b);
		b=false;
		//Doc->Data.SummErrFlux;
		//Doc->Data.SummFlux;
		if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
		this->PumpMessage();
	}	
	m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
}
void CMyTeApp::OnActionAny()
{
	//COMMENT Мошкалев к ISO спектрам добавил строку 2.36000==2.36050

	CCalculateDlg calcdlg;
	calcdlg.DoModal();	

	bool b=FileList.Over;
	for(ULONG32 i=0;i<FileList.Count;i++)
	{	
		m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
		CMyTeDoc* Doc;
		Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);

		//Добавить работу с документами сюда!!!!

		if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
	}	
	m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
}

void CMyTeApp::OnSaveData()
{
	//CFileDialog dlgFile(false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, NULL, 0);
	CFileDialog dlgFile(true);
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

	int count= theApp.GetOpenDocumentCount();
	if(count>0)
	{
		POSITION pos=theApp.GetFirstDocTemplatePosition();
		CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
		POSITION pos1=dtempl->GetFirstDocPosition();
		for(int i=0;i<count;i++)
		{	
			//CDocument* doc=dtempl->GetNextDoc(pos1);
			CMyTeDoc* doc=(CMyTeDoc*)dtempl->GetNextDoc(pos1);
			if(doc->IsFocus) 
			{
				POSITION fpos=dlgFile.GetStartPosition();	
				CString data;				
				data=dlgFile.GetNextPathName(fpos);				
				//data.CharToOemA				
				CMyTeMath::SaveChangedFile(data,&doc->Data,Options,true);
			}
		}
	}
}
void CMyTeApp::OnFileOpen(void)
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
		CString data;
		int i=0;
		do
		{		
			data=dlgFile.GetNextPathName(fpos);
			OpenDocumentFile(data);
			i++;
		} while (fpos!=NULL);
		
	}
	fileName.ReleaseBuffer();
}

void CMyTeApp::OnActionNorm5500()
{
	CCalculateDlg calcdlg;
	calcdlg.DoModal(true);	
	CString OutfName;
	bool b=FileList.Over;
	for(ULONG32 i=0;i<FileList.Count;i++)
	{	
		m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
		CMyTeDoc* Doc;
		Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
		ULONG32 pos=0;	
		OutfName=FileList.Files[i];		
		for(int j=OutfName.GetLength()-1;j>=0;j--)
		{
			if(OutfName.GetBuffer()[j]=='\\') 
			{
				pos=j;				
				break;
			}
		}
		CString fileName=OutfName;
		for(int k=pos;k<OutfName.GetLength();k++)
		{
			fileName.GetBuffer()[k-pos]=OutfName.GetBuffer()[k];
		}
		fileName.Truncate(fileName.GetLength()-pos);
		CString ResultFName=FileList.OutFile;
		//ResultFName.Append(_T("\\"));
		ResultFName.Append(fileName);
		CMyTeMath::Norm5500(&Doc->Data,Options);
		CMyTeMath::SaveChangedFile(ResultFName,&Doc->Data,Options,b);

		if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
		this->PumpMessage();
	}	
	m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
}
void CMyTeApp::OnActionAid()
{
	CCalculateDlg calcdlg;
	if(calcdlg.DoModal()==IDOK)
	{

		double* VegaArray=new double[BandCount];
		for(int i=0;i<BandCount;i++) VegaArray[i]=0.0;
		CString RString;



		RString.AppendFormat(_T("%-10s"),_T("File Name"));
		for(int i=0;i<BandCount;i++)
		{
			CString outfname(BandArray[i].FName);
			outfname.Truncate(outfname.GetLength()-4);
			int pos=outfname.ReverseFind('\\');		
			RString.AppendFormat(_T(" %10s "),outfname.Right(outfname.GetLength()-pos-1));	
			RString.AppendFormat(_T(" %10s "),_T("Red x1"));
			RString.AppendFormat(_T(" %10s "),_T("Red x2"));
		}	
		RString.Append(_T("\n"));
		CMyTeMath::WriteResult1(FileList.OutFile,RString,true);

		bool b=FileList.Over;
		for(ULONG32 i=0;i<FileList.Count;i++)
		{	
			m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
			CMyTeDoc* Doc;
			Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
			CString outfname(FileList.Files[i]);

			int pos=outfname.ReverseFind('.');
			outfname.Truncate(pos);
			pos=outfname.ReverseFind('\\');
			CString ResultString;
			ResultString.AppendFormat(_T("%-10s"),outfname.Right(outfname.GetLength()-pos-1));	
			/*for(ULONG32 i=0;i<Doc->Data.Count;i++)
			{
			Doc->Data.Lambda[i]*=10000.0;
			}*/
			for(int j=0;j<BandCount-1;j++)
			{
				//ResultString.AppendFormat(_T(" %10.5lf "),
					//CMyTeMath::SIRIUS(&Doc->Data,Options,&BandArray[j],&BandArray[BandCount-1],1.5));
					//CMyTeMath::Aid(&Doc->Data,Options,&BandArray[j],&BandArray[BandCount-1],1.5));
				/*	CMyTeMath::SBand(&Doc->Data,Options,&BandArray[j],VegaArray[j],NULL,0));
				ResultString.AppendFormat(_T(" %10.5lf "),
				CMyTeMath::SBand(&Doc->Data,Options,&BandArray[j],VegaArray[j],&Redden,1));
				ResultString.AppendFormat(_T(" %10.5lf "),
				CMyTeMath::SBand(&Doc->Data,Options,&BandArray[j],VegaArray[j],&Redden,2));*/
			}
			ResultString.Append(_T("\n"));
			CMyTeMath::WriteResult1(FileList.OutFile,ResultString,false);
			b=false;
			//Doc->Data.SummErrFlux;
			//Doc->Data.SummFlux;
			if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
			this->PumpMessage();
		}	
		m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);

		//int count= theApp.GetOpenDocumentCount();
		//if(count>0)
		//{
		//	POSITION pos=theApp.GetFirstDocTemplatePosition();
		//	CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
		//	POSITION pos1=dtempl->GetFirstDocPosition();
		//	for(int i=0;i<count;i++)
		//	{	
		//		//CDocument* doc=dtempl->GetNextDoc(pos1);
		//		CMyTeDoc* doc=(CMyTeDoc*)dtempl->GetNextDoc(pos1);
		//		if(doc->IsFocus) 
		//		{
		//			CMyTeMath::Aid(&doc->Data,Options,&BandArray[0],Redden,1.5);
		//		}
		//	}
		//}

	}
}
void CMyTeApp::OnActionBands()
{
	CCalculateDlg calcdlg;
	if(calcdlg.DoModal()==IDOK)
	{
		double* VegaArray=new double[BandCount];
		for(int i=0;i<BandCount;i++) VegaArray[i]=0.0;
		CString RString;

		if(FileList.VegaFile!="")
		{
			CMyTeDoc* VDoc;
			//подсчет Bеги
			VDoc=(CMyTeDoc*)OpenDocumentFile(FileList.VegaFile);
			for(int i=0;i<BandCount;i++)
			{
				VegaArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,NULL,0);
				//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
			}
			int cou=GetOpenDocumentCount();
			if(cou>0) VDoc->OnCloseDocument();
		}	

		RString.AppendFormat(_T("%-12s"),_T("File_Name"));
		RString.AppendFormat(_T("%11s "),_T("       RedX"));
		for(int i=0;i<BandCount;i++)
		{
			CString outfname(BandArray[i].FName);
			outfname.Truncate(outfname.GetLength()-4);
			int pos=outfname.ReverseFind('\\');		
			RString.AppendFormat(_T(" %10s "),outfname.Right(outfname.GetLength()-pos-1));	
			//RString.AppendFormat(_T(" %10s "),_T("Red x2"));
		}	
		RString.Append(_T("\n"));
			

		if(CMyTeMath::WriteResult1(FileList.OutFile,RString,true)==0)		
		{
			bool b=FileList.Over;	
			for(ULONG32 i=0;i<FileList.Count;i++)
			{	
				m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
				CMyTeDoc* Doc;
				Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
				CString outfname(FileList.Files[i]);

				int pos=outfname.ReverseFind('.');
				outfname.Truncate(pos);
				pos=outfname.ReverseFind('\\');
				CString ResultString=NULL;
				CString ResultStringZip=NULL;
				ResultString.AppendFormat(_T("%-10s"),outfname.Right(outfname.GetLength()-pos-1));	
				ResultStringZip=ResultString;
				UINT Nred=10;//0;//5;//5;	//number of random i.s.ext. amount (thickness, depth) 
				double X=0.0;
				//ResultString.AppendFormat(_T(" %10.3lf "),Rand);
				/*for(int j=0;j<BandCount;j++)
				{*/
				//		ResultString.AppendFormat(_T(" %10.4e "),//lf ")
				//			CMyTeMath::SBand_2(&Doc->Data,&BandArray[j],VegaArray[j],NULL,0));
				//	}
				//ResultString.Append(_T("\n"));
				//CMyTeMath::WriteResult1(FileList.OutFile,ResultString,false);   

				for(UINT jj=0;jj<Nred;jj++){
					if (jj==0) X=0.0;
					else X=CMyTeMath::RANDisex();

					ResultString.Empty();//.Append(_T("\n"));
					ResultString=ResultStringZip;
					ResultString.AppendFormat(_T(" %10.3lf "),X);
					for(UINT j=0;j<BandCount;j++)
					{			 
						ResultString.AppendFormat(_T(" %10.4lf "),//e ")
							CMyTeMath::SBand_2(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X));
					}
					ResultString.Append(_T("\n"));
					if(CMyTeMath::WriteResult1(FileList.OutFile,ResultString,false)!=0) break;
				}
				//ResultString.Append(_T("\n"));
				//CMyTeMath::WriteResult1(FileList.OutFile,ResultString,false);
				b=false;
				//Doc->Data.SummErrFlux;
				//Doc->Data.SummFlux;
				int cou=GetOpenDocumentCount();
				if(cou>0) Doc->OnCloseDocument();
				//fcloseall();
				this->PumpMessage();
			}	
			m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
		}else
		{
			m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Ошибка записи в файл."), TRUE);
		}
		
	}
}
//position-смещение в файле, откуда начинается загрузка
bool CMyTeApp::LoadRowFile(CString fname,double* &DataBuffer,ULONG& count,ULONG& position,CString* Filename)
{	
	BYTE*	szBuffer; 
	ULONG32    nActual = 0; 		
	ULONG32 colcount=0; 
	CFile* myFile;
	myFile=new CFile;
	CFileException ex;

	if (!myFile->Open(fname,CFile::modeRead,&ex ))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
		return false;
	}else
	{	
		//загрузка файла lambda
		ULONG filesize=myFile->GetLength();
		if (position>=filesize)
		{
			return false;
		}
		szBuffer= new BYTE [filesize];
		nActual = myFile->Read( szBuffer, filesize ); 
		myFile->Close();
		if(nActual<filesize) filesize=nActual;	
		
		ULONG i=position;
		ULONG beg;
		ULONG end;
		ULONG BegSting=0;
		colcount=0;
		//подсчет столбцов
		wchar_t fname[100];
		for (UINT j=0;j<100;j++)
		{
			fname[j]=0x0000;
		}
		ULONG f=0;
		ULONG j=i;
		if (szBuffer[j]=='#')
		{
			j++;
			while(szBuffer[j]==0x20)
			{j++;}
			while((szBuffer[j]!=0x20)&&(szBuffer[j]!=0x0A)&&(szBuffer[j]!=0x0D))
			{
				fname[f]=szBuffer[j];
				f++;
				j++;
			}
			fname[f]=0x00;
			Filename->Empty();
			Filename->SetString(fname);
		}
		while ((i<filesize)&&(szBuffer[i]!=0x0D)&&(szBuffer[i]!=0x0A))
		{
			double test=CMyTeMath::MyTe_scanf(szBuffer,filesize,i,beg,end);
			colcount++;
		}	
		DataBuffer=new double[colcount];
		ULONG pos=position;
		for (ULONG i=0;i<colcount;i++)
		{
			DataBuffer[i]=CMyTeMath::MyTe_scanf(szBuffer,filesize,pos,beg,end);
		}
		position=pos;
		delete[] szBuffer;	
		count=colcount;
		return true;
	}
};
void CMyTeApp::OnConvert()
{
	CRowToColDlg Dlg;
	Dlg.SourceFileLambda=_T("D:\\MOSH\\STAR TRACK\\МАЛЫЙ ДАТЧИК\\uvk_pic.grd");//uvk_pic_spc.txtMosh_Kom\\Электро\\ЭЛЕКТРО_New\\Wizard\\program\\uvk_picls.grd";
	Dlg.SourceFileFlux=_T("D:\\MOSH\\STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt");//Mosh_Kom\\Электро\\ЭЛЕКТРО_New\\Wizard\\program\\uvk_picls.spc");
	Dlg.DestFolder=_T("D:\\");
	if(Dlg.DoModal()==IDOK)
	{

		ULONG32 strcount=0;
		ULONG pos=0;
		ULONG count=0;
		double* Lambda=NULL;
		CString Filename;
		if(LoadRowFile(Dlg.SourceFileLambda,Lambda,count,pos,&Filename))
		{

		}

		pos=0;
		double* Flux=NULL;
		DocDataType Data;
		Data.Lambda=Lambda;
		Data.Count=count;
		Filename.Empty();
		while(LoadRowFile(Dlg.SourceFileFlux,Flux,count,pos,&Filename))
		{		
			Data.Flux=Flux;		
			CString destfname;
			destfname+=Dlg.DestFolder;
			destfname+=Filename;
			//destfname.AppendChar('\\');		
			CMyTeMath::SaveFile(destfname,&Data,Options,true);		
			delete[] Flux;
			Flux=NULL;
			pos+=2;
			Filename.Empty();
		}
		delete[] Lambda;
		//
	}
}

void CMyTeApp::OnRemoveMinus()
{
	CCalculateDlg calcdlg;
	calcdlg.NeedFolder=true;
	if (calcdlg.DoModal()==IDOK)
	{
		bool b=FileList.Over;
		for(ULONG32 i=0;i<FileList.Count;i++)
		{	
			m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
			CMyTeDoc* Doc;
			Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);
			
			Doc->OnMinusRemove();

			CString outfname(FileList.Files[i]);
			int pos=outfname.ReverseFind('.');
			outfname.Truncate(pos);
			pos=outfname.ReverseFind('\\');
			CString ResultString=NULL;
			CString ResultStringZip(FileList.OutFile);
			ResultStringZip.AppendChar(_T('\\'));
			ResultString.AppendFormat(_T("%-10s"),outfname.Right(outfname.GetLength()-pos-1));	
			ResultStringZip.Append(ResultString);
			ResultStringZip.Append(_T(".txt"));

			
			CMyTeMath::SaveFile(ResultStringZip,&Doc->Data,theApp.Options,true);
			int cou=GetOpenDocumentCount();
			if(cou>0) Doc->OnCloseDocument();
			//fcloseall();
			this->PumpMessage();
		}
	}
}
void CMyTeApp::OnMenuOH_8()
{
	CMyTeMath::PROBKA(BandArray);//CMyTeBand* Band
	// TODO: добавьте свой код обработчика команд
}

void CMyTeApp::OnActionTesting()
{
	CMyTeMath::TESTING();
	// TODO: добавьте свой код обработчика команд
}
