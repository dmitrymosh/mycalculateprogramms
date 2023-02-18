// findDoc.h : интерфейс класса CfindDoc
//


#pragma once


class CfindDoc : public CDocument
{
protected: // создать только из сериализации
	CfindDoc();
	DECLARE_DYNCREATE(CfindDoc)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Реализация
public:
	virtual ~CfindDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};


