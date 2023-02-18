#pragma once


// COptionDialog dialog

class COptionDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(COptionDialog)
	
public:
	COptionDialog();   // standard constructor
	virtual ~COptionDialog();

// Dialog Data
	enum { IDD = IDD_OPTION_DIALOG};
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	//afx_msg void OnBnClickedVegaButton();
	

	
};
