#pragma once

typedef struct st_dataRECT
{
	double top;
	double left;
	double right;
	double bottom;
} dataRECT;

class CGraphDraw
{
public:
	CGraphDraw(CDC* _pDC,CRect GPos,dataRECT DPos, CPen* _Pen, CFont* _font);
	~CGraphDraw(void);
	CString Header;
	CString Footer;
	// контекст окна
	CDC* pDC;
	// Положение рафика в окне
	CRect GraphPos;
	CRect WindowPos;
	// Отступ от границ
	CSize TextSize;
	// Позиция окна данных
	dataRECT DataPos;
	//ручка
	CPen* Pen;	
	// Рисует координатные оси и разметку
	void DrawPad(void);
	CFont* font;
	double AspectX;
	double AspectY;
	
	void DrawGrid(void);
	void DrawGraph(DocDataType* Data);
};
