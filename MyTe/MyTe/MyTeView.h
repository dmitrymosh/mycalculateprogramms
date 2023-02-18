// MyTeView.h : interface of the CMyTeView class
//


#pragma once


class CMyTeView : public CView
{
protected: // create from serialization only
	CMyTeView();
	DECLARE_DYNCREATE(CMyTeView)

// Attributes
public:
	CMyTeDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	
// Implementation
public:
	virtual ~CMyTeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void OnRButtonDown(ULONG32 nFlags, CPoint point);
	
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in MyTeView.cpp
inline CMyTeDoc* CMyTeView::GetDocument() const
   { return reinterpret_cast<CMyTeDoc*>(m_pDocument); }
#endif

