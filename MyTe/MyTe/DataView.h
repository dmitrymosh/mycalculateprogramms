#pragma once
#include "resource.h"
// #define MYTE_COLUMNS_COUNT 100
// typedef struct COLUMNS
// {
// 	double		*Column;
// 	double		ColLen;	
// 	wchar_t *	ColumnName;
// } Columns;
// ���������� ���� CDataView

class CDataView : public CDialog
{
	DECLARE_DYNAMIC(CDataView)

public:
	CDataView(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDataView();

// ������ ����������� ����
	enum { IDD = IDD_VIEW_DATA_DLG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_StoreItems;
	BOOL OnInitDialog();
	void AddColumn(wchar_t* ColumnName, double* Cloumn, DWORD Len, DWORD Position, DWORD ColumnWidth  );
	void AddColumn(char* ColumnName, double* Cloumn, DWORD Len, DWORD Position, DWORD ColumnWidth  );
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
private:
	//Columns ColumnsData[MYTE_COLUMNS_COUNT];
	//int ColumnsCount;
	
};
