// findDoc.cpp : ���������� ������ CfindDoc
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


// ��������/����������� CfindDoc

CfindDoc::CfindDoc()
{
	// TODO: �������� ��� ��� ������������ ������ ������������

}

CfindDoc::~CfindDoc()
{
}

BOOL CfindDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �������� ��� ��������� �������������
	// (��������� SDI ����� �������� ������������ ���� ��������)

	return TRUE;
}




// ������������ CfindDoc

void CfindDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �������� ��� ����������
	}
	else
	{
		// TODO: �������� ��� ��������
	}
}


// ����������� CfindDoc

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


// ������� CfindDoc
