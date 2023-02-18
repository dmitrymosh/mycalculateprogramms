// MyTeView.cpp : implementation of the CMyTeView class
//

#include "stdafx.h"
#include "MyTeDoc.h"
#include "MyTeView.h"
#include "MyTe.h"
#include "GraphDraw.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTeView

IMPLEMENT_DYNCREATE(CMyTeView, CView)

BEGIN_MESSAGE_MAP(CMyTeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMyTeView construction/destruction

CMyTeView::CMyTeView()
{
	// TODO: add construction code here

}

CMyTeView::~CMyTeView()
{
}

BOOL CMyTeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	/*
	cs.style |= WS_SYSMENU ;
	CMenu m_NewMenu;
	CMenu m_FileMenu;

	VERIFY(m_NewMenu.CreateMenu());

	// Create a "File" popup menu and insert this popup menu to the
	// new menu of the application window. The "File" menu has only
	// one menu item, i.e. "Exit".
	VERIFY(m_FileMenu.CreatePopupMenu());
	m_FileMenu.AppendMenu(MF_STRING, ID_APP_EXIT, (LPCTSTR)"E&xit");
	m_NewMenu.AppendMenu(MF_POPUP, (ULONG32) m_FileMenu.m_hMenu, _T("&File"));

	// Add new menu.
	//SetMenu(&m_NewMenu);

	// Assign default menu
	cs.hMenu = m_NewMenu.m_hMenu;
*/
	return CView::PreCreateWindow(cs);
}

// CMyTeView drawing

void CMyTeView::OnDraw(CDC* pDC)
{
	CMyTeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(pDoc->Data.Count<=0) return;
	int count= theApp.GetOpenDocumentCount();
	POSITION pos=theApp.GetFirstDocTemplatePosition();
	CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
	POSITION pos1;
	

	int RectLeftPos=60;
	int RectRightPos=25;
	int RectTopPos=+25;
	int RectBottomPos=150;

	CWnd* hwnd=pDC->GetWindow();
	CRect lpRect;
	hwnd->GetClientRect(lpRect);
	CFont font;
	VERIFY(font.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	CPen BlackPen(0,1,RGB(0,0,0));
	CPen RedPen(0,1,RGB(255,0,0));	
	CPen GreenPen(0,1,RGB(0,255,0));
	CPen LightGreenPen(0,1,RGB(0,128,0));
	COLORREF colorArray[40];	
	int i=0;
	for(int r=64;r<256;r+=64)
	for(int g=64;g<256;g+=64)
	for(int b=64;b<256;b+=64)
	if((r!=256)&&(g!=256)&&(b!=256))
	{
		colorArray[i]=RGB(r,g,b);
		i++;
		if(i>=40) {r=256;g=256;b=256;}
	}
	CPen PenArray[40];
	for(int i=0; i<40;i++)
		PenArray[i].CreatePen(0,1,colorArray[i]);
	
		

	//pDC->Rectangle(lpRect.left+RectLeftPos,lpRect.top+RectTopPos,lpRect.right-RectRightPos+1,lpRect.bottom-RectBottomPos);	

	CPen TempPen(PS_SOLID,1,RGB(170,170,170));
	pDC->SelectObject(&TempPen);	
	//pDC->Rectangle(lpRect.left+RectLeftPos,lpRect.top+RectTopPos,lpRect.right-RectRightPos+1,lpRect.bottom-RectBottomPos);	
	
	
	
	double MaxLambda=0.0;	
	double MaxFlux=0.0;

	for(ULONG32 i=0;i<pDoc->Data.Count;i++)
	{
		if(MaxLambda<pDoc->Data.Lambda[i]) MaxLambda=pDoc->Data.Lambda[i];		
		if(MaxFlux<pDoc->Data.Flux[i]) MaxFlux=pDoc->Data.Flux[i];
		
	}

	double MinLambda=MaxLambda;	
	double MinFlux=MaxFlux;
	for(ULONG32 i=0;i<pDoc->Data.Count;i++)
	{		
		if(MinLambda>pDoc->Data.Lambda[i]) MinLambda=pDoc->Data.Lambda[i];		
		if(MinFlux>pDoc->Data.Flux[i]) MinFlux=pDoc->Data.Flux[i];
	}
	///
	RECT GPos;
	GPos.bottom=lpRect.bottom;
	GPos.left=lpRect.left;
	GPos.right=lpRect.right;
	GPos.top=lpRect.top;
	dataRECT DPos;
	DPos.bottom=MaxFlux;
	DPos.left=MinLambda;
	DPos.right=MaxLambda;
	DPos.top=MinFlux;

	CGraphDraw* uuuu=new CGraphDraw(pDC,GPos,DPos,&TempPen, &font);
	uuuu->DrawPad();
	uuuu->DrawGrid();


	///
	if(count>0)
	{
		pos1=dtempl->GetFirstDocPosition();
		for(int di=0;di<count;di++)
		{	
			CMyTeDoc* doc=(CMyTeDoc*)dtempl->GetNextDoc(pos1);			
			CString docTitle=doc->GetTitle();
			if(pDoc->GraphListLast>0)
			{
				for(int j=0;j<40;j++)
				{
					if(pDoc->GraphList[j]==docTitle)
					{
						for(ULONG32 i=0;i<doc->Data.Count;i++)
						{
							if(MaxLambda<doc->Data.Lambda[i]) MaxLambda=doc->Data.Lambda[i];		
							if(MaxFlux<doc->Data.Flux[i]) MaxFlux=doc->Data.Flux[i];		
						}
						for(ULONG32 i=0;i<doc->Data.Count;i++)
						{		
							if(MinLambda>doc->Data.Lambda[i]) MinLambda=doc->Data.Lambda[i];		
							if(MinFlux>doc->Data.Flux[i]) MinFlux=doc->Data.Flux[i];
						}
					}
				}
			}
		}
	}
	if(theApp.Options.GraphEnd!=0) MaxLambda=theApp.Options.GraphEnd;
	if(theApp.Options.GraphBeg!=0)MinLambda=theApp.Options.GraphBeg;


	double AspectX=(lpRect.Size().cx-(RectLeftPos+RectRightPos)-2)/(MaxLambda-MinLambda);
	double AspectY=(lpRect.Size().cy-(RectTopPos+RectBottomPos)-2)/(MaxFlux-MinFlux);
	
	//Вывод исходных данных
	if(pDoc->MainGraph)
	{
		pDC->SetTextColor(RGB(255,0,0));//RED
		if(!pDoc->Band)
		{
			CString Title=pDoc->GetTitle();
			pDC->TextOut(300,lpRect.bottom-RectBottomPos+20,Title);
		}
		pDC->SelectObject(&RedPen);
		bool b=true;
		uuuu->DrawGraph(&pDoc->Data);
		/*
		for(ULONG32 i=0;i<pDoc->Data.Count;i++)
		{
			if((pDoc->Data.Lambda[i]>=MinLambda)&&(pDoc->Data.Lambda[i]<=MaxLambda))
			{		
				double X=(pDoc->Data.Lambda[i]-MinLambda)*AspectX+RectLeftPos+1;
				double Y=(MaxFlux-pDoc->Data.Flux[i])*AspectY+RectTopPos+1;
				if(i==0) pDC->MoveTo(X,Y);
				else pDC->LineTo(X,Y);
			}
		}*/
	}
	if(pDoc->MainWGraph)
	{
		pDC->SelectObject(&GreenPen);		
		for(ULONG32 i=0;i<pDoc->Data.Count;i++)
		{
			if((pDoc->Data.Lambda[i]>=MinLambda)&&(pDoc->Data.Lambda[i]<=MaxLambda))
			{		
				double FluxIn = pDoc->Data.Flux[i]*3/(pDoc->Data.Lambda[i]*pDoc->Data.Lambda[i]);
				double X=(pDoc->Data.Lambda[i]-MinLambda)*AspectX+RectLeftPos+1;
				double Y=(MaxFlux-FluxIn)*AspectY+RectTopPos+1;
				if(i==0) pDC->MoveTo((int)X,(int)Y);
				else pDC->LineTo((int)X,(int)Y);
			}
		}
	}
	//Вывод результата
	if(pDoc->OutGraph)
	if(pDoc->Data.OutCount>0)
	{
		pDC->SelectObject(&LightGreenPen);
		for(ULONG32 i=0;i<pDoc->Data.OutCount;i++)
		{
			if((pDoc->Data.LambdaOut[i]>=MinLambda)&&(pDoc->Data.LambdaOut[i]<=MaxLambda))
			{
				double FluxOut=pDoc->Data.FluxOutW[i];//*3/(pDoc->Data.LambdaOut[i]*pDoc->Data.LambdaOut[i]);
				double X=(pDoc->Data.LambdaOut[i]-MinLambda)*AspectX+RectLeftPos+1;
				double Y=(MaxFlux-FluxOut)*AspectY+RectTopPos+1;
				if(pDoc->Data.FluxOutW[i]>0)
				{
					pDoc->Data.FluxOutW[i]=pDoc->Data.FluxOutW[i];
				}
				if(i==0) pDC->MoveTo((int)X,(int)Y);
				else
				{
					RECT r;
					int s=2;
					r.bottom=(int)Y+s;
					r.left=(int)X-s;
					r.right=(int)X+s;
					r.top=(int)Y-s;
					pDC->Ellipse(&r); 
					pDC->LineTo((int)X,(int)Y);
				}
			}
		}
	}
	

	
	if(count>0)
	{
		pos1=dtempl->GetFirstDocPosition();
		for(int di=0;di<count;di++)
		{	
			CMyTeDoc* doc=(CMyTeDoc*)dtempl->GetNextDoc(pos1);			
			CString docTitle=doc->GetTitle();
			if(pDoc->GraphListLast>0)
			{
				for(int j=0;j<40;j++)
				{
					if(pDoc->GraphList[j]==docTitle)
					{
						pDC->SelectObject(&PenArray[di]);
						if(!pDoc->Band)
						{
							pDC->SetTextColor(colorArray[di]);
							int xpos;int ypos;
							if(di<6){xpos=300; ypos=lpRect.bottom-RectBottomPos+16*(di+1)+20;} 
							if((di>=6)&&(di<14)){xpos=500;ypos=lpRect.bottom-RectBottomPos+16*(di-6)+20;}
							if((di>=14)){xpos=700;ypos=lpRect.bottom-RectBottomPos+16*(di-14)+20;}
							pDC->TextOut(xpos,ypos,pDoc->GraphList[di]);
						}
						bool b=true;
						for(ULONG32 i=0;i<doc->Data.Count;i++)
						{
							if((doc->Data.Lambda[i]>=MinLambda)&&(doc->Data.Lambda[i]<=MaxLambda))
							{		
								double X=(doc->Data.Lambda[i]-MinLambda)*AspectX+RectLeftPos+1;
								double Y=(MaxFlux-doc->Data.Flux[i])*AspectY+RectTopPos+1;
								if(i==0) pDC->MoveTo((int)X,(int)Y);
								if(b) 
								{
									pDC->MoveTo((int)X,(int)Y);
									b=false;
								}
                                                                else { 
                                                                    pDC->LineTo((int)X,(int)Y);
                                                                }
							}
						}			
					}
				}
			} 
		}
	}
	if(pDoc->Responce&&(theApp.BandCount>0))
	for(ULONG32 j = 0; j < theApp.BandCount; j++) {	
		CPen Pen(1,1,theApp.BandArray[j].GraphColor);
		pDC->SelectObject(&Pen);
		bool b=true;// theApp.BandArray[0]
		for(ULONG32 i=0;i<pDoc->Data.Count;i++)
		{
			if((pDoc->Data.Lambda[i]>=MinLambda)&&(pDoc->Data.Lambda[i]<=MaxLambda))
			{		
				double TY=theApp.BandArray[j].GetFlux(pDoc->Data.Lambda[i])*MaxFlux;
				if(TY!=0)
				{
					double X=(pDoc->Data.Lambda[i]-MinLambda)*AspectX+RectLeftPos+1;
					double Y=(MaxFlux-TY)*AspectY+RectTopPos+1;
					if(i==0) 
                                            pDC->MoveTo((int)X,(int)Y);
					else 
                                            pDC->LineTo((int)X,(int)Y);
				}
			}
		}
	}
	if(pDoc->Responce&&(theApp.Redden.Count>0))
	{
		CPen Pen(1,1,theApp.Redden.GraphColor);
		pDC->SelectObject(&Pen);
		bool b=true;// theApp.BandArray[0]
		for(ULONG32 i=0;i<pDoc->Data.Count;i++)
		{
			if((pDoc->Data.Lambda[i]>=MinLambda)&&(pDoc->Data.Lambda[i]<=MaxLambda))
			{		
				double TY=theApp.Redden.GetFlux(pDoc->Data.Lambda[i])*MaxFlux;
				if(TY!=0)
				{
					double X=(pDoc->Data.Lambda[i]-MinLambda)*AspectX+RectLeftPos+1;
					double Y=(MaxFlux-TY)*AspectY+RectTopPos+1;
					if(i==0) 
                                            pDC->MoveTo((int)X,(int)Y);
					else 
                                            pDC->LineTo((int)X,(int)Y);
				}
			}
		}
	}	

	

	// Do something with the font just created...
	 /*
	CFont* def_font = pDC->SelectObject(&font);
	pDC->SelectObject(def_font);
	font.DeleteObject(); 
	//pDC->TextOut(5, 5, _T("Hello"), 5);
	
	wchar_t buf[20];
	int c=swprintf( buf, 15, L"%7.2G", MinFlux);
	pDC->TextOut(2,lpRect.bottom-RectBottomPos-12,buf,c);
	c=swprintf( buf, 15, L"%7.2G", MaxFlux);
	pDC->TextOut(2,lpRect.top+RectTopPos-0,buf,c);
	c=swprintf( buf, 15, L"%7.2G", MinLambda);
	pDC->TextOut(lpRect.left+RectLeftPos-2*c,lpRect.bottom-RectBottomPos+5,buf,c);
	c=swprintf( buf, 15, L"%7.2G", MaxLambda);
	pDC->TextOut(lpRect.right-RectRightPos-6*c,lpRect.bottom-RectBottomPos+5,buf,c);

	CString OutText1=(_T("Summ Flux: "));
	CString OutText2=(_T("  Summ Err Flux: "));
	CString OutTextAll;
	if(pDoc->Summ)
	{

		for(int i=0;i<theApp.Options.NumberBand;i++)
		{	
			swprintf( buf, 20, L"%10.4f", pDoc->Data.SummFlux[i]);
			OutTextAll=OutText1;
			OutTextAll.Append(buf);
			swprintf( buf, 15, L"%5.4f", pDoc->Data.SummErrFlux[i]);
			OutTextAll.Append(OutText2);
			OutTextAll.Append(buf);
			pDC->TextOut(lpRect.left+10,lpRect.bottom-16*i,OutTextAll);
		}
	}

	OutText1=(_T("Lambda from : "));
	OutText2=(_T("  to:  "));	

	swprintf( buf, 20, L"%10.4f", theApp.Options.GraphBeg);
	OutTextAll=OutText1;
	OutTextAll.Append(buf);
	swprintf( buf, 20, L"%5.4f", theApp.Options.GraphEnd);
	OutTextAll.Append(OutText2);
	OutTextAll.Append(buf);
	pDC->TextOut(lpRect.left+10,lpRect.top+3,OutTextAll);
	//OutTextAll=_T("Foton Count= ");
	if(pDoc->Band)
	for(int i=0;i<theApp.Options.NumberBand;i++)
	{

		OutTextAll=_T("Band:[");
		OutTextAll.AppendFormat(_T("%5.2f"),theApp.Options.Lambda_Beg[i]);
		OutTextAll.Append(_T(":"));
		OutTextAll.AppendFormat(_T("%5.2f"),theApp.Options.Lambda_End[i]);
		OutTextAll.Append(_T("] FC="));
		OutTextAll.AppendFormat(_T("%5.8le"),pDoc->Data.FotonCount[i]);
		OutTextAll.Append(_T(", Em="));
		OutTextAll.AppendFormat(_T("%5.8le"),pDoc->Data.EmFluxWsm2A_7[i]);
		pDC->TextOut(lpRect.left+400,lpRect.bottom-16-16*i,OutTextAll);
	}
	*/


	/*
	for(int i=0;i<6;i++)
	{	
		pDC->SetTextColor(colorArray[i]);
		pDC->TextOut(300,lpRect.bottom-RectBottomPos+16*(i+1)+20,pDoc->GraphList[i]);
	}
	for(int i=6;i<14;i++)
	{
		pDC->SetTextColor(colorArray[i]);
		pDC->TextOut(500,lpRect.bottom-RectBottomPos+16*(i-6)+20,pDoc->GraphList[i]);
	}
	*/


	// Done with the font.  Delete the font object.
	//pDC->SelectObject(def_font);
	//font.DeleteObject(); 
	delete uuuu;
	

}


// CMyTeView printing

BOOL CMyTeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyTeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyTeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMyTeView diagnostics

#ifdef _DEBUG
void CMyTeView::AssertValid() const
{
	CView::AssertValid();
}

void CMyTeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyTeDoc* CMyTeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyTeDoc)));
	return (CMyTeDoc*)m_pDocument;
}
#endif //_DEBUG

