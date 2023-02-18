#pragma once


// ���������� ���� CRowToColDlg

class CRowToColDlg : public CDialog
{
	DECLARE_DYNAMIC(CRowToColDlg)

public:
	CRowToColDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CRowToColDlg();

	virtual void OnFinalRelease();

// ������ ����������� ����
	enum { IDD = IDD_ROW_TO_COL };
	CString SourceFileLambda;
	CString SourceFileFlux;
	CString DestFolder;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedBrowseFileSource();
	afx_msg void OnBnClickedBrowseFileSource2();
	afx_msg void OnBnClickedBrowseFolderDest();
	afx_msg void OnBnClickedOk();
};
