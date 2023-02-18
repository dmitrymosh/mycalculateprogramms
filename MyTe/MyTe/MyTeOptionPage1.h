#pragma once


// CMyTeOptionPage1 dialog

class CMyTeOptionPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyTeOptionPage1)

public:
	CMyTeOptionPage1();
	virtual ~CMyTeOptionPage1();

// Dialog Data
	enum { IDD = IDD_OPTION_PAGE1 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedAddBand();
	afx_msg void OnBnClickedRemoveBand();
	DECLARE_MESSAGE_MAP()
};
