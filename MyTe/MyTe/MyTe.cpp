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
#define MAX(a,b) a>=b?a:b
#define MIN(a,b) a<=b?a:b


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
	//ON_COMMAND(ID_ACTION_TEST, &CMyTeApp::OnActionTest)
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
	theApp.BandArray.resize(theApp.BandCount);
	for(ULONG32 i=0;i<BandCount;i++)
	{
		CString b;
		subKey=pszKey;
		subKey.Append(_T("\\Band"));
		b.AppendFormat(_T("%d"),i);
		theApp.BandArray.at(i).LoadFromFile(GetProfileString(subKey, b).GetBuffer());
	}
	for(ULONG32 i=0;i<Options.NumberBand;i++)
	{
		swprintf(buf, 5, L"%d", i);	
		subKey=pszKey;
		subKey.Append(_T("\\Lambda_MIN"));
		swscanf_s(GetProfileString(subKey, buf),_T("%lf"),&Options.Lambda_Beg[i]);//добавить массив
	}
	Redden.FName=GetProfileString(pszKey, _T("ReddenFile"));
	//Redden.FName=_T("D:\\MOSH\\SOURCE\\STELLAR EXTINCTION\\STELLAR_EXTINCTIONnm.txt");//StEXT_50nm.txt");//D:\\mecalculateprogramm\\MyTe\\EXTINCT_nm.txt");
	if(Redden.FName.size())
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


};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}



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
		WriteProfileString(subKey,KeyName, BandArray[i].FilePath.c_str());	
	}
	WriteProfileString(pszKey, _T("VegaFile"), Options.VegaFile);	
	WriteProfileString(pszKey, _T("ReddenFile"), Redden.FilePath.c_str());
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
		for(ULONG32 i=0;i<FileList.Files.size();i++)
		{	
			m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
			CMyTeDoc* Doc;
			Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
			ULONG32 pos=0;	
			OutfName=FileList.Files[i].c_str();
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
			CString ResultFName=FileList.OutFile.c_str();
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
	for(ULONG32 i=0;i<FileList.Files.size();i++)
	{	
		m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
		CMyTeDoc* Doc;
		Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
		CMyTeMath::MagnK(&Doc->Data,Options);
		CMyTeMath::WriteResult(FileList.OutFile.c_str(),FileList.Files[i].c_str(),&Doc->Data,b);
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
FILE *inpf,*outf,*errf;
#define len 16384
char InLin[16384];
char *ptr[16384];
	//COMMENT Мошкалев к ISO спектрам добавил строку 2.36000==2.36050
			char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\pp_GAIA.txt";//BC_7m_BASE.txt";//BC_7m_BASE_AAA_1_Ss.txt";//BC_7m_BASE_4.txt";////
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\BC_7m_BASE_8.txt";//J_Vt_AAA.txt";//BtVtJHK_WBVR
		//char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\JHG.txt";//J_Vt_AAA.txt";//BtVtJHK_WBVR
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));                                                                   
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "w") !=0 ){
	//	AfxMessageBox(_T("ERR NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	CString ResultString=NULL;
	//CString InLin=NULL;
	CMyTeBand AZDK;
	AZDK.FName=_T("D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\AZDK_GAIA_S.txt");//H_K_GAIA_.txt");//J_H_Vt_J_.txt");//AZDK_GAIA_S.txt");
	AZDK.LoadFromFile(AZDK.FName);
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OUT FALSE"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	 	// for (UINT i=0; i<AZDK.Count-1; i++){
			// fprintf(errf,"%7.4lf|%7.4lf|\n",AZDK.Lambda[i],AZDK.Flux[i]);
			// //fputs("\n",errf);
		 //}
		 //fclose(errf);
		//fputs("\n",errf);
	 	// for (UINT i=0; i<AZDK.Count; i--){
			// fprintf(errf,"%7.4lf|%7.4lf|",AZDK.Lambda,AZDK.Flux);
 		//	 fputs("\n",outf);
		 //}


	 int Count_str=CMyTeMath::CountDataFile(inpf);
		double GB_GR=0; 
		//double AZDK_GI; 
		//double GI; 
		int pp; 
	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
 
				CMyTeMath::StrPtr(InLin,'|',ptr);
//				int pp=strncmp(ptr[6],"      ",5);
//				int a=strncmp(ptr[91],"M",1);//GI
				int c=strncmp(ptr[0],"M",1);//Vt_J//GB_GR
				int data=strncmp(ptr[1],"M",1);//pp
				//int BVbs=strncmp(ptr[32],"      ",6);
				//int UBbs=strncmp(ptr[33],"      ",6);
					if(c==0 || data==0){
						fprintf(outf,"| | |");
						//fprintf(outf,"%s |",InLin);
						fputs("\n",outf);
						continue;
					}else{
						sscanf(ptr[0],"%lf",&GB_GR);
						//sscanf(ptr[119],"%lf",&AZDK_GI);
						//sscanf(ptr[91],"%lf",&GI);
						sscanf(ptr[1],"%d",&pp);
						if(pp==22201){
							pp=pp;
						}
						if(GB_GR>=-0.5 && GB_GR<3.5){
							double AZDK_=AZDK.GetFlux(GB_GR);//AZDK->Lambda[i]);
							//AZDK_+=GI;
					//if(GB_GR!=0 && AZDK_GI!=0 && ){
							fprintf(outf,"%5d|%7.4lf|",pp,AZDK_);
							fputs("\n",outf);

						}
					else {
						fprintf(outf,"| | |");
						//fprintf(outf,"%s |",InLin);
						fputs("\n",outf);
						continue;
					}
				}
			}
		}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			// YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
 		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);

	fcloseall();






	//CCalculateDlg calcdlg;
	//calcdlg.DoModal();	

	//bool b=FileList.Over;
	//for(ULONG32 i=0;i<FileList.Count;i++)
	//{	
	//	m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i], TRUE);
	//	CMyTeDoc* Doc;
	//	Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i]);

	//	//Добавить работу с документами сюда!!!!

	//	if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
	//}	
	//m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
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
	for(ULONG32 i=0;i<FileList.Files.size();i++)
	{	
		m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
		CMyTeDoc* Doc;
		Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
		ULONG32 pos=0;	
		OutfName=FileList.Files[i].c_str();
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
		CString ResultFName=FileList.OutFile.c_str();
		//ResultFName.Append(_T("\\"));
		ResultFName.Append(fileName);
		CMyTeMath::Norm5500(&Doc->Data,Options);
		CMyTeMath::SaveChangedFile(ResultFName,&Doc->Data,Options,b);

		if(GetOpenDocumentCount()>0) Doc->OnCloseDocument();
		this->PumpMessage();
	}	
	m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
}
#ifdef QQUY
void CMyTeApp::OnActionAid()
{
	CCalculateDlg calcdlg;
	if(calcdlg.DoModal()==IDOK)	
	{
		double* VegaArray=new double[BandCount];
		double* VegaXArray=new double[BandCount];
		for(int i=0;i<BandCount;i++) VegaArray[i]=0.0;
		for(int i=0;i<BandCount;i++) VegaXArray[i]=0.0;
		CString RString;
		CMyTeDoc* VDoc;
		//CString ResultString=NULL;
		//CString ResultStringZip=NULL;

		if(FileList.VegaFile!="")
		{
			//CMyTeDoc* VDoc;
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
			//for(int i=0;i<BandCount;i++)
			//{
			//	//VegaXArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,&Redden,X);
			//	ResultString.AppendFormat(_T(" %10.4lf "),VegaArray[i]);
			//	//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
			//}
			//ResultString.Append(_T("\n"));

		RString.AppendFormat(_T("%-12s"),_T("File_Name"));
		RString.AppendFormat(_T("%7s "),_T("   RedX"));
		RString.AppendFormat(_T("%11s "),_T("       Mz"));
		for(int i=0;i<BandCount;i++)
		{
			CString outfname(BandArray[i].FName);
			outfname.Truncate(outfname.GetLength()-4);
			int pos=outfname.ReverseFind('\\');		
			RString.AppendFormat(_T(" %10s "),outfname.Right(outfname.GetLength()-pos-1));	
//			RString.AppendFormat(_T(" %10s "),_T("Ai"));
		}	
		RString.AppendFormat(_T("%s "),_T("          AB          AR          AV          AW          LB          LR          LV           LW"));
		RString.Append(_T("\n"));
		
	double RX=0;
	bool FlagYes=false;
	double X=0.0;
	double Mz=1.0;
	CMyTeBand EXTIN;
	EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\ОБРАБОТКА\\EXTATM\\200186\\281185.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\MAR_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\JULE_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\DEC_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\SEP_S.txt");
	EXTIN.LoadFromFile(EXTIN.FName);

	int Nred=6;//0;//0;//0;//5;//5;	//number of   i.s.ext. amount (thickness, depth) 
	//CString ResultString=NULL;
	//CString ResultStringZip=NULL;

		if(CMyTeMath::WriteResult1(FileList.OutFile,RString,true)==0)		
		{

			bool b=FileList.Over;	
			for(ULONG32 i=0;i<FileList.Count;i++)//FileList.Count
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
				CString midName=outfname.Right(outfname.GetLength()-pos-1);//-pos-1
				for(int jj=0;jj<Nred;jj++){
					if (jj==0) X=0.0;
					else {

						X=CMyTeMath::RANDisex()*4.0;///**RedMax[i]RANDisex()*///X+=0.1;1;//
					//	//X*=RedMax[i];//2
					}
					ResultString.Empty();//.Append(_T("\n"));
					ResultString=ResultStringZip;
					ResultString.AppendFormat(_T(" %10.3lf "),X);
					//ResultStringZip=ResultString;
					//if(FileList.VegaFile!="")
					//{
						//CMyTeDoc* VDoc;
						//подсчет Bеги
						//VDoc=(CMyTeDoc*)OpenDocumentFile(FileList.VegaFile);
						//for(int i=0;i<BandCount;i++)
						//{
						//	VegaXArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,&Redden,X);
						//	//ResultString.AppendFormat(_T(" %10.4lf "),VegaXArray[i]);
						//	//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
						//}

						//}	


						for(UINT ij=0;ij<6;ij++){//Mz
							ResultString.Empty();//.Append(_T("\n"));
							ResultString=ResultStringZip;
							ResultString.AppendFormat(_T(" %10.3lf "),X);
							//ResultString=ResultStringZip;
							//if(ij==0) Mz=0.0;
							//	else {
									if(ij==0) Mz=0.0;
								else {
									Mz=1.0+CMyTeMath::RANDisex()*2.2;///**RedMax[i]RANDisex()*///X+=0.1;1;//
								} 
							//} 
							ResultString.AppendFormat(_T(" %10.4lf "),Mz);
							//for(UINT j=0;j<BandCount;j++){//BandCount
							//	ResultString.AppendFormat(_T(" %10.4lf "),VegaArray[j]);
							//}
							//	ResultString.Append(_T("\n"));
							for(UINT j=0;j<BandCount;j++){//BandCount
								ResultString.AppendFormat(_T(" %10.4lf "),
									CMyTeMath::SBand_4(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN));
							}
							for(UINT j=0;j<BandCount;j++){//BandCount
							ResultString.AppendFormat(_T(" %10.4lf "),
								CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN));//-VegaXArray[j]);
								//CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN)-VegaXArray[j]);
							}
							for(UINT j=0;j<BandCount;j++){//BandCount
							ResultString.AppendFormat(_T(" %10.4lf "),
								1.086*(CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN)-
								 CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,0.0,&EXTIN)));//-VegaXArray[j]);
							}
							//for(UINT j=0;j<BandCount;j++){//BandCount
							//ResultString.AppendFormat(_T(" %10.4lf "),
							//	CMyTeMath::SBand_L2(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN)-CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,0.0,&EXTIN));//-VegaXArray[j]);
							//	//CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN)-VegaXArray[j]);
							//}
								ResultString.Append(_T("\n"));
							if(CMyTeMath::WriteResult1(FileList.OutFile,ResultString,false)!=0) break;					
						}
					}
		
						b=false;
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
#endif
void CMyTeApp::OnActionAid()// много прозрачностей
{
    CCalculateDlg calcdlg;
    if (calcdlg.DoModal() == IDOK)
    {
	vector <double> VegaArray;
	vector <double> VegaXArray;
	vector <CMyTeBand> AdvFilters;

	CString HeadStr;

	if (FileList.VegaFile.size() > 0) {
	    DocDataType Vega;
	    Vega.LoadFromFile(FileList.VegaFile);
	    //подсчет Bеги
	    for (int i = 0; i < BandCount; i++) {
		const double res = CMyTeMath::SBand_2(&Vega, &BandArray[i], 0, NULL, 0);
		VegaArray.push_back(res);
	    }
	}
	if (FileList.AdvFilterFile.size() > 0) {
	    AdvFilters.resize(FileList.AdvFilterFile.size());
	    for (size_t i = 0; i < FileList.AdvFilterFile.size(); i++) {
		AdvFilters.at(i).LoadFromFile(FileList.AdvFilterFile.at(i));
	    }	    
	}
	
	wstring HeaderStr;
	vector <wstring> Header;
	HeaderStr = wformat(TEXT("%-12s %7s %11s "), TEXT("File_Name"), TEXT("   RedX"), TEXT("       Mz"));
	for (int i = 0; i < BandCount; i++)
	{
	    wstring hB = wformat(TEXT(" %10s "), BandArray[i].Name.c_str());
	    HeaderStr.append(hB);
	}
	HeaderStr += wformat(_T("%s "), _T("          AB          AR          AV          AW          LB          LR          LV           LW"));
	//HeaderStr += wformat(_T("%s "), _T("          AB          AR          AV          AW          LB          LR          LV           LW"));

	Header.push_back(HeaderStr);
	for (size_t j = 0; j < FileList.ExtintFile.size(); j++) {
	    CMyTeBand Extint;
	    Extint.LoadFromFile(FileList.ExtintFile.at(j));
	    vector <wstring> FileNames;
	    VectorArray OutData;
	    wstring OutFile = FileList.OutFile;
	    size_t p = OutFile.find_last_of(_T("\\"));
	    OutFile.erase(p + 1);
	    OutFile.append(Extint.Name);
	    OutFile.append(_T("_out.txt"));

	    OutData.clear();
	    FileNames.clear();
	    for (size_t i = 0; i < FileList.Files.size(); i++) {
		DocDataType Data;		
		Data.LoadFromFile(FileList.Files[i].c_str());

		int res = CMyTeMath::Ai(Data, BandArray, Redden, Extint, AdvFilters, VegaArray, OutData);
		if(res == 0) return;
		for (size_t k = FileNames.size(); k < OutData.size(); k++) {
		    wstring s = Data.FileName;
		    size_t t = s.find_last_of(_T('.'));
		    s.erase(t);
		    FileNames.push_back(s);
		    FileNames.push_back(_T("|"));
		}
	    }
	    WriteDataHead(OutFile, FileNames, OutData, Header, _T(" %10.5lf|"));
	}
    }
}

#ifdef EMOE


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
#endif

class MyString
{
public:
	MyString(const std::wstring& s2)
	{
		s = s2;
	}

	operator LPCWSTR() const
	{
		return s.c_str();
	}
private:
	std::wstring s;
};

void CMyTeApp::OnActionBands()
{
	CCalculateDlg calcdlg;
	if(calcdlg.DoModal()==IDOK)
	
	{
	    vector <double> VegaArray;// = new double[BandCount];
		//for(int i=0;i<BandCount;i++) VegaArray[i]=0.0;
		CString RString;

		if(FileList.VegaFile.size())
		{
		    DocDataType Data;
		    Data.LoadFromFile(FileList.VegaFile);
			
			for(int i=0;i<BandCount;i++)
			{
				VegaArray.push_back(CMyTeMath::SBand_2(&Data,&BandArray[i],0,NULL,0));
			}
		}	

		RString.AppendFormat(_T("%-12s"),_T("File_Name"));
		RString.AppendFormat(_T("%11s "),_T("       RedX"));
//		RString.AppendFormat(_T("%11s "),_T("         Mz"));
		for(int i=0;i<BandCount;i++)
		{
			CString outfname(BandArray[i].FName.c_str());
			outfname.Truncate(outfname.GetLength()-4);
			int pos=outfname.ReverseFind('\\');		
			RString.AppendFormat(_T(" %10s "),outfname.Right(outfname.GetLength()-pos-1));	
			//RString.AppendFormat(_T(" %10s "),_T("Red x2"));
		}	
		RString.Append(_T("\n"));
		
#ifdef ZAKH
//ReferenceFile = _T("D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\LIST_BC_13_W_2E_0.5.txt");
char REDXfile[]="D:\\MOSH\\SOURCE\\RED_FAQTOR.txt";
//char NameSp[10];
double *RedMax=new double[FileList.Count];
char NaSp[11]={0,0,0,0,0,0,0,0,0,0,0};/*=new MyString[7];*/
char InLin[1024];
char** ptr=new char* [5000];
//for(UINT i=0;i<FileList.Count;i++){
//	NaSp[i]=new char[7];
//}
FILE *inpf;
	if (fopen_s(&inpf, REDXfile,"r") !=0 ){
		AfxMessageBox(_T("Error Opening REDX File"));
		//      return;
	}
	fgets(InLin,1024,inpf);
	CMyTeMath::StrPtrP(InLin,'|',ptr);
	//int N=strlen(ptr[1]);
	//memcpy(NaSp,ptr[1],N-1);	//TYC
	//NaSp[N-1]=0;
	//CString mdNam=_T(NaSp);
	for(UINT i=0;i<FileList.Count;i++)
		sscanf_s(ptr[0],"%lf",&RedMax[i]);
	fclose(inpf);
c1
c2


#endif

double RX=0;
	bool FlagYes=false;
	double X=0.0;
	int Nred=10;//0;//0;//0;//5;//5;	//number of random i.s.ext. amount (thickness, depth) 
		if(CMyTeMath::WriteResult1(FileList.OutFile.c_str(),RString,true)==0)
		{
			bool b=FileList.Over;	
			for(ULONG32 i=0;i<FileList.Files.size();i++)//FileList.Count
			{	
				m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
				CMyTeDoc* Doc;
				Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
				CString outfname(FileList.Files[i].c_str());

				int pos=outfname.ReverseFind('.');
				outfname.Truncate(pos);
				pos=outfname.ReverseFind('\\');
				CString ResultString=NULL;
				CString ResultStringZip=NULL;
				ResultString.AppendFormat(_T("%-10s"),outfname.Right(outfname.GetLength()-pos-1));	
				ResultStringZip=ResultString;
				CString midName=outfname.Right(outfname.GetLength()-pos-1);//-pos-1

//c1
					//strcmp((LPCWSTR )midName,NaSp);

				//CString theString( "This is a test" );
/*				LPTSTR lpsz = new TCHAR[midName.GetLength()+1];
				_tcscpy(lpsz, midName);
	*/			//strcmp(midName.GetBuffer(midName.GetLength(),NaSp);
				 //lpsz.Compare( _T(NaSp ) ) == -1 ; // Сравниваются с LPTSTR строкой.
				//for(UINT j=0;j<FileList.Count;j++){
				//	if(_tcscmp(lpsz,(LPCTSTR) NaSp)==0){
				//		RX=RedMax[j];
				//		FlagYes=true;
				//		break;
				//	}else {
				//		FlagYes=false;
				//		continue;
				//	}
				//}
				//	if(!Flag
				//Yes)
				//		RX=1;
					//else RX=1;
					//strcmp
				//CString aCString = "A string";
				//char myString[256];
				//strcpy(myString, (LPCTSTR)midName);

//c2
				for(int jj=0;jj<Nred;jj++){
					if (jj==0) X=0.0;
					/*if (jj==1) X=1.5;
					if (jj==2) X=2.0;
					if (jj==3) X=2.5;
					
					//if (jj==1) X=1.25;
					//if (jj==2) X=1.5;
					//if (jj==3) X=0.75;
					*/
					else {
						X=CMyTeMath::RANDisex()*3.0;///**RedMax[i]RANDisex()*///X+=0.1;1;//
					//	//X*=RedMax[i];//2
					}


					ResultString.Empty();//.Append(_T("\n"));
					ResultString=ResultStringZip;
					ResultString.AppendFormat(_T(" %10.3lf "),X);
					for(UINT j=0;j<BandCount;j++)//BandCount
					{			 
						ResultString.AppendFormat(_T(" %10.4lf "),//e ")
							CMyTeMath::SBand_2(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X));
					}
					ResultString.Append(_T("\n"));
					if(CMyTeMath::WriteResult1(FileList.OutFile.c_str(),ResultString,false)!=0) break;
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
void CMyTeApp::OnConvert_1()
{
#define len 16384
char InLin[16384];
char InLinSp[16384];
FILE *inpf,*outf;
char *ptr[len];
char *ptrS[len];
UINT NC = 0;
UINT NCS = 0;
UINT Count_str;
	if (fopen_s(&inpf,"D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV_nm_.txt", "r") !=0 )		AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);

	if (fopen_s(&outf,"D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV_C_.txt", "w") !=0 )			AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	char* Sfold="D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV";
	char* Filename="";
	//	UINT Count_str;//CMyTeMath::CountDataFile(inpf);
	Count_str=122;
//		fgets(InLinSp,len,inpf); //Name Spectra
//	 NCS=CMyTeMath::StrPtr(InLinSp,' ',ptrS);

	 double** SpSV = new double*[Count_str];        // STEP 1: SET UP THE ROWS.
	for (UINT i=0;i<Count_str;i++){
		SpSV[i]=new double [150];
	}
//CMyTeMath::InpData(Data0,inpf, Count_str, NULL); 
		char **NameSp=new char* [NCS];
		for(UINT i = 0; i < NCS; i++){
			NameSp[i]=new char [7];
			memset(NameSp[i],0,7);
		}
	fgets(InLinSp,len,inpf); //sp
		 NCS=CMyTeMath::StrPtr(InLin,'|',ptrS);
	//char buff[10];
	for(UINT i=0;i<NCS;i++){
		memcpy((void*)NameSp[i],(void*)ptrS[i],7);
		//NameSp[i][3]=0;
	}

	 for(UINT i=0;i<Count_str;i++){
		fgets(InLin,len,inpf); //Data
double b=0.0;
		 NC=CMyTeMath::StrPtr(InLin,'|',ptr);
		for(UINT j=0;j<NC;j++){
			if(sscanf_s(ptr[j],"%lf",&SpSV[i][j])==NULL) {
			}
			b=SpSV[i][j];
		}
	}
	fclose(inpf);
	 for(UINT j=0;j<NC;j++){
		for(UINT i=0;i<Count_str;i++){
				 //for(UINT j=0;j<NC;j++){
			fprintf(outf,"\t%10.1lf",SpSV[i][j]);
		}
		fputs("\n",outf);

	}
		fclose(outf);
}
void CMyTeApp::OnConvert()
{
#define len 16384
char InLin[16384];
char InLinSp[16384];
FILE *inpf,*outf;
char *ptr[len];
char *ptrS[len];
UINT NC = 0;
UINT NCS = 0;
UINT Count_str;
	if (fopen_s(&inpf,"D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV_nm_.txt", "r") !=0 )		AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);

	//if (fopen_s(&outf,"D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV_C_.txt", "w") !=0 )			AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		CString Sfold=_T("D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SV_SP");
		CString Filename;
	//	UINT Count_str;//CMyTeMath::CountDataFile(inpf);
	Count_str=122;
	fgets(InLinSp,len,inpf); //sp
		 NCS=CMyTeMath::StrPtr(InLinSp,'|',ptrS);

	 double** SpSV = new double*[Count_str];        // STEP 1: SET UP THE ROWS.
	for (UINT i=0;i<Count_str;i++){
		SpSV[i]=new double [150];
	}
//CMyTeMath::InpData(Data0,inpf, Count_str, NULL); 
		char **NameSp=new char* [NCS];
		for(UINT i = 0; i < NCS-1; i++){
			NameSp[i]=new char [8];
			memset(NameSp[i],0,8);
		}
	//char buff[10];
	for(UINT i=0;i<NCS-1;i++){
		memcpy((void*)NameSp[i],(void*)ptrS[i+1],8);
		NameSp[i][8]=0;
	}

	 for(UINT i=0;i<Count_str;i++){
		fgets(InLin,len,inpf); //Data
//double b=0.0;
		 NC=CMyTeMath::StrPtr(InLin,'|',ptr);
		for(UINT j=0;j<NC-1;j++){
			if(sscanf_s(ptr[j+1],"%lf",&SpSV[i][j])==NULL) {
			}
//			b=SpSV[i][j];
		}
	}
	fclose(inpf);
//		ULONG pos=0;
////	 fseek(inpf, 0L, SEEK_SET);
//
//	 for(UINT j=0;j<NC;j++){
//		for(UINT i=0;i<Count_str;i++){
//				 //for(UINT j=0;j<NC;j++){
//			fprintf(outf,"%10.1lf",SpSV[i][j]);
//		}
//		fputs("\n",outf);
//
//	}
//		fclose(outf);
	UINT* Lambda=new UINT[Count_str];
	for (int i = 0; i < Count_str; i++)
	{
		Lambda[i]=300+5*i;
	}
	CString fmode;
	fmode=_T("w"); //else fmode=_T("a");	
	 for(UINT j=0;j<NCS-1;j++){
		Filename=NameSp[j];
		CString destfname;
//		char*destfname="";
		destfname=Sfold;
		destfname.AppendChar('\\');		
		destfname+=Filename;
//		if (fopen_s(&outf,destfname, "w") !=0 )			AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
//	if (fopen_s(&outf,"D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\SP_SV_C_.txt", "w") !=0 )			AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	if (_wfopen_s(&outf,destfname,fmode)!=0 )			AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		for(UINT i=0;i<Count_str;i++){
			fprintf_s(outf,"%5d  %10.1lf\n",Lambda[i],SpSV[i][j]);

		}
		fclose(outf);
	 }
	 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
}
//#ifdef ENASHE
void CMyTeApp::OnConvert_2()//из столбцов в строки
{
	CRowToColDlg Dlg;
//

FILE *inpf,*outf,*errf;
#define len 16384
char InLin[16384];
	Dlg.SourceFileLambda=_T("D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\ST88FULL_lambda.txt");//uvk_pic_spc.txtMosh_Kom\\Электро\\ЭЛЕКТРО_New\\Wizard\\program\\uvk_picls.grd";
	Dlg.SourceFileFlux=_T("D:\\MOSH\\SOURCE\\СВИДЕРСКЕНЕ FLUX\\ST88FULL_s.txt");//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt");//Mosh_Kom\\Электро\\ЭЛЕКТРО_New\\Wizard\\program\\uvk_picls.spc");
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
		pos=0;
		double* Flux=NULL;
		DocDataType Data;
		//Data.Lambda=Lambda;
		//Data.Count=count;
		Filename.Empty();
		while(LoadRowFile(Dlg.SourceFileFlux,Flux,count,pos,&Filename))
		{		
			//Data.Flux=Flux;		
			CString destfname;
			destfname+=Dlg.DestFolder;
			destfname+=Filename;
//			destfname.AppendChar('\\');		
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
}

void CMyTeApp::OnRemoveMinus()
{
	CCalculateDlg calcdlg;
	calcdlg.NeedFolder=true;
	if (calcdlg.DoModal()==IDOK)
	{
		bool b=FileList.Over;
		for(ULONG32 i=0;i<FileList.Files.size();i++)
		{	
			m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
			CMyTeDoc* Doc;
			Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
			
			Doc->OnMinusRemove();
			
			CString outfname(FileList.Files[i].c_str());
			int pos=outfname.ReverseFind('.');
			outfname.Truncate(pos);
			pos=outfname.ReverseFind('\\');
			CString ResultString=NULL;
			CString ResultStringZip(FileList.OutFile.c_str());
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

#ifdef EMOE
	void CMyTeApp::OnActionTesting1()
{

FILE *inpf,*outf,*errf;
//#define len 16384
//char InLin[16384];
//#ifdef EMOE
//char *ptr[16384];
			//char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_GI_GB_GR.txt";//BC_7M_BASE_T2_AAA.txt";//BC_7m_BASE.txt";
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\H_K_GAIA.txt";//J_H_Vt_J_.txt";//J_Vt_AAA.txt";//BtVtJHK_WBVR
	//if (fopen_s(&inpf, FileInput, "r") !=0 ){
	//	AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	//if (fopen_s(&outf, FileOut, "w") !=0 ){
	//	AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
		CMyTeBand AZDK;
	AZDK.FName=_T("D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\GB_GR_H_K.txt");//Vt_J_J_H.txt");//AZDK_GI_GB_GR.txt");
	AZDK.LoadFromFile(AZDK.FName);
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OUT FALSE"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}


	 //int Count_str=CMyTeMath::CountDataFile(inpf);
	 //double *GB_GR=new double[Count_str];
	 //double *M_GI=new double[Count_str];
	 //double *K_Vt=new double[Count_str];
	 //double *Bt_Vt=new double[Count_str];
double GB_GR;
double AZDK_GI;
UINT Count=0;
UINT k=0;
UINT j=0;
double Sum=0;
double Sum1=0;
double d=0;
double *Vt_J=new double[AZDK.Count-1];
double *J_K=new double[AZDK.Count-1];
	 	 for (UINT i=0; i<AZDK.Count-1;i++ ){
				 if(AZDK.Lambda[i]<7.0 && AZDK.Lambda[i]>-1.0 && AZDK.Flux[i]>-0.5 && AZDK.Flux[i]<0.8) {
					 Vt_J[j]=AZDK.Lambda[i];
					 J_K[j]=AZDK.Flux[i];
					 j++;
					 Count++;
					 //continue;
				 }else continue;
		 }
		 double LamBeg=Vt_J[0];
	 	 for (UINT i=0; i<Count-1;/*i++*/ ){
			 do{
				Sum+=Vt_J[i];
				Sum1+=J_K[i];
				k++;
				if(i==Count-1) break;
				d=Vt_J[i+1]-LamBeg;
				//if(i==6699) {
				//	i=i;
				//}
				i++;
			 }while(d<=0.1); 
				GB_GR=Sum/(double)k;
				AZDK_GI=Sum1/(double)k;
				LamBeg=Vt_J[i];
				fprintf(outf,"%8.4lf %8.4lf %5d",GB_GR,AZDK_GI,k);
				fputs("\n",outf);
				Sum=0;
				Sum1=0;
				k=0;
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);

	fcloseall();
	//exit(0);
}
//#endif
//GB_GR[j]=LamBeg+j*0.1;
// AZDK.Lambda
				//CMyTeMath::StrPtr(InLin,'|',ptr);
				//sscanf(ptr[0],"%lf",GB_GR[i]);
				//sscanf(ptr[1],"%lf",M_GI[i]);
				//sscanf(ptr[130],"%lf",K_Vt[i]);
				//sscanf(ptr[128],"%lf",Bt_Vt[i]);
				//int Bt=strncmp(ptr[6],"      ",6);
				//int V=strncmp(ptr[18],"      ",6);
				//int Vbs=strncmp(ptr[31],"      ",6);
				//int BVbs=strncmp(ptr[32],"      ",6);


//	CMyTeMath::TESTING();
	// TODO: добавьте свой код обработчика команд
//FILE *inpf,*outf/*,*errf;*/
//#define len 16384
char InLin[16384];
char *ptr[16384];
char *ptrg[16384];
bool flag=false;

	//	char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\OUT\\AZDK_MIX_1.txt";//BC_For_GEO.txt";//BC_7m_BASE_U.txt";//BC_END_export.txt";//DATA SHIT
	//	char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\BC_GEO_1.txt";//AZDK_END_.txt";//BtVtJHK_WBVR
	//	//char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\GEO_DAT.txt";
	//	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	//	AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	//if (fopen_s(&outf, FileOut, "w") !=0 ){
	//	AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	////if (fopen_s(&errf, FileErr, "r") !=0 ){
	////	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	////	exit(1);//0U);
	////	//AfxMessageBox(_T("Error Opening data_total File"));
 //////      return;
	////}
	// int Count_str=CMyTeMath::CountDataFile(inpf);
	// //int Count_geo=CMyTeMath::CountDataFile(errf);

	//char** Data0 = new char*[Count_geo];        // STEP 1: SET UP THE ROWS.
	//CMyTeMath::InpData(Data0,errf, Count_geo, NULL); 
	//fclose(errf);
	// for (UINT i=0; i<Count_str; ){
	//	try {                      // TEST FOR EXCEPTIONS.
 //  			if ( fgets(InLin,len,inpf)==0 ) break;
	//		if (InLin[0]=='*'){ continue;
	//		}else{
 	//char buff[2048];
	 	// for (UINT i=0; i<Count_str; i++ ){
   //			if ( fgets(InLin,len,inpf)==0 ) break;
			//if (InLin[0]=='*'){ i--;
			//	continue;
			//}else{
				//if(i==585)
				//	i=i;

		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\AZDK_MIX_1.txt";//BC_7m_BASE_U.txt";//BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\AZDK_END_1.txt";//BC_GEO_.txt";//BtVtJHK_WBVR
//		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "r") !=0 ){
	//	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 //int Count=CMyTeMath::CountDataFile(errf);

	 //	 for (UINT i=0; i<Count_str; ){
		//try {                      // TEST FOR EXCEPTIONS.
  // 			if ( fgets(InLin,len,inpf)==0 ) break;
		//	if (InLin[0]=='*'){ continue;
		//	}else{
 	//char buff[2048];
	//char** Data0 = new char*[Count];        // STEP 1: SET UP THE ROWS.
	//CMyTeMath::InpData(Data0,errf, Count, NULL); 
	//fclose(errf);
//#ifdef EMOE
	 	 for (UINT i=0; i<Count_str; i++ ){
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ i--;
				continue;
			}else{
				//if(i==585)
				//	i=i;
			int NA=	CMyTeMath::StrPtr(InLin,'|',ptr);
			double *M=new double[NA-1];
			double SumM=0;
			//int pp=0;
			int K=0;
	 		for (UINT j=0; j<NA-1; j++ ){
				int MISS=strncmp(ptr[j],"M",1);
				if(MISS==0) {
					//flag=false;
					//j--;
					continue;
				}else 	{
					sscanf(ptr[j],"%lf",&M[K]);
					if(M[K]<-0.5 || M[K]>8.0) continue;
					SumM+=M[K];
					K++;
					//flag=true;
				}
			}
		if(K>0){
			double MIN=1000.0;
			double MAX=-1000.0;
			double MIDL=0.0;
			double Sigm=0.0;
			double dM=0.0;
			MIDL=SumM/(double)K;
	 		for (UINT j=0; j<K; j++ ){
				dM=M[j]-MIDL;
				dM*=dM;
				Sigm+=dM;
				MIN=MIN(MIN,M[j]);
				MAX=MAX(MAX,M[j]);
			}
			if(K>1){
				Sigm=sqrt(Sigm)/(double)(K*(K-1));
				fprintf(outf,"%7.3lf|%7.3lf|%7.3lf|%7.3lf|%3d|%6d|\n",MIDL,Sigm,MIN,MAX,K,i+1);
			}else
				fprintf(outf,"%7.3lf|%7.3lf|%7.3lf|%7.3lf|%3d|%6d|\n",MIDL,Sigm,0.0,0.0,K,i+1);
				//fprintf(outf,"%7.3lf|%7.3lf|%7.3lf|0.000|%6d|\n",MIN,MIDL,MAX,i+1);
		}else
				fprintf(outf," | | | | |%6d|\n",i+1);
		}
//#endif
			int pp=0;
			flag=false;
	 		for (UINT j=0; j<Count_geo; j++ ){
				CMyTeMath::StrPtr(InLin,'|',ptr);
				CMyTeMath::StrPtr(Data0[j],'|',ptrg);
				int n=strncmp(ptr[0],ptrg[0],16);
				memcpy(buff,ptr[0],16);
				buff[16]=0;
				int pp=0;
				double M;
				sscanf(ptrg[1],"%lf",&M);
				sscanf(ptr[1],"%d",&pp);

			//	//int Vbs=strncmp(ptr[31],"      ",6);
				//int BVbs=strncmp(ptr[32],"      ",6);
				//int UBbs=strncmp(ptr[33],"      ",6);
				//int RIbs=strncmp(ptr[34],"      ",6);
				//int WB=strncmp(ptr[19],"      ",6);
				//int BV=strncmp(ptr[20],"      ",6);
				//int VR=strncmp(ptr[21],"      ",6);
				//int Vt=strncmp(ptr[8],"      ",6);
				//int J=strncmp(ptr[60],"      ",6);
				//int H=strncmp(ptr[62],"      ",6);
				//int K=strncmp(ptr[64],"      ",6);
				//if(V==0 && Vbs==0 && BVbs==0 && UBbs==0 && RIbs==0 && /*Hp==0 &&*//*Bt==0 && Vt==0 && J==0 && H==0 && K==0 &&*/ WB==0 && BV==0 && VR==0 ) continue;
				if(n==0){
					fprintf(outf,"%16s|%7.3lf|%6d|\n",buff,M,pp);
					flag=true;

					//fprintf(outf,"%16c|%5c|%6.0d|\n",ptr[58],ptrg[1],pp);
					break;
				}

				else{
						continue;
				}
					//if(j==Count_geo-1 && n!=0){
					//	fprintf(outf,"  |  |%6d|\n",pp);
					//	break;
					//}	else
					//	continue;
////						if(V!=0 && Vbs!=0 && BVbs!=0 && UBbs!=0 && RIbs!=0 &&/*Bt!=0 && Vt!=0 && J!=0 && H!=0 && K!=0 && */WB!=0 && BV!=0 && VR!=0 ){
//						fprintf(outf,"%s",InLin);
//					}
//					else continue;
	
					}
					if(flag==false){
						fprintf(outf,"  |  |%6d|\n",pp);
						//continue;
					}
				}
			}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
				
#endif
#ifdef ETVOE
void CMyTeApp::OnActionTesting(){//Усреднение двойных
FILE *inpf,*outf,*errf;
char InLin[16384];
char *ptr[16384];
char *ptrg[16384];
bool flag=false;
		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\b7_double_FOR_C1.txt";//BC_7m_BASE_U.txt";//AZDK_MIX_1.txt";////BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\b7_double_CALC.txt";//BC_AZDK_Cstar.txt";//BC_GEO_.txt";//BtVtJHK_WBVR
//		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\C_star.txt";//AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "r") !=0 ){
	//	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 //int Count=CMyTeMath::CountDataFile(errf);
	 long double RA;
	 long double DE;
	 long double RA1;
	 long double DE1;
	 long double RA2;
	 long double DE2;
	 double Bt1;
	 double Vt1;
	 double Bt2;
	 double Vt2;
	 double J;
	 double H;
	 double K;
	 double Bt;
	 double Vt;
	 char buff[20];
	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*' || InLin[0]=='|'){ continue;
			}else{
 	//char buff[2048];
			CMyTeMath::StrPtr(InLin,'|',ptr);
			memcpy(buff,ptr[0],16);
			buff[16]=0;

			//sscanf(ptr[6],"%lf",&Bt1);
			//sscanf(ptr[8],"%lf",&Vt1);
			sscanf(ptr[6],"%lf",&Bt1);
			sscanf(ptr[8],"%lf",&Vt1);
			sscanf(ptr[10],"%lf",&J);
			sscanf(ptr[12],"%lf",&H);
			sscanf(ptr[14],"%lf",&K);
			sscanf(ptr[1],"%lf",&RA1);
			sscanf(ptr[2],"%lf",&DE1);

			if ( fgets(InLin,len,inpf)==0 ) break;
			CMyTeMath::StrPtr(InLin,'|',ptr);
			//sscanf(ptr[6],"%lf",&Bt1);
			//sscanf(ptr[8],"%lf",&Vt1);
			sscanf(ptr[6],"%lf",&Bt2);
			sscanf(ptr[8],"%lf",&Vt2);
			sscanf(ptr[1],"%lf",&RA2);
			sscanf(ptr[2],"%lf",&DE2);

			long double IB1=pow(10.0,-Bt1*0.4);
			long double IV1=pow(10.0,-Vt1*0.4);
			long double IB2=pow(10.0,-Bt2*0.4);
			long double IV2=pow(10.0,-Vt2*0.4);

			long double IB=IB1+IB2;
			long double IV=IV1+IV2;
			Bt=-2.5*log10l(IB);
			Vt=-2.5*log10l(IV);
			RA=RA1*IV1/IV+RA2*IV2/IV;
			DE=DE1*IV1/IV+DE2*IV2/IV;
			double Bt_Vt=Bt-Vt;
			double J_H=J-H;
			double H_K=H-K;
			double Vt_J=Vt-J;
//double a1=	0.07213078596482563;
//double a2=	-0.1364455053124737;
//double a3=	-0.258708520516089;
//double b1=	-0.39439088447677245;
//double b2=	-0.04673489790259197;
//double b3=	0.15057257619261641;
////b4=	-0.1805031032488205;
//b5=	0.2287292874839638;
//b8=	-0.160695301049240;
//b9=	0.130584488133696;
//b10=	0.50166483947413076;
//c1=	0.17884417594517213;
//c2=	-0.01514442015946991;
//c4=	0.27682520071280897;
//c5=	-0.12158712866439111;
//c7=	0.077550567502602877;
//c8=	0.048585226088267791;
//c9=	0.079066029190058826;
//c11=	0.30915155226117919;
//c12=	-0.65770926635601989;
//c13=	-0.28053575519978863;
//c17=	-0.19891698895345861;
double AZDK_BtVtJHK=Vt+0.07213*Bt_Vt-0.13644*Vt_J-0.2587*J_H-0.39439*pow(Bt_Vt,2.0)-0.04673*pow(Vt_J,2.0)+0.15057*pow(J_H,2.0)
	-0.1805*pow(H_K,2.0)+0.228729*Bt_Vt*Vt_J-0.1606953*Vt_J*H_K+0.130584488*Vt_J*J_H+0.5016648*J_H*H_K+0.178844*pow(Bt_Vt,3.0)-0.0151444*pow(Vt_J,3.0)
	+0.276825*pow(H_K,3.0)-0.121587*pow(Bt_Vt,2.0)*Vt_J+0.07755*pow(Bt_Vt,2.0)*H_K+0.048585*pow(Vt_J,2.0)*Bt_Vt+0.079066*pow(Vt_J,2.0)*J_H+
	0.30915*pow(J_H,2.0)*Bt_Vt-0.6577*pow(J_H,2.0)*H_K-0.28054*pow(J_H,2.0)*Vt_J-0.19892*Bt_Vt*Vt_J*J_H;
			fprintf(outf,"%16s|%12.8lf|%12.8lf|D|%7.3lf|%7.3lf|%7.3lf|%7.3lf|%7.3lf|%8.3lf|\n",buff,RA,DE,Bt,Vt,J,H,K,AZDK_BtVtJHK);
			}
		}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
//		}
	fcloseall();
}
#endif
//#ifdef ETVOE
void CMyTeApp::OnActionTesting(){//выбор строк Err данных в порядке Input каталога
FILE *inpf,*outf,*errf;
char InLin[16384];
char *ptr[16384];
char *ptrg[16384];
bool flag=false;
		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\TYC_FOR_AZDK_PROBLEM_OUT_1.txt";//AZDK_LAST_PROBLEM.txt";//AZDK_PROBLEM_CORR_MIX.txt";//TYC_FOR_TRIPL_DP.txt";//AZDK_P_SINGL.txt";//AZDK_PROBLEM_OUT_MIX.txt";//TYC_FOR_PROBLEM_AZDK.txt";//d7_Double_DP_1.txt";//BC_7m_BASE.txt";//DATA SHIT\\_U1.txt";//b7_SINGL.txt";//b7_double_FOR_C.txt";//BC_7m_BASE_U.txt";//AZDK_MIX_1.txt";////BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\pm_PROBLEM_OUT_1.txt";//b7_A_FOR_double_DP_1.txt";//BC_A_SINGL.txt";////b7_SINGL_Cstar.txt";//b7_double_CALC.txt";//BC_AZDK_Cstar.txt";//BC_GEO_.txt";//BtVtJHK_WBVR
		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\TYC_FOR_PROBLEM_AZDK.txt";//BC_FOR_OUT.txt";//b7_DOUBLE_1.txt";//BC_A_For_SINGL.txt";//DATA SHITC_star.txt";//BC_FOR_A.txt";//C_star.txt";\\OUT//AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&errf, FileErr, "r") !=0 ){
		AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 int Count=CMyTeMath::CountDataFile(errf);
	 char buff[20];
	char** Data0 = new char*[Count];        // STEP 1: SET UP THE ROWS.
	for (UINT i=0;i<Count;i++){
		Data0[i]=new char[16384];
		memset(Data0[i],0,16384);
	}
	
	CMyTeMath::InpData(Data0,errf, Count, NULL); 
	fclose(errf);

	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*' /*|| InLin[0]=='|'*/){ continue;
			}else
			{
 	//char buff[2048];
			CMyTeMath::StrPtr(InLin,'|',ptr);
			memcpy(buff,ptr[0],16);
			buff[16]=0;

			//sscanf(ptr[6],"%lf",&Bt1);
			flag=false;
	 		for (UINT j=0; j<Count; j++ ){
				//CMyTeMath::StrPtr(InLin,'|',ptr);
				CMyTeMath::StrPtr(Data0[j],'|',ptrg);
				int n=strncmp(ptr[0],ptrg[0],16);

				//memcpy(buff,ptr[0],16);
				//buff[16]=0;
				//int pp=0;
				//double M;
				//sscanf(ptrg[1],"%lf",&M);
				//sscanf(ptr[1],"%d",&pp);

				if(n==0){
					fprintf(outf,"%s",/*InLin*/Data0[j]);/*|%7.3lf|%6d|\n*/
					flag=true;

					//fprintf(outf,"%16c|%5c|%6.0d|\n",ptr[58],ptrg[1],pp);
					break;
				}

				else{
						continue;
				}
					//if(j==Count_geo-1 && n!=0){
					//	fprintf(outf,"  |  |%6d|\n",pp);
					//	break;
					//}	else
					//	continue;
			}
			if(flag==false)
				fprintf(outf,"%s|       |\n",buff);
			//
		}
		}
//		}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
//		}
		 	delete [] Data0;
//	de_allocate(Data0,Count_str);
	fcloseall();
}
//#endif
#ifdef ETVOE
void CMyTeApp::OnActionTesting(){//FINAL COUNTDOWN
FILE *inpf,*outf,*errf;
char InLin[16384];
char *ptr[16384];
		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\A_LP.txt";//A_FOR_C_LAST_PROBLEM_.txt";//A_FOR_PROBLEM_CORR.txt";//A_TRIPL_DP_.txt";//A_FOR_b7_tripl_DP.txt";//A_FOR_AZDK_P_MIX.txt";//A_AZDK_DP_FP.txt";//A_FOR_double_PROBLEM_AZDK.txt";//A_FOR_double_C_1_2.txt";//A_FOR_double_DP_1.txt";//OUT\\BC_A_For_SINGL_С.txt";//DATA SHIT\\AZDK_MIX_1.txt";//BC_7m_BASE_U.txt";//BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_LP.txt";//LAST_PROBLEM_OUT.txt";//BC_GEO_.txt";//BtVtJHK_WBVR DATA SHIT OUT\\
//		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "r") !=0 ){
	//	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 //int Count=CMyTeMath::CountDataFile(errf);

	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{

	 	// for (UINT i=0; i<Count_str; i++ ){
   //			if ( fgets(InLin,len,inpf)==0 ) break;
			//if (InLin[0]=='*'){ i--;
			//	continue;
			//}else{
			//	//if(i==585)
				//	i=i;
			int NA=	CMyTeMath::StrPtr(InLin,'|',ptr);
			double *M=new double[NA-1];
			double SumM=0;
			//int pp=0;
			int K=0;
	 		for (UINT j=0; j<NA-1; j++ ){
				int MISS=strncmp(ptr[j],"M",1);
				if(MISS==0) {
					//flag=false;
					//j--;
					continue;
				}else 	{
					sscanf(ptr[j],"%lf",&M[K]);
					//if(M[K]<-0.5 || M[K]>8.0) continue;
					SumM+=M[K];
					K++;
					//flag=true;
				}
			}
		if(K>0){
			double MIN=1000.0;
			double MAX=-1000.0;
			double MIDL=0.0;
			double Sigm=0.0;
			double dM=0.0;
			MIDL=SumM/(double)K;
	 		for (UINT j=0; j<K; j++ ){
				dM=M[j]-MIDL;
				dM*=dM;
				Sigm+=dM;
				MIN=MIN(MIN,M[j]);
				MAX=MAX(MAX,M[j]);
			}
			if(K>1){
				Sigm=sqrt(Sigm)/(double)(/*K**/(K-1));
				fprintf(outf,"%7.3lf|%7.3lf|%7.3lf|%7.3lf|%3d|\n",MIDL,Sigm,MIN,MAX,K);
			}else
				fprintf(outf,"%7.3lf|       |       |       |%3d|\n",MIDL,K);
				//fprintf(outf,"%7.3lf|%7.3lf|%7.3lf|0.000|%6d|\n",MIN,MIDL,MAX,i+1);
		}else
				fprintf(outf," | | | | |%6d|\n",i+1);
		}
		}
		//}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
//		}
		 	//delete [] Data0;
//	de_allocate(Data0,Count_str);
	fcloseall();
}
#endif
#ifdef EVOE

