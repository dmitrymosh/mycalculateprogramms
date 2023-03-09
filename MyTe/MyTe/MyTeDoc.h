// MyTeDoc.h : interface of the CMyTeDoc class
//
#include "DataView.h"

#pragma once


class CMyTeDoc : public CDocument
{
protected: // create from serialization only
	CMyTeDoc();
	DECLARE_DYNCREATE(CMyTeDoc)
	
// Attributes
public:

// Operations
public:
	DocDataType Data;
	CString GraphList[40];
	ULONG32 GraphListLast;
	bool MainGraph;
	bool OutGraph;
	bool MainWGraph;
	bool Summ;
	bool IsFocus;
	bool Band;
	bool Responce;
	CDataView* Dlg;
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMyTeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//double GetFlux(DocDataType* data,double lambda);
	void LoadFile(wstring FileName);
	void LoadFITSFile(CFile* myFile);
	void RemoveItem(UINT i);
	ULONG32 SkipNotNumber(BYTE* szBuffer, ULONG32 begin,ULONG32 bufsize);
	ULONG32 CopyStrNumber(BYTE* szBuffer, ULONG32 begin, ULONG32 bufsize, wchar_t* dest,ULONG32 dsize);
	afx_msg void OnPop(ULONG32 nID);
	afx_msg void OnPop1(ULONG32 nID);
	afx_msg void OnPopMux();
	afx_msg void OnSubMagn();
	afx_msg void OnSubEnergy();
	afx_msg void OnSubEnergyCount();
	afx_msg void OnSubResponce();
	afx_msg void OnSaveEnergyCount(void);	
	afx_msg void OnSubNorm5500(void);
	afx_msg void OnViewData(void);
	afx_msg void OnViewResult(void);
	afx_msg void OnMinusRemove(void);
};


