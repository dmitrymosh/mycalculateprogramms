// findDoc.cpp : реализаци€ класса CfindDoc
//

#include "stdafx.h"
#include "find.h"

#include "findDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfindDoc

IMPLEMENT_DYNCREATE(CfindDoc, CDocument)

BEGIN_MESSAGE_MAP(CfindDoc, CDocument)
END_MESSAGE_MAP()


// создание/уничтожение CfindDoc

CfindDoc::CfindDoc()
{
	// TODO: добавьте код дл€ одноразового вызова конструктора

}

CfindDoc::~CfindDoc()
{
}

BOOL CfindDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (ƒокументы SDI будут повторно использовать этот документ)

	return TRUE;
}




// сериализаци€ CfindDoc

void CfindDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранени€
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}


// диагностика CfindDoc

#ifdef _DEBUG
void CfindDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CfindDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// команды CfindDoc