void CMyTeApp::OnActionTesting(){//Усреднение двойных
FILE *inpf,*outf,*errf;
char InLin[16384];
char *ptr[16384];
char *ptrg[16384];
bool flag=false;
		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_LAST_PROBLEM.txt";//AZDK_PROBLEM_dbl_CORR.txt";//AZDK_PROBLEM_OUT.txt";//b7_double_FOR_CG1.txt";//BC_7m_BASE_U.txt";//AZDK_MIX_1.txt";////BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_LAST_PROBLEM_MIX.txt";//b7_double_CALC_G1.txt";//BC_AZDK_Cstar.txt";//BC_GEO_.txt";//BtVtJHK_WBVR
//		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\C_star.txt";//AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "r") !=0 ){
	//	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 //int Count=CMyTeMath::CountDataFile(errf);
	 long double RA;
	 long double DE;
	 long double RA1;
	 long double DE1;
	 long double RA2;
	 long double DE2;
	 double Bt1;
	 double Vt1;
	 double Bt2;
	 double Vt2;
	 double J;
	 double H;
	 double K;
	 double Bt;
	 double Vt;
	 double pmRA;
	 double pmDE;
	 char buff[20];
	 //double GI1;
	 //double GI;
	 //double AZ1;
	 //double AZ2;
	 //double AZ;
	 /*double GI2;
	 double GB2;
	 double GR2;*/
	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*' || InLin[0]=='|'){ continue;
			}else{
 	//char buff[2048];
			CMyTeMath::StrPtr(InLin,'|',ptr);
			memcpy(buff,ptr[0],16);
			buff[16]=0;
			//int MISS=strncmp(ptr[5],"       ",7);
			//int MISS2=strncmp(ptr[10],"       ",7);
			//int MISS3=strncmp(ptr[10],"       ",7);
					//sscanf(ptr[6],"%lf",&Bt1);
					sscanf(ptr[8],"%lf",&Vt1);
					//sscanf(ptr[1],"%lf",&RA1);
					//sscanf(ptr[2],"%lf",&DE1);
					//sscanf(ptr[6],"%lf",&Bt1);
					//sscanf(ptr[8],"%lf",&Vt1);
					//fprintf(outf,"%16s|       |\n",buff);
				////	continue;}
				////else{
				//if(MISS!=0){

			//sscanf(ptr[5],"%lf",&AZ1);
			//sscanf(ptr[12],"%lf",&GB1);
			//sscanf(ptr[14],"%lf",&GR1);
			sscanf(ptr[1],"%lf",&RA1);
			sscanf(ptr[2],"%lf",&DE1);
			sscanf(ptr[3],"%lf",&pmRA);
			sscanf(ptr[4],"%lf",&pmDE);
			sscanf(ptr[11],"%lf",&J);
			sscanf(ptr[13],"%lf",&H);
			sscanf(ptr[15],"%lf",&K);

			if ( fgets(InLin,len,inpf)==0 ) break;
			CMyTeMath::StrPtr(InLin,'|',ptr);
			//int MISS1=strncmp(ptr[5],"       ",7);
		//if(MISS1!=0){
			//sscanf(ptr[5],"%lf",&AZ2);
			//sscanf(ptr[1],"%lf",&RA2);
			//sscanf(ptr[2],"%lf",&DE2);
			//long double IZ1=pow(10.0,-AZ1*0.4);
			//long double IZ2=pow(10.0,-AZ2*0.4);
			//long double IZ=IZ1+IZ2;
			//AZ=-2.5*log10l(IZ);
			//RA=RA1*IZ1/IZ+RA2*IZ2/IZ;
			//DE=DE1*IZ1/IZ+DE2*IZ2/IZ;
			//fprintf(outf,"%16s|%13.8lf|%13.8lf|%7.3lf|\n",buff,RA,DE,AZ);
			//continue;
			
			//			fprintf(outf,"%16s|%7.3lf|\n",buff,GI1);
			//sscanf(ptr[6],"%lf",&Bt1);
			//sscanf(ptr[8],"%lf",&Vt1);
			//sscanf(ptr[6],"%lf",&Bt2);
			sscanf(ptr[8],"%lf",&Vt2);
			sscanf(ptr[1],"%lf",&RA2);
			sscanf(ptr[2],"%lf",&DE2);

			//long double IB1=pow(10.0,-Bt1*0.4);
			long double IV1=pow(10.0,-Vt1*0.4);
			//long double IB2=pow(10.0,-Bt2*0.4);
			long double IV2=pow(10.0,-Vt2*0.4);

			//long double IB=IB1+IB2;
			long double IV=IV1+IV2;
			//Bt=-2.5*log10l(IB);
			Vt=-2.5*log10l(IV);
			RA=RA1*IV1/IV+RA2*IV2/IV;
			DE=DE1*IV1/IV+DE2*IV2/IV;
			//double Bt_Vt=Bt-Vt;
			double J_H=J-H;
			double H_K=H-K;
			double Vt_J=Vt-J;
////double a1=	0.07213078596482563;
////double a2=	-0.1364455053124737;
////double a3=	-0.258708520516089;
////double b1=	-0.39439088447677245;
////double b2=	-0.04673489790259197;
////double b3=	0.15057257619261641;
//////b4=	-0.1805031032488205;
////b5=	0.2287292874839638;
////b8=	-0.160695301049240;
////b9=	0.130584488133696;
////b10=	0.50166483947413076;
////c1=	0.17884417594517213;
////c2=	-0.01514442015946991;
////c4=	0.27682520071280897;
////c5=	-0.12158712866439111;
////c7=	0.077550567502602877;
////c8=	0.048585226088267791;
////c9=	0.079066029190058826;
////c11=	0.30915155226117919;
////c12=	-0.65770926635601989;
////c13=	-0.28053575519978863;
////c17=	-0.19891698895345861;
//double AZDK_BtVtJHK=Vt+0.07213*Bt_Vt-0.13644*Vt_J-0.2587*J_H-0.39439*pow(Bt_Vt,2.0)-0.04673*pow(Vt_J,2.0)+0.15057*pow(J_H,2.0)
//	-0.1805*pow(H_K,2.0)+0.228729*Bt_Vt*Vt_J-0.1606953*Vt_J*H_K+0.130584488*Vt_J*J_H+0.5016648*J_H*H_K+0.178844*pow(Bt_Vt,3.0)-0.0151444*pow(Vt_J,3.0)
//	+0.276825*pow(H_K,3.0)-0.121587*pow(Bt_Vt,2.0)*Vt_J+0.07755*pow(Bt_Vt,2.0)*H_K+0.048585*pow(Vt_J,2.0)*Bt_Vt+0.079066*pow(Vt_J,2.0)*J_H+
//	0.30915*pow(J_H,2.0)*Bt_Vt-0.6577*pow(J_H,2.0)*H_K-0.28054*pow(J_H,2.0)*Vt_J-0.19892*Bt_Vt*Vt_J*J_H;

//fprintf(outf,"%16s|%13.8lf|%13.8lf|%7.1lf|%7.1lf|/*%7.3lf|*/%7.3lf|%7.3lf|%7.3lf|%7.3lf|/*%7.3lf|*/\n",
				//buff,RA,DE,pmRA,pmDE,/*Bt,*/Vt,J,H,K/*,AZDK_BtVtJHK*/);
fprintf(outf,"%16s|%13.8lf|%13.8lf|%7.1lf|%7.1lf|%7.3lf|%7.3lf|%7.3lf|%7.3lf|\n",
				buff,RA,DE,pmRA,pmDE,/*Bt,*/Vt,J,H,K/*,AZDK_BtVtJHK*/);
			}
			}

		//}
		//else{
		//	fprintf(outf,"%16s|       |\n",buff);
		//	if ( fgets(InLin,len,inpf)==0 ) break;
		//	continue;
		//}
			//}
		//}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
