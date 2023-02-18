// find.cpp : Определяет поведение классов для приложения.
//

#include "stdafx.h"
#include "find.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "findDoc.h"
#include "findView.h"
#include "FindStr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfindApp

BEGIN_MESSAGE_MAP(CfindApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CfindApp::OnAppAbout)
	// Стандартные команды по работе с файлами документов
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Стандартная команда настройки печати
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FINDSTR_1, &CfindApp::OnFindstr1)
END_MESSAGE_MAP()


// создание CfindApp

CfindApp::CfindApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CfindApp

CfindApp theApp;


// инициализация CfindApp

BOOL CfindApp::InitInstance()
{
	// InitCommonControlsEx() требуются для Windows XP, если манифест
	// приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
	// стилей отображения. В противном случае будет возникать сбой при создании любого окна.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
	// в вашем приложении.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Инициализация библиотек OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующего
	// конкретные процедуры инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));
	LoadStdProfileSettings(4);  // Загрузите стандартные параметры INI-файла (включая MRU)
	// Зарегистрируйте шаблоны документов приложения. Шаблоны документов
	//  выступают в роли посредника между документами, окнами фреймов и представлениями
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_findTYPE,
		RUNTIME_CLASS(CfindDoc),
		RUNTIME_CLASS(CChildFrame), // настраиваемый дочерний фрейм MDI
		RUNTIME_CLASS(CfindView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// создайте главное окно фрейма MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// вызов DragAcceptFiles только при наличии суффикса
	//  В приложении MDI это должно произойти сразу после задания m_pMainWnd


	// Синтаксический разбор командной строки на стандартные команды оболочки, DDE, открытие файлов
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Команды диспетчеризации, указанные в командной строке. Значение FALSE будет возвращено, если
	// приложение было запущено с параметром /RegServer, /Register, /Unregserver или /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Главное окно было инициализировано, поэтому отобразите и обновите его
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	/*ReferenceFile = _T("D:\\Mosh_Kom\\COMMON\\WORK\\ALL_ID_TYC.txt");
	SourceFile = _T("D:\\Mosh_Kom\\COMMON\\WORK\\MOUT_U_K_.txt");
	DestFile = _T("D:\\Mosh_Kom\\COMMON\\WORK\\ALL_ID_TYC_out.txt");*/
	ReferenceFile = _T("D:\\mecalculateprogramm\\MyTe\\LIST_WBVR8766_hd_S.txt");
	SourceFile = _T("D:\\mecalculateprogramm\\MyTe\\RES_TOT_DIFF_1006_S.txt");
	DestFile = _T("D:\\mecalculateprogramm\\MyTe\\resDAT_8766.txt");

	return TRUE;
}

static CString* LoadFromFile(CString fname,UINT& Count);

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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

static int WriteResult(CString fname, CString* data,UINT Count)
{
	FILE *outf;	
	CString fmode;
	if(TRUE)fmode=_T("w"); else fmode=_T("a");		
	_wfopen_s(&outf,fname,fmode);
	for (UINT i=0;i<Count;i++)
	{
		fputws(data[i],outf);
	}	
	fclose(outf);
	return 0;	
}
// Команда приложения для запуска диалога
void CfindApp::OnAppAbout()
{
	CAboutDlg aboutDlg;	
	if(aboutDlg.DoModal()==IDOK)
	{

	}

}
static CString* LoadFromFile(CString fname,UINT& Count)
{
	BYTE*	szBuffer; 
	ULONG32    nActual = 0; 	
	ULONG32 strcount=0;
	CFile* myFile;
	myFile=new CFile;
	CFileException ex;
	CString* outdata;
	//if( !f.Open( pFileName, CFile::modeCreate | CFile::modeWrite, &e ) )

	if (!myFile->Open(fname,CFile::modeRead,&ex ))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
	}else
	{	
		//myFile->Open(fname,CFile::modeRead);
		ULONG32 filesize=myFile->GetLength();
		szBuffer= new BYTE [filesize];
		nActual = myFile->Read( szBuffer, filesize ); 
		myFile->Close();
		if(nActual<filesize) filesize=nActual;	
		
		/*CString color;
		int r,g,b;
		wchar_t s[200];
		{
			wchar_t temp[200];
			int i=0;
			int bi=0;
			do{			
				temp[i]=szBuffer[bi];
				bi++;i++;
			}while((szBuffer[bi]!='\n')&&(bi<filesize));
			swscanf_s((wchar_t*)&temp[6],_T("%d %d %d"),&r,&g,&b);
		}
		GraphColor=RGB(r,g,b);*/
		//пропускаем заголовок
		//int head=CMyTeMath::SkipNotNumber(szBuffer,0,filesize);

		//подсчет строк
		for(ULONG32 i=0;i<filesize;i++)
		{
			if (szBuffer[i]==0x09)
			{
				szBuffer[i]=0x20;
			}
			/*
			if((szBuffer[i]==0x0A)&&(szBuffer[i+1]==0x20)&&(szBuffer[i-1]!=0x0D))
			{
			szBuffer[i]=0x0D;
			szBuffer[i+1]=0x0A;
			}*/
			if(/*((szBuffer[i]==0x0D)&&(szBuffer[i+1]==0x0A))||*/(szBuffer[i]==0x0A)) 
			{
				strcount++;
			}
		}
		//Data.Init(strcount,theApp.Options.NumberBand,strcount);	

		//создаём массивы
		//Count=strcount;
		//Lambda		=	new double[strcount];
		//Flux		=	new double[strcount];
		//FName		=	fname;
		//загружаем данные в массивы	
		outdata=new CString[strcount];
		wchar_t temp[10240];
		int bi=0;
		//bi=CMyTeMath::SkipNotNumber(szBuffer,bi,filesize);	
		for(ULONG32 j=0;j<strcount;j++)
		{	
			int i=0;		
			while((bi<filesize)&&(szBuffer[bi]!=0x0D)&&(szBuffer[bi]!=0x0A))
			{			
				temp[i]=szBuffer[bi];
				bi++;i++;
				temp[i]=0x00;
			}
			while((szBuffer[bi]==0x0D)||(szBuffer[bi]==0x0A))
			{
				bi++;
			}
			outdata[j].Append(temp);
		}
		delete[] szBuffer;
	}
	Count=strcount;
	return outdata;
}

