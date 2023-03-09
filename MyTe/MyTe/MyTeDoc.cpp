// MyTeDoc.cpp : implementation of the CMyTeDoc class
//

#include "stdafx.h"
#include "MyTe.h"
#include "MyTeDoc.h"
#include "MyTeMath.h"
#include "MyTeRespSelectDlg.h"
#include "DataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTeDoc

IMPLEMENT_DYNCREATE(CMyTeDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyTeDoc, CDocument)
	//ON_COMMAND(ID_POP_1, &CMyTeDoc::OnPop1)
	ON_COMMAND_RANGE(ID_1_1,ID_1_40,&CMyTeDoc::OnPop)
	ON_COMMAND_RANGE(ID_POP_10,ID_POP_12,&CMyTeDoc::OnPop1)
	ON_COMMAND(ID_POP_MUX,&CMyTeDoc::OnPopMux)
	ON_COMMAND(ID_FUNC_MAGN,&CMyTeDoc::OnSubMagn)
	ON_COMMAND(ID_FUNC_ENERGY,&CMyTeDoc::OnSubEnergy)
	ON_COMMAND(ID_FUNC_COUNT,&CMyTeDoc::OnSubEnergyCount)
	ON_COMMAND(ID_FUNC_RESPONCE,&CMyTeDoc::OnSubResponce)
	ON_COMMAND(ID_FUNC_NORM_5500,&CMyTeDoc::OnSubNorm5500)
	ON_COMMAND_RANGE(ID_POP_FOT,ID_POP_FOT,&CMyTeDoc::OnPop1)
	ON_COMMAND(ID_SAVE_ENERGY_COUNT,&CMyTeDoc::OnSaveEnergyCount)	
	ON_COMMAND_RANGE(ID_POP_RESPONCE,ID_POP_RESPONCE,&CMyTeDoc::OnPop1)
	ON_COMMAND(ID_VIEW_VIEWDATA,&CMyTeDoc::OnViewData)
	ON_COMMAND(ID_VIEW_VIEWRESULT,&CMyTeDoc::OnViewResult)
	ON_COMMAND(ID_ACTION_MINUS_REMOVE,&CMyTeDoc::OnMinusRemove)
END_MESSAGE_MAP()


// CMyTeDoc construction/destruction

CMyTeDoc::CMyTeDoc()
{
	// TODO: add one-time construction code here
	GraphListLast=0;
	for(int i=0;i<10;i++)
	{
		GraphList[i]=_T("");
	}
	MainGraph=true;
	OutGraph=false;
	MainWGraph=false;
	Summ=false;
	Band=false;
	Responce=false;
	/*
	Data.Count=0;
	Data.Lambda		=	NULL;
	Data.Flux		=	NULL;
	Data.ErrFlux	=	NULL;
	Data.NormErrFlux=	NULL;
	Data.SummErrFlux=	NULL;
	Data.SummNormErrFlux =NULL;
	Data.SummFlux	=	NULL;
	Data.OutCount=0;
	Data.ErrOut		=	NULL;
	Data.FluxOutW	=	NULL;
	Data.LambdaOut	=	NULL;
	Data.EmFluxWsm2A_7=	NULL;
	Data.FotonCount	=	NULL;
*/
}

CMyTeDoc::~CMyTeDoc()
{ 
	

}

BOOL CMyTeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMyTeDoc serialization

void CMyTeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		wstring filename = ar.GetFile()->GetFilePath();
		LoadFile(filename);
		//ar.GetFile()->Close();
		//LoadFITSFile(ar.GetFile());
	}
}


// CMyTeDoc diagnostics

#ifdef _DEBUG
void CMyTeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyTeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyTeDoc commands


void CMyTeDoc::LoadFile(wstring FileName)
{	
	this->Data.LoadFromFile(FileName);
	
	
	double LmBeg=0.0;
	double LmEnd=0.0;
	double LmStep=0.0;
	if (theApp.Options.StepLmkm>0)
	{
		LmStep=theApp.Options.StepLmkm;
		if (theApp.Options.LmkmBeg>0)
		{	
			LmBeg=theApp.Options.LmkmBeg;
		}else
		{
			LmBeg=Data.Lambda[0];
		}
		if(theApp.Options.LmkmEnd>0)
		{
			LmEnd=theApp.Options.LmkmEnd;
		}else
		{
			LmEnd=Data.Lambda[Data.Count-1];
		}
		Data.LmBeg=LmBeg;
		Data.LmEnd=LmEnd;
		Data.LmStep=LmStep;
		//OutCount=(int)(((LmEnd-LmBeg)/LmStep)+1);		
	}
	//Data.InitOut(OutCount);
	
	/*
	for(ULONG32 i=0;i<Data.Count;i++)
	{
		Data.Lambda[i]*=10000.0;
	}
	*/
	
	//CMyTeMath::MagnK(&Data,theApp.Options);
	//CMyTeMath::Energy_Distribution1(&Data,theApp.Options);
	//CMyTeMath::EnergyCount(&Data,theApp.Options);
}