//		}
	fcloseall();
}
#endif
#ifdef EEMOE
//#ifdef TMOE
void CMyTeApp::OnActionTesting(){//Усреднение кратных
FILE *inpf,*outf,*errf;
char InLin[16384];
char *ptr[16384];
char *ptrg[16384];
bool flag=false;
		char FileInput[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_tripl_DP_FOR_C.txt";//AZDK_PROBLEM_OUT.txt";//b7_double_FOR_CG1.txt";//BC_7m_BASE_U.txt";//AZDK_MIX_1.txt";////BC_END_export.txt";//
		char FileOut[]="D:\\MOSH\\STAR TRACK NEW\\AZDK_tripl_OUT_MIX.txt";//b7_double_CALC_G1.txt";//BC_AZDK_Cstar.txt";//BC_GEO_.txt";//BtVtJHK_WBVR
//		char FileErr[]="D:\\MOSH\\STAR TRACK NEW\\DATA SHIT\\C_star.txt";//AZDK_BS_JC.txt";//GEO_DAT.txt";//BtVtJHK_WBVR\\ГЕОФИЗИКА\\
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	if (fopen_s(&outf, FileOut, "w") !=0 ){
		AfxMessageBox(_T("OutPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//if (fopen_s(&errf, FileErr, "r") !=0 ){
	//	AfxMessageBox(_T("Err NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
	//	exit(1);//0U);
	//	//AfxMessageBox(_T("Error Opening data_total File"));
 ////      return;
	//}
	 int Count_str=CMyTeMath::CountDataFile(inpf);
	 //int Count=CMyTeMath::CountDataFile(errf);
	 long double RA;
	 long double DE;
	 long double* RA[10];
	 long double* DE[10];
	 //long double RA2;
	 //long double DE2;
	 
	 char buff[20];
	 double GI1;
	 double GI;
	 double* AZ[10];
	 //double AZ2;
	 //double AZ;
	 //double GI2;
	 //double GB2;
	 //double GR2;
	 	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*' || InLin[0]=='|'){ continue;
			}else{
 	//char buff[2048];
			CMyTeMath::StrPtr(InLin,'|',ptr);
			memcpy(buff,ptr[0],16);
			buff[16]=0;
			int K=0;
	do{
		int MISS=strncmp(ptr[1],"M",1);
		int MISS2=strncmp(ptr[2],"M",1);
		int MISS3=strncmp(ptr[5],"M",1);
		if(	MISS==0 || MISS2==0 || MISS3==0) continue;
		else{
			sscanf(ptr[1],"%lf",&RA[K]);
			sscanf(ptr[2],"%lf",&DE[K]);
			sscanf(ptr[5],"%lf",&AZ[K]);

   			if ( fgets(InLin,len,inpf)==0 ) break;
	}while(InLin[0]!='|')
						sscanf(ptr[1],"%lf",&RA[K]);
						sscanf(ptr[2],"%lf",&DE[K]);
						sscanf(ptr[5],"%lf",&AZ[K]);

//			//sscanf(ptr[6],"%lf",&Bt1);
//			//sscanf(ptr[8],"%lf",&Vt1);
//			//sscanf(ptr[6],"%lf",&Bt1);
//			//sscanf(ptr[8],"%lf",&Vt1);
//			//fprintf(outf,"%16s|       |\n",buff);
//			//continue;}
//		//else{
//		if(MISS!=0){

			sscanf(ptr[5],"%lf",&AZ1);
			//sscanf(ptr[12],"%lf",&GB1);
			//sscanf(ptr[14],"%lf",&GR1);
			sscanf(ptr[1],"%lf",&RA1);
			sscanf(ptr[2],"%lf",&DE1);

			if ( fgets(InLin,len,inpf)==0 ) break;
			CMyTeMath::StrPtr(InLin,'|',ptr);
			int MISS1=strncmp(ptr[5],"M",1);
		if(MISS1!=0){
			sscanf(ptr[5],"%lf",&AZ2);
			sscanf(ptr[1],"%lf",&RA2);
			sscanf(ptr[2],"%lf",&DE2);
			long double IZ1=pow(10.0,-AZ1*0.4);
			long double IZ2=pow(10.0,-AZ2*0.4);
			long double IZ=IZ1+IZ2;
			AZ=-2.5*log10l(IZ);
			RA=RA1*IZ1/IZ+RA2*IZ2/IZ;
			DE=DE1*IZ1/IZ+DE2*IZ2/IZ;
			fprintf(outf,"%16s|%13.8lf|%13.8lf|%7.3lf|\n",buff,RA,DE,AZ);
			continue;
			
			//if(MISS1==0){
			//fprintf(outf,"%16s|       |\n",buff);
			//continue;
		}
		//else{
		//	fprintf(outf,"%16s|%7.3lf|\n",buff,GI1);
			//sscanf(ptr[6],"%lf",&Bt1);
			//sscanf(ptr[8],"%lf",&Vt1);
			//sscanf(ptr[6],"%lf",&Bt2);
			//sscanf(ptr[8],"%lf",&Vt2);
			//sscanf(ptr[1],"%lf",&RA2);
			//sscanf(ptr[2],"%lf",&DE2);

//			long double IB1=pow(10.0,-GI1*0.4);
////			long double IV1=pow(10.0,-Vt1*0.4);
//			long double IB2=pow(10.0,-GI2*0.4);
//			long double IV2=pow(10.0,-Vt2*0.4);

			//long double IB=IB1+IB2;
			////long double IV=IV1+IV2;
			//GI=-2.5*log10l(IB);
			//Vt=-2.5*log10l(IV);
			//RA=RA1*IV1/IV+RA2*IV2/IV;
			//DE=DE1*IV1/IV+DE2*IV2/IV;
//			double Bt_Vt=Bt-Vt;
//			double J_H=J-H;
//			double H_K=H-K;
//			double Vt_J=Vt-J;
////double a1=	0.07213078596482563;
////double a2=	-0.1364455053124737;
////double a3=	-0.258708520516089;
////double b1=	-0.39439088447677245;
////double b2=	-0.04673489790259197;
////double b3=	0.15057257619261641;
//////b4=	-0.1805031032488205;
////b5=	0.2287292874839638;
////b8=	-0.160695301049240;
////b9=	0.130584488133696;
////b10=	0.50166483947413076;
////c1=	0.17884417594517213;
////c2=	-0.01514442015946991;
////c4=	0.27682520071280897;
////c5=	-0.12158712866439111;
////c7=	0.077550567502602877;
////c8=	0.048585226088267791;
////c9=	0.079066029190058826;
////c11=	0.30915155226117919;
////c12=	-0.65770926635601989;
////c13=	-0.28053575519978863;
////c17=	-0.19891698895345861;
//double AZDK_BtVtJHK=Vt+0.07213*Bt_Vt-0.13644*Vt_J-0.2587*J_H-0.39439*pow(Bt_Vt,2.0)-0.04673*pow(Vt_J,2.0)+0.15057*pow(J_H,2.0)
//	-0.1805*pow(H_K,2.0)+0.228729*Bt_Vt*Vt_J-0.1606953*Vt_J*H_K+0.130584488*Vt_J*J_H+0.5016648*J_H*H_K+0.178844*pow(Bt_Vt,3.0)-0.0151444*pow(Vt_J,3.0)
//	+0.276825*pow(H_K,3.0)-0.121587*pow(Bt_Vt,2.0)*Vt_J+0.07755*pow(Bt_Vt,2.0)*H_K+0.048585*pow(Vt_J,2.0)*Bt_Vt+0.079066*pow(Vt_J,2.0)*J_H+
//	0.30915*pow(J_H,2.0)*Bt_Vt-0.6577*pow(J_H,2.0)*H_K-0.28054*pow(J_H,2.0)*Vt_J-0.19892*Bt_Vt*Vt_J*J_H;
			}
		//}
		//else{
		//	fprintf(outf,"%16s|       |\n",buff);
		//	if ( fgets(InLin,len,inpf)==0 ) break;
		//	continue;
		//}
			//}
		//}
			catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			 //YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
				AfxMessageBox(_T("Error Reading File"));
				exit(-1);
			}
		}
		 m_nWnd->m_wndStatusBar.SetPaneText(0,_T("Подсчет закончен."), TRUE);
//		}
	fcloseall();
}
#endif
void CMyTeApp::OnActionAid1()// много прозрачностей
{
	CCalculateDlg calcdlg;
	if(calcdlg.DoModal()==IDOK)	
	{
		double* VegaArray=new double[BandCount];
		double* VegaXArray=new double[BandCount];
		for(int i=0;i<BandCount;i++) VegaArray[i]=0.0;
		for(int i=0;i<BandCount;i++) VegaXArray[i]=0.0;
		CString RString;
		CMyTeDoc* VDoc;
		//CString ResultString=NULL;
		//CString ResultStringZip=NULL;

		if(FileList.VegaFile.size())
		{
			//CMyTeDoc* VDoc;
			//подсчет Bеги
			VDoc=(CMyTeDoc*)OpenDocumentFile(FileList.VegaFile.c_str());
			for(int i=0;i<BandCount;i++)
			{
				VegaArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,NULL,0);
				//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
			}
			int cou=GetOpenDocumentCount();
			if(cou>0) VDoc->OnCloseDocument();
		}	
			//for(int i=0;i<BandCount;i++)
			//{
			//	//VegaXArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,&Redden,X);
			//	ResultString.AppendFormat(_T(" %10.4lf "),VegaArray[i]);
			//	//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
			//}
			//ResultString.Append(_T("\n"));

		RString.AppendFormat(_T("%-12s"),_T("File_Name"));
		RString.AppendFormat(_T("%7s "),_T("   RedX"));
		RString.AppendFormat(_T("%11s "),_T("       Mz"));
		for(int i=0;i<BandCount;i++)
		{
			CString outfname(BandArray[i].FName.c_str());
			outfname.Truncate(outfname.GetLength()-4);
			int pos=outfname.ReverseFind('\\');		
			RString.AppendFormat(_T(" %10s "),outfname.Right(outfname.GetLength()-pos-1));	
//			RString.AppendFormat(_T(" %10s "),_T("Ai"));
		}	
		RString.AppendFormat(_T("%s "),_T("          AB          AR          AV          AW          LB          LR          LV           LW"));
		RString.Append(_T("\n"));
		
	double RX=0;
	bool FlagYes=false;
	double X=0.0;
	double Mz=1.0;
	CMyTeBand EXTIN;
	EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\ОБРАБОТКА\\EXTATM\\200186\\281185.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\MAR_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\JULE_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\DEC_S.txt");
	//EXTIN.FName=_T("D:\\MOSH\\СИРИУС NEW\\SEP_S.txt");
	EXTIN.LoadFromFile(EXTIN.FName);

	int Nred=6;//0;//0;//0;//5;//5;	//number of random i.s.ext. amount (thickness, depth) 
	//CString ResultString=NULL;
	//CString ResultStringZip=NULL;

		if(CMyTeMath::WriteResult1(FileList.OutFile.c_str(),RString,true)==0)
		{

			bool b=FileList.Over;	
			for(ULONG32 i=0;i<FileList.Files.size();i++)//FileList.Count
			{	
				m_nWnd->m_wndStatusBar.SetPaneText(0,FileList.Files[i].c_str(), TRUE);
				CMyTeDoc* Doc;
				Doc=(CMyTeDoc*)OpenDocumentFile(FileList.Files[i].c_str());
				CString outfname(FileList.Files[i].c_str());

				int pos=outfname.ReverseFind('.');
				outfname.Truncate(pos);
				pos=outfname.ReverseFind('\\');
				CString ResultString=NULL;
				CString ResultStringZip=NULL;
				ResultString.AppendFormat(_T("%-10s"),outfname.Right(outfname.GetLength()-pos-1));	
				ResultStringZip=ResultString;
				CString midName=outfname.Right(outfname.GetLength()-pos-1);//-pos-1
				for(int jj=0;jj<Nred;jj++){
					if (jj==0) X=0.0;
					else {

						X=CMyTeMath::RANDisex()*4.0;///**RedMax[i]RANDisex()*///X+=0.1;1;//
					//	//X*=RedMax[i];//2
					}
					ResultString.Empty();//.Append(_T("\n"));
					ResultString=ResultStringZip;
					ResultString.AppendFormat(_T(" %10.3lf "),X);
					//ResultStringZip=ResultString;
					//if(FileList.VegaFile!="")
					//{
						//CMyTeDoc* VDoc;
						//подсчет Bеги
						//VDoc=(CMyTeDoc*)OpenDocumentFile(FileList.VegaFile);
						//for(int i=0;i<BandCount;i++)
						//{
						//	VegaXArray[i]=CMyTeMath::SBand_2(&VDoc->Data,&BandArray[i],0,&Redden,X);
						//	//ResultString.AppendFormat(_T(" %10.4lf "),VegaXArray[i]);
						//	//			VegaArray[i]=CMyTeMath::SBand(&VDoc->Data,Options,&BandArray[i],0,NULL,0);
						//}

						//}	


						for(UINT ij=0;ij<6;ij++){//Mz
							ResultString.Empty();//.Append(_T("\n"));
							ResultString=ResultStringZip;
							ResultString.AppendFormat(_T(" %10.3lf "),X);
							//ResultString=ResultStringZip;
							//if(ij==0) Mz=0.0;
							//	else {
									if(ij==0) Mz=0.0;
								else {
									Mz=1.0+CMyTeMath::RANDisex()*2.2;///**RedMax[i]RANDisex()*///X+=0.1;1;//
								} 
							//} 
							ResultString.AppendFormat(_T(" %10.4lf "),Mz);
							//for(UINT j=0;j<BandCount;j++){//BandCount
							//	ResultString.AppendFormat(_T(" %10.4lf "),VegaArray[j]);
							//}
							//	ResultString.Append(_T("\n"));
							for(UINT j=0;j<BandCount;j++){//BandCount
								ResultString.AppendFormat(_T(" %10.4lf "),
									CMyTeMath::SBand_4(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN));
							}
							for(UINT j=0;j<BandCount;j++){//BandCount
							ResultString.AppendFormat(_T(" %10.4lf "),
								CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN));//-VegaXArray[j]);
								//CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN)-VegaXArray[j]);
							}
							for(UINT j=0;j<BandCount;j++){//BandCount
							ResultString.AppendFormat(_T(" %10.4lf "),
								1.086*(CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN)-
								 CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,0.0,&EXTIN)));//-VegaXArray[j]);
							}
							//for(UINT j=0;j<BandCount;j++){//BandCount
							//ResultString.AppendFormat(_T(" %10.4lf "),
							//	CMyTeMath::SBand_L2(&Doc->Data,&BandArray[j],0,&Redden,X,Mz,&EXTIN)-CMyTeMath::SBand_L(&Doc->Data,&BandArray[j],0,&Redden,X,0.0,&EXTIN));//-VegaXArray[j]);
							//	//CMyTeMath::SBand_3(&Doc->Data,&BandArray[j],VegaArray[j],&Redden,X,Mz,&EXTIN)-VegaXArray[j]);
							//}
								ResultString.Append(_T("\n"));
							if(CMyTeMath::WriteResult1(FileList.OutFile.c_str(),ResultString,false)!=0) break;					
						}
					}
		
						b=false;
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




