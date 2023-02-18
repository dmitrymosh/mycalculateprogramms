#pragma once


// CCalculateDlg dialog

class CCalculateDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalculateDlg)

public:
	CCalculateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalculateDlg();

// Dialog Data
	enum { IDD = IDD_MULTI_FLUX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddFile();
	afx_msg void OnBnClickedRemoveFile();
	afx_msg void OnBnClickedBrowseFile();
	afx_msg void OnBnClickedAddFolder();
	INT_PTR DoModal(bool NeedFolder=false);
	afx_msg void OnBnClickedBrowseFileVega();
	bool NeedFolder;
	//bool ToFolder;
};
