// findView.h : интерфейс класса CfindView
//


#pragma once


class CfindView : public CView
{
protected: // создать только из сериализации
	CfindView();
	DECLARE_DYNCREATE(CfindView)

// Атрибуты
public:
	CfindDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CfindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // отладочная версия в findView.cpp
inline CfindDoc* CfindView::GetDocument() const
   { return reinterpret_cast<CfindDoc*>(m_pDocument); }
#endif