// CMyTeView message handlers
void CMyTeView::OnRButtonDown(ULONG32 nFlags, CPoint point) 
{
	CView::OnRButtonDown(nFlags, point);

	//CMenu* menu_bar = new CMenu();// AfxGetMainWnd()->GetMenu();
	//menu_bar->LoadMenu(IDR_MENU1);
	//menu_bar->CreatePopupMenu();
	//ASSERT(menu_bar);ID_1_40
	
	CMenu* PopUpMenuTop = new CMenu();    
	ASSERT(PopUpMenuTop);
	PopUpMenuTop->CreatePopupMenu();

	CMenu* MenuSave = new CMenu();   
	ASSERT(MenuSave);
	MenuSave->CreatePopupMenu();
	MenuSave->AppendMenu(MF_STRING, ID_SAVE_ENERGY_COUNT,_T("Сохранить количество"));

	CMenu* MenuGraphs = new CMenu();   
	ASSERT(MenuGraphs);
	MenuGraphs->CreatePopupMenu();
	
	CMenu* MenuFunction = new CMenu();   
	ASSERT(MenuFunction);
	MenuFunction->CreatePopupMenu();
	//MenuFunction->AppendMenu(MF_SEPARATOR);
	MenuFunction->AppendMenu(MF_STRING, ID_FUNC_MAGN,_T("Магнитуда"));
	MenuFunction->AppendMenu(MF_STRING, ID_FUNC_ENERGY,_T("Распределение энегрий"));
	MenuFunction->AppendMenu(MF_STRING, ID_FUNC_COUNT,_T("Количество энегрии"));
	MenuFunction->AppendMenu(MF_STRING, ID_FUNC_RESPONCE,_T("Обработать фильтрами"));
	MenuFunction->AppendMenu(MF_STRING, ID_FUNC_NORM_5500,_T("Нормализовать по 5500"));

	PopUpMenuTop->AppendMenu(MF_POPUP,(INT_PTR)MenuFunction->GetSafeHmenu(), _T("Функции"));
	PopUpMenuTop->AppendMenu(MF_POPUP,(INT_PTR)MenuSave->GetSafeHmenu(), _T("Сохранить"));
	
	//PopUpMenuTop->AppendMenu(MF_BYPOSITION,ID_FUNCT,_T("Функции"));
	PopUpMenuTop->AppendMenu(MF_SEPARATOR);
	CMyTeDoc* Curdoc=(CMyTeDoc*)GetDocument();
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

			CString docTitle=doc->GetTitle();
			bool notinlist=true;
			if(Curdoc->GetTitle()==docTitle)
			{
				MenuGraphs->AppendMenu(MF_STRING|MF_CHECKED|MF_DISABLED, ID_1_1+i, doc->GetTitle().GetString());
			}else 
			{	
				if(Curdoc->GraphListLast>0)
				{
					for(int j=0;j<40;j++)
					{
						if(Curdoc->GraphList[j]==docTitle)
						{
							MenuGraphs->AppendMenu(MF_STRING|MF_CHECKED, ID_1_1+i, doc->GetTitle().GetString());	
							notinlist=false;
						}

					}
				} 
				if(notinlist)
					MenuGraphs->AppendMenu(MF_STRING, ID_1_1+i, doc->GetTitle().GetString());
			}
				//PopUpMenuTop->AppendMenu(MF_STRING, ID_APP_EXIT, doc->GetTitle().GetString());
			//dtempl->InitialUpdateFrame(dtempl->GetRoutingFrame_(),doc);
		}	
	}
	PopUpMenuTop->AppendMenu(MF_POPUP,(INT_PTR)MenuGraphs->GetSafeHmenu(), _T("наложить график"));
	PopUpMenuTop->AppendMenu(MF_SEPARATOR);
	ULONG32 GraphStatusM=Curdoc->MainGraph?MF_CHECKED:MF_UNCHECKED;
	ULONG32 GraphStatusMW=Curdoc->MainWGraph?MF_CHECKED:MF_UNCHECKED;
	ULONG32 GraphStatusO=Curdoc->OutGraph?MF_CHECKED:MF_UNCHECKED;
	ULONG32 GraphSumm=Curdoc->Summ?MF_CHECKED:MF_UNCHECKED;
	ULONG32 GraphBand=Curdoc->Band?MF_CHECKED:MF_UNCHECKED;
	ULONG32 GraphResp=Curdoc->Responce?MF_CHECKED:MF_UNCHECKED;
	PopUpMenuTop->AppendMenu(MF_STRING|GraphStatusM, ID_POP_10, _T("Исходный"));
	PopUpMenuTop->AppendMenu(MF_STRING|GraphStatusMW, ID_POP_11, _T("Исходный в W/cm*mkm"));
	PopUpMenuTop->AppendMenu(MF_STRING|GraphStatusO, ID_POP_12, _T("Результат"));
	PopUpMenuTop->AppendMenu(MF_SEPARATOR, ID_POP_10, _T("-"));
	PopUpMenuTop->AppendMenu(MF_STRING|GraphSumm, ID_POP_MUX, _T("Отображать суммы"));
	PopUpMenuTop->AppendMenu(MF_STRING|GraphBand, ID_POP_FOT, _T("Отображать Фотоны"));
	PopUpMenuTop->AppendMenu(MF_STRING|GraphResp, ID_POP_RESPONCE, _T("Кривые реакций"));
	RECT rect;
	this->GetWindowRect(&rect);
	PopUpMenuTop->TrackPopupMenu(TPM_RIGHTALIGN |TPM_RIGHTBUTTON,rect.left+point.x, 
		rect.top+point.y, this);
	PopUpMenuTop->DestroyMenu();
}




void CMyTeView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
//IsFocus
	// TODO: Add your message handler code here
	CMyTeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CMyTeDoc* Curdoc=(CMyTeDoc*)GetDocument();
	int count= theApp.GetOpenDocumentCount();
	if(count>0)
	{
		POSITION pos=theApp.GetFirstDocTemplatePosition();
		CMultiDocTemplate* dtempl=(CMultiDocTemplate*)theApp.GetNextDocTemplate(pos);
		POSITION pos1=dtempl->GetFirstDocPosition();
		for(int i=0;i<count;i++)
		{	
			CMyTeDoc* doc=(CMyTeDoc*)dtempl->GetNextDoc(pos1);
			doc->IsFocus=false;
		}
		pDoc->IsFocus=true;
	}
}