void CMyTeDoc::LoadFITSFile(CFile* myFile)
{	
	BYTE*	szBuffer; 
	UINT    nActual = 0; 	
	UINT strcount=0;

	UINT filesize = (UINT)myFile->GetLength();
	szBuffer= new BYTE [filesize];
	nActual = myFile->Read( szBuffer, filesize ); 
	//myFile->Close();
	if(nActual<filesize) filesize=nActual;	
	using namespace std;

	//пропускаем заголовок
	//int head=CMyTeMath::SkipNotNumber(szBuffer,0,filesize);
	string str2 ( "Let me make this perfectly clear." );
	basic_string <char>::size_type indexCh1a;
	const char *cstr2 = "BINTABLE";
	indexCh1a = str2.find ( cstr2 , 0 );
	if ( indexCh1a != string::npos )

		AfxMessageBox(_T("The index of the 1st element of 'perfect' after the 5th position in str2"));
	else
		AfxMessageBox(_T("The substring 'perfect' was not found in str2 ."));

	
	////подсчет строк
	//for(ULONG32 i=head;i<filesize;i++)
	//{
	//	if (szBuffer[i]==0x09)
	//	{
	//		szBuffer[i]=0x20;
	//	}
	//	/*
	//	if((szBuffer[i]==0x0A)&&(szBuffer[i+1]==0x20)&&(szBuffer[i-1]!=0x0D))
	//	{
	//		szBuffer[i]=0x0D;
	//		szBuffer[i+1]=0x0A;
	//	}*/
	//	if(/*((szBuffer[i]==0x0D)&&(szBuffer[i+1]==0x0A))||*/(szBuffer[i]==0x0A)) 
	//	{
	//		strcount++;
	//	}
	//}
	//
	//Data.Init(strcount,theApp.Options.NumberBand);	

	////загружаем данные в массивы	
	//wchar_t temp[200];
	////int FieldCount;
	//LONG32 bi=0;
	//ULONG32 tj=0;
	//bool First=true;
	//bi=CMyTeMath::SkipNotNumber(szBuffer,bi,filesize);	
	//for(ULONG32 j=0;j<strcount;j++)
	//{	
	//	int i=0;		
	//	while((bi<filesize)&&(szBuffer[bi]!=0x0D)&&(szBuffer[bi]!=0x0A))
	//	{			
	//		temp[i]=szBuffer[bi];
	//		bi++;i++;
	//		temp[i]=0x00;
	//	}		
	//	while((szBuffer[bi]==0x0D)||(szBuffer[bi]==0x0A))
	//	{
	//		bi++;
	//	}
	//	
	//	swscanf_s((wchar_t*)&temp,_T("%lf%lf%lf%lf"),&Data.Lambda[j],&Data.Flux[j],
	//				&Data.ErrFlux[j],&Data.NormErrFlux[j]);
	//				
	//}
	//Data.Count=strcount;
	//UINT OutCount=strcount;
	//double LmBeg=0.0;
	//double LmEnd=0.0;
	//double LmStep=0.0;
	//if (theApp.Options.StepLmkm>0)
	//{
	//	LmStep=theApp.Options.StepLmkm;
	//	if (theApp.Options.LmkmBeg>0)
	//	{	
	//		LmBeg=theApp.Options.LmkmBeg;
	//	}else
	//	{
	//		LmBeg=Data.Lambda[0];
	//	}
	//	if(theApp.Options.LmkmEnd>0)
	//	{
	//		LmEnd=theApp.Options.LmkmEnd;
	//	}else
	//	{
	//		LmEnd=Data.Lambda[Data.Count-1];
	//	}
	//	Data.LmBeg=LmBeg;
	//	Data.LmEnd=LmEnd;
	//	Data.LmStep=LmStep;
	//	OutCount=((LmEnd-LmBeg)/LmStep)+1;		
	//}
	//Data.InitOut(OutCount);
	//
	///*
	//for(ULONG32 i=0;i<Data.Count;i++)
	//{
	//	Data.Lambda[i]*=10000.0;
	//}
	//*/
	//delete[] szBuffer;
	////CMyTeMath::MagnK(&Data,theApp.Options);
	////CMyTeMath::Energy_Distribution1(&Data,theApp.Options);
	////CMyTeMath::EnergyCount(&Data,theApp.Options);
}

void CMyTeDoc::OnPop(ULONG32 nID)
{
	// TODO: Add your command handler code here
	int count= theApp.GetOpenDocumentCount();
	if(count>0)
	{
		POSITION pos=theApp.GetFirstDocTemplatePosition();
		CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
		POSITION pos1=dtempl->GetFirstDocPosition();
		//нашли указанный график
		CDocument* doc;
		bool clear=false;
		for(int i=0;i<count;i++)
		{	
			doc=dtempl->GetNextDoc(pos1);
			if(i+ID_1_1==nID) 
			{
				i=count;				
			}
		}
		CString docTitle=doc->GetTitle();	
	
			bool b=false;
			for(int i=0;i<40;i++)
			{
				if(GraphList[i]==docTitle)
				{
					GraphList[i]=_T("");
					GraphListLast--;
					b=true;
				}
				if((b)&&(i<39))
				{
					GraphList[i]=GraphList[i+1];
				}
			}
		if(b==false)
		{
			GraphList[GraphListLast]=docTitle;
			GraphListLast++;
		}
	}
	//POSITION pos=theApp.GetFirstDocTemplatePosition();
	//CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
	//dtempl->InitialUpdateFrame(dtempl->GetRoutingFrame_(),this);
	//CMultiDocTemplate* dtempl=(CMultiDocTemplate*)this->GetDocTemplate();
	//dtempl->InitialUpdateFrame(this->GetRoutingFrame(),this);
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();


}

