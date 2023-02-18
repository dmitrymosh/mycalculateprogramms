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
	// �������� ����
	CDC* pDC;
	// ��������� ������ � ����
	CRect GraphPos;
	CRect WindowPos;
	// ������ �� ������
	CSize TextSize;
	// ������� ���� ������
	dataRECT DataPos;
	//�����
	CPen* Pen;	
	// ������ ������������ ��� � ��������
	void DrawPad(void);
	CFont* font;
	double AspectX;
	double AspectY;
	
	void DrawGrid(void);
	void DrawGraph(DocDataType* Data);
};
