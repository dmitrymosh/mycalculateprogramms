// findView.cpp : ���������� ������ CfindView
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
	// ����������� ������� ������
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// ��������/����������� CfindView

CfindView::CfindView()
{
	// TODO: �������� ��� ��������

}

CfindView::~CfindView()
{
}

BOOL CfindView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// ��������� CfindView

void CfindView::OnDraw(CDC* /*pDC*/)
{
	CfindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �������� ����� ��� ��������� ��� ����������� ������
}


// ������ CfindView

BOOL CfindView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ���������� �� ���������
	return DoPreparePrinting(pInfo);
}

void CfindView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �������� �������������� ������������� ����� �������
}

void CfindView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �������� ������� ����� ������
}


// ����������� CfindView

#ifdef _DEBUG
void CfindView::AssertValid() const
{
	CView::AssertValid();
}

void CfindView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfindDoc* CfindView::GetDocument() const // �������� ������������ ������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfindDoc)));
	return (CfindDoc*)m_pDocument;
}
#endif //_DEBUG


// ����������� ��������� CfindView
