// findDoc.h : ��������� ������ CfindDoc
//


#pragma once


class CfindDoc : public CDocument
{
protected: // ������� ������ �� ������������
	CfindDoc();
	DECLARE_DYNCREATE(CfindDoc)

// ��������
public:

// ��������
public:

// ���������������
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����������
public:
	virtual ~CfindDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
};