static CString RemoveSpace(CString str)
{
	
	UINT Size1 = str.GetLength();
	UINT i=0;
	while ((i<Size1)&&((str[i] == ' ') || (str[i] == '\t')))
	{
		i++;
	}
	UINT Size2=Size1 - i +1;
	CString strout;	
	for (UINT j=i;j<Size1;j++)
	{
		if ((str[j]!=' ')&&(str[j]!='\t')&&(str[j]!='|'))
		{
			strout.AppendChar(str[j]);			
		}
// 		else {
// 			break;
// 		}		
	}
	return strout;
}

//Если вторая строка начинается с первой, за исключением пробелов, выдаёт true
//например:
//Str1="   62345234 ";
//Str2=" 62345234   dfgsrt453gadfg";
//
static bool CmpStr(CString str1,CString str2)
{
	
	UINT Size1=str1.GetLength();
	UINT Size2=str2.GetLength();
	if (Size1>Size2)
	{
		return false;
	}
	CString buf1 = RemoveSpace(str1);
	CString buf2 = RemoveSpace(str2);
	//CString buf1 = str1;
	//CString buf2 = str2;
	int result = str2.CompareNoCase(str1);
	if (result == 0)
	{
		return true;
	}
	return false;
}


// обработчики сообщений CfindApp


