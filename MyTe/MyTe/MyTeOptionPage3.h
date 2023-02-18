#pragma once


// CMyTeOptionPage3 dialog

class CMyTeOptionPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyTeOptionPage3)

public:
	CMyTeOptionPage3();
	virtual ~CMyTeOptionPage3();

// Dialog Data
	enum { IDD = IDD_OPTION_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddBand();
	afx_msg void OnBnClickedRemoveBand();
	afx_msg void OnBnClickedAddRedden();
};
