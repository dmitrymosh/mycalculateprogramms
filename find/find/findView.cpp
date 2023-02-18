// findView.cpp : реализация класса CfindView
//

#include "stdafx.h"
#include "find.h"

#include "findDoc.h"
#include "findView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfindView

IMPLEMENT_DYNCREATE(CfindView, CView)

BEGIN_MESSAGE_MAP(CfindView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// создание/уничтожение CfindView

CfindView::CfindView()
{
	// TODO: добавьте код создания

}

CfindView::~CfindView()
{
}

BOOL CfindView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// рисование CfindView

void CfindView::OnDraw(CDC* /*pDC*/)
{
	CfindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
}


// печать CfindView

BOOL CfindView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CfindView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CfindView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// диагностика CfindView

#ifdef _DEBUG
void CfindView::AssertValid() const
{
	CView::AssertValid();
}

void CfindView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfindDoc* CfindView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfindDoc)));
	return (CfindDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CfindView
