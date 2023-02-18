// findView.h : ��������� ������ CfindView
//


#pragma once


class CfindView : public CView
{
protected: // ������� ������ �� ������������
	CfindView();
	DECLARE_DYNCREATE(CfindView)

// ��������
public:
	CfindDoc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual void OnDraw(CDC* pDC);  // �������������� ��� ��������� ����� �������������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����������
public:
	virtual ~CfindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ���������� ������ � findView.cpp
inline CfindDoc* CfindView::GetDocument() const
   { return reinterpret_cast<CfindDoc*>(m_pDocument); }
#endif