// Вычисление часовых углов для Сириуса_NEW
void CfindApp::OnFindstr2()
{
	//CString str1 = _T("   62345234 ");
	//CString str2 = _T(" 62345234   dfgsrt453gadfg");
	//CmpStr(str1,str2);
	CFindStr dlg;	
	dlg.v_ReferenceFile = ReferenceFile;// исходный файл с координатами
	dlg.v_SourceFile = SourceFile;//текущий файл наблюдений
	dlg.v_DestFile = DestFile;//результат
	if(dlg.DoModal()==IDOK)
	{
		CFile InEtalon;
		CFile InData;
		CFile OutData;
		UINT EtalonCount;
		UINT DataCount;
		CString* InEtalonStr = LoadFromFile(dlg.v_ReferenceFile,EtalonCount);
		CString* InDataStr = LoadFromFile(dlg.v_SourceFile,DataCount);
		
		CString* OutDataStr=new CString[DataCount];	

		bool good=false;
		UINT k=0;
		for(UINT i=0;i<DataCount;i++)
//		for(UINT i=0;i<EtalonCount;i++)
		{						
			int Eint=0;
			//swscanf_s(InEtalonStr[i],_T("%d"),&Eint);		
			/*for (UINT j=k;j<DataCount;j++)
			{*/					
				//swscanf_s(InDataStr[j],_T("%d"),&Dint);	


				//int l1 = InEtalonStr[i].GetLength();
				//int l2 = InDataStr[j].GetLength();

				//const wchar_t* pdest;				
				//const wchar_t* str1 = InDataStr[j];				
				//const wchar_t* str2 = InEtalonStr[i];				
				//pdest = wcsstr( InDataStr[j], InDataStr[j]);
				//pdest = wcsstr( str1, str2);
				//bool pdest = CmpStr(InEtalonStr[i],InDataStr[j]);

				//UINT t=0;
				//if (pdest) //if(Dint==Eint)
				//{
				//	k=j;
				//	good=true;
				//	break;
				//}				
			//}
			//if (good)
			//{
			//	OutDataStr[i]=InEtalonStr[i];//InDataStr[k];
			//	good=false;
			//}else
			//{
				OutDataStr[i]=InEtalonStr[i];
			//}
			OutDataStr[i].AppendChar(_T('\n'));
		}
		WriteResult(dlg.v_DestFile, OutDataStr,EtalonCount);
		
			AfxMessageBox(_T("End"));

		delete[] OutDataStr;
		delete[] InEtalonStr;
		delete[] InDataStr;
		ReferenceFile = dlg.v_ReferenceFile;
		SourceFile = dlg.v_SourceFile;
		DestFile = dlg.v_DestFile;
	}
}
#ifdef EMOE
void CfindApp::OnFindstr1()
{
	//CString str1 = _T("   62345234 ");
	//CString str2 = _T(" 62345234   dfgsrt453gadfg");
	//CmpStr(str1,str2);
	CFindStr dlg;	
	dlg.v_ReferenceFile = ReferenceFile;
	dlg.v_SourceFile = SourceFile;
	dlg.v_DestFile = DestFile;
	if(dlg.DoModal()==IDOK)
	{
		CFile InEtalon;
		CFile InData;
		CFile OutData;
		UINT EtalonCount;
		UINT DataCount;
		CString* InEtalonStr = LoadFromFile(dlg.v_ReferenceFile,EtalonCount);
		CString* InDataStr = LoadFromFile(dlg.v_SourceFile,DataCount);
		
		CString* OutDataStr=new CString[EtalonCount];	

		bool good=false;
		UINT k=0;
		
		for(UINT i=0;i<EtalonCount;i++)
		{						
			int Eint=0;
			//swscanf_s(InEtalonStr[i],_T("%d"),&Eint);		
			for (UINT j=k;j<DataCount;j++)
			{					
				//swscanf_s(InDataStr[j],_T("%d"),&Dint);	


				//int l1 = InEtalonStr[i].GetLength();
				//int l2 = InDataStr[j].GetLength();

				//const wchar_t* pdest;				
				//const wchar_t* str1 = InDataStr[j];				
				//const wchar_t* str2 = InEtalonStr[i];				
				//pdest = wcsstr( InDataStr[j], InDataStr[j]);
				//pdest = wcsstr( str1, str2);
				bool pdest = CmpStr(InEtalonStr[i],InDataStr[j]);

				UINT t=0;
				if (pdest) //if(Dint==Eint)
				{
					k=j;
					good=true;
					break;
				}				
			}
			if (good)
			{
				OutDataStr[i]=InDataStr[k];
				good=false;
			}else
			{
				OutDataStr[i]=InEtalonStr[i];
			}
			OutDataStr[i].AppendChar(_T('\n'));
		}
		WriteResult(dlg.v_DestFile, OutDataStr,EtalonCount);
		
			AfxMessageBox(_T("End"));

		delete[] OutDataStr;
		delete[] InEtalonStr;
		delete[] InDataStr;
		ReferenceFile = dlg.v_ReferenceFile;
		SourceFile = dlg.v_SourceFile;
		DestFile = dlg.v_DestFile;
	}
}
#endif
void CfindApp::OnFindstr1()
{
	CFindStr dlg;	
	dlg.v_ReferenceFile = ReferenceFile;
	dlg.v_SourceFile = SourceFile;
	dlg.v_DestFile = DestFile;
	if(dlg.DoModal()==IDOK)
	{
		CFile InEtalon;
		CFile InData;
		CFile OutData;
		UINT EtalonCount;
		UINT DataCount;
		CString* InEtalonStr = LoadFromFile(dlg.v_ReferenceFile,EtalonCount);
		CString* InDataStr = LoadFromFile(dlg.v_SourceFile,DataCount);
		
		CString* OutDataStr=new CString[EtalonCount];	

		bool good=false;
		UINT k=0;
		
		for(UINT i=0,k=0;i<EtalonCount;i++)
		{						
			for (UINT j=0;j<DataCount;j++)
			{
				//char str1[10];
				//char str2[10];
				//strcpy(str1,InEtalonStr[i],9);
				//memcpy(str2,InDataStr[j],9);
				bool pdest = CmpStr(InEtalonStr[i],InDataStr[j]);
				//if(strncmp(str1,str2,9)==0){
				//UINT t=0;
				if (pdest) 
				{
					OutDataStr[k]=InDataStr[j];
					OutDataStr[k].AppendChar(_T('\n'));
					k++;
					//good=true;
					//break;
				}				
			}
			//if (good)
			//{
			//	OutDataStr[i]=InDataStr[k];
			//	good=false;
			//}else
			//{
			//	OutDataStr[i]=InEtalonStr[i];
			//}
			//OutDataStr[i].AppendChar(_T('\n'));
		}
		WriteResult(dlg.v_DestFile, OutDataStr,EtalonCount);
		
			AfxMessageBox(_T("End"));

		delete[] OutDataStr;
		delete[] InEtalonStr;
		delete[] InDataStr;
		ReferenceFile = dlg.v_ReferenceFile;
		SourceFile = dlg.v_SourceFile;
		DestFile = dlg.v_DestFile;
	}
}