void CMyTeDoc::OnPop1( ULONG32 nID )
{
	switch(nID)
	{
	case ID_POP_10:MainGraph=MainGraph?false:true; break;
	case ID_POP_11:MainWGraph=MainWGraph?false:true; break;
	case ID_POP_12:OutGraph=OutGraph?false:true; break;
	case ID_POP_FOT:Band=Band?false:true;break;
	case ID_POP_RESPONCE:Responce=Responce?false:true;break;
		
	}

	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
	
}

void CMyTeDoc::OnPopMux()
{
	/*
	for(int i=0;i<Data.Count;i++)
	{	
		Data.Flux[i]*=10.0e+7;
	}
	*/
	Summ=Summ?false:true;
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}

void CMyTeDoc::OnSubMagn()
{
	CMyTeMath::MagnK(&Data,theApp.Options);
	Summ=true;
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}

void CMyTeDoc::OnSubEnergy()
{
	CMyTeMath::Energy_Distribution1_0(&Data,theApp.Options);
	OutGraph=true;
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}

void CMyTeDoc::OnSubEnergyCount()
{
	CMyTeMath::EnergyCount(&Data,theApp.Options);
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}
void CMyTeDoc::OnSaveEnergyCount(void)
{
	CFileDialog dlgFile(false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);
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
		CMyTeMath::SaveEnergy(&Data,theApp.Options,dlgFile.GetPathName(),true);
	}
	
}


void CMyTeDoc::OnSubResponce()
{
	CMyTeRespSelectDlg Dlg;
	Dlg.DoModal();
	for(UINT i=0;i<Dlg.BandCount;i++)
	{		 
		for(ULONG32 j=0;j<theApp.BandCount;j++)
		{
			if(theApp.BandArray[j].FName == Dlg.BandNameArray[i])
			{
				CMyTeMath::Energy_Distribution2(&Data,theApp.Options,&theApp.BandArray[j]);
			}
		}		
	}
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}
void CMyTeDoc::OnSubNorm5500(void)
{
	CMyTeMath::Norm5500(&Data,theApp.Options);
	POSITION pos= GetFirstViewPosition();
	CView* view=GetNextView(pos);
	view->RedrawWindow();
}

void CMyTeDoc::OnViewData( void )
{
	//CDataView Dlg;


	Dlg = new CDataView();
	//Check if new succeeded and we got a valid pointer to a dialog object
	if(Dlg != NULL)
	{
		BOOL ret = Dlg->Create(IDD_VIEW_DATA_DLG, 0);

		if(!ret)   //Create failed.
			AfxMessageBox(_T("Error creating Dialog"));
		Dlg->SetWindowText(this->GetPathName());
		//Dlg->AddColumn(_T("Lambda"), Data.Lambda, Data.Count,0, 120);
		//Dlg->AddColumn(_T("Flux"), Data.Flux, Data.Count,1 ,120);
		//Dlg->AddColumn("Lambda", Data.Lambda, Data.Count,2, 120);
		Dlg->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(_T("Error Creating Dialog Object"));
	}
}

void CMyTeDoc::OnMinusRemove( void )
{
	for (UINT i=0;i<Data.Count;i++)
	{
		while((Data.Flux[i]<0)&&(i<Data.Count))
		{
			RemoveItem(i);
		}
	}
}

void CMyTeDoc::RemoveItem( UINT i )
{
	if (Data.Count>0)
	{	
		if ((i<Data.Count)&&(i>=0))
		{
			for (size_t j=i;j<Data.Count-1;j++)
			{
				Data.Lambda[j]=Data.Lambda[j+1];
				Data.Flux[j]=Data.Flux[j+1];
			}
			//Data.Count--;
		}
	}
}

void CMyTeDoc::OnViewResult( void )
{
	//CDataView Dlg;


	Dlg = new CDataView();
	//Check if new succeeded and we got a valid pointer to a dialog object
	if(Dlg != NULL)
	{
		BOOL ret = Dlg->Create(IDD_VIEW_DATA_DLG, 0);
		if(!ret)   //Create failed.
			AfxMessageBox(_T("Error creating Dialog"));
		Dlg->SetWindowText(this->GetPathName());
		//Dlg->AddColumn(_T("Lambda"), Data.Lambda, Data.Count,0, 120);
		//Dlg->AddColumn(_T("Flux"), Data.Flux, Data.Count,1 ,120);
		Dlg->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(_T("Error Creating Dialog Object"));
	}
}