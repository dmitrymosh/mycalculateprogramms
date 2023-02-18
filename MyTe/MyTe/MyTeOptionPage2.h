#pragma once


// CMyTeOptionPage2 dialog

class CMyTeOptionPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyTeOptionPage2)

public:
	CMyTeOptionPage2();
	virtual ~CMyTeOptionPage2();

// Dialog Data
	enum { IDD = IDD_OPTION_PAGE2 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
