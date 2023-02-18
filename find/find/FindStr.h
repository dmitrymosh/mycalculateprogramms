#pragma once
#include "afxwin.h"
#include "Resource.h"


// диалоговое окно CFindStr

class CFindStr : public CDialog
{
	DECLARE_DYNAMIC(CFindStr)

public:
	CFindStr(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CFindStr();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit ReferenceFile;
	CString v_ReferenceFile;
	CEdit c_SourceFile;
	CString v_SourceFile;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit c_DestFile;
	CString v_DestFile;
};
