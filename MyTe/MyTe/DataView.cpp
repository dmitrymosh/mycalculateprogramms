// DataView.cpp: файл реализации
//

#include "stdafx.h"
#include "MyTe.h"
#include "DataView.h"


// диалоговое окно CDataView

IMPLEMENT_DYNAMIC(CDataView, CDialog)

CDataView::CDataView(CWnd* pParent /*=NULL*/)
	: CDialog(CDataView::IDD, pParent)
{
// 	ColumnsCount = 0;
// 	for(int i = 0; i < MYTE_COLUMNS_COUNT; i++){
// 		ColumnsData[i].ColumnName = _T("");
// 		ColumnsData[i].Column = NULL;
// 		ColumnsData[i].ColLen = 0;		
// 	}
}

CDataView::~CDataView()
{
}

void CDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_VIEW_LIST, m_StoreItems);
}


BEGIN_MESSAGE_MAP(CDataView, CDialog)
	ON_WM_SIZING()
END_MESSAGE_MAP()

BOOL CDataView::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: добавьте свой код обработчика уведомлений
	m_StoreItems.SetExtendedStyle(LVS_EX_GRIDLINES);
	
	return true;
}

// обработчики сообщений CDataView
void CDataView::OnSizing( UINT nSide, LPRECT lpRect )
{
	CDialog::OnSizing(nSide,lpRect);
	//SWP_NOMOVE
	RECT lpRectClient;
	this->GetClientRect(&lpRectClient);
	int cx=lpRectClient.right - lpRectClient.left-20;
	int cy=lpRectClient.bottom-lpRectClient.top-20;

	m_StoreItems.SetWindowPos(&CWnd::wndBottom,0,0,cx,cy,SWP_NOMOVE);
}

void CDataView::AddColumn( wchar_t* ColumnName, double* Column, DWORD Len, DWORD Position, DWORD ColumnWidth )
{	
	LVITEM lvItem; 	
 	LVCOLUMN lvColumn;
	if(Position == 0){
 		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
 		lvColumn.fmt = LVCFMT_LEFT;
 		lvColumn.cx = 40;
 		lvColumn.pszText = _T("Index");		
 		m_StoreItems.InsertColumn(0, &lvColumn);
	}
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 40;
	lvColumn.pszText = ColumnName;
	m_StoreItems.InsertColumn(Position + 1, &lvColumn);
	m_StoreItems.SetColumnWidth(Position + 1, ColumnWidth);
	
 	int nItem;	
 	CString DataStr=_T(""); 	
 	CString Indexstr=_T("");
 	for (UINT i=0;i<Len;i++)
 	{
		if(Position == 0){
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			Indexstr.AppendFormat(_T("%d"),i);
			lvItem.pszText = Indexstr.GetBuffer();
			nItem = m_StoreItems.InsertItem(&lvItem);
		} else {
			nItem = i;
		}

 		DataStr.AppendFormat(_T("%8.4e"),Column[i]);	
 		m_StoreItems.SetItemText(nItem, Position + 1, DataStr.GetBuffer()); 		
 		
 		Indexstr=_T("");
 		DataStr=_T(""); 		
 	}	

}

void CDataView::AddColumn( char* ColumnName, double* Cloumn, DWORD Len, DWORD Position, DWORD ColumnWidth  )
{
	WCHAR wszColumnName[256+1]; 
	MultiByteToWideChar( CP_ACP, 0, ColumnName,
		strlen(ColumnName)+1, wszColumnName,   
		sizeof(wszColumnName)/sizeof(wszColumnName[0]) );
	AddColumn( wszColumnName, Cloumn, Len, Position, ColumnWidth );

}