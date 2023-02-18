#include "StdAfx.h"
#include "GraphDraw.h"

CGraphDraw::CGraphDraw(CDC* _pDC,CRect GPos,dataRECT DPos, CPen* _Pen,CFont* _font)
:font(_font)
,pDC(_pDC)
,Pen(_Pen)
,WindowPos(GPos)
,DataPos(DPos)
{
	CString temp;	
	temp.AppendFormat(_T("%8.2G"), -2.6E-100);
	int r=4;
	CFont* def_font = pDC->SelectObject(font);
	TextSize=pDC->GetOutputTabbedTextExtent(temp,0,&r);	
	GraphPos.top = WindowPos.top+TextSize.cy+5;
	GraphPos.left = WindowPos.left+TextSize.cx+5;
	GraphPos.right = WindowPos.right-5;
	GraphPos.bottom = WindowPos.bottom-TextSize.cy*2;
	AspectX=(GraphPos.right-GraphPos.left)/(DataPos.right-DataPos.left);
	AspectY=(GraphPos.bottom-GraphPos.top)/(DataPos.bottom-DataPos.top);
	
	
}

CGraphDraw::~CGraphDraw(void)
{
	//font.DeleteObject(); 
}



// Рисует координатные оси и разметку
void CGraphDraw::DrawPad(void)
{
	CRect t=GraphPos;
	t.bottom+=3;
	t.right+=3;
	pDC->SelectObject(Pen);	
	pDC->Rectangle(&t);	
	//pDC->Rectangle(GraphPos.top,GraphPos.left,GraphPos.right,GraphPos.bottom);	

}

void CGraphDraw::DrawGrid(void)
{
	double dx = DataPos.right-DataPos.left;
	double dy = DataPos.bottom-DataPos.top;
	double sizex=10.0;
	double sizey=10.0;
	double pow10x=1;
	double pow10y=1;
	if (dx>0)
	{
		if (dx<=sizex)
		{	
			while ((UINT)dx<=sizex)
			{
				pow10x*=10;
				dx*=10.0;
			}
		}
		else
		{
			if(dx>=sizex)
			{
				while ((UINT)dx>=sizex)
				{
					pow10x/=10;
					dx/=10.0;
				}
			}
		}
	}
	if (dy>0)
	{
		if (dy<=sizey)
		{	
			while ((UINT)dy<=sizey)
			{
				pow10y*=10;
				dy*=10.0;
			}
		}
		else
		{
			if(dy>=sizey)
			{
				while ((UINT)dy>=sizey)
				{
					pow10y/=10;
					dy/=10.0;
				}
			}
		}
	}
	{
		wchar_t buf[20];
		CFont* def_font = pDC->SelectObject(font);
		pDC->SelectObject(def_font);		
		UINT corrx = DataPos.left*pow10x;
		UINT corry = DataPos.top*pow10y;
		for (UINT i=0;i<dx;i++)
		{
			double X=((i+corrx)/(pow10x)-DataPos.left)*AspectX+GraphPos.left;
			double Y=GraphPos.bottom;
			
			double mm = fmod((i+corrx), 10.0);
			if ((X>=GraphPos.left)&&(X<=GraphPos.right))
			{
				if ( mm == 0 )
				{
					pDC->MoveTo(X,Y-1);	
					pDC->LineTo(X,GraphPos.top);
					int c=swprintf( buf, 15, L"%7.2G", (i+corrx)/pow10x);
					pDC->TextOut(X-TextSize.cx/2,GraphPos.bottom+5,buf,c);

				}else 
				{
					pDC->MoveTo(X,GraphPos.top);	
					pDC->LineTo(X,GraphPos.top+5);
					pDC->MoveTo(X,Y-1);	
					pDC->LineTo(X,Y-5);
				}
			}
		}
		
		for (double i=0;i<dy;i++)
		{
			double X=GraphPos.left;
			double Y=((i+corry)/(pow10y)-DataPos.top)*AspectY+GraphPos.top;

			double mm = fmod((i+corry), 10.0);
			if ((Y>=GraphPos.top)&&(Y<=GraphPos.bottom))
			{
				if ( mm == 0 )
				{
					pDC->MoveTo(X,Y);	
					pDC->LineTo(GraphPos.right,Y);
					int c=swprintf( buf, 15, L"%7.2G", (dy-(i+corry))/pow10y);
					pDC->TextOut(WindowPos.left+5,Y-TextSize.cy/2,buf,c);
				}else 
				{
					pDC->MoveTo(X,Y);	
					pDC->LineTo(X+5,Y);
					pDC->MoveTo(GraphPos.right,Y);	
					pDC->LineTo(GraphPos.right-5,Y);
				}
			}
		}
	}
}

void CGraphDraw::DrawGraph(DocDataType* Data)
{
	for(ULONG32 i=0;i<Data->Count;i++)
	{
		if((Data->Lambda[i]>=DataPos.left)&&(Data->Lambda[i]<=DataPos.right))
		{		
			//double FluxIn=Data.Flux[i]*3/(Data.Lambda[i]*Data.Lambda[i]);
			double X=(Data->Lambda[i]-DataPos.left)*AspectX+GraphPos.left+1;
			double Y=(DataPos.bottom-Data->Flux[i])*AspectY+GraphPos.top+1;
			if(i==0) pDC->MoveTo(X,Y);
			else pDC->LineTo(X,Y);
		}
	}
}

