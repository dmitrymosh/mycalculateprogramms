#pragma once
//#include "MyTeBand.h"

// CMyTeRespSelectDlg dialog

class CMyTeRespSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyTeRespSelectDlg)

public:
	CMyTeRespSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyTeRespSelectDlg();
	CString* BandNameArray;
	UINT BandCount;
// Dialog Data
	enum { IDD = IDD_RESP_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
