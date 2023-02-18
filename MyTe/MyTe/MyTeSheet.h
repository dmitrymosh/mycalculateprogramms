#pragma once

#include "MyTeOptionPage1.h"
#include "OptionDialog.h"
#include "MyTeOptionPage2.h"
#include "MyTeOptionPage3.h"
#include "stdafx.h"

// CMyTeSheet

class CMyTeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyTeSheet)

public:
	CMyTeSheet(ULONG32 nIDCaption, CWnd* pParentWnd = NULL, ULONG32 iSelectPage = 0);
	CMyTeSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, ULONG32 iSelectPage = 0);
	virtual ~CMyTeSheet();
	COptionDialog PageMain;
	CMyTeOptionPage1 Page1;	
	CMyTeOptionPage2 Page2;
	CMyTeOptionPage3 Page3;

	SData_option TempOptions;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void AddPages(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};


