
#include "stdafx.h"
#include "MyTeMath.h"

#include <math.h>
#include <new>
#define MAX(a,b) a>=b?a:b
#define MIN(a,b) a<=b?a:b
#define MaxBand 4
#undef EMOE
#define EConst -1.0857362047581295691278222972915
#define AsRad 206264.8062470964
#define Pi2  6.283185307179586476925286766559
#define Pi  3.1415926535897932384626433832795
double SINfi=0.682887823460;
double COSfi=0.730523251217;
FILE *inpf,*outf,*errf;
#define len 16384
char InLin[16384];
//int SP_Cod[6][36]={ 
//					{0,80,81,82,83,84,85,86,87,88,89,90,// I sp class ic=0
//						91,92,93,94,95,96,97,99,-1},
//					{0,70,71,46,47,48,49,72,73,52,74,// II sp class ic=1
//						75,76,77,78,79,99,-1},
//					{0,70,71,46,47,48,49,50,51,52,53,// III sp class ic=2
//						54,55,56,57,58,59,60,61,62,63,
//						64,65,66,67,68,69,99,-1},
//					{0,70,71,46,47,34,35,36,37,38,39,// IV sp class ic=3
//						40,41,42,43,44,45,60,65,99,-1},
//					{0,1,2,3,4,5,6,7,8,9,10,11,12,13,// V sp class ic=4
//					14,15,16,17,18,19,20,21,22,23,
//					24,25,26,27,28,29,30,31,32,33,99,-1}, 
//					{0,1,2,3,4,5,6,7,8,9,10,11,12,13,// VI sp class ic=5
//						14,15,16,17,18,19,20,21,22,23,
//						24,25,26,27,28,29,30,31,32,33,99,-1} 
//				  };	
CMyTeMath::CMyTeMath(void) 
{
}

CMyTeMath::~CMyTeMath(void) 
{
}

double CMyTeMath::swscanfd(CString str)
{
	//
	double t=0.0;
	double d=0.0;
	double f=1.0;
	bool p=false;
	for(int i=0;i<str.GetLength();i++)
	{
		switch (str[i])
		{
		case ' ': break;
		case '1': t=1.0; break;
		case '2': t=2.0; break;
		case '3': t=3.0; break;
		case '4': t=4.0; break;
		case '5': t=5.0; break;
		case '6': t=6.0; break;
		case '7': t=7.0; break;
		case '8': t=8.0; break;
		case '9': t=9.0; break;
		case '0': t=0.0; break;
		case '.': p=true; break;
		default: break;				
		}
		if(str[i]!='.')
		{
			if(!p) d=(d*10.0+t); else 
			{
				f=f/10.0;
				d=d+t*f;
			}
		}
		
	}
	return d;
}

double CMyTeMath::MyTe_scanf(BYTE* buffer,ULONG size, ULONG& pos, ULONG& beg, ULONG& end)
{	
	double First=0.0;
	double Second=0.0;
	double Secondmul=0.1;
	double Third=0.0;
	double firstsign=1.0;
	double secondsign=1.0;
	bool mant=false;
	bool rate=false;
	
	if (pos>=size)
	{
		return -1;
	}
	//  
	bool go=true;
	for (;(pos<size)&&go;)
	{
		pos++;
		if ((buffer[pos]>='0')&&(buffer[pos]<='9')||(buffer[pos]=='+')||(buffer[pos]=='-'))
		{
			if ((buffer[pos-1]==0x20)||(buffer[pos-1]==0x09))
			{
				go=false;
			}
		}
	}
	beg=pos;	
	if (buffer[pos]=='+')
	{
		firstsign=1.0;
		pos++;
	}
	if (buffer[pos]=='-')
	{
		firstsign=-1.0;
		pos++;
	}
	while ((buffer[pos]>='0')&&(buffer[pos]<='9'))
	{
		First=First*10.0+(buffer[pos]-'0');
		pos++;
	}
	if ((buffer[pos]=='.'))
	{
		mant=true;
		pos++;
	}else 
	{
		end=pos;
		return First*firstsign;
	}
	//только, если есть часть после точки
	if (mant)
	{
		while ((buffer[pos]>='0')&&(buffer[pos]<='9'))
		{
			Second+=(buffer[pos]-'0')*Secondmul;
			Secondmul*=0.1;
			pos++;
		}
		if ((buffer[pos]=='e')||(buffer[pos]=='E'))
		{
			rate=true;
			pos++;
		}else 
		{
			end=pos;
			return (First+Second)*firstsign;
		}
		if (rate)
		{
			if (buffer[pos]=='+')
			{
				secondsign=1.0;
				pos++;
			}
			if (buffer[pos]=='-')
			{
				secondsign=-1.0;
				pos++;
			}
			while ((buffer[pos]>='0')&&(buffer[pos]<='9'))
			{
				Third=Third*10.0+(buffer[pos]-'0');
				pos++;
			}
			end=pos;
			return ((First+Second)*firstsign*pow(10,Third*secondsign));
		}
	}
	return 0.0;
}
// пропустить все строки, в которых первым знаком идет не цифра. 
//вернуть позицию с первой значащей цифрой после начала строки
UINT CMyTeMath::SkipNotNumber(BYTE* szBuffer, UINT begin,UINT bufsize)
{

	bool b=true;			
	for(UINT i=begin;i<bufsize;i++)
	{
		switch (szBuffer[i])
		{
		case ' ':case 0x09: break;
		case '\n': b=true; break;
		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case '-': 
		case 'e':case '+':
			if(b){return i;}
			break;
		default : b=false;
		}

	}
	return 0;
}

UINT CMyTeMath::CopyStrNumber(BYTE* szBuffer, UINT begin, UINT bufsize, wchar_t* dest,UINT dsize)
{
	bool b=true;
	UINT d=0;
	for(UINT j=0;j<dsize;j++) dest[j]='\0';
	for(UINT i=begin;i<bufsize;i++)
	{
		switch (szBuffer[i])
		{		
		case '0':case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case '-':
		case '+':
			{
				if(d<dsize)
				{
					dest[d]=szBuffer[i];
					d++;
				}
			}
			break;
		default : return i;
		}	
	}
	return 0;
}

int CMyTeMath::WriteResult(CString fname, CString rfname, DocDataType* data,bool Over)
{
	FILE *outf;	
	CString fmode;
	if(Over)fmode=_T("w"); else fmode=_T("a");
	CString outstr;
	CString outfname(rfname);
	
	outfname.Truncate(outfname.GetLength()-4);
	int pos=outfname.ReverseFind('\\');
	outstr=outfname.Right(outfname.GetLength()-pos-1);	
	//CString  outstring[255];
	wchar_t outstring[255];
	swprintf_s(&outstring[0],254,_T("%-20s %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n"),
		outstr.GetString(),
		data->SummFlux[0],data->SummErrFlux[0],
		data->SummFlux[1],data->SummErrFlux[1],
		data->SummFlux[2],data->SummErrFlux[2],
		data->SummFlux[1]-data->SummFlux[0],
		data->SummFlux[1]-data->SummFlux[2],
		data->SummFlux[0]-data->SummFlux[2]);
	_wfopen_s(&outf,fname,fmode);
	fputws(outstring,outf);
	fclose(outf);
	return 0;	
}
int CMyTeMath::WriteResult1(CString fname, CString data,bool Over)
{
	FILE *outf;	
	CString fmode;
	if(Over)fmode=_T("w"); else fmode=_T("a");		
	errno_t err;

	if( (err  = _wfopen_s(&outf,fname,fmode)) !=0 )
	{		
		//AfxMessageBox(_T("Файл не может быть открыт для записи."));
		return err;
	}
	else
	{
		fputws(data,outf);
		fclose(outf);
		return 0;	
	}
	
}
int CMyTeMath::SaveChangedFile(CString fname, DocDataType* data,SData_option option,bool Over)
{
	FILE *outf;	
	CString fmode;
	if(Over)fmode=_T("w"); else fmode=_T("a");	
	CString outstr;	
	_wfopen_s(&outf,fname,fmode);	
	wchar_t outstring[255];
	for(ULONG32 i=0;i<data->OutCount;i++)
	{
		swprintf_s(outstring,255,_T("%5.5f  %8.5E\n"),
										data->LambdaOut[i],data->FluxOutW[i]);//,data->ErrOut[i]);		
		fputws(outstring,outf);
	}
	fclose(outf);	
	return 0;
}
int CMyTeMath::SaveFile(CString fname, DocDataType* data,SData_option option,bool Over)
{
/*
	CFile myFile;
	CFileException fileException;
	if ( !myFile.Open( fname, CFile::modeCreate |   
		CFile::modeReadWrite, &fileException ) )
	{
		TCHAR szError[1024];
		fileException.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);	
		return -1;
	}	
	char outstring[255];
	for(ULONG32 i=0;i<data->Count;i++)
	{
		sprintf_s(outstring,255,"%5.5f %8.5e \n",
					data->Lambda[i],data->Flux[i]);	
		myFile.Write( outstring, 25 ); 
	}
	myFile.Flush();	
	myFile.Close();
*/

	FILE *outf;	
	CString fmode;
	if(Over)fmode=_T("w"); else fmode=_T("a");	
	CString outstr;	
	_wfopen_s(&outf,fname,fmode);
	if(!outf) return 0;
	wchar_t outstring[255];
	for(ULONG32 i=0;i<data->Count;i++)
	{
		swprintf_s(outstring,255,_T("%5.5f  %8.5e \n"),
			data->Lambda[i],data->Flux[i]);		
		fputws(outstring,outf);
	}
	fclose(outf);	
	return 0;
}
#ifdef BLOCK_OFF_LINE
int CMyTeMath::MagnK(DocDataType* data,SData_option option)
{
	double Summ_Flux=0.0f;

}
#endif
int CMyTeMath::MagnK(DocDataType* data,SData_option option)
{
	double Summ_Flux=0.0f;
	double Summ_ErrFlux=0.0f;
	ULONG32 StepCount=0;
	for(ULONG32 j=0;j<option.NumberBand;j++)
	{
		Summ_Flux=0.0;
//		Summ_ErrFlux=0.0;
		StepCount=0;
		for (ULONG32 i=1;i<data->Count;i++)
		{
			//if ()
			//{
			//}
			if((data->Lambda[i]>=option.Lambda_Beg[j]) && (data->Lambda[i]<=option.Lambda_End[j]))
			{
				Summ_Flux+=data->Flux[i]*
					(data->Lambda[i]-data->Lambda[i-1])/*/
					(data->Lambda[i]*data->Lambda[i])*/;			
				double a=data->NormErrFlux[i]*(data->Lambda[i]-data->Lambda[i-1])/
					(data->Lambda[i]*data->Lambda[i]);
//				Summ_ErrFlux+=a*a;
				StepCount++;	
			}
		}
		if(StepCount<=0) 
		{
//			data->SummErrFlux[j]=-1000.0;
			data->SummFlux[j]=-1000.0;
			return -1;
		}
//		data->SummErrFlux[j]=3.0*sqrt(Summ_ErrFlux);///(double)StepCount);

		if((option.Lambda_End[j]-option.Lambda_Beg[j]==0)||(Summ_Flux<=0))
		{
//			data->SummErrFlux[j]=-2000.0;
			data->SummFlux[j]=-2000.0;
			return -1;
		};
		data->SummFlux[j]=-2.5*log10(Summ_Flux);// /**3.0*//*/(option.Lambda_End[j]-option.Lambda_Beg[j])*/
		//data->SummFlux[j]-=option.VegaFlux[j];
	}
	return 0;
}
//			
int CMyTeMath::Energy_Distribution(DocDataType* data,SData_option option)
{
		
	double Summ_Flux=0.0;
	double Summ_ErrFlux=0.0;
	double Summ_Step=0.0;
	ULONG32 StepCount=0;
	double L,a,st; 

	st=option.StepLmkm/2.0f;	
	//data->LambdaOut[0]=option.LmkmBeg;
	for (ULONG32 J=0;J<data->OutCount;J++)
	{
		data->LambdaOut[J]=option.LmkmBeg +(double)J*option.StepLmkm;	
		
		Summ_Step=0.0;
		Summ_Flux=0.0;
		Summ_ErrFlux=0.0;
		for (ULONG32 i=0;i<data->Count;i++)
		{
			if ((data->Lambda[i]>=data->LambdaOut[J]-st)&&(data->Lambda[i+1]<data->LambdaOut[J]+st))
			{
				L=data->Lambda[i+1]-data->Lambda[i];
				//поток в IRTF SpLab Flux [Watt]/([m2]*[mkm]):
				Summ_Flux+=data->Flux[i]*L;//(data->Lambda[i]*data->Lambda[i]);
				Summ_Step+=L;
				a=data->NormErrFlux[i]*L;
				Summ_ErrFlux+=a*a;
			}else {
				if(data->Lambda[i+1]>=data->LambdaOut[J]+st)
				{
					data->FluxOutW[J]=LinInt(data->Flux[i-1],data->Flux[i],
						data->Lambda[i-1],data->Lambda[i],data->LambdaOut[J]);
				}
				break;
			}
		}
		data->FluxOutW[J]=Summ_Flux/Summ_Step;
		data->ErrOut[J]=sqrt(Summ_ErrFlux)/Summ_Step; 
	}
	return 0;
}
//int CMyTeMath::Energy_Distribution(DocDataType* data,SData_option option)
//{
//	///*
//	double Summ_Flux=0.0;
//	double Summ_ErrFlux=0.0;
//	double Summ_Step=0.0;
//	ULONG32 StepCount=0;
//	double L,a,st; 
//
//	st=option.StepLmkm/2.0f;
//	//data->LambdaOut[0]=option.LmkmBeg;
//	for (ULONG32 i=0,J=0;J<data->OutCount;J++)
//	{
//		data->LambdaOut[J]=option.LmkmBeg +(double)J*option.StepLmkm;	
//		
//		Summ_Step=0.0;
//		Summ_Flux=0.0;
//		Summ_ErrFlux=0.0;
//		for (;i<data->Count;i++)
//		{
//			if ((data->Lambda[i]>=data->LambdaOut[J]-st)&& 
//				(data->Lambda[i+1]<data->LambdaOut[J]+st))
//			{
//				L=data->Lambda[i+1]-data->Lambda[i];
//				//поток в E-16 [Watt]/([m2]*[mkm]):
//				Summ_Flux+=data->Flux[i]*L/(data->Lambda[i]*data->Lambda[i]);
//				Summ_Step+=L;
//				a=data->NormErrFlux[i]*L;
//				Summ_ErrFlux+=a*a;
//
//
//			}else {
//				if(data->Lambda[i+1]>=data->LambdaOut[J]+st)
//				{
//					data->FluxOutW[J]=3.0*LinInt(data->Flux[i-1],data->Flux[i],
//						data->Lambda[i-1],data->Lambda[i],data->LambdaOut[J])
//						/(data->LambdaOut[J]*data->LambdaOut[J]);
//				}
//				break;
//			}
//		}
//		data->FluxOutW[J]=Summ_Flux*3.0/Summ_Step;
//		data->ErrOut[J]=sqrt(Summ_ErrFlux)/Summ_Step; 
//	}
//	return 0;
//}
double CMyTeMath::LinInt(double a,double b,double ta,double tb,double t)
//$$+							y1		y2		x1		x2		x
{
	if(ta!=tb) {
		return a+(b-a)*(t-ta)/(tb-ta);
	}else return a;
}
float CMyTeMath::LinInt(float a,float b,float ta,float tb,float t)
//$$+
{
	if(ta!=tb) {
		return a+(b-a)*(t-ta)/(tb-ta);
	}else return a;
}

int CMyTeMath::Energy_Distribution1_0( DocDataType* data,SData_option option )
{
	double SummFlux=0.0;	
	double ST=option.StepLmkm/2.0;
	bool b=true;
	bool bt=false;
	bool noI=false;
	
	//double dFlux1=0.0;
	//double dFlux2=0.0;
	double FluxBeg=0.0;
	double FluxEnd=0.0;
	double ErrBeg=0.0;
	double ErrEnd=0.0;
	double Summ_ErrFlux=0.0;
	double Err2=0.0;

	double FluxOut=0.0;
	double FluxUI=0.0;
	double FluxUIp1=0.0;
	double FluxUIm1=0.0;
	ULONG32 ILastend=0;
	ULONG32 Inext=0;
	double* DataFlux=new double[data->Count];
	double* DataNormErr=new double[data->Count];

	for(ULONG32 i=0;i<data->Count;i++)
	{
		//заполняем массив данными перевод в W/sm2*mkm
		DataFlux[i]=data->Flux[i];//*3.0/(data->Lambda[i]*data->Lambda[i]);
		DataNormErr[i]=data->NormErrFlux[i];//*3.0/(data->Lambda[i]*data->Lambda[i]);
	}
	for(ULONG32 J=0;J<data->OutCount;J++)
	{
		//выводим длинны волн с заданным интервалом
		data->LambdaOut[J]=data->LmBeg+data->LmStep*J;//option.LmkmBeg+option.StepLmkm*J;
	}
	for(ULONG32 J=0;J<data->OutCount;J++)
	{
		bool LFirst=true;//Первое значение диапазона
		bool LEnd=false;
		//bool lost=false;
		SummFlux=0.0;		
		ULONG32 Ibeg=0;		
		ULONG32 Icount=0;
		for(ULONG32 i=0;i<data->Count;i++)
		{
			//поиск точек в диапазоне
			if((data->Lambda[i]>=data->LambdaOut[J]-ST)&&(data->Lambda[i]<=data->LambdaOut[J]+ST))
			{
				if(LFirst)
				{
					LFirst=false;
					LEnd=true;
					Ibeg=i;
				}
				Icount++;
			}else
			if(LEnd)
			{				
				LEnd=false;
				ILastend=i-1;
				i=data->Count;//останавливаем перебор
			}
			//обработка найденных точек
			if(Icount>0)//есть как минимум 1 точка в интервале
			{
				if(Ibeg==0)//и до этой точки нет более точек
				{
					//вычислим в диапазоне от начала интервала до первой точки в интервале
					FluxBeg=LinInt(DataFlux[Ibeg],DataFlux[Ibeg],
						data->LambdaOut[J]-ST,data->Lambda[Ibeg],data->LambdaOut[J]-ST);
					SummFlux=FluxBeg*(data->Lambda[Ibeg]-(data->LambdaOut[J]-ST));
					
					ErrBeg=LinInt(DataNormErr[Ibeg],DataNormErr[Ibeg],
						data->LambdaOut[J]-ST,data->Lambda[Ibeg],data->LambdaOut[J]-ST);
					Err2=ErrBeg*(data->Lambda[Ibeg]-(data->LambdaOut[J]-ST));
					Summ_ErrFlux=Err2*Err2;

					if(Icount>1)//если точек больше одной
					{
						for(ULONG32 i=Ibeg;i<(Ibeg+Icount-1);i++)
						{
							SummFlux+=DataFlux[i]*(data->Lambda[i+1]-data->Lambda[i]);
							Err2=DataNormErr[i]*(data->Lambda[i+1]-data->Lambda[i]);
							Summ_ErrFlux+=Err2*Err2;
						}
					}
					//вычислим в диапазоне от последней точки в интервале до конца интервала
					FluxEnd=LinInt(DataFlux[Ibeg+Icount-1],DataFlux[Ibeg+Icount],
						data->Lambda[Ibeg+Icount-1],data->Lambda[Ibeg+Icount],data->LambdaOut[J]+ST);
					SummFlux+=FluxEnd*((data->LambdaOut[J]+ST)-data->Lambda[Ibeg+Icount-1]);
					
					ErrEnd=LinInt(DataNormErr[Ibeg+Icount-1],DataNormErr[Ibeg+Icount],
						data->Lambda[Ibeg+Icount-1],data->Lambda[Ibeg+Icount],data->LambdaOut[J]+ST);
					Err2=DataNormErr[Ibeg+Icount-1]*((data->LambdaOut[J]+ST)-data->Lambda[Ibeg+Icount-1]);					
					Summ_ErrFlux+=Err2*Err2;
					
					data->FluxOutW[J]=SummFlux/(data->LmStep);
					data->ErrOut[J]=sqrt(Summ_ErrFlux)/(data->LmStep); 

				}else //до этой точки есть точки Ibeg-1
				{
					//вычислим в диапазоне от начала интервала до первой точки в интервале
					FluxBeg=LinInt(DataFlux[Ibeg-1],DataFlux[Ibeg],
						data->Lambda[Ibeg-1],data->Lambda[Ibeg],data->LambdaOut[J]-ST);
					SummFlux=FluxBeg*(data->Lambda[Ibeg]-(data->LambdaOut[J]-ST));
					
					ErrBeg=LinInt(DataNormErr[Ibeg-1],DataNormErr[Ibeg],
						data->Lambda[Ibeg-1],data->Lambda[Ibeg],data->LambdaOut[J]-ST);
					Err2=ErrBeg*(data->Lambda[Ibeg]-(data->LambdaOut[J]-ST));
					Summ_ErrFlux=Err2*Err2;					
					
					if(Icount>1)//если точек больше одной
					{
						for(ULONG32 i=Ibeg;i<(Ibeg+Icount-1);i++)
						{
							SummFlux+=DataFlux[i]*(data->Lambda[i+1]-data->Lambda[i]);
							Err2=DataNormErr[i]*(data->Lambda[i+1]-data->Lambda[i]);
							Summ_ErrFlux+=Err2*Err2;
						}
					}
					//вычислим в диапазоне от последней точки в интервале до конца интервала
					FluxEnd=LinInt(DataFlux[Ibeg+Icount-1],DataFlux[Ibeg+Icount],
						data->Lambda[Ibeg+Icount-1],data->Lambda[Ibeg+Icount],data->LambdaOut[J]+ST);
					SummFlux+=FluxEnd*((data->LambdaOut[J]+ST)-data->Lambda[Ibeg+Icount-1]);
					ErrEnd=LinInt(DataNormErr[Ibeg+Icount-1],DataNormErr[Ibeg+Icount],
						data->Lambda[Ibeg+Icount-1],data->Lambda[Ibeg+Icount],data->LambdaOut[J]+ST);
					Err2=DataNormErr[Ibeg+Icount-1]*((data->LambdaOut[J]+ST)-data->Lambda[Ibeg+Icount-1]);					
					Summ_ErrFlux+=Err2*Err2;
					data->FluxOutW[J]=SummFlux/(data->LmStep);
					data->ErrOut[J]=sqrt(Summ_ErrFlux)/(data->LmStep); 
				}
			}else//нет точек в интервале			
			{
				FluxBeg=LinInt(DataFlux[ILastend],DataFlux[ILastend+1],
					data->Lambda[ILastend],data->Lambda[ILastend+1],data->LambdaOut[J]);

				ErrBeg=LinInt(DataNormErr[ILastend],DataNormErr[ILastend+1],
					data->Lambda[ILastend],data->Lambda[ILastend+1],data->LambdaOut[J]);				
				data->FluxOutW[J]=FluxBeg;	
				data->ErrOut[J]=ErrBeg; 
			}
		}
	}
	delete[] DataFlux;
	delete[] DataNormErr;
	return 0;
}

int CMyTeMath::Energy_Distribution2( DocDataType* data,SData_option option,CMyTeBand* Band )
{
	for(ULONG32 i=0;i<data->Count;i++)
	{
		data->FluxOutW[i]+=data->Flux[i]*Band->GetFlux(data->Lambda[i]);
		data->LambdaOut[i]=data->Lambda[i];
	}
	return 0;
}
// ВГ-maiden function
double CMyTeMath::Integral(double* SubInt,ULONG32 MaxCount,double* lambda,double lambdaBeg,double lambdaEnd,
						   double LBREAK,bool ERRF)
{
	double Summa=0.0;
	double SummaInt=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;
	double Err;

	Summa=0.0;
	SummaInt=0.0;
	for(UINT i=0;i<MaxCount;i++)
	{
		if((lambda[i]>=lambdaEnd)||(lambda[i]>=LBREAK)) break;
		if((lambda[i]<lambdaBeg) &&(lambda[i+1]>lambdaEnd)){
			la=lambdaBeg;
			lb=lambda[i+1];
			b=SubInt[i+1];
			a=LinInt(SubInt[i],b,lambda[i],lb,la);
		}else {
			if(lambda[i]>=lambdaBeg && lambda[i+1]<lambdaEnd){
				la=lambda[i];
				lb=lambda[i+1];
				a=SubInt[i];
				b=SubInt[i+1];
			}else{
				if((lambda[i]<lambdaEnd) && (lambda[i+1]>=lambdaEnd)){
					lb=lambdaEnd;
					la=lambda[i];
					a=SubInt[i];
					b=LinInt(a,SubInt[i+1],la,lambda[i+1],lb);
				}else continue;
			}
		}
		if(ERRF){
			Err=(a+b)*(lb-la)/2.0;
			Summa+=Err*Err;
		}else{
			Summa+=(a+b)*(lb-la)/2.0;
		}
		SummaInt+=lb-la;
	}	
	if(ERRF){
		return (sqrt(Summa)/SummaInt);
	}
	return (Summa/SummaInt);
}

// ВГ-maiden function
ULONG32 CMyTeMath::JtoWm_2mkm_1( DocDataType* data,double* DataFlux,double* DataNormErr,double* DL,
							/*ULONG32 Count,*/SData_option option ){
	//for(ULONG32 i=0;i<Count;i++)
	for(ULONG32 i=0,j=0;i<data->Count;i++)
	{
		//заполняем массив данными перевод в W/sm2*mkm
		//if(i==0 && data->Lambda[i]>option.LmkmBeg){
		//	DataFlux[0]=data->Flux[0]*3.0e-16/(data->Lambda[0]*data->Lambda[0]);
		//	DataNormErr[0]=data->NormErrFlux[0]*3.0e-16/(data->Lambda[0]*data->Lambda[0]);
		//	DL[0]=option.LmkmBeg;
		//}else{
			if(data->Flux[i]<=0) continue;
			DataFlux[j]=data->Flux[i]*3.0e-16/(data->Lambda[i]*data->Lambda[i]);
			DataNormErr[j]=data->NormErrFlux[i]*3.0e-16/(data->Lambda[i]*data->Lambda[i]);
			DL[j]=data->Lambda[i];
			if(DL[j]>=option.LmkmEnd){
				return j;
			}
			j++;
		//}
	}
	return 0;
}
//ULONG32 CMyTeMath::JtoWm_2mkm_1( DocDataType* data,double* DataFlux,double* DataNormErr,double* DL,
//								/*ULONG32 Count,*/SData_option option ){
//									//for(ULONG32 i=0;i<Count;i++)
//	for(ULONG32 i=0,j=0;data->Count;i++)
//	{
//		if(data->Flux[i]<=0) continue;
//		//заполняем массив данными перевод в W/sm2*mkm
//		if(j==0 && data->Lambda[i]>option.LmkmBeg)
//		{
//			DataFlux[0]=data->Flux[0];
//			DataNormErr[0]=data->NormErrFlux[0];
//			DL[0]=option.LmkmBeg;
//			j++;
//		}else
//		{
//			DataFlux[j]=data->Flux[i];
//			DataNormErr[j]=data->NormErrFlux[i];
//			DL[j]=data->Lambda[i];
//			if(DL[j]>=option.LmkmEnd){
//				return j;
//			}
//			j++;
//		}
//	}
//	//return 0;
//}
// ВГ-maiden function
//int CMyTeMath::Energy_Distribution1( DocDataType* data,SData_option option )
//{
//	double SummFlux=0.0;	
//	double ST=option.StepLmkm/2.0;
//	double Summ_ErrFlux=0.0;
//	double FluxUI=0.0;
//	double FluxUIp1=0.0;
//	double FluxUIm1=0.0;
//	ULONG32 ILastend=0;
//	ULONG32 Inext=0;
//	ULONG32 Count=data->Count+1;
//
//	double* DataFlux=new double[data->Count];
//	double* DataNormErr=new double[data->Count];
//	double* DL=new double[data->Count];
//	double LBREAK=option.LmkmEnd;
//
//	//Count=JtoWm_2mkm_1( data,DataFlux,DataNormErr,DL,option );
//	for(ULONG32 J=0;;J++)
//	{
//		//выводим длины волн с заданным интервалом
//		data->LambdaOut[J]=option.LmkmBeg+option.StepLmkm*J;
//		if(data->LambdaOut[J]>=LBREAK){ 
//			Count=J+1;
//			break;
//		}
//	}
//	for(ULONG32 J=0;;J++)
//	{
//		SummFlux=Integral(DataFlux,DL,
//			data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,false);
//		Summ_ErrFlux=Integral(DataNormErr,DL,
//			data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,true);
//		data->FluxOutW[J]=SummFlux/10000.0;	// м2	/=10000.0 ->см2
//		data->ErrOut[J]=Summ_ErrFlux/10000.0;
//		if(data->LambdaOut[J]>=LBREAK){
//			data->OutCount=Count;
//			break;
//		}
//	}
//	delete DataFlux;
//	delete DataNormErr;
//	return 0;
//}
// ВГ-maiden function from Jy to W/cm2um
int CMyTeMath::Energy_Distribution1( DocDataType* data,SData_option option )
{
	double SummFlux=0.0;	
	double ST=option.StepLmkm/2.0;
	double Summ_ErrFlux=0.0;
	double FluxUI=0.0;
	double FluxUIp1=0.0;
	double FluxUIm1=0.0;
	ULONG32 ILastend=0;
	ULONG32 Inext=0;
	ULONG32 Count=data->Count+1;

	double* DataFlux=new double[data->Count];
	double* DataNormErr=new double[data->Count];
	double* DL=new double[data->Count];
	double LBREAK=option.LmkmEnd;

	//Count=JtoWm_2mkm_1( data,DataFlux,DataNormErr,DL,option );
	for(ULONG32 J=0;;J++)
	{
		//выводим длины волн с заданным интервалом
		data->LambdaOut[J]=option.LmkmBeg+option.StepLmkm*J;
		if(data->LambdaOut[J]>=LBREAK){ 
			Count=J+1;
			break;
		}
	}
	for(ULONG32 J=0;;J++)
	{
		//SummFlux=Integral(DataFlux,DL,
		//	data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,false);
		//Summ_ErrFlux=Integral(DataNormErr,DL,
		//	data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,true);
		data->FluxOutW[J]=SummFlux/10000.0;	// м2	/=10000.0 ->см2
		data->ErrOut[J]=Summ_ErrFlux/10000.0;
		if(data->LambdaOut[J]>=LBREAK){
			data->OutCount=Count;
			break;
		}
	}
	delete DataFlux;
	delete DataNormErr;
	return 0;
}
double* CMyTeMath::Vec_Mult(DocDataType* data,double* a)
{
	for(ULONG32 i=0;i<data->Count; i++) a[i]*=data->Flux[i];
	return (a);
}

// ВГ-maiden function
//void CMyTeMath::SubIntPre(DocDataType* data,double* SubInt){
	//for(UINT i=0;i<Count;i++){
	//SubInt=Vec_Mult(data->Flux);
//}
// ВГ-maiden function
void CMyTeMath::SubIntPre0(DocDataType* data,double* SubInt,double* DeDeTe)
{
	//SubInt=Vec_Mult(DeDeTe->GetFlux(data,data->Lambda),SubInt);	
}

// ВГ-maiden function
int CMyTeMath::Energy_Distribution1_1( DocDataType* data,SData_option option )
{
	double SummFlux=0.0;	
	double ST=option.StepLmkm/2.0;
	double Summ_ErrFlux=0.0;
	//double FluxUI=0.0;
	//double FluxUIp1=0.0;
	//double FluxUIm1=0.0;
	//ULONG32 ILastend=0;
	//ULONG32 Inext=0;
	ULONG32 Count=0;

	double* DataFlux=new double[data->Count];
	//double* DataFlux1=new double[data->Count];
	double* DataNormErr=new double[data->Count];
	double* DL=new double[data->Count];
	double LBREAK=option.LmkmEnd;
//prepare Jy->W/cm2um
	for(ULONG32 J=0;J<data->Count;J++){
		DataFlux[J]=1.0;
		DataNormErr[J]=2.0;
		DL[J]=data->Lambda[J];
	}
	//Для перевода из янских в Wсм-2мкм-1
	Count=JtoWm_2mkm_1( data,DataFlux,DataNormErr,DL,option );

	//SubIntPre0(data,DataFlux,DataFlux1);
	//ULONG32 Count=0;
	for(ULONG32 J=0;J < data->OutCount;J++)
	{
		//выводим длины волн с заданным интервалом
		data->LambdaOut[J]=option.LmkmBeg+option.StepLmkm*J;
		
	}
	for(ULONG32 J=0;J<data->OutCount;J++)
	{
		SummFlux=Integral(DataFlux,data->Count,DL,
			data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,false);
		Summ_ErrFlux=Integral(DataNormErr,data->Count,DL,
			data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,true);
		data->FluxOutW[J]=SummFlux;
		data->ErrOut[J]=Summ_ErrFlux;
		if(data->LambdaOut[J]>=LBREAK){			
			break;
		}
	}
	delete[] DataFlux;
	delete[] DataNormErr;
	delete[] DL;
	return 0;
}
int CMyTeMath::Energy_Distribution1_2( DocDataType* data,SData_option option )
//This variant For turn Jy into W/cm2um
{
	double SummFlux=0.0;	
	//double ST=option.StepLmkm/2.0;
	double Summ_ErrFlux=0.0;
	//double FluxUI=0.0;
	//double FluxUIp1=0.0;
	//double FluxUIm1=0.0;
	//ULONG32 ILastend=0;
	//ULONG32 Inext=0;
	ULONG32 Count=0;

	double* DataFlux=new double[data->Count];
	//double* DataFlux1=new double[data->Count];
	double* DataNormErr=new double[data->Count];
	double* DL=new double[data->Count];
//	double LBREAK=option.LmkmEnd;
//prepare Jy->W/cm2um
	//for(ULONG32 J=0;J<data->Count;J++){
	//	DataFlux[J]=1.0;
	//	DataNormErr[J]=2.0;
	//	DL[J]=data->Lambda[J];
	//}
	//Для перевода из янских в Wсм-2мкм-1
	//Count=JtoWm_2mkm_1( data,DataFlux,DataNormErr,DL,option );

	//SubIntPre0(data,DataFlux,DataFlux1);
	//ULONG32 Count=0;
	//for(ULONG32 J=0;J < data->OutCount;J++)
	for(ULONG32 J=0;J < data->Count;J++)
	{
		//выводим длины волн с заданным интервалом
		//data->LambdaOut[J]=option.LmkmBeg+option.StepLmkm*J;
		data->LambdaOut[J]=data->Lambda[J];
		data->FluxOutW[J]=data->Flux[J]*3.0e-16/(data->Lambda[J]*data->Lambda[J]);
		data->ErrOut[J]=data->NormErrFlux[J]*3.0e-16/(data->Lambda[J]*data->Lambda[J]);
	}
//	for(ULONG32 J=0;J<data->OutCount;J++)
	//for(ULONG32 J=0;J < data->Count;J++)
	//{
	//	//SummFlux=Integral(DataFlux,data->Count,DL,
	//	//	data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,false);
	//	//Summ_ErrFlux=Integral(DataNormErr,data->Count,DL,
	//	//	data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,true);
	//	//data->FluxOutW[J]=DataFlux[J];
	//	//data->ErrOut[J]=DataNormErr[J];
	//	/*SummFlux=Integral(DataFlux,data->Count,DL,
	//		data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,false);
	//	Summ_ErrFlux=Integral(DataNormErr,data->Count,DL,
	//		data->LambdaOut[J]-ST,data->LambdaOut[J]+ST,LBREAK,true);
	//	data->FluxOutW[J]=SummFlux;
	//	data->ErrOut[J]=Summ_ErrFlux;
	//	if(data->LambdaOut[J]>=LBREAK){			*/
	//	//if(data->LambdaOut[J]>=LBREAK){			
	//	//	break;
	//	//}
	//}
	delete[] DataFlux;
	delete[] DataNormErr;
	delete[] DL;
	return 0;
}
#if defined BLOCK_OFF_LINE
	// ВГ-maiden function
int CMyTeMath::EnergyCountIntegrBand( DocDataType* data, SData_option option)
{
	double SummEn=0.0;
	double SummEL=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;
	for(UINT J=0;J<option.NumberBand;J++)
	{		
		SummEn=0.0;
		SummEL=0.0;
		for(UINT i=0;i<data->Count-1;i++)
		{
			if((data->Lambda[i]<option.Lambda_Beg[J]) && (data->Lambda[i+1]>option.Lambda_Beg[J])){
				//исходные Flux в [эргах/(с см2 А)] для Веги, Lambda в А,  a для ИК звезд Flux в W/см2/мкм
				la=option.Lambda_Beg[J];
				lb=data->Lambda[i+1];
				//GetFlux(option.Lambda_Beg[J],data->Lambda,data-> Flux, data->Count);
				b=data-> Flux[i+1];
				a=CMyTeMath::LinInt(data-> Flux[i],b,data->Lambda[i],lb,la);
				//continue;
				//
			}else {
				if((data->Lambda[i]>=option.Lambda_Beg[J]) && (data->Lambda[i+1]<option.Lambda_End[J])){
					la=data->Lambda[i];
					lb=data->Lambda[i+1];
					a=data->Flux[i];
					b=data->Flux[i+1];
				}else{
					if((data->Lambda[i]<option.Lambda_End[J]) && (data->Lambda[i+1]>=option.Lambda_End[J])){
						lb=option.Lambda_End[J];
						la=data->Lambda[i];
						a=data-> Flux[i];
						b=CMyTeMath::LinInt(a,data-> Flux[i+1],la,data->Lambda[i+1],lb);
					}else continue;
				}
			}
			SummEn+=(a*la+b*lb)*(lb-la)/2.0;
			SummEL+=(a+b)*(lb-la)/2.0;
		}	

		//c=3 *e+18[А/сек]=2.99792.5e+8[m/сек]  h=6.6262e-27[эрг][сек]=6.6256e-34[W][сек2]
		//Sun [W]/[m3]			hc=1.98786e-8[эрг][A]([сек]*e+18[А/сек])=1.98786e-25[W][m][s]
		data->FotonCount[J]=option.Coeff*SummEn*(1.0e+4)/1.98786e-8;     //2.0e-19; для ИК W/cm^2 mkm  1.98786e-8;    //[эрг А]
		data->EmFluxWsm2A_7[J]=option.Coeff*SummEL*1.0e-7*(1.0e+4); //*e+4=> m2;out W/см2; a для ИК звезд Flux в W/см2/мкм
	}
	return 0;
}

#endif

// ВГ-maiden function
int CMyTeMath::EnergyCount( DocDataType* data, SData_option option)
{
	double SummEn=0.0;
	double SummEL=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;
	for(UINT J=0;J<option.NumberBand;J++)
	{		
		SummEn=0.0;
		SummEL=0.0;
		for(UINT i=0;i<data->Count-1;i++)
		{
			if((data->Lambda[i]<option.Lambda_Beg[J]) && (data->Lambda[i+1]>option.Lambda_Beg[J])){
//исходные Flux в [эргах/(с см2 А)] для Веги, Lambda в А,  a для ИК звезд Flux в [J]*3e-12/(l*l[мкм])=>[ [W]/[м2 мкм]
				la=option.Lambda_Beg[J];
				lb=data->Lambda[i+1];
				b=data-> Flux[i+1];
				a=LinInt(data-> Flux[i],b,data->Lambda[i],lb,la);
				//continue;
					//GetFlux(option.Lambda_Beg[J],data->Lambda,data-> Flux, data->Count);
			}else {
				if((data->Lambda[i]>=option.Lambda_Beg[J]) && (data->Lambda[i+1]<option.Lambda_End[J])){
					la=data->Lambda[i];
					lb=data->Lambda[i+1];
					a=data->Flux[i];
					b=data->Flux[i+1];
				}else{
					if((data->Lambda[i]<option.Lambda_End[J]) && (data->Lambda[i+1]>=option.Lambda_End[J])){
						lb=option.Lambda_End[J];
						la=data->Lambda[i];
						a=data-> Flux[i];
						b=LinInt(a,data-> Flux[i+1],la,data->Lambda[i+1],lb);
					}else continue;
				}
			}
			SummEn+=(a*la+b*lb)*(lb-la)/2.0;
			SummEL+=(a+b)*(lb-la)/2.0;
		}	
		
		//c=3 *e+18[А/сек]=2.99792.5e+8[m/сек]  h=6.6262e-27[эрг][сек]=6.6256e-34[W][сек2]
		//Sun [W]/[m3]			hc=1.98786e-8[эрг][A]([сек]*e+18[А/сек])=1.98786e-25[W][m][s]
		data->FotonCount[J]=option.Coeff*SummEn*(1.0e+4)/1.98786e-8;     //2.0e-19; для ИК W/cm^2 mkm  1.98786e-8;    //[эрг А]
		data->EmFluxWsm2A_7[J]=option.Coeff*SummEL*1.0e-7*(1.0e+4); //*e+4=> out m2; (out W/см2;) a для ИК звезд Flux в W/см2/мкм
		//
	}
	return 0;
}

// Сохранение энергий в файл
int CMyTeMath::SaveEnergy(DocDataType* data, SData_option option,CString rfname,bool Over)
{
	FILE *outf;	
	CString fmode;
	if(Over)fmode=_T("w"); else fmode=_T("a");	
	CString outfname(rfname);
	CString OutTextAll;
	_wfopen_s(&outf,rfname,fmode);
	for(ULONG32 i=0;i<option.NumberBand;i++)
	{
		OutTextAll=_T("Band:[");
		OutTextAll.AppendFormat(_T("%5.2f"),option.Lambda_Beg[i]);
		OutTextAll.Append(_T(":"));
		OutTextAll.AppendFormat(_T("%5.2f"),option.Lambda_End[i]);
		OutTextAll.Append(_T("] FC="));
		OutTextAll.AppendFormat(_T("%5.8le"),data->FotonCount[i]);
		OutTextAll.Append(_T(", Em="));
		OutTextAll.AppendFormat(_T("%5.8le"),data->EmFluxWsm2A_7[i]);
		OutTextAll.Append(_T("\n"));
		fputws(OutTextAll,outf);
	}	
	
	fclose(outf);
	return 0;
}

int CMyTeMath::Norm5500(DocDataType* data, SData_option option)
{
	//ищем значение Lambda==5500 и получаем значение яркости в этой точке
	//делим все значения яркости на яркость в точке 5500
	delete[] data->LambdaOut;
	delete[] data->FluxOutW;
	data->OutCount=data->Count;
	data->LambdaOut=new double[data->OutCount];
	data->FluxOutW=new double[data->OutCount];
	//double Divider;
	/*for(ULONG32 i=0;i<data->Count;i++)
	{
		if(data->Lambda[i]>=5500.0)
		{
			Divider=LinInt(data->Flux[i-1],data->Flux[i],data->Lambda[i-1],data->Lambda[i],5500.0);
			i=data->Count;
		}
	}*/
	for(ULONG32 i=0;i<data->Count;i++)
	{
		data->LambdaOut[i]=data->Lambda[i];
		data->FluxOutW[i]=data->Flux[i]*2.997925*1e-16/(data->Lambda[i]*data->Lambda[i]);
	}
	return 0;
}

#if defined BLOCK_OFF_LINE

double FindSgn(double *str, char C){
	for(i=0;str[i];i++){
		if(str[i]==C) return(i);
	}
	return 0;
}
#endif
#if defined BLOCK_OFF_LINE
double CMyTeMath::SIRIUS(DocDataType* DateStr, SData_option option,CMyTeBand* Band,CMyTeBand* Extint,double Mz)
{
    double SummFlux=0.0;
    double SummPlFlux=0.0;
    double TEMPERATURE;
    double PRESSURE; //$$$
    UINT eq;
    UINT BegSign;
    UINT SecndSing;
    char temp[80];
    // Открытие файлов ddmmyy.FEA.txt

    // Чтение в DateStr[i][k].

    // Расфасовка
    for(ULONG32 i=0;i<DateStr->Count-1;i++) {
		FirstSign=Str[i][0];
		SecndSign=Str[i][1];
		switch(BegSign){
			case 'W': FlagPere[i]="W";
				break;
			case 'E': FlagPere[i]="E";
				break;
			case 'P': eq=FindSgn(DatStr, "=");
				memcpy(temp,DatStr[eq],5);
				PRESSURE=atof(temp);
				break;
			case 'T': eq=FindSgn(DatStr, "=");
				memcpy(temp,DatStr[eq],3);
				TEMPERATURE=atof(temp);
				break;
			case 'B': memcpy(temp,DatStr,11);
				StarName[i]=atof(temp);
				break;
			case '{': LimMark=true;
				break;
			case '}': LimMark=false;
				break;
			case 'M':switch(SecndSign){
						case '+':
						break;
						case '-':
						break;
					 }
				break;
			case 'I':switch(SecndSign){
						case '+':
						break;
						case '-':
						break;
					 }
				break;
			case 'D':switch(SecndSign){
						case 'C':
						break;
						case '+':
						break;
					 }
			default break;
		}
        }

//

//
//	double PlambdaXMz=0;
//	for(ULONG32 i=0;i<data->Count-1;i++)
//	{
//		double BandFlux=Band->GetFlux(data->Lambda[i]);
//		double ExtintFlux=Extint->GetFlux(data->Lambda[i]);
//	
//		if((ExtintFlux>0) && (BandFlux>0.0))
//		{	
//			//PlambdaXMz=exp(Mz*log(Extint->GetFlux(data->Lambda[i])));
//			PlambdaXMz=pow(ExtintFlux,Mz);
//			SummFlux+=(data->Flux[i]*BandFlux)*(data->Lambda[i+1]-data->Lambda[i]);		
//			SummPlFlux+=(data->Flux[i]*PlambdaXMz*BandFlux)*(data->Lambda[i+1]-data->Lambda[i]);
//		}
//		
//		//double BandFlux=Band->GetFlux(data->Lambda[i]);
//		//SummFlux+=(data->Flux[i]*BandFlux)*(data->Lambda[i+1]-data->Lambda[i]);		
//		//SummPlFlux+=(data->Flux[i]*PlambdaXMz*BandFlux)*(data->Lambda[i+1]-data->Lambda[i]);		
//	}
//	Res=-1.086*log(SummPlFlux/SummFlux);	//Ab=1.0479
//	return Res;//(data->Lambda[data->Count-1]-data->Lambda[0]);
}
#endif //BLOCK_OFF_LINE

#if defined BLOCK_OFF_LINE

double CMyTeMath::SBand(DocDataType* data, SData_option option,CMyTeBand* Band,
						double Vega,CMyTeBand* Redden,double RedThik)
{
	double SummFlux=0.0;
	double Res=0.0;
	//double temp;
	double red=1.0;
	for(ULONG32 i=0;i<data->Count-1;i++)
	{
		if(Redden!=NULL) 
		{
			if(data->Lambda[i]>=Redden->Lambda[0])
			{
				red=1.0;
				for(int j=0;j<RedThik;j++)		//
				{				
					red*=Redden->GetFlux(data->Lambda[i]);
				}	
			}			
		}	
		double BandFlux=Band->GetFlux(data->Lambda[i]);
		SummFlux+=(data->Flux[i]*red*BandFlux)*(data->Lambda[i+1]-data->Lambda[i]);		
	}
	return Res=-1.086*log(SummFlux);//-Vega;//(data->Lambda[data->Count-1]-data->Lambda[0]);
}
#endif //BLOCK_OFF_LINE

#if defined BLOCK_OFF_LINE
double CMath::Integral1(DocDataType*SubInt,double lambdaBeg,double lambdaEnd)
{
	double Summa=0.0;
	double SummaInt=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;
	Summa=0.0;
	SummaInt=0.0;
	for(UINT i=0;;i++)
	{
		if(SubInt->GetLambda(i)>=lambdaEnd) break;
		if((SubInt->GetLambda(i)<lambdaBeg) &&(SubInt->GetLambda(i+1)>lambdaEnd)){
			la=lambdaBeg;
			lb=SubInt->GetLambda(i+1);
			b=SubInt->GetFlux(SubInt->GetLambda(i+1));
			a=SubInt->GetFlux(la);
			//a=Interpol(SubInt[i],b,SubInt->GetLambda(i),lb,la);
		}else {
			if(SubInt->GetLambda(i)>=lambdaBeg && SubInt->GetLambda(i+1)<lambdaEnd){
				la=SubInt->GetLambda(i);
				lb=SubInt->GetLambda(i+1);
				a=SubInt->GetFlux(SubInt->GetLambda(i));
				b=SubInt->GetFlux(SubInt->GetLambda(i+1));
			}else{
				if((SubInt->GetLambda(i)<lambdaEnd) && (SubInt->GetLambda(i+1)>=lambdaEnd)){
					lb=lambdaEnd;
					la=SubInt->GetLambda(i);
					a=SubInt->GetFlux(SubInt->GetLambda(i));
					b=SubInt->GetFlux(lb);
					//b=Interpol(a,SubInt[i+1],la,SubInt->GetLambda(i+1),lb);
				}else continue;
			}
		}
		Summa+=(a+b)*(lb-la)/2.0;
		SummaInt+=lb-la;
	}	
	return (Summa/SummaInt);
}
#endif //BLOCK_OFF_LINE

double CMyTeMath::RANDisex()
{
	double rv;
	do{
		rv = -2.3*log((double)rand()/(double)RAND_MAX);
	} while (rv>1.0);
	return rv;
}
double CMyTeMath::RANDisexN(double N)
{
	double rv;
	do{
		rv = -2.3*log((double)rand()/(double)N);
	} while (rv>1.0);
	return rv;
}
							//DocDataType
double CMyTeMath::Integral_1(DocDataType* data,CMyTeBand* Band,double lambdaBeg,double lambdaEnd)
{
	double Summa=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;

	Summa=0.0;
	for (size_t i = 0; i < data->Count - 2; i++)
	{
		if ((data->Lambda[i] < lambdaBeg) && (data->Lambda[i + 1] > lambdaEnd)) {
			la = lambdaBeg;
			lb = data->Lambda[i + 1];
			b = data->Flux[i + 1];
			a = LinInt(data->Flux[i], b, data->Lambda[i], lb, la);
		}
		else {
			if (data->Lambda[i] >= lambdaBeg && data->Lambda[i + 1] < lambdaEnd) {
				la = data->Lambda[i];
				lb = data->Lambda[i + 1];
				a = data->Flux[i];
				b = data->Flux[i + 1];
			}
			else {
				if ((data->Lambda[i] < lambdaEnd) && (data->Lambda[i + 1] >= lambdaEnd)) {
					lb = lambdaEnd;
					la = data->Lambda[i];
					a = data->Flux[i];
					b = LinInt(a, data->Flux[i + 1], la, data->Lambda[i + 1], lb);
				}
				else continue;
			}
		}
		Summa += (a + b) * (lb - la) / 2.0;
	}
	return (Summa);
}
double CMyTeMath::SBand(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik)
{
	//double SummFlux=0.0;
	double Res=0.0;
	double lambdaBeg=0.0;
	double lambdaEnd=0.0;
	for(UINT i=0;i<data->Count-1;i++)
	{
		double BandFlux0=Band->GetFlux(data->Lambda[i]);
		double BandFlux1=Band->GetFlux(data->Lambda[i+1]);
		if (BandFlux0==0 && BandFlux1!=0)lambdaBeg=data->Lambda[i];
		if (BandFlux0!=0 && BandFlux1==0)lambdaEnd=data->Lambda[i+1];
		else continue;
	}
	//Res=Integral_1(data,Band,lambdaBeg,lambdaEnd);
	return Res=-1.08573620*log(Res)-Vega;//(data->Lambda[data->Count-1]-data->Lambda[0]);
}
//Function To COUNT random redden
// m = -2.5 * lg(integral(Lk, Ld)(El * Fl ^ (st)dL) + const1
double CMyTeMath::SBand_2(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik)
{
	if (data == NULL) return -1.0;
	DocDataType Subint = *data;

	long double Res=0.0;
	double red=1.0;
	double lambdaBeg=0.0;
	double lambdaEnd=0.0;
	bool Lbegin=false;
	bool Lend=false;

	lambdaBeg = Band->LBeg;
	lambdaEnd = Band->LEnd;
	for (UINT i = 0; i < data->Count - 1; i++) {
		if (Redden != NULL) {
			red = Redden->GetFlux(data->Lambda[i]);
			red = pow(red, RedThik);
		}
		double BandFlux = Band->GetFlux(data->Lambda[i]);
		
		//Jy->W/cm2um
		Subint.Flux[i] = data->Flux[i] * red * BandFlux;		
	}
	Res=Integral_1(&Subint,Band,lambdaBeg,lambdaEnd);
	
	return Res=-2.5*log10l(Res)-Vega;
}
double CMyTeMath::SBand_3(DocDataType* data, CMyTeBand* Band, double Vega, CMyTeBand* Redden, double RedThik, double Mz, CMyTeBand* EXTIN)

{
	if (data == NULL) return -1.0;
	DocDataType Subint = *data;
	DocDataType Subint0 = *data;

	long double Res = 0.0;
	long double Res0 = 0.0;
	double red = 1.0;
	double lambdaBeg = Band->LBeg;
	double lambdaEnd = Band->LEnd;

	for (UINT i = 0; i < data->Count - 1; i++)
	{
		if (Redden != NULL) {
			red = Redden->GetFlux(data->Lambda[i]);
			red = pow(red, RedThik);
		}
		const double BandFlux = Band->GetFlux(data->Lambda[i]);
		double EXT = EXTIN->GetFlux(data->Lambda[i]);

		EXT = pow(EXT, Mz);
		Subint.Flux[i] = data->Flux[i] * red * BandFlux * EXT;
		Subint0.Flux[i] = data->Flux[i] * red * BandFlux;
	}

	Res = Integral_1(&Subint, Band, lambdaBeg, lambdaEnd);
	Res0 = Integral_1(&Subint0, Band, lambdaBeg, lambdaEnd);
	Res = 2.5 * log10l(Res / Res0);

	return Res;
}
double CMyTeMath::SBand_4(DocDataType* data, CMyTeBand* Band, double Vega, CMyTeBand* Redden, double RedThik, double Mz, CMyTeBand* EXTIN)

{
	if (data == NULL) return -1.0;
	if (Band == NULL) return -1.0;
	DocDataType Subint = *data;
	DocDataType Subint0 = *data;

	long double Res = 0.0;
	double red = 1.0;
	const double lambdaBeg = Band->LBeg;
	const double lambdaEnd = Band->LEnd;

	for (UINT i = 0; i < data->Count - 1; i++) {
		if (Redden != NULL) {
			red = Redden->GetFlux(data->Lambda[i]);
			red = pow(red, RedThik);
		}
		const double BandFlux = Band->GetFlux(data->Lambda[i]);
		double EXT = EXTIN->GetFlux(data->Lambda[i]);

		EXT = pow(EXT, Mz);
		Subint.Flux[i] = data->Flux[i] * red * BandFlux * EXT;
	}

	Res = Integral_1(&Subint, Band, lambdaBeg, lambdaEnd);
	Res = -2.5 * log10l(Res) - Vega;
	return Res;
}
double CMyTeMath::SBand_L(DocDataType* data, CMyTeBand* Band, double Vega, CMyTeBand* Redden, double RedThik, double Mz, CMyTeBand* EXTIN)

{
	if (data == NULL) return -1.0;
	if (Band == NULL) return -1.0;
	DocDataType Subint = *data;
	DocDataType Subint0 = *data;

	long double Res = 0.0;
	long double Res0 = 0.0;
	double red = 1.0;
	const double lambdaBeg = Band->LBeg;
	const double lambdaEnd = Band->LEnd;

	for (UINT i = 0; i < data->Count - 1; i++)
	{
		if (Redden != NULL)
		{
			red = Redden->GetFlux(data->Lambda[i]);
			red = pow(red, RedThik);
		}
		const double BandFlux = Band->GetFlux(data->Lambda[i]);
		double EXT = EXTIN->GetFlux(data->Lambda[i]);
		EXT = pow(EXT, Mz);
		Subint.Flux[i] = data->Flux[i] * data->Lambda[i] * red * BandFlux * EXT;
		Subint0.Flux[i] = data->Flux[i] * red * BandFlux * EXT;
	}

	Res = Integral_1(&Subint, Band, lambdaBeg, lambdaEnd);
	Res0 = Integral_1(&Subint0, Band, lambdaBeg, lambdaEnd);
	Res =/*2.5*log10l*/(Res / Res0);

	return Res;
}

double CMyTeMath::SBand_L2(DocDataType* data, CMyTeBand* Band, double Vega, CMyTeBand* Redden, double RedThik, double Mz, CMyTeBand* EXTIN)

{
    if (data == NULL) return -1.0;
    if (Band == NULL) return -1.0;
    DocDataType Subint = *data;
    DocDataType Subint0 = *data;

    long double Res = 0.0;
    long double Res0 = 0.0;
    double red = 1.0;
    const double lambdaBeg = Band->LBeg;
    const double lambdaEnd = Band->LEnd;

    for (UINT i = 0; i < data->Count - 1; i++) {
	if (Redden != NULL) {
	    red = Redden->GetFlux(data->Lambda[i]);
	    red = pow(red, RedThik);
	}
	const double BandFlux = Band->GetFlux(data->Lambda[i]);
	double EXT = EXTIN->GetFlux(data->Lambda[i]);
	EXT = pow(EXT, Mz);
	Subint.Flux[i] = data->Flux[i] * data->Lambda[i] * data->Lambda[i] * red * BandFlux * EXT;
	Subint0.Flux[i] = data->Flux[i] * red * BandFlux * EXT;
    }

    Res = Integral_1(&Subint, Band, lambdaBeg, lambdaEnd);
    Res0 = Integral_1(&Subint0, Band, lambdaBeg, lambdaEnd);
    Res = 2.5 * log10l(Res / Res0);

    return Res;
}

#ifdef EMOE
void CMyTeMath::ProcessCatalog(CString CatInFName, CString CatOutFName, CString AdvDataFName, SData_option option , CMyTeBand Bands)
{	
	FILE* InFile;
	FILE* OutFile;
	FILE* AdvFile;
	//FILE* AdvFile2;

	const double Ln10=2.3025850929940456840179914546844;
	const double RadToGrad=180/3.1415926535897932384626433832795;
	const double GradToRad=1/RadToGrad;
	const double Beta=0.9;//килопарсеки 0.11

	double MatrixA[3][3]={	
		{-0.0548755601367195,-0.8734370902532698,-0.4838350155472244},
		{+0.4941094280132430,-0.4448296298016944,+0.7469822445004389},
		{-0.8676661489582886,-0.1980763737056720,+0.4559837761713720}};

	ULONG32 Number;
	ULONG32 BadStar=0;
	double Alpha;
	double Delta;
	double J;
	double H;
	double K;
	double J_K;

	double GLong;
	double GLat;
	ULONG32 count;
	BYTE*	szBuffer;
	ULONG32    nActual = 0; 
	ULONG32 strcount=0;
	//double* DataJ_K;
	//double* DataJ0;
	//ULONG32 DataCount;
	//double** A_All_J_K;
	//ULONG32 A_All_J_K_Count;
	//double** Mo_J_Ko;
	//ULONG32 Mo_J_Ko_Count;
	double** Mo_am_J_K;
	CString* Mo_am_J_K_Class;
	ULONG32 Mo_am_J_K_Count;
	//_wfopen_s(&AdvFile,AdvDataFName,_T("r"));
	//_wfopen_s(&AdvFile2,AdvDataFName2,_T("r"));
	_wfopen_s(&InFile,CatInFName,_T("r"));
	_wfopen_s(&OutFile,CatOutFName,_T("w"));
	

	//загрузка файла Mo_am_J_K массив 27хN
	{
		CFile CAdvFile=CFile(AdvDataFName,CFile::modeRead);
		ULONG32 filesize=CAdvFile.GetLength();
		szBuffer= new BYTE [filesize];
		nActual = CAdvFile.Read( szBuffer, filesize ); 
		CAdvFile.Close();
		if(nActual<filesize) filesize=nActual;	
		//пропускаем заголовок
		int head=CMyTeMath::SkipNotNumber(szBuffer,0,filesize);

		//подсчет строк
		strcount=0;
		for(ULONG32 i=head;i<filesize;i++)
		{
			if((szBuffer[i]==0x0A)&&(szBuffer[i+1]==0x20)&&(szBuffer[i-1]!=0x0D))
			{
				szBuffer[i]=0x0D;
				szBuffer[i+1]=0x0A;
			}
			if(szBuffer[i]==0x09) szBuffer[i]=0x20;
			if((szBuffer[i]==0x0D)&&(szBuffer[i+1]==0x0A)) 
			{
				strcount++;
			}
		}

		//создаём массивы
		Mo_am_J_K_Count=strcount;
		Mo_am_J_K=new double*[26];
		Mo_am_J_K_Class=new CString[strcount];
		for(ULONG32 i=0;i<26;i++)
		{
			Mo_am_J_K[i]=new double[strcount];
		}	
		//загружаем данные в массивы	
		wchar_t temp[200];
		ULONG32 FieldCount;
		ULONG32 bi=0;
		bool First=true;
		bi=CMyTeMath::SkipNotNumber(szBuffer,bi,filesize);	
		for(ULONG32 j=0;j<strcount;j++)
		{		
			for(int k=0;k<26;k++)
			{
				//for(int b=0;b<200;b++) temp[b]=0x0;
				bi=CMyTeMath::SkipNotNumber(szBuffer,bi,filesize);
				bi=CopyStrNumber(szBuffer,bi,filesize,temp,200);
				swscanf_s((wchar_t*)&temp,_T("%lf"),&Mo_am_J_K[k][j]);
							
			}		
			for(int b=0;b<200;b++) temp[b]=0x0;
			ULONG32 i=0;
			do{			
				temp[i]=szBuffer[bi];
				bi++;i++;
			}while((bi<filesize)&&(szBuffer[bi]!='\n'));
			//Mo_am_J_K_Class[j]=(_T("          "));
			//swscanf_s((wchar_t*)&temp,_T("%s"),Mo_am_J_K_Class[j].GetBuffer());				
		}	
		delete[] szBuffer;
	}		
	//for(int j=0;j<Mo_am_J_K_Count;j++)
		//for(int k=0;k<26;k++)
			//fwprintf_s(OutFile,_T("[%5d][%5d]%10lf\n"),k,j,Mo_am_J_K[k][j]); 
	//вывод заголовка в выходной файл
	//fwprintf_s(OutFile,_T("%10s %10s %10s %10s %10s %10s\n"),_T("Number"),_T("GLong"),_T("GLat"),_T("J"),_T("H"),_T("K"));
	//fwprintf_s(OutFile,_T("%10s %12s %12s %12s %12s %12s %12s %12s %12s %10s\n"),
	//	_T("Number"),_T("J"),_T("J-K"),_T("SinB"),_T("J0"),_T("Dist"),_T("Gj"),_T("Ejk"),
	//	_T("J_K"),_T("BadStar"));
	ULONG32 Mout_Count[12][18][18];//[36];
	//ULONG32 GalL_Count[36];
	for(UINT i=0;i<12;i++)
		for(UINT j=0;j<18;j++)
			for(UINT k=0;k<18;k++) 
				//for(UINT t=0;t<36;t++) 
					Mout_Count[i][j][k]=0;//[t]=0;
	double Distance=0.0;
	
	double J0=0.0;
	double CoeffB=1.0;
	double Mout;
	double Ejk=0.0;
	double GalL=0.0;

	int EndOfFile=0;
	UINT StarCount=0;
	UINT StarJCount=0;
	do
	{
		//J=-20;
		//K=-20;


		EndOfFile=fwscanf_s(InFile,_T("%d%lf%lf%lf%lf%lf"),&Number,&Alpha,&Delta,&J,&H,&K);
		if(EndOfFile==EOF) break;
		if(J<3)StarJCount++; 
		//фильтрация звезд
		//if(false)
		if((J-K<-0.5)||(J<-5)||(J>18)||(K<-5)||(K>18))
		{
			//плохая звезда
			BadStar++;
		}else
		{
			J_K=J-K;
			double A=cos(Alpha*GradToRad)*cos(Delta*GradToRad);
			double B=cos(Delta*GradToRad)*sin(Alpha*GradToRad);
			double G=sin(Delta*GradToRad);
			double SinB=abs(MatrixA[2][0]*A+MatrixA[2][1]*B+MatrixA[2][2]*G);
			double SinB1=(MatrixA[2][0]*A+MatrixA[2][1]*B+MatrixA[2][2]*G);
			double X=(MatrixA[0][0]*A+MatrixA[0][1]*B+MatrixA[0][2]*G);
			double Y=(MatrixA[1][0]*A+MatrixA[1][1]*B+MatrixA[1][2]*G);
			GalL=atan2(Y,X)*RadToGrad;
			//double Aj=0.0;
			//double Ak=0.0;
			//double Ejk=0.0;
			//начало итераций
			double Gj=0.0;			
			for(ULONG32 i=0;i<5;i++)
			{	
				J0=GetFlux(J_K,Mo_am_J_K[0],Mo_am_J_K[15],Mo_am_J_K_Count);
				Distance=exp((((J-J0-Gj)/5.0)+1.0)*Ln10)/1000.0;//в килопарсеках
				CoeffB=Beta/SinB*(1-exp((-Distance*SinB)/Beta));				
				double Aj=GetFlux(J_K,Mo_am_J_K[0],Mo_am_J_K[16],Mo_am_J_K_Count); // [8]=J,[12]=8-12
				double Ak=GetFlux(J_K,Mo_am_J_K[0],Mo_am_J_K[20],Mo_am_J_K_Count);
				if(SinB<0.001)
				{
					Gj=Aj*Distance;
					Ejk=(Aj-Ak)*Distance;
				}else
				{
					Gj=Aj*CoeffB;
					Ejk=(Aj-Ak)*CoeffB;
				}
				if((J_K-Ejk<-0.33) || (J_K-Ejk>2.0)) break; // break 
				J_K=J-K-Ejk;
				//fwprintf_s(OutFile,_T("%10d %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %10d\n"),
					//Number,J,J-K,SinB,J0,Distance,Gj,Ejk,J_K,BadStar);				
			}
			//конец итераций
			//J_K, Distance
			double Gm;
			for(ULONG32 i=2;i<25;i+=2)
			{
				//if(i==16)continue;

				double Am=GetFlux(J_K,Mo_am_J_K[0],Mo_am_J_K[i],Mo_am_J_K_Count); // [8]=J,[12]=8-12
				if(SinB<0.001)
				{
					Gm=Am*Distance;
				}else
				{
					Gm=Am*CoeffB;
				}
						// выбор Mo_am_J_K по J_K
				J0=GetFlux(J_K,Mo_am_J_K[0],Mo_am_J_K[i-1],Mo_am_J_K_Count);
				Mout=J0+5.0*log10(Distance)+10.0+Gm;
				//if(Mout<3.0){
					//fwprintf_s(OutFile,_T("%10d %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %10d\n"),
						//Number,Mout,J,J-K,SinB,J0,Distance,Gj,Ejk,J_K,BadStar);
				//}
				GLat=asin(SinB1)*RadToGrad;//Gala b in degrees
				
				ULONG32 k=0;
				if(Mout>3.0) k=Mout-2;
				if(Mout<20.0) 
				{														
					ULONG32 Im=(i/2)-1;
					if(GalL<0) 
						GalL+=360.0;

					ULONG32 Xm=(GalL/10);
					ULONG32 Ym=(GLat+90)/10;
					Mout_Count[Im][k][Ym]++;
					/*if (GLat<5.0)
					{							
						Mout_Count[Im][k][0][Xm]++;
					}else
					if (GLat<10.0)
					{
						Mout_Count[Im][k][1][Xm]++;
					}else
					if (GLat<30.0)
					{
						Mout_Count[Im][k][2][Xm]++;
					}else
					if (GLat<60.0)
					{
						Mout_Count[Im][k][3][Xm]++;
					}else
					if (GLat>60.0)
					{
						Mout_Count[Im][k][4][Xm]++;
					}*/
				}

			}
			GLong=0.0;
			//fwprintf_s(OutFile,_T("%10d %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf\n"),Number,GLong,GLat,J,H,K);
		}
		StarCount++;		
	}while(EndOfFile!=-1);
	CString OutStr;
	for(UINT i=0;i<12;i++)
	{
		fwprintf_s(OutFile,_T("%s %d, StarCount %d, BadStar %d\n"),_T("Table "),i,StarCount,BadStar);	
		for(UINT j=0;j<18;j++)
		{
			OutStr.Truncate(0);
			for(UINT k=0;k<18;k++)
			{
				//for(UINT t=0;t<36;t++)
				{
					OutStr.AppendFormat(_T("%10d  "),Mout_Count[i][j][k]/*[t]*/) ;
				}
				OutStr.AppendChar('|');
			}			
			fwprintf_s(OutFile,_T("%s\n"),OutStr.GetString());	
		}		
	}
				
	fclose(InFile);
	fclose(OutFile);
}
#endif
#ifdef EMOE
//*************************************************************************
double CMyTeMath::PROBKA_1(CMyTeBand* Band)		//CMyTeBand* Band
{
	//CMyTeBand* Band;
	long double Pi2=6.283185307179586476925286766559;
	//UINT BandCount=4;
	FILE *inpf;
	int len=2048;
	//int NCRV=4;
	char InLin[2048];
	//int SP_Cod[5][36]={{0,80,81,82,83,84,85,86,87,88,89,90,
	//									91,92,93,94,95,96,97,99,-1},
	//							{0,70,71,46,47,48,49,72,73,52,74,
	//									75,76,77,78,79,99,-1},
	//							{0,70,71,46,47,48,49,50,51,52,53,
	//									54,55,56,57,58,59,60,61,62,63,
	//                  64,65,66,67,68,69,99,-1},
	//							{0,70,71,46,47,34,35,36,37,38,39,
	//									40,41,42,43,44,45,60,65,-1},
	//							{0,1,2,3,4,5,6,7,8,9,10,11,12,13,
	//									14,15,16,17,18,19,20,21,22,23,
	//                  24,25,26,27,28,29,30,31,32,33,99,-1} };
	bool FlagYes=false;
	char FileInput[]="C:\\mycalculateprogramm\\MyTe\\DATA_TOT_LAST.txt";//DAT_6000.txt";//DAT_Q_6000.txt";//DATA_TOT_LAST_Q.txt";//D_010986.txt";//D_010986.txt";//DATA_6000_NL.txt";//DAT_SS.txt";////DATA_6000_NL.txt";//STD_6000.txt";//DAT_1505.txt";//DAT_WE_STAND_.txt";//hd_103945.txt";//D_S_S.txt";//
	//char FileTypesC[]="C:\\mycalculateprogramm\\MyTe\\C2000_2902.txt";//_S//CAT_LAST.txt";//
	char FileTypes[]="C:\\mycalculateprogramm\\MyTe\\CAT_LAST.txt";//CATALO_2000_2404.txt";//_S_Q
	char FileWBVR[]="C:\\mycalculateprogramm\\MyTe\\CAT_WBVR.txt";
	//char FileInput[]="C:\\mycalculateprogramm\\MyTe\\D__STAND_1.txt";		//DAT_S_WE-FON_S.txt";
	//char FileTypes[]="C:\\mycalculateprogramm\\MyTe\\2000_C_MAG_S.txt";
	char FileSpect[]="C:\\mycalculateprogramm\\MyTe\\SP_SV_A.txt";
	//char FileExtin[]="C:\\mycalculateprogramm\\MyTe\\EXTINCT.txt";
	char ErrFile[]="C:\\mycalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	char OutFile[]="C:\\mycalculateprogramm\\MyTe\\RES.txt";
	char OutNormFile[]="C:\\mycalculateprogramm\\MyTe\\RES_TOT.txt";//RES_N_6000.txt";//RES_Q_N_6000.txt";
	char OutNormEndFile[]="C:\\mycalculateprogramm\\MyTe\\RES_TOT_DIFF.txt";//RES_Q_DIFF_6000.txt";//RES_Q_DIFF_6000.txt";//RES6000_N_End.txt";
    CMyTeBand Extint;// пользоваться как обычным бандом
    // задаём Имя файла
    Extint.FName=_T("C:\\mycalculateprogramm\\MyTe\\EXTINCT_AA.txt");

    if(Extint.FName!="") {
        // загружаем файл
        Extint.LoadFromFile(Extint.FName);	
    }
    

//открываем входной файл данных по всем ночам
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//загружаем ВСЕ строковые данные (включая управляющие символы типа 'W,E,M,I,{,}...)
	//в массив Date0, кроме помеченных '*' в начале строки 
	UINT  Count_str=0;  // Подсчёт кол-ва строк в файле прототипов
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//AfxMessageBox(_T("End Reading data_total File"));
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	 fseek(inpf, 0L, SEEK_SET);
	 char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
    			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
 				memset(data[i],0,len);
	   			data[i][0]=0;
    			memcpy(data[i],InLin,strlen(InLin));
				i++;
			}// 	
	   }
	   catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		  //Application->MessageBox("Could not allocate. Bye ...",
			   //"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
	   }
	 }
	 fclose(inpf);
	//открыли файл типов с координатами
	 if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
			AfxMessageBox(_T("Error Opening Coord File"));
		  return -1.0;
	 }
	// Подсчёт кол-ва строк в файле координат
	 UINT  Count_coord=0;  
	 if (inpf) {
	   while (!feof(inpf)){
    		if ( fgets(InLin,len,inpf)==0 ){
				//AfxMessageBox(_T("End Reading Coord File"));
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_coord++;
			}
		}
	  }
	 fseek(inpf, 0L, SEEK_SET);
	 char** data1 = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_coord; ){
		if ( fgets(InLin,len,inpf)==0 ) break;
		if (InLin[0]=='*'){ continue;
		}else{
			data1[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
			data1[i][0]=0;
			memcpy(data1[i],InLin,strlen(InLin));
			i++;// 	
		}
	 }
	 fclose(inpf);
 //ввод старого каталога WBVR
	 if (fopen_s(&inpf, FileWBVR, "r") !=0 ){
		 AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR
	 UINT  Count_WBVR=0;  
	 if (inpf) {
		 while (!feof(inpf)){
			 if ( fgets(InLin,len,inpf)==0 ){
				 //AfxMessageBox(_T("End Reading WBVR File"));
				 break;
			 }
			 if (InLin[0]=='*'){ continue;
			 }else{
				 Count_WBVR++;
			 }
		 }
	 }
	 fseek(inpf, 0L, SEEK_SET);
	 char** Data2 = new char*[Count_WBVR];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_WBVR; ){
		 //try {                      // TEST FOR EXCEPTIONS.
			 if ( fgets(InLin,len,inpf)==0 ) break;
			 if (InLin[0]=='*'){ continue;
			 }else{
				 Data2[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
				 Data2[i][0]=0;
				 memcpy(Data2[i],InLin,strlen(InLin));
				 i++;// 	
			 }
		 //}
		 //catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			// // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			// //Application->MessageBox("Could not allocate. Bye ...",
			// //"Error...", MB_ICONERROR);
			// AfxMessageBox(_T("Error Reading File WBVR"));
			// exit(-1);
		 //}
	 }
	 fclose(inpf);

	 //if(fopen_s(&inpf, FileTypesC, "r") !=0 ){ 
		// AfxMessageBox(_T("Error Opening Coord File"));
		// return -1.0;
	 //}
	 //// Подсчёт кол-ва строк в файле координат
	 //UINT  Count_coordC=0;  
	 //if (inpf) {
		// while (!feof(inpf)){
		//	 if ( fgets(InLin,len,inpf)==0 ){
		//		 //AfxMessageBox(_T("End Reading Coord File"));
		//		 break;
		//	 }
		//	 if (InLin[0]=='*'){ continue;
		//	 }else{
		//		 Count_coordC++;
		//	 }
		// }
	 //}
	 //fseek(inpf, 0L, SEEK_SET);
	 //char** data1C = new char*[Count_coordC];        // STEP 1: SET UP THE ROWS.
	 //for (UINT i=0; i<Count_coordC; ){
		// if ( fgets(InLin,len,inpf)==0 ) break;
		// if (InLin[0]=='*'){ continue;
		// }else{
		//	 data1C[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
		//	 data1C[i][0]=0;
		//	 memcpy(data1C[i],InLin,strlen(InLin));
		//	 i++;// 	
		// }
	 //}
	 //fclose(inpf);

	//считаем число строк без управляющих символов Count_str
	//создаем массивы входных данных 
		typedef struct Date0 {
			bool FlagYes;
			char Name[14];
			char Spect[4];
			char DDMOYY[7];
			char HHMMSS[7];
			char KLAD[2];
			char STAND[2];
			double* SpED;
			double UT;
			double JD;
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double Ri[MaxBand];
			double RO[MaxBand];
			double RM[MaxBand];
			double RD[MaxBand];
			double* eR;
			double TimeAngle;
			double Mz;
			double SidTime;
		} Date0;
		typedef struct WBVR_O{
			char Name[12];
			double RO[MaxBand];
		}WBVR_O;
		typedef struct TYPES{
			char Name[12];
			char TyName[13];
			char TSp[4];
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double eRA;
			double eDE;
			double RT[2];
			double eRT[2];
		}TYPES;
	char** ptr=new char* [200];
	TYPES* TYP = new TYPES[Count_coord];
	//TYPES* TYPC= new TYPES[Count_coord];
	//char** Types_Name = new char*[Count_coord];
	//char** Types_Sp = new char*[Count_coord];
	//for(UINT i=0;i<Count_coord;i++){
	//	Types_Name[i]=new char [10];
	//	memset(Types_Name[i],0,10);
	//	Types_Sp[i]=new char [4];
	//	memset(Types_Sp[i],0,4);
	//}
	//double* Types_RA=new double[Count_coord];
	//double* Types_pmRA=new double[Count_coord];
	//double* Types_DE=new double[Count_coord];
	//double* Types_pmDE=new double[Count_coord];
	//if (fopen_s(&errf, ErrFile, "a") !=0 ){
	//	AfxMessageBox(_T("Error Opening Err File"));
	//}
	for(UINT i=0;i<Count_coord;i++){
		if(StrPtr(data1[i],'|',ptr)!=0)
		{
			sscanf_s(ptr[3],"%lf",&TYP[i].RA);
			sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
			sscanf_s(ptr[4],"%lf",&TYP[i].DE);
			sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
			sscanf_s(ptr[7],"%lf",&TYP[i].RT[0]);
			sscanf_s(ptr[8],"%lf",&TYP[i].eRT[0]);
			sscanf_s(ptr[9],"%lf",&TYP[i].RT[1]);
			sscanf_s(ptr[10],"%lf",&TYP[i].eRT[1]);
			memcpy((void*)TYP[i].Name,(const void*)ptr[2],9);
			TYP[i].Name[9]=0;
			memcpy((void*)TYP[i].TyName,(const void*)ptr[1],12);
			TYP[i].TyName[12]=0;
			memcpy((void*)TYP[i].TSp,(void*)ptr[11],3);
			TYP[i].TSp[3]=0;
		}
	}
#ifdef ETBOE
	for(UINT i=0;i<Count_coordC;i++){
		if(StrPtr(data1C[i],'|',ptr)!=0)
		{
			//sscanf_s(ptr[3],"%lf",&TYP[i].RA);
			//sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
			//sscanf_s(ptr[4],"%lf",&TYP[i].DE);
			//sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
			//sscanf_s(ptr[7],"%lf",&TYP[i].RT[0]);
			//sscanf_s(ptr[8],"%lf",&TYP[i].eRT[0]);
			//sscanf_s(ptr[9],"%lf",&TYP[i].RT[1]);
			//sscanf_s(ptr[10],"%lf",&TYP[i].eRT[1]);
			memcpy((void*)TYPC[i].Name,(const void*)ptr[2],9);
			TYPC[i].Name[9]=0;
			memcpy((void*)TYPC[i].TyName,(const void*)ptr[1],12);
			TYPC[i].TyName[12]=0;
			memcpy((void*)TYPC[i].TSp,(void*)ptr[11],3);
			TYPC[i].TSp[3]=0;
		}else {
			fprintf_s(errf,"%s|",TYPC[i].TyName);
			return -1;
		}
	}
	bool FlagYes0=false;
	for(UINT i=0;i<Count_coord;i++){
		FlagYes=false;
		FlagYes0=false;
		for(UINT j=0;j<Count_coordC;j++){
			if(strncmp(TYP[i].Name,TYPC[j].Name,9)==0){
				if(strncmp(TYP[i].TSp,TYPC[j].TSp,3)!=0 ){
					memcpy((void*)TYP[i].TSp,TYPC[j].TSp,4);//TYPC[i].TSp=TYP[j].TSp;
					fprintf(errf,"%s|%s|\n",TYP[i].Name,TYP[i].TSp);
					FlagYes=true;
					break;
				}else FlagYes0=true;
			}else continue;
		}
		if(!FlagYes ){
			if(!FlagYes0) fprintf(errf,"%s|%s|\n",TYP[i].Name,TYP[i].TSp);//TYPC[i].TSp);
			else fprintf(errf,"%s|%s|\n",TYP[i].Name,TYP[i].TSp);
		}
	}
	fclose(errf);
	//delete [] DS;
	//delete [] DJ;
	//delete [] WbvrO;
	delete [] TYP;
	delete [] TYPC;
	de_allocate(data,Count_str);
	de_allocate(data1,Count_coord);
	de_allocate(data1C,Count_coord);
	de_allocate(Data2,Count_WBVR);
	_fcloseall();
	return 0.0;
}
//#ifdef ETBOE
//#ifdef DONETKNOW
#endif

	//создаем массив структур=строк
	//W_O* WO = 0;
	WBVR_O* WbvrO = new WBVR_O[Count_WBVR];
	//WO = new W_O[Count_WBVR];
//#endif
	Date0* DS = new Date0[Count_str];
	Date0* DJ = new Date0[Count_str];
	//W_O* WO = new W_O[Count_WBVR];
	//долгота Алматы 76 58 04.5=5 07 52.3=76.9679167 deg = 1.343343564233 rad
	//широта Алматы 43 04 11 = 43.069722 deg = 0.751708 rad
	//double SINfi=0.682887823460;
	//double CoSfi=0.730523251217;
//читаем дату из названия файла (DDMMYY.FEA.txt) как DD,MO,YY;
//либо из файла
	int DD,MO,YY,HH,MM,SS;
	int Year;
	int WBVR[MaxBand];
	for(UINT i=0;i<MaxBand;i++){
		wchar_t NC = Band[i].Name;
		switch(NC){
			case _T('W'):
			case _T('w'):
				WBVR[0] = i;				
				break;
			case _T('B'):
			case _T('b'):
                WBVR[1] = i;
				break;
			case _T('V'):
			case _T('v'):
                WBVR[2] = i;
				break;
			case _T('R'):
			case _T('r'):
                WBVR[3] = i;
				break;
			default:break;
		}
		//Search_Band(&Band[i],&Band[i].LBeg,&Band[i].LEnd);
		//double LB=287;
		//double LE=362;
		Search_Band(&Band[i],&Band[i].LBeg,&Band[i].LEnd);
		//Search_Band(&Band[i],&LB,&LE);
		//Band[i].LBeg=LB;
	}

//************ ВВод спектров *******************
//==============================================
	 if(fopen_s(&inpf, FileSpect, "r") !=0 ){ 
		AfxMessageBox(_T("Error Opening Spectra File"));
		//goto error_return;
/*		break;*///return -1.0;
	 }
	 fgets(InLin,len,inpf); //Name Spectra
	int NCS=0;
	 NCS=StrPtr(&InLin[8],' ',ptr);
	//NCS; // Кол-во колонок в SpData без волн = число типов спектра
	char **NameSp=new char* [NCS];
		for(UINT i=0;i<NCS;i++){
			NameSp[i]=new char [4];
			memset(NameSp[i],0,4);
		}
	//char buff[10];
	for(UINT i=0;i<NCS;i++){
		memcpy((void*)NameSp[i],(void*)ptr[i],3);
		NameSp[i][3]=0;
		//sscanf_s(ptr[i],"%s",buff);//NameSp[i]);
	//		memcpy((void*)&DS[i].Name,(void*)&data[i][17],9);
		//memcpy((void*)NameSp[i],(void*)&buff,strlen(buff)+1);
		//fprintf(errf,"%s ",NameSp[i]);
	}
	UINT DLRSP=0;
	while(1){
		if(fgets(InLin,len,inpf)==0) break;
	  ++DLRSP;
	}
	double *SpLambd = new double[DLRSP];
	double **SpSV = new double*[NCS];
	for(UINT i=0;i<NCS;i++){
		SpSV[i] = new double[DLRSP];  // STEP 2: SET UP THE COLUMNS
	}
	fseek(inpf, 0L, SEEK_SET);
	fgets(InLin,len,inpf);//съели заголовок
	for(UINT j=0;j<DLRSP;j++){
		fgets(InLin,len,inpf);
		sscanf_s(InLin,"%lf",&SpLambd[j]);
	  StrPtr(&InLin[8],' ',ptr);
	  for(UINT i=0;i<NCS;i++){
  		if(sscanf_s(ptr[i],"%lf",&SpSV[i][j])==NULL) {
    		SpSV[i][j]=99999.9;
		}// break;}
		//fprintf(errf,"%10.1lf ",SpSV[i][j]);
	  }
	  //fputs("\n",errf);
	}	
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Err File"));
	}
//заполняем DS[i]
char SpBUFF[]="A05 ";
SpBUFF[3]=0;
//вводим старые величины WBVR
double R[MaxBand];
for(UINT i=0;i<Count_WBVR;i++){
	bool FlagYes=false;
	if(StrPtr(Data2[i],'|',ptr)!=0){
		memcpy((void*)WbvrO[i].Name,(void*)ptr[0],11);
		WbvrO[i].Name[11] = '\0';
		for(UINT k=0;k<MaxBand;k++){
			sscanf_s(ptr[k+5],"%lf",&R[k]);
		}
	}//W=W_B+B_V+V
	WbvrO[i].RO[0]=R[1]+R[2]+R[0];
	WbvrO[i].RO[1]=R[2]+R[0];
	WbvrO[i].RO[2]=R[0];
	WbvrO[i].RO[3]=R[0]-R[3];
}
//наполнение DS
	double N;
	for(UINT i=0;i<Count_str;i++){//
		bool FlagYes=false;
		if(StrPtr(data[i],'|',ptr)!=0){
			memcpy((void*)&DS[i].Name,(void*)ptr[2],11);
			DS[i].Name[11] = '\0';
			DS[i].SpED=new double [DLRSP];
			DS[i].FlagYes=false;
			//DS[i].Ri=new double [MaxBand];
			//sscanf(ptr[1],"%1c",&DS[i].KLAD);
			memcpy((void*)&DS[i].KLAD,(void*)ptr[1],1);
			DS[i].KLAD[1]=0;
			//sscanf(ptr[9],"%1c",&DS[i].STAND);
			memcpy((void*)&DS[i].STAND,(void*)ptr[9],1);
			DS[i].STAND[1]=0;
			for(UINT k=0;k<MaxBand;k++){
				sscanf_s(ptr[k+3],"%lf",&DS[i].Ri[k]);
				N=DS[i].Ri[k];
				if (N<=0.0){
					DS[i].Ri[k]=0.0;
				} else{
					DS[i].Ri[k]=-2.5*log10(N/(1-28e-9*N));
				}
			}
			memcpy((void*)&DS[i].DDMOYY,(void*)ptr[7],6);
			memcpy((void*)&DS[i].HHMMSS,(void*)ptr[8],6);
			//sscanf_s(ptr[7],"%s",&DS[i].DDMOYY);
			//sscanf_s(ptr[8],"%s",&DS[i].HHMMSS);
			DS[i].DDMOYY[6] = '\0';
			DS[i].HHMMSS[6] = '\0';

			sscanf_s(ptr[7],"%2d%2d%2d",&DD,&MO,&YY);
			sscanf_s(ptr[8],"%2d%2d%2d",&HH,&MM,&SS);
		}
		int Dt=MO*100+DD;
		if(YY<40) Year=2000+YY;
		else Year=1900+YY;
		DS[i].UT=(double)HH+(double)MM/60.0+(double)SS/3600.0;
	//вычисляем порядковый номер дня от 0 января 1950 года
		int JD50 = JDm1950(Year, Dt);
	//Вычисляем среднее звездное гринвичское время в радианах на 0 часов UT на данное число
	//JD50 - число суток от 0 января 1950 г.
		double S0=Stims(JD50); //S0 в радианах
		double JD= (double)JD50+DS[i].UT/24;
		DS[i].JD=JD;
		DS[i].SidTime=fmod( (double)(S0 + (double)(1.0027379*DS[i].UT*
						0.2617993877983 + 1.34334356423)),(double)Pi2 );
//получение и рассылка координат звезд
		FlagYes=false;
		for(UINT j=0;j<Count_coord;j++){
			int a=strncmp(TYP[j].Name,DS[i].Name,9);
			if(a==0){
				//DS[i].Spect = Types_Sp[j];
				memcpy(DS[i].Spect,TYP[j].TSp,4);
				DS[i].Spect[3]=0;
				DS[i].RA=TYP[j].RA;
				DS[i].DE=TYP[j].DE;
				DS[i].pmRA=TYP[j].pmRA;
				DS[i].pmDE=TYP[j].pmDE;
				if((DS[i].TimeAngle = DS[i].SidTime - TYP[j].RA * Pi/180)<0)
					DS[i].TimeAngle += Pi2;//t=s-RA в радианах		
				DS[i].Mz=Mz(DS[i].TimeAngle, SINfi, COSfi, DS[i].DE*Pi/180);
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			//wchar_t buf[60];
			//wchar_t buf2[60];
			//memset(buf,0,sizeof(buf)*sizeof(wchar_t));
			//memset(buf2,0,sizeof(buf)*sizeof(wchar_t));
			// перевод из char* в wchar_t* ( макрос _T() работает только для констант)
			//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, DS[i].Name, strlen(DS[i].Name), buf2,sizeof(buf2));
			//wsprintf(buf, _T("Type Not Found '%s'"), buf2);
			//AfxMessageBox(buf);
			fprintf(errf,"%s\n",DS[i].Name);
			DS[i].TimeAngle=0;
			DS[i].DE=0;
			DS[i].Mz=1.0;
			memcpy(DS[i].Spect,SpBUFF,4);
			//DS[i].Spect=SpBUFF;
		}
		FlagYes=false;
		for(UINT j=0;j<Count_WBVR;j++){
			if(strncmp((const char*)DS[i].Name,(const char*)WbvrO[j].Name,9)==0){
				for(UINT k=0;k<MaxBand;k++){
					DS[i].RO[k]=WbvrO[j].RO[k];
				}
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			//fprintf(errf,"%s\n",DS[i].Name);
			for(UINT k=0;k<MaxBand;k++){
				DS[i].RO[k]=0.0;
			}
		}
		//fprintf(outf,"%s|  %s| %s",DS[i].Name,DS[i].KLAD,DS[i].STAND);
		//fprintf(outf,"  %7.3lf|  %9.5lf|  %8.4lf|",DS[i].TimeAngle/Pi*12,DS[i].DE,DS[i].Mz);
		//for(UINT k=0;k<MaxBand;k++){
		//	fprintf(outf,"  %7.3lf|",DS[i].Ri[k]);
		//}
		//fprintf(outf,"  %9.5lf|",DS[i].JD);
		//fprintf(outf,"%s|  %02d%02d%02d|%02d%02d%02d",DS[i].Spect,DD,MO,YY,HH,MM,SS);
		//for(UINT k=0;k<MaxBand;k++){
		//	fprintf(outf,"|  %7.3lf",DS[i].RO[k]);
		//}
		//fputs("\n",outf);
	}
//#endif
	//fclose(outf);
//#ifdef EMOE

	//************ Выбор Типов Спектров ********************//
	//======================================================//

	int ic=0;
	UINT p1,p2,cc;

	for(UINT i=0;i<Count_str;i++){
	//void _FLambda(float cc,TYPES ptt,UINT NN){
	UINT m=0;
		cc=SpSearch(DS[i].Spect[0],DS[i].Spect[1]);
		if(DS[i].Spect[2]==' '){
			if (cc>58){ ic=2;
		} else	ic=4;
		}else {
			  sscanf_s(&DS[i].Spect[2],"%1d", &ic);
	  	  ic-=1; // Класс светимости
	  }
	  while(1){
  		if(SP_Cod[ic][m+1]<0){
    		for(UINT j=0;j<DLRSP;j++){
	    		DS[i].SpED[j]=SpSV[SP_Cod[ic][m]][j];
				//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",DS[i].Spect,SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
			}
			break;//return;
  		}else{
			// 		p1=SpSearch(NameSp[m][0],NameSp[m][1]);
			//p2=SpSearch(NameSp[m+1][0],NameSp[m+1][1]);
			p1=SpSearch(NameSp[SP_Cod[ic][m]][0],NameSp[SP_Cod[ic][m]][1]);
			p2=SpSearch(NameSp[SP_Cod[ic][m+1]][0],NameSp[SP_Cod[ic][m+1]][1]);
  			if( (cc>=p1)&&(cc<p2) ){
				for(\\\\\\\\\\\\\\\UINT j=0;j<DLRSP;j++){
					DS[i].SpED[j]= LinInt(SpSV[SP_Cod[ic][m]][j],SpSV[SP_Cod[ic][m+1]][j],
							(double)p1,(double)p2,(double)cc);
					//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",NameSp[SP_Cod[ic][m]],SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
				}
				break;//	      return;
			}else{ m++;}
		}
	  }
	}
	fclose(errf);
//Подготовка подинтегральных функций для вычисления Ai
	double **SubintP = new double* [MaxBand];
	double **Subint = new double* [MaxBand];
	for(UINT k=0;k<MaxBand;k++){
		SubintP[k] = new double [DLRSP];
		Subint[k] = new double [DLRSP];
	}
	for(UINT i=0;i<Count_str;i++){		
		for(UINT k=0;k<MaxBand;k++){
			for(UINT j=0;j<DLRSP;j++){
				double a=DS[i].SpED[j];
				double b=Band[WBVR[k]].GetFlux(SpLambd[j]);
				double c=Extint.GetFlux(SpLambd[j]);
				double cc=pow(c,DS[i].Mz);//Extint.GetFlux(SpLambd[j])
				SubintP[k][j] = a*b*cc;
				Subint[k][j] = a*b;
					//DS[i].SpED[j]*Band[WBVR[k]].GetFlux(SpLambd[j])*pow(Extint.GetFlux(SpLambd[j]),DS[i].Mz);// Sp*Band*p^Mz 		sscanf(InLin,"%lf%lf",&EXT_Lambd[i],&EXT_Flux[i]);
			}
			double d= Integral_M(SubintP[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
			double dd=Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
			double ddd=	-2.5*log10(d/dd);		//double dd=

			DS[i].Ri[k]-=ddd; //-2.5*log10(Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd));
		}
		i=i;
	}
	//delete [] SubintP[k];
	//delete [] Subint[k];

	//открыли выходной файл
	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening result File"));
	}
	fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|\n");

//#endif
	//double a=100000.0;
	UINT NN=Count_str;
	//double b=0;
//сортир по JD
	for(UINT i=0;i<NN;i++){
		double a=100000.0;
		for(UINT j=0;j<NN;j++){		//Count_str
			if (DS[j].FlagYes) continue;
			a=MIN(a,DS[j].JD);
		}
		for(UINT j=0;j<NN;j++){		//Count_str
			if (DS[j].FlagYes) continue;
			if (a==DS[j].JD){
				DJ[i]=DS[j];
				DS[j].FlagYes=true;
				fprintf(outf,"%s|  %s| %s|",DJ[i].Name,DJ[i].KLAD,DJ[i].STAND);
				fprintf(outf,"  %7.3lf|  %9.5lf|  %8.4lf|",DJ[i].TimeAngle/Pi*12,DJ[i].DE,DJ[i].Mz);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %7.3lf|",DJ[i].Ri[k]);
				}
				fprintf(outf,"%11.5lf|",DJ[i].JD);
				fprintf(outf,"%s|%s|%s",DJ[i].Spect,DJ[i].DDMOYY,DJ[i].HHMMSS);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"|%7.3lf",DJ[i].RO[k]);
				}
				fputs("\n",outf);
				break;
			}else continue;
		}
	}
	fclose(outf);
	if (fopen_s(&outf, OutNormFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening result NormFile"));
	}
	fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|\n");
// нормализация
		for(UINT i=0;i<NN;i++){
			if(DJ[i].STAND[0]=='S'){
				for(UINT k=0;k<MaxBand;k++)	
					DJ[i].RM[k]=DJ[i].RO[k]-DJ[i].Ri[k];
			}else 
				for(UINT k=0;k<MaxBand;k++)	
					DJ[i].RM[k]=DJ[i].Ri[k];
		}
	//}
		int a=0;
	int KK=0;//число наблюдений в ночи
	int kk=0;//число стандартов в ночи
	int CountTypSt=0;//число типов стандартов в ночи
	//			double A=
#define	MaxTypSt 100
#define	MaxSt 300
	Date0** DDST = new Date0*[MaxTypSt];
	for (UINT i=0;i<MaxTypSt;i++){
		DDST[i]=new Date0 [MaxSt];
	}
	Date0* DDS = new Date0[MaxSt];
	double DDD[MaxTypSt][MaxBand];
	UINT NS[MaxTypSt];
// НАЧАЛО КОНЕЧНОЙ ОБРАБОТКИ
	UINT I=0;
	for( UINT i=0;i<NN;){
		kk=0;
		KK=0;
		int MA=(int)DJ[i].JD;
		do{
			if ( kk==0 && (int)DJ[i+1].JD!=MA )	{
				break;
			}
			if(DJ[i].STAND[0]=='S'){
				DDS[kk]=DJ[i];//[si]
				kk++;
			}
			KK++;
			i++;
		} while( (int)DJ[i].JD==MA );  //

		I=i-KK;
		if( kk==0){
			for(UINT k=0;k<MaxBand;k++){
				DJ[I].RM[k]=DJ[I].RO[k];
			}
			i++;
			continue;
		}else{
			double RA=0;
			double DE=0;
			for(UINT jk=0,ii=0;jk<kk;jk++){ //перетряс стандартов ночи
				if(!DDS[jk].FlagYes){
					RA=DDS[jk].RA;
					DE=DDS[jk].DE;
				}else continue;
				int ij=0;
				for(UINT k=0;k<MaxBand;k++)
					DDD[ii][k]=0;
				for (UINT ik=0;ik<kk;ik++){
					if(DDS[ik].RA==RA && DDS[ik].DE==DE){
						DDST[ii][ij]=DDS[ik];
						for(UINT k=0;k<MaxBand;k++){
							DDD[ii][k]+=DDST[ii][ij].RM[k];
						}
						DDS[ik].FlagYes=true;
						ij++;
						NS[ii]=ij;
					}else continue;
				}
				for(UINT k=0;k<MaxBand;k++){
					DDD[ii][k]/=ij;
				}
				ii++;//if(ij<kk)
			}
		}
	//}

//	for(UINT i=0;i<NN;i++){
//		fprintf(outf,"%s|  %s| %s|",DJ[i].Name,DJ[i].KLAD,DJ[i].STAND);
//		fprintf(outf,"  %7.3lf|  %9.5lf|  %9.5lf|  %8.4lf|",DJ[i].TimeAngle/Pi*12,DJ[i].RA,DJ[i].DE,DJ[i].Mz);
//		for(UINT k=0;k<MaxBand;k++){
//			fprintf(outf,"  %7.3lf|",DJ[i].RM[k]);
//		}
//		fprintf(outf,"%11.5lf|",DJ[i].JD);
//		fprintf(outf,"%s|%s|%s",DJ[i].Spect,DJ[i].DDMOYY,DJ[i].HHMMSS);
//		for(UINT k=0;k<MaxBand;k++){
//			fprintf(outf,"|%7.3lf",DJ[i].RO[k]);
//		}
//		fputs("\n",outf);
//	}
//	_fcloseall();
//	return 0.0;
//}
//#ifdef ETBOE
		for(UINT ik=I;ik<KK+I;ik++){//Обработка ночи
			//if (ik==23163)
			//{
			//	ik=ik;
			//}
			if (DJ[ik].STAND[0]=='S'){
				for(UINT ii=0;ii<MaxTypSt;ii++){
					if (DJ[ik].RA == DDST[ii][0].RA && DJ[ik].DE == DDST[ii][0].DE){
						for(UINT k=0;k<MaxBand;k++) {
							DJ[ik].RM[k]=DDD[ii][k]+DJ[ik].Ri[k];
						}
						break;
					}else{ continue;}
				}
			} else{	// не стандарты
				bool FlagYes=false;
			for(UINT ii=0;ii<MaxTypSt;ii++){
				if ((abs(DJ[ik].Mz - DDST[ii][0].Mz)<0.15)&& (abs(DJ[ik].RA - DDST[ii][0].RA)<20) && (abs(DJ[ik].DE - DDST[ii][0].DE)<15 ))
				{
					if( DJ[ik].JD<DDST[ii][0].JD ){
						for(UINT k=0;k<MaxBand;k++){
							DJ[ik].RM[k]+=DDST[ii][0].RM[k];
						}
						FlagYes=true;
						break;
					}
					if( DJ[ik].JD>DDST[ii][NS[ii]-1].JD ){
						for(UINT k=0;k<MaxBand;k++){
							DJ[ik].RM[k]+=DDST[ii][NS[ii]-1].RM[k];
						}
						FlagYes=true;
						break;
					}
					for(UINT ij=0;ij<kk;ij++){
						if( (DJ[ik].JD>DDST[ii][ij].JD) && (DJ[ik].JD<DDST[ii][ij+1].JD) )/* &&*/ {
							for(UINT k=0;k<MaxBand;k++){
								double b=LinInt(DDST[ii][ij].RM[k],DDST[ii][ij+1].RM[k],DDST[ii][ij].JD,DDST[ii][ij+1].JD,DJ[ik].JD);
								DJ[ik].RM[k]+=b;
							}
							FlagYes=true;
							break;
						}else continue;//ij++;
					}
				}else continue;
				break;
			}
			if (!FlagYes){	//если не найдены подходящие стандарты - любые соседние
				for(UINT ij=0;ij<kk;ij++){
					if( (DJ[ik].JD>DDS[ij].JD) && (DJ[ik].JD<DDS[ij+1].JD) ){
						for(UINT k=0;k<MaxBand;k++){
							double b=LinInt(DDS[ij].RM[k],DDS[ij+1].RM[k],DDS[ij].JD,DDS[ij+1].JD,DJ[ik].JD);
							DJ[ik].RM[k]+=b;
						}
						break;
					}
					if( DJ[ik].JD<DDS[0].JD ){
						for(UINT k=0;k<MaxBand;k++){
							DJ[ik].RM[k]+=DDS[0].RM[k];
						}
						break;
					}
					if( DJ[ik].JD>DDS[kk-1].JD ){
						for(UINT k=0;k<MaxBand;k++){
							DJ[ik].RM[k]+=DDS[kk-1].RM[k];
						}
						break;
					}
				}
			}/*else{
			*/	}
		}
		i=I+KK;
	}
//	контрольная выдача величин
	for(UINT i=0;i<NN;i++){
		fprintf(outf,"%s|%s|%s|",DJ[i].Name,DJ[i].KLAD,DJ[i].STAND);
		fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DJ[i].TimeAngle/Pi*12,DJ[i].RA,DJ[i].DE,DJ[i].Mz);
		for(UINT k=0;k<MaxBand;k++){
			fprintf(outf,"  %10.6lf|",DJ[i].RM[k]);
		}
		fprintf(outf,"%11.5lf|",DJ[i].JD);
		fprintf(outf,"%s|%s|%s",DJ[i].Spect,DJ[i].DDMOYY,DJ[i].HHMMSS);
		for(UINT k=0;k<MaxBand;k++){
			fprintf(outf,"|%7.3lf",DJ[i].RO[k]);
		}
		fputs("\n",outf);
	}
		fclose(outf);
#endif
		//fcloseall();
//	return 0.0;
//}
#ifdef ETBOE

	for(UINT k=0;k<MaxBand;k++){DJ[i].RM[k]}
	}
			for(UINT k=0;k<MaxBand;k++)
				DDD[k]=Ddd[k]/(double)kk;
			for(UINT ik=0;ik<KK;ik++){
				i=ik+I;
				if(DJ[i].STAND[0]=='S'){
					for(UINT k=0;k<MaxBand;k++){
						DJ[i].RM[k]=DDD[k]+DJ[i].Ri[k];
					}
				}
				else{
					for(UINT ij=0;ij<kk;ij++){
						if( (DJ[i].JD>DSJ[ij]) && (DJ[i].JD<DSJ[ij+1]) ){
							for(UINT k=0;k<MaxBand;k++){
								double b=LinInt(DSK[ij][k],DSK[ij+1][k],DSJ[ij],DSJ[ij+1],DJ[i].JD);
								DJ[i].RM[k]+=b;
							}
							break;
						}
						if( DJ[i].JD<DSJ[0] ){
							for(UINT k=0;k<MaxBand;k++){
								DJ[i].RM[k]+=DSK[0][k];
							}
							break;
						}
						if( DJ[i].JD>DSJ[kk-1] ){
							for(UINT k=0;k<MaxBand;k++){
								DJ[i].RM[k]+=DSK[kk-1][k];
							}
							break;
						}
					}
				}
			}
		}
		i++;
	}
	//for(UINT i=0;i<NN;i++){
	//	fprintf(outf,"%s|  %s| %s|",DJ[i].Name,DJ[i].KLAD,DJ[i].STAND);
	//	fprintf(outf,"  %7.3lf|  %9.5lf|  %9.5lf|  %8.4lf|",DJ[i].TimeAngle/Pi*12,DJ[i].RA,DJ[i].DE,DJ[i].Mz);
	//	for(UINT k=0;k<MaxBand;k++){
	//		fprintf(outf,"  %7.3lf|",DJ[i].RM[k]);
	//	}
	//	fprintf(outf,"%11.5lf|",DJ[i].JD);
	//	fprintf(outf,"%s|%s|%s",DJ[i].Spect,DJ[i].DDMOYY,DJ[i].HHMMSS);
	//	for(UINT k=0;k<MaxBand;k++){
	//		fprintf(outf,"|%7.3lf",DJ[i].RO[k]);
	//	}
	//	fputs("\n",outf);
	//}
//#endif
\\
\\\\\\\\\\\\\\\\\\\\if (fopen_s(&outf, OutNormEndFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening result NormFile"));
	}
	fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|dW|dB|dV|dR|JD|Sp|Date|Time|WO|BO|VO|RO|\n");

	for(UINT i=0;i<NN;i++){
		DJ[i].FlagYes=false;
	}
	char buff[12];
	for(UINT i=0;i<NN;i++){
		//fprintf(outf,"%7d|",i);
		if(!DJ[i].FlagYes)	
			memcpy(buff,DJ[i].Name,12);
		else continue;
		kk=0;
		double Ddd[MaxBand]={0,0,0,0};
		for(UINT j=0,ij=0;j<NN;j++){
			int a=strncmp(buff,DJ[j].Name,11);
			if(a==0){
				DS[ij]=DJ[j];
				DJ[j].FlagYes=true;
				for(UINT k=0;k<MaxBand;k++){
					Ddd[k]+=DS[ij].RM[k];
				}
				kk++;
				ij++;
			}else{continue;}
		}
		if(kk>1){
			for(UINT k=0;k<MaxBand;k++){	
				Ddd[k]/=(double)kk;
				for(UINT ij=0;ij<kk;ij++)	
					DS[ij].RD[k]=DS[ij].RM[k]-Ddd[k];
			}
			for(UINT ij=0;ij<kk;ij++){
				fprintf(outf,"%s|%s|%s|",DS[ij].Name,DS[ij].KLAD,DS[ij].STAND);
				fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DS[ij].TimeAngle/Pi*12,DS[ij].RA,DS[ij].DE,DS[ij].Mz);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DS[ij].RM[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DS[ij].RD[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				fprintf(outf,"%11.5lf|",DS[ij].JD);
				fprintf(outf,"%s|%s|%s",DS[ij].Spect,DS[ij].DDMOYY,DS[ij].HHMMSS);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"|%7.3lf",DS[ij].RO[k]);
				}
				fputs("\n",outf);
			}
		} //else DJ[i].FlagYes=true;
	}
//#endif//endif
	fclose(outf);
		//else
		//{
		//}
	delete [] DS;
	delete [] DJ;
	delete [] WbvrO;
	delete [] TYP;
	de_allocate(data,Count_str);
	de_allocate(data1,Count_coord);
	de_allocate(Data2,Count_WBVR);
_fcloseall();
  return 0.0;
}
#endif
//endif
//*************************************************************************
#ifdef TVOE
// ПРОГРАММА ВЫНЕСЕНИЯ ЗА АТМОСФЕРУ "SIRIUS" OH-8
double CMyTeMath::PROBKA(vector <CMyTeBand> Band)		//CMyTeBand* Band
{
//#ifdef ETBOE
	//CMyTeBand* Band;
//	long double Pi2=6.283185307179586476925286766559;
	//UINT BandCount=4;
	//FILE *inpf;
	//int len=2048;
	//int NCRV=4;
	char InLin[len];
	int SP_Cod[5][36]={{0,80,81,82,83,84,85,86,87,88,89,90,
										91,92,93,94,95,96,97,99,-1},
								{0,70,71,46,47,48,49,72,73,52,74,
										75,76,77,78,79,99,-1},
								{0,70,71,46,47,48,49,50,51,52,53,
										54,55,56,57,58,59,60,61,62,63,
	                  64,65,66,67,68,69,99,-1},
								{0,70,71,46,47,34,35,36,37,38,39,
										40,41,42,43,44,45,60,65,-1},
								{0,1,2,3,4,5,6,7,8,9,10,11,12,13,
										14,15,16,17,18,19,20,21,22,23,
	                  24,25,26,27,28,29,30,31,32,33,99,-1} };
	bool FlagYes=false; 
	//Исходный файл с наблюдениями по ночам:
	char FileInput[]="D:\\mecalculateprogramm\\MyTe\\DATA_TOTAL_170710.txt";//DATA_TOTAL_140623.txt";//DATA_TOTAL_140513  DATA_050187_ALL.txt";//DATA_050187_ALL.txt";//DATA_050187.txt";////DATA_TOTAL_140224_LSQ.txt";//DATA_TOTAL_140402_.txt";//DATA_TOTAL_140224.txt";//DATA_TOTAL_Fcorr.txt";//
	//Файл типов звезд:
	char FileTypes[]="D:\\mecalculateprogramm\\MyTe\\CATALOG_SIRIUS_170710.txt";//CATALO_2000_2404.txt";//_S_Q
	//файл величин из старого каталога:
	char FileWBVR[]="D:\\mecalculateprogramm\\MyTe\\CAT_WBVR.txt";
	//Файл спектров из Свидерскене
	char FileSpect[]="D:\\mecalculateprogramm\\MyTe\\SP_SV_A.txt";
	// Список первоочередных звезд
	char FileListDP[]="D:\\mecalculateprogramm\\MyTe\\LIST_CAT_LAST_.txt";//LIST_6485_hd_LSQ.txt";//LIST_6485_hd.txt";////LIST_WBVR8766_hd.txt";
	//вспомогательные файлы для промежуточного контроля
	char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	char OutFile[]="D:\\mecalculateprogramm\\MyTe\\RES.txt";
	char OutNormFile[]="D:\\mecalculateprogramm\\MyTe\\RES_220817.txt";//050187_LSQ.txt";//140513 RES_200889.txt";//RES_N_6000.txt";//RES_Q_N_6000.txt";
	//файл конечной обработки всего каталога:
	char OutNormEndFile[]="D:\\mecalculateprogramm\\MyTe\\RES_TOTAL_170710.txt";//RES_050187_LSQ.txt";//RES_200889_DIFF.txt";//RES_Q_DIFF_6000.txt";//RES_Q_DIFF_6000.txt";//RES6000_N_End.txt";
    CMyTeBand Extint;// пользоваться как обычным бандом
    // задаём Имя файла средней экстинкции
    Extint.FName=_T("D:\\mecalculateprogramm\\MyTe\\EXTINCT_AA.txt");
    if(Extint.FName!="") {
        // загружаем файл
        Extint.LoadFromFile(Extint.FName);	
    }   
//БЛОК №1 ВВОД ДАННЫХ
//_____________________
//создаем массивы входных данных 
//Структура строки наблюдений
		typedef struct Date0 {
			bool FlagYes;
			char Name[14];
			char Spect[4];
			char DDMOYY[7];
			char HHMMSS[7];
			char KLAD[2];
			char STAND[2];
			int  N_P;
			double TC;
			double* SpED;
			double UT;
			double JD;
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double Bt;
			double Vt;
			double Ri[MaxBand];
			double RO[MaxBand];
			double RM[MaxBand];
			double RD[MaxBand];
			double RC[MaxBand];
			double* eR;
			double TimeAngle;
			double Mz;
			double Z;
			double PA;
			double SidTime;
		} Date0;
		typedef struct WBVR_O{
			char Name[12];
			double RO[MaxBand];
		}WBVR_O;
//Структура типов звезд
		typedef struct TYPES{
			char Name[12];
			char TyName[16];
			char TSp[4];
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double eRA;
			double eDE;
			double Bt;
			double Vt;
			double eBt;
			double eVt;
		}TYPES;
//***********************************
// $ 1.1 ВВод данных по всем ночам 
//***********************************
//открываем входной файл 
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//загружаем ВСЕ строковые данные (включая управляющие символы типа {,},...)
	//в массив Date0, кроме помеченных '*' в начале строки 
// Подсчёт кол-ва строк в файле прототипов
	UINT Count_str=CountDataFile(inpf);
	 char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	 fclose(inpf);
// $ 1.2 ВВод данных типов звезд
//***********************************
	//открыли файл типов с координатами
	 if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
		AfxMessageBox(_T("TYPES NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
			//AfxMessageBox(_T("Error Opening Coord File"));
		  return -1.0;
	 }
	// Подсчёт кол-ва строк в файле координат
	 //UINT  Count_coord=0;  
	UINT Count_coord=CountDataFile(inpf);
	 char** Data1 = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
	InpData(Data1,inpf, Count_coord, NULL); 
	 fclose(inpf);
// $ 1.3 ВВод данных спектров 
//*******************====================
	 if(fopen_s(&inpf, FileSpect, "r") !=0 ){ 
		AfxMessageBox(_T("SPECTRA NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		//AfxMessageBox(_T("Error Opening Spectra File"));
		//goto error_return;
	 }
	 fgets(InLin,len,inpf); //Name Spectra
	UINT NCS = 0;
	char *ptrS[len];
	 NCS=StrPtr(&InLin[8],' ',ptrS);
	//NCS; // Кол-во колонок в SpData без волн = число типов спектра
	char **NameSp=new char* [NCS];
		for(UINT i = 0; i < NCS; i++){
			NameSp[i]=new char [4];
			memset(NameSp[i],0,4);
		}
	//char buff[10];
	for(UINT i=0;i<NCS;i++){
		memcpy((void*)NameSp[i],(void*)ptrS[i],3);
		NameSp[i][3]=0;
	}
	UINT DLRSP=0;
	while(1){
		if(fgets(InLin,len,inpf)==0) break;
	  ++DLRSP;
	}
	double *SpLambd = new double[DLRSP];
	double **SpSV = new double*[NCS];
	for(UINT i=0;i<NCS;i++){
		SpSV[i] = new double[DLRSP];  // STEP 2: SET UP THE COLUMNS
	}
	fseek(inpf, 0L, SEEK_SET);
	fgets(InLin,len,inpf);//съели заголовок
	for(UINT j=0;j<DLRSP;j++){
		fgets(InLin,len,inpf);
		sscanf_s(InLin,"%lf",&SpLambd[j]);
	  StrPtr(&InLin[8],' ',ptrS);
	  for(UINT i=0;i<NCS;i++){
  		if(sscanf_s(ptrS[i],"%lf",&SpSV[i][j])==NULL) {
    		SpSV[i][j]=99999.9;
		}// break;}
		//fprintf(errf,"%10.1lf ",SpSV[i][j]);
	  }
	  //fputs("\n",errf);
	}	
	if (fopen_s(&errf, ErrFile, "a") !=0 ){
			AfxMessageBox(_T("ERR NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		//AfxMessageBox(_T("Error Opening Err File"));
	}
//char SpBUFF[]="A05 "; 
//SpBUFF[3]=0;

// $ 1.4 ВВод старого каталога WBVR
 //***********************************
	 if (fopen_s(&inpf, FileWBVR, "r") !=0 ){
		AfxMessageBox(_T("WBVR NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		 //AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR
	 //UINT  Count_WBVR=0;  
	UINT Count_WBVR=CountDataFile(inpf);
	 char** Data2 = new char*[Count_WBVR];        // STEP 1: SET UP THE ROWS.
	InpData(Data2,inpf, Count_WBVR, NULL); 
	 fclose(inpf);
// $ 1.5 ВВод списка первоочередных звезд
 //***********************************
	 if (fopen_s(&inpf, FileListDP, "r") !=0 ){
		AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		 //AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR8766
	 //UINT  Count_WBVR=0;  
	UINT Count_DPCat=CountDataFile(inpf);
	 char** Data3 = new char*[Count_DPCat];        // STEP 1: SET UP THE ROWS.
	InpData(Data3,inpf, Count_DPCat, NULL); 
	 fclose(inpf);
//конец БЛОК №1
//***********************************

//БЛОК № 2 РАССЫЛКА ВСПОМОГАТЕЛЬНЫХ ДАННЫХ 
//***********************************
// $ 2.1 Рассылка типов
//***********************************
	char** ptr=new char* [200];
	TYPES* TYP = new TYPES[Count_coord];

	for(UINT i=0;i<Count_coord;i++){
		if(StrPtr(Data1[i],'|',ptr)!=0)
		{
			sscanf_s(ptr[3],"%lf",&TYP[i].RA);
			sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
			sscanf_s(ptr[4],"%lf",&TYP[i].DE);
			sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
			if(sscanf_s(ptr[8],"%lf",&TYP[i].Bt)==0) {
				TYP[i].Bt=0;//???? сигнал, что не нашлось ничего
			}
			if(sscanf_s(ptr[9],"%lf",&TYP[i].eBt)==0) TYP[i].eBt=0;
			if(sscanf_s(ptr[10],"%lf",&TYP[i].Vt)==0) TYP[i].Vt=0;
			if(sscanf_s(ptr[11],"%lf",&TYP[i].eVt)==0) TYP[i].eVt=0;
			//sscanf_s(ptr[8],"%lf",&TYP[i].eBt);
			//sscanf_s(ptr[9],"%lf",&TYP[i].Vt);
			//sscanf_s(ptr[10],"%lf",&TYP[i].eVt);
			memcpy((void*)TYP[i].Name,(const void*)ptr[0],9);
			TYP[i].Name[9]=0;
			memcpy((void*)TYP[i].TyName,(const void*)ptr[14],16);
			TYP[i].TyName[16]=0;
			memcpy((void*)TYP[i].TSp,(void*)ptr[2],3);
			TYP[i].TSp[3]=0;
		}
	}
//#ifdef ETBOE
//#ifdef ETBOE
//#ifdef DONETKNOW
//#endif
// $ 2.2 Рассылка WBVR
//***********************************
	//создаем массив структур=строк WBVR
	WBVR_O* WbvrO = new WBVR_O[Count_WBVR];
//#endif
// Упорядочивание кривых реакции WBVR
	int WBVR[MaxBand];
	for(UINT i=0;i<MaxBand;i++){
		wchar_t NC = Band[i].Name;
		switch(NC){
			case _T('W'):
			case _T('w'):
				WBVR[0] = i;				
				break;
			case _T('B'):
			case _T('b'):
                WBVR[1] = i;
				break;
			case _T('V'):
			case _T('v'):
                WBVR[2] = i;
				break;
			case _T('R'):
			case _T('r'):
                WBVR[3] = i;
				break;
			default:break;
		}
		Search_Band(&Band[i],&Band[i].LBeg,&Band[i].LEnd);
	}
//вводим старые величины WBVR
double R[MaxBand];
for(UINT i=0;i<Count_WBVR;i++){
	bool FlagYes=false;
	if(StrPtr(Data2[i],'|',ptr)!=0){
		memcpy((void*)WbvrO[i].Name,(void*)ptr[0],11);
		WbvrO[i].Name[11] = '\0';
		for(UINT k=0;k<MaxBand;k++){
			sscanf_s(ptr[k+5],"%lf",&R[k]);
		}
	}//W=W_B+B_V+V
	WbvrO[i].RO[0]=R[1]+R[2]+R[0];
	WbvrO[i].RO[1]=R[2]+R[0];
	WbvrO[i].RO[2]=R[0];
	WbvrO[i].RO[3]=R[0]-R[3];
}
// $ 2.2 Рассылка номеров дополнительного каталога
//***********************************
char **NameDPCat=new char* [Count_DPCat];
for (UINT i=0;i<Count_DPCat;i++){
	NameDPCat[i]=new char [11];
}
for(UINT i=0;i<Count_DPCat;i++){
	if(StrPtr(Data3[i],'|',ptr)!=0){
		memcpy((void*)NameDPCat[i],(void*)ptr[0],9);
		NameDPCat[i][9]=0;
		fprintf(errf,"%s\n",NameDPCat[i]);
	}
}		//sscanf_s(Data3[k+5],"%lf",&R[k]);

// БЛОК № 3 ОБРАБОТКА ДАННЫХ
//***********************************
//наполнение DS
#define	MaxTypSt 200 // максимальное количество типов стандартов
#define	MaxSt 300	// максимальное количество наблюдений стандартов в куске
int CountSt=0;//число стандартов в куске
int CountTypSt=0;//число типов стандартов куске

UINT IOUT=0; // счетчик выходного массива обработанных строк 
Date0* DS1=0;//new Date0[Count_str]
Date0* DSI = new Date0[Count_str];// вспомогательный массив для обработки
Date0** DDST = new Date0*[MaxTypSt];// вспомогательный массив для стандартов в куске
for (UINT i=0;i<MaxTypSt;i++){
	DDST[i]=new Date0 [MaxSt];
}
Date0* DDS = new Date0[MaxSt];// вспомогательный массив для обработки
double DDD[MaxTypSt][MaxBand];// вспомогательный массив для обработки
UINT NS[MaxTypSt];
double N;
UINT N_Part=0;//количество строк в куске
UINT COUNT_PART=0;//номер куска
UINT IDPC=0; //  количество наблюдений звезд в выборке по ДоП. каталогу

UINT Beg_Part=0;// адрес начала куска
UINT ENd_Part=0;// адрес конца куска
int DD,MO,YY;
int HH,MM,SS;
int Year;
char SpBUFF[]="A05 ";
// $ 3.1 Обработка по кускам
//***********************************

for(UINT I0=0;I0<Count_str;){//НАЧАЛО ГЛОБАЛЬНОГО ЦИКЛА ПО КУСКАМ
	N_Part=0;
	if (Data0[I0][0]=='{') //НАЧАЛО СЧЕТА КУСКА
	{
		Beg_Part=I0+1;
		for (UINT j=0;;j++)
		{		
			if (Data0[j+Beg_Part][0]=='}'){
				ENd_Part=j+Beg_Part;
				break;
			}else {
				N_Part++;	//считаем число строк без управляющих символов Count_str
				continue;
			}
		}
		I0=Beg_Part;
		DS1=new Date0[N_Part];//Date0* 
		
	for(UINT i=0;i<N_Part;i++){// ЦИКЛ рассылки данных по куску
		bool FlagYes=false;
		if(StrPtr(Data0[i+I0],'|',ptr)!=0){
			memcpy((void*)&DS1[i].Name,(void*)ptr[0],12);
			DS1[i].Name[12] = '\0';
			DS1[i].SpED=new double [DLRSP];
//#ifdef MOXU
			DS1[i].FlagYes=false;
			sscanf_s(ptr[1],"%lf",&DS1[i].TC);
			memcpy((void*)&DS1[i].KLAD,(void*)ptr[2],1);
			DS1[i].KLAD[1]=0;
			memcpy((void*)&DS1[i].STAND,(void*)ptr[3],1);
			DS1[i].STAND[1]=0;
			for(UINT k=0;k<MaxBand;k++){
				sscanf_s(ptr[k+4],"%lf",&DS1[i].Ri[k]);
				N=DS1[i].Ri[k];
				if (N<=0.0){
					DS1[i].Ri[k]=0.0;
				} else{
					DS1[i].Ri[k]=-2.5*log10(N/(1-28e-9*N));
				}
			}
			memcpy((void*)&DS1[i].DDMOYY,(void*)ptr[8],6);
			memcpy((void*)&DS1[i].HHMMSS,(void*)ptr[9],6);
			DS1[i].DDMOYY[6] = '\0';
			DS1[i].HHMMSS[6] = '\0';

			sscanf_s(ptr[8],"%2d%2d%2d",&DD,&MO,&YY);
			sscanf_s(ptr[9],"%2d%2d%2d",&HH,&MM,&SS);
			DS1[i].N_P=COUNT_PART; //  для каждой строки запоминается номер куска наблюдения
		}
		int Dt=MO*100+DD;
		if(YY<40) Year=2000+YY;
		else Year=1900+YY;
		DS1[i].UT=(double)HH+(double)MM/60.0+(double)SS/3600.0;
	//вычисляем порядковый номер дня от 0 января 1950 года
		int JD50 = JDm1950(Year, Dt);
	//Вычисляем среднее звездное гринвичское время в радианах на 0 часов UT на данное число
	//JD50 - число суток от 0 января 1950 г.
		double S0=Stims(JD50); //S0 в радианах
		double JD= (double)JD50+DS1[i].UT/24;
		DS1[i].JD=JD;
		DS1[i].SidTime=fmod( (double)(S0 + (double)(1.0027379*DS1[i].UT*
						0.2617993877983 + 1.34334356423)),(double)Pi2 );
//получение и рассылка координат звезд
		FlagYes=false;
		for(UINT j=0;j<Count_coord;j++){
			int a=strncmp(TYP[j].Name,DS1[i].Name,9);
			if(a==0){
				//DS[i].Spect = Types_Sp[j];
				memcpy(DS1[i].Spect,TYP[j].TSp,4);
				DS1[i].Spect[3]=0;
				DS1[i].RA=TYP[j].RA;
				DS1[i].DE=TYP[j].DE;
				DS1[i].pmRA=TYP[j].pmRA;
				DS1[i].pmDE=TYP[j].pmDE;
				if((DS1[i].TimeAngle = DS1[i].SidTime - TYP[j].RA * Pi/180)<0)
					DS1[i].TimeAngle += Pi2;//t=s-RA в радианах		
				DS1[i].Z =Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,0 );
				DS1[i].Mz=Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,1);
				DS1[i].PA=Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,2);
				DS1[i].Bt=TYP[j].Bt;
				DS1[i].Vt=TYP[j].Vt;
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			fprintf(errf,"%s\n",DS1[i].Name);
			DS1[i].TimeAngle=0;
			DS1[i].DE=0;
			DS1[i].Mz=1.0;
			memcpy(DS1[i].Spect,SpBUFF,4);
			//DS[i].Spect=SpBUFF;
		}
		FlagYes=false;
		for(UINT j=0;j<Count_WBVR;j++){
			if(strncmp((const char*)DS1[i].Name,(const char*)WbvrO[j].Name,9)==0){
				for(UINT k=0;k<MaxBand;k++){
					DS1[i].RO[k]=WbvrO[j].RO[k];
				}
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			//fprintf(errf,"%s\n",DS[i].Name);
			for(UINT k=0;k<MaxBand;k++){
				DS1[i].RO[k]=0.0;
			}
		}
//#ifdef EMOE
	//************ Выбор Типов Спектров ********************//
	//======================================================//
int ic=0;
UINT p1,p2,cc;
UINT m=0;
		cc=SpSearch(DS1[i].Spect[0],DS1[i].Spect[1]);
		if(DS1[i].Spect[2]==' '){
			if (cc>58){ ic=2;
			} else	ic=4;
		}else {
			sscanf_s(&DS1[i].Spect[2],"%1d", &ic);
if(ic<=0) 
	ic=ic;
			ic-=1; // Класс светимости
		}
		while(1){
  			if(SP_Cod[ic][m+1]<0){
    			for(UINT j=0;j<DLRSP;j++){
	    			DS1[i].SpED[j]=SpSV[SP_Cod[ic][m]][j];
					//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",DS[i].Spect,SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
				}
				break;//return;
  			}else{
				p1=SpSearch(NameSp[SP_Cod[ic][m]][0],NameSp[SP_Cod[ic][m]][1]);
				p2=SpSearch(NameSp[SP_Cod[ic][m+1]][0],NameSp[SP_Cod[ic][m+1]][1]);
  				if( (cc>=p1)&&(cc<p2) ){
					for(UINT j=0;j<DLRSP;j++){
						DS1[i].SpED[j]= LinInt(SpSV[SP_Cod[ic][m]][j],SpSV[SP_Cod[ic][m+1]][j],
							(double)p1,(double)p2,(double)cc);
					}
					break;//	      return;
				}else{ m++;}
			}
		}
//#endif
	}//КОНЕЦ ЦИКЛА рассылки данных по куску
		//fclose(errf);
//Вычисление Ai по куску
//Подготовка подинтегральных функций для вычисления Ai
double **SubintP = new double* [MaxBand];
double **Subint = new double* [MaxBand];
	for(UINT k=0;k<MaxBand;k++){
		SubintP[k] = new double [DLRSP];
		Subint[k] = new double [DLRSP];
	}
	for(UINT i=0;i<N_Part;i++){		
		for(UINT k=0;k<MaxBand;k++){
			for(UINT j=0;j<DLRSP;j++){
				double a=DS1[i].SpED[j];
				double b=Band[WBVR[k]].GetFlux(SpLambd[j]);
				double c=Extint.GetFlux(SpLambd[j]);
				double cc=pow(c,DS1[i].Mz);//Extint.GetFlux(SpLambd[j])
				SubintP[k][j] = a*b*cc;
				Subint[k][j] = a*b;
					//DS[i].SpED[j]*Band[WBVR[k]].GetFlux(SpLambd[j])*pow(Extint.GetFlux(SpLambd[j]),DS[i].Mz);// Sp*Band*p^Mz 		sscanf(InLin,"%lf%lf",&EXT_Lambd[i],&EXT_Flux[i]);
			}
			double d= Integral_M(SubintP[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
			double dd=Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
			double ddd=	-2.5*log10(d/dd);		//double dd=

			DS1[i].Ri[k]-=ddd; //-2.5*log10(Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd));
		}
//		i=i;
	}	//Конец вычислений Ai по куску
		de_allocate(SubintP,MaxBand);
		de_allocate(Subint,MaxBand);

//#ifdef EMOE
// контрольная печать в файл ПРОБКИ
	//открыли выходной файл печати ПРОБКИ
#ifdef EMOE
		if (fopen_s(&outf, OutFile, "a") !=0 ){
			AfxMessageBox(_T("OUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
			//AfxMessageBox(_T("Error Opening result File"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");

		for(UINT i=0;i<N_Part;i++){
			//if(strncmp((const char*)DS1[i].DDMOYY,"050187",6)==0){
			fprintf(outf,"%s|  %s| %s|",DS1[i].Name,DS1[i].KLAD,DS1[i].STAND);
			fprintf(outf,"  %7.3lf|  %12.8lf|  %12.8lf|  %8.4lf|",DS1[i].TimeAngle/Pi*12,DS1[i].RA,DS1[i].DE,DS1[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %7.3lf|",DS1[i].Ri[k]);
			}
			fprintf(outf,"%11.5lf|",DS1[i].JD);
			fprintf(outf,"%s|%s|%s",DS1[i].Spect,DS1[i].DDMOYY,DS1[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DS1[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DS1[i].Bt);
			fprintf(outf,"  %7.3lf|",DS1[i].Vt);
			fputs("\n",outf);
			//}
		}
		fclose(outf);
#endif
// нормализация
//		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
//			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
////AfxMessageBox(_T("Error Opening result NormFile"));
//		}
//		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
CountSt=0;//число стандартов в куске
CountTypSt=0;//число типов стандартов куске

//#ifdef EMOE вычисление нормализованных величин для стандартов и .RM для других  
	for(UINT i=0;i<N_Part;i++){
		if(DS1[i].STAND[0]=='S'){
			for(UINT k=0;k<MaxBand;k++)	
				DS1[i].RM[k]=DS1[i].RO[k]-DS1[i].Ri[k];
			DDS[CountSt]=DS1[i];//[si]
			CountSt++;
		}else 
			for(UINT k=0;k<MaxBand;k++)	
				DS1[i].RM[k]=DS1[i].Ri[k];
	}
	double RA=0;
	double DE=0;
	for(int jk=0;jk<CountSt;jk++){ //перетряс стандартов куска
		if(!DDS[jk].FlagYes){
			RA=DDS[jk].RA;
			DE=DDS[jk].DE;
		}else continue;
		int ij=0;
		for (int k = 0; k<MaxBand; k++){
			DDD[CountTypSt][k]=0;
		}
		for (int ik = 0; ik < CountSt; ik++){
			if(DDS[ik].RA==RA && DDS[ik].DE==DE){
				DDST[CountTypSt][ij]=DDS[ik];
				for(UINT k=0;k<MaxBand;k++){
					DDD[CountTypSt][k]+=DDST[CountTypSt][ij].RM[k];
				}
				DDS[ik].FlagYes=true;
				ij++;
				NS[CountTypSt]=ij;
			}else continue;
		}
		for(int k=0; k<MaxBand;k++){
			DDD[CountTypSt][k]/=ij;
		}
		CountTypSt++;
		//if(ij<kk)
	}//конец перетряски стандартов
//#endif
	//}else 	{exit(1);}//конец куска
	//	AfxMessageBox(_T("Begin of part NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);

//}
//#ifdef EMOE
//#ifdef ETBOE
	for(UINT i=0;i<N_Part;i++){//нормализация куска
		//if(strncmp((const char*)DSI[i].DDMOYY,"050187",6)==0 &&
		//	strncmp((const char*)DSI[i].HHMMSS,"133154",6)==0){
	  //if(DS1[i].DDMOYY=="050187" && DS1[i].HHMMSS=="133154"){	
		if (DS1[i].STAND[0]=='S'){		//
			for(UINT ii=0;ii<CountTypSt;ii++){	//MaxTypSt
				if (DS1[i].RA == DDST[ii][0].RA && DS1[i].DE == DDST[ii][0].DE){
					for(UINT k=0;k<MaxBand;k++) {
						DS1[i].RM[k]=DDD[ii][k]+DS1[i].Ri[k];
					}
					break;
				}else{ continue;}
			}
		} else{	// не стандарты
			bool FlagYes=false;
			double Ro=0;
			for(UINT ii=0;ii<CountTypSt;ii++){
				double dX=cos(DS1[i].DE)*cos(DS1[i].RA)-cos(DDST[ii][0].DE)*cos(DDST[ii][0].RA);
				double dY=cos(DS1[i].DE)*sin(DS1[i].RA)-cos(DDST[ii][0].DE)*sin(DDST[ii][0].RA);
				double dZ=sin(DS1[i].DE)-sin(DDST[ii][0].DE);//RA???!!!
				double dMz=abs(DS1[i].Mz - DDST[ii][0].Mz);
				//if (/*(abs(DS[i].Mz - DDST[ii][0].Mz)<0.15)&& (abs(DS[i].RA - DDST[ii][0].RA)<20) && (abs(DS[i].DE - DDST[ii][0].DE)<15 ))*/
				Ro=2*asin(0.5*sqrt(dX*dX+dY*dY+dZ*dZ));
				if ( Ro<0.4 && dMz<0.15){//>0.5???
					if( DS1[i].JD<DDST[ii][0].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDST[ii][0].RM[k];
						}
						FlagYes=true;
						break;
					}
					if( DS1[i].JD>DDST[ii][NS[ii]-1].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDST[ii][NS[ii]-1].RM[k];
						}
						FlagYes=true;
						break;
					}
					for(int ij=0;ij<CountSt;ij++){
						if( (DS1[i].JD>DDST[ii][ij].JD) && (DS1[i].JD<DDST[ii][ij+1].JD) )/* &&*/ {
							for(int k=0;k<MaxBand;k++){
								double b=LinInt(DDST[ii][ij].RM[k],DDST[ii][ij+1].RM[k],DDST[ii][ij].JD,DDST[ii][ij+1].JD,DS1[i].JD);
								DS1[i].RM[k]+=b;
							}
							FlagYes=true;
							break;
						}else continue;//ij++;
					}
				}else continue;
				break;
			}
			if (!FlagYes){	//если не найдены подходящие стандарты - любые соседние
				for(int ij=0;ij<CountSt;ij++){
					if( (DS1[i].JD>DDS[ij].JD) && (DS1[i].JD<DDS[ij+1].JD) ){
						for(int k=0;k<MaxBand;k++){
							double b=LinInt(DDS[ij].RM[k],DDS[ij+1].RM[k],DDS[ij].JD,DDS[ij+1].JD,DS1[i].JD);
							DS1[i].RM[k]+=b;
						}
						break;
					}
					if( DS1[i].JD<DDS[0].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDS[0].RM[k];
						}
						break;
					}
					if( DS1[i].JD>DDS[CountSt-1].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDS[CountSt-1].RM[k];
						}
						break;
					}
				}
			}
		}
	  //}
//i=I+KK;
	}//конец нормализации куска 
//формирование выходного массива
		//for(UINT i=0;i<N_Part;i++,IOUT++){
		//	DSI[IOUT]=DS[i];
		//	//delete DS;
		//}
		//delete DS;
	//for(UINT j=0/*,ij=0*/;j<Count_DPCat;j++){ // j-по звездам ДПК ij- по количеству звезд в наблюдениях из ДПК
	//	for(UINT i=0;i<N_Part;i++){							//  ii- по числу mi кроме нормстар hd 5015
	//		if(strncmp((const char*)DS1[i].Name,(const char*)NameDPCat[j],9)==0){
	//			DSI[IDPC]=DS1[i];
	//			//ij++;
	//			IDPC++;
	//		}else continue;
	//	}
	//}
//	контрольная выдача величин
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<N_Part;i++){
			//if(strncmp((const char*)DS1[i].DDMOYY,"050187",6)==0){

			fprintf(outf,"%s|%s|%s|",DS1[i].Name,DS1[i].KLAD,DS1[i].STAND);
			fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DS1[i].TimeAngle/Pi*12,DS1[i].RA,DS1[i].DE,DS1[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DS1[i].RM[k]);
			}
			fprintf(outf,"%11.5lf|",DS1[i].JD);
			fprintf(outf,"%s|%s|%s",DS1[i].Spect,DS1[i].DDMOYY,DS1[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DS1[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DS1[i].Bt);
			fprintf(outf," %7.3lf|",DS1[i].Vt);
			fputs("\n",outf);
		  }
		//}
		delete DS1;

	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
	}
	COUNT_PART++;
	I0=ENd_Part+1;
}//КОНЕЦ ГЛОБАЛЬНОГО ЦИКЛА ПО КУСКАМ
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|Rw|Rb|Rv|Rr|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
			fprintf(outf,"%s|%s|%s|",DSI[i].Name,DSI[i].KLAD,DSI[i].STAND);
			fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSI[i].TimeAngle/Pi*12,DSI[i].RA,DSI[i].DE,DSI[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].Ri[k]);
			}
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].RM[k]);
			}
			fprintf(outf,"%11.5lf|",DSI[i].JD);
			fprintf(outf,"%s|%s|%s|",DSI[i].Spect,DSI[i].DDMOYY,DSI[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"%7.3lf|",DSI[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSI[i].Bt);
			fprintf(outf," %7.3lf|",DSI[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
//fcloseall();
//	return 0.0;
//}
//#ifdef ETBOE
//#endif
//БЛОК  ВЫЧИСЛЕНИЕ уклонений от средних значений
	if (fopen_s(&outf, OutNormEndFile, "w") !=0 ){
		//AfxMessageBox(_T("Error Opening result NormEndFile"));
	}
	fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|W|B|V|R|dW|dB|dV|dR|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
	for(UINT i=0;i<IDPC;i++){
		DSI[i].FlagYes=false;
	}
	char buff[12];
	Date0* DSJ = new Date0[1000];
	for(UINT i=0;i<IDPC;i++){
		//Date0* DSJ = new Date0[1000];
		//fprintf(outf,"%7d|",i);
		if(!DSI[i].FlagYes)	
			memcpy(buff,DSI[i].Name,12);
		else continue;
		int kk=0;
		double Ddd[MaxBand]={0,0,0,0};
		for(UINT j=0,ij=0;j<IDPC;j++){//IOUT
			int a=strncmp(buff,DSI[j].Name,11);
			if(a==0){
				DSJ[ij]=DSI[j];
				DSI[j].FlagYes=true;
				for(UINT k=0;k<MaxBand;k++){
					Ddd[k]+=DSI[j].RM[k];
				}
				kk++;
				ij++;
			}else{continue;}
		}
		if(kk>1){
			for(UINT k=0;k<MaxBand;k++){	
				Ddd[k]/=(double)kk;
				for(UINT ij=0;ij<kk;ij++)	{
					DSJ[ij].RD[k]=DSJ[ij].RM[k]-Ddd[k];
					ij=ij;
				}
			}
			for(UINT ij=0;ij<kk;ij++){
				fprintf(outf,"%s|%s|%s|%4d|",DSJ[ij].Name,DSJ[ij].KLAD,DSJ[ij].STAND,DSJ[ij].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSJ[ij].TimeAngle/Pi*12,DSJ[ij].Z/Pi*180,DSJ[ij].PA/Pi*180,
					DSJ[ij].RA,DSJ[ij].DE,DSJ[ij].Mz);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DSJ[ij].RM[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DSJ[ij].RD[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				fprintf(outf,"%11.5lf|",DSJ[ij].JD);
				fprintf(outf,"%s|%s|%s",DSJ[ij].Spect,DSJ[ij].DDMOYY,DSJ[ij].HHMMSS);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"|%7.3lf",DSJ[ij].RO[k]);
				}
				fprintf(outf,"|  %7.3lf|",DSJ[ij].Bt);
				fprintf(outf," %7.3lf|",DSJ[ij].Vt);
				fputs("\n",outf);
			}
		} //else DJ[i].FlagYes=true; end IF(kk>1)
	//}
//#endif//endif
	} //КОНЕЦ ВЫЧИСЛЕНИЯ уклонений от средних значений 
	fclose(outf);
//delete DS;
//БЛОК ВЫБОРА ЗВЕЗД 
	//Date0* DSC = new Date0[IDPC];//IOUT, Count_DPCat //выборка звезд доп.каталога в порядке номеров
//UINT IDPC=0; //  количество наблюдений звезд в выборке по ДоП. каталогу
//	for(UINT i=0;i<IDPC;i++){							//  ii- по числу mi кроме нормстар hd 5015
//		DSC[i]=DSI[i];
//	}
//delete DSI;
//#ifdef GYGYGY
UINT IMi=0;	//  количество столбцов величин, кроме hd 5015
short** IName_DC = new short*[IDPC];// вспомогательный массив для линейных уравнений по количеству звезд
for (UINT i=0;i<IDPC;i++){
	IName_DC[i]=new short [Count_DPCat];// длиной в число звезд в DP каталоге
	for(UINT j=0;j<Count_DPCat;j++){
		IName_DC[i][j]=0;
		//IMi=j;
	}
	IMi=i;
}
	IMi=IMi;
////double** IDate_DC = new double*[IOUT];// вспомогательный массив для линейных уравнений по кускам
////for (UINT i=0;i<IOUT;i++){
////	IDate_DC[i]=new double [COUNT_PART];//длиной в количество кусков
////	for(UINT j=0;j<COUNT_PART;j++)
////		IDate_DC[i][j]=0;
//}
//for(UINT j=0,ij=0;j<Count_DPCat;j++){ // j-по звездам ДПК ij- по количеству звезд в наблюдениях из ДПК
//}

		//for(UINT j=0;j<NN;j++){		//Count_str
		//	if (DS[j].FlagYes) continue;
		//	a=MIN(a,DS[j].JD);
		//}
//ПЕРЕБОР И УПОРЯДОЧИВАНИЕ КУСКОВ ПО ПОРЯДКУ
//#ifdef GYGYGY
char Buff[10];
UINT **NLIST=new UINT* [IDPC];
for (UINT i=0;i<IDPC;i++){
	NLIST[i]=new UINT [COUNT_PART];
	for(UINT j=0;j<COUNT_PART;j++)
		NLIST[i][j]=0;
}

for(UINT i=0;i<IDPC;i++){
	DSI[i].FlagYes=false;
}
UINT IP=0;
	for(int ii=IDPC;ii>0;IP++){
	double a=100000;
		for(UINT i=0;i<IDPC;i++){
			if (DSI[i].FlagYes) continue;
			a=MIN(a,DSI[i].N_P);
		}
		for(UINT i=0;i<IDPC;i++){
			if(DSI[i].N_P==a){
				DSI[i].FlagYes=true;
				NLIST[i][IP]=1;
				ii--;
			}
		}
	}
//
	for(UINT i=0,ii=0;i<IDPC;/*i++,ii++*/){
	//for(UINT ii=0;ii<Count_DPCat-1;/*ii++*/){
		//UINT i=0;
		memcpy(Buff,DSI[i].Name,9);
		Buff[9]=0;
	//if(strncmp((const char*)DSC[i].Name,Buff,9)==0){
//	//	IDate_DC[i][DSC[i].N_P]=1;
		if(strncmp((const char*)DSI[i].Name,"hd   5015",9)!=0){
			while(strncmp((const char*)DSI[i].Name,Buff,9)==0){
				IName_DC[i][ii]=1;
				//fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);
				i++;
			}
			ii++;
		}else{
			while(strncmp((const char*)DSI[i].Name,"hd   5015",9)==0){
			//continue;
				//fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);//IName_DC[i][ii]=0;
				i++;
			}
		}
		//ii++;
//	//		IDate_DC[i][DSC[i].N_P]=1;
	}
	//for(UINT i=0,ii=0;i<IDPC;i++/*,ii++*/){
	//	fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);
	//}
	//fclose(errf);

//	//	IMi++;// считаем столбцы по числу mi кроме нормстар hd 5015
//	//}else {
//		while(strncmp((const char*)DSC[i].Name,Buff,9)==0){
//			IDate_DC[i][DSC[i].N_P]=1;
//			i++;
//		}
//	//}
//	//ij++;
//}
//}
			//if(strncmp((const char*)DSC[ij].Name,"hd   5015",9)!=0){
			//	IName_DC[ij][ii]=1;
			//	IDate_DC[ij][DSC[ij].N_P]=1;
		//}


		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
				fprintf(outf,"%s|%s|%s|%4d|",DSI[i].Name,DSI[i].KLAD,DSI[i].STAND,DSI[i].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSI[i].TimeAngle/Pi*12,DSI[i].Z/Pi*180,DSI[i].PA/Pi*180,
					DSI[i].RA,DSI[i].DE,DSI[i].Mz);
			//fprintf(outf,"%s|%s|%s|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND);
			//fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].Ri[k]);
			}
			for(UINT kj=0;kj<IMi;kj++){
				fprintf(outf,"  %7.3lf|",IName_DC[i][kj]);
			}
			for(UINT kj=0;kj<IP;kj++){
				fprintf(outf,"  %3d|",NLIST[i][kj]);
			}

			fprintf(outf,"%11.5lf|",DSI[i].JD);
			fprintf(outf,"%s|%s|%s",DSI[i].Spect,DSI[i].DDMOYY,DSI[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DSI[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSI[i].Bt);
			fprintf(outf," %7.3lf|",DSI[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);

//БЛОК СОСТАВЛЕНИЯ МАТРИЦ И РЕШЕНИЯ УРАВНЕНИЙ МНК
//#ifdef GYGYGY
int PLSQ = IP+Count_DPCat-1;//COUNT_PART+Count_DPCat;// Count_DPCat-1  количество оцениваемых параметров без звезды hd 5015 (нуль-пункт)
SolverLSQ MNK(PLSQ);
double **B = new double* [IDPC]; // правая часть уравнений МНК
for (UINT i=0;i<IDPC;i++)
	B[i]=new double [MaxBand];
double **A = new double* [IDPC]; // левая часть уравнений МНК
for (UINT i=0;i<IDPC;i++)
	A[i]=new double [PLSQ];

double **Res = new double* [MaxBand]; // правая часть уравнений МНК
for (UINT k=0;k<MaxBand;k++)
	Res[k]=new double [PLSQ];

// Cоставление и решение уравнений для WBVR
for(UINT k=0;k<MaxBand;k++){//
	for(UINT i=0;i<IDPC;i++){
		//for(UINT k=0;k<MaxBand;k++)
		if(strncmp((const char*)DSI[i].Name,"hd   5015",9)==0/* && IName_DC[i][j]!=0*/)
			B[i][k]=DSI[i].Ri[k]-DSI[i].RO[k];
		else B[i][k]=DSI[i].Ri[k];
	}
}
for(UINT i=0;i<IDPC;i++){
	UINT J=0;
	for(UINT j=0;j<Count_DPCat-1;j++){
		//if(strncmp((const char*)DSC[i].Name,"hd   5015",9)==0 /*&& IName_DC[i][j]!=0*/){
			//A[i][j]=0;//j--;
			//j++;
			//break;//continue;//J--;//
		//}else {
			A[i][j]=IName_DC[i][j];
			J++;
		//}
	}
	for(UINT j=0;j<IP;j++)//COUNT_PART
		A[i][j+J]=NLIST[i][j];
}
//fclose(errf);
for(UINT i=0;i<IDPC;i++){
	for(UINT k=0;k<MaxBand;k++)//
		fprintf(errf,"%7.3lf|",B[i][k]);
	for(UINT ij=0;ij<PLSQ;ij++){
		fprintf(errf,"%7.3lf|",A[i][ij]);
	}
	fputs("\n",errf);
}
fclose(errf);
//int PLSQ=2;//2;
//#ifdef GYGYGY
//#ifdef ETBOE
for(UINT k=0;k<MaxBand;k++){//
	MNK.Init();
	for(UINT i=0;i<IDPC;i++){
  		MNK.Accumulate(A[i],B[i][k]);//[k]
	}
	MNK.Solve(Res[k]);
}

	for(UINT i=0;i<IDPC;i++){
		for(UINT k=0;k<MaxBand;k++){
			for(UINT j=0;j<PLSQ;j++){
				DSC[i].RC[k]=DSC[i].Ri[k]-Res[k][j];
				fprintf(errf,"     %9.5lf| %9.5lf| %9.5lf|",A[i][j],Res[k][j]*A[i][j],DSC[i].RC[k]*A[i][j]);
			}
		}
		fprintf(errf,"\n");
	}
		//for(UINT i=0;i<IDPC;i++){
		//	for(UINT k=0;k<MaxBand;k++){
		//		fprintf(errf,"%7.3lf|",B[i][k]);
		//		for(UINT j=0;j<PLSQ;j++){
		//			DSC[i].RC[k]=B[i][k]-Res[k][j];
		//			fprintf(errf,"     %9.5lf| %9.5lf| %9.5lf|",A[i][j],Res[j]*A[i][j],RC[i]*A[i][j]);

		//			//RC[i][k]=B[i][k]+Res[k][j];
		//			//fprintf(outf,"%7.3lf| %7.3lf| %7.3lf| %7.3lf|",B[i][k],A[i][j],Res[k][j],RC[i][k]);
		//		}
		//		fprintf(errf,"\n");

		//	//}
		//}
	fclose(errf);
//#endif
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|Wi|Bi|Vi|Ri|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
				fprintf(outf,"%s|%s|%s|%4d|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND,DSC[i].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].Z/Pi*180,DSC[i].PA/Pi*180,
					DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			//fprintf(outf,"%s|%s|%s|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND);
			//fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSC[i].Ri[k]);
			}
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSC[i].RC[k]);
			}
			fprintf(outf,"%11.5lf|",DSC[i].JD);
			fprintf(outf,"%s|%s|%s",DSC[i].Spect,DSC[i].DDMOYY,DSC[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DSC[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSC[i].Bt);
			fprintf(outf," %7.3lf|",DSC[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
////#endif
//#ifdef EMOE
		//else
		//{
		//}
//	delete [] DS;
	//delete [] DJ;
	delete [] WbvrO;
	delete [] TYP;
	de_allocate(Data0,Count_str);
	de_allocate(Data1,Count_coord);
	de_allocate(Data2,Count_WBVR);
_fcloseall();
//#endif //endif
return 0.0;

}
//}
#endif

double CMyTeMath::Integral_M(double* Flux,double* Lambda,UINT Count,double lambdaBeg,double lambdaEnd)
{
	double Summa=0.0;
	double BandFlux0=0.0;
	double BandFlux1=0.0;
	double a=0.0;
	double b=0.0;
	double la=0.0;
	double lb=0.0;

	Summa=0.0;
	for(UINT i=0;i<Count-1;i++)
	{
		if((Lambda[i]<lambdaBeg) &&(Lambda[i+1]>lambdaEnd)){
			la=lambdaBeg;
			lb=Lambda[i+1];
			b=Flux[i+1];
			a=LinInt(Flux[i],b,Lambda[i],lb,la);
		}else {
			if(Lambda[i]>=lambdaBeg && Lambda[i+1]<lambdaEnd){
				la=Lambda[i];
				lb=Lambda[i+1];
				a=Flux[i];
				b=Flux[i+1];
			}else{
				if((Lambda[i]<lambdaEnd) && (Lambda[i+1]>=lambdaEnd)){
					lb=lambdaEnd;
					la=Lambda[i];
					a=Flux[i];
					b=LinInt(a,Flux[i+1],la,Lambda[i+1],lb);
				}else continue;
			}
		}
		Summa+=(a+b)*(lb-la)/2.0;
	}
	return (Summa);
}
//double CMyTeMath::Aid()
int CMyTeMath::StrPtr(char* Str, char c, char** pt) {
    //$$+
    //static N=0;
    if (Str == NULL) return -1;
    for (UINT j = 0, i = 0, N = 0;; j++, i++) {
	for (;; i++) {
	    if (Str[i] == '\0' || Str[i] == '\n')return N;
	    if (i != 0) {
		if ((Str[i] != c && Str[i] != '\t')) {
		    if ((Str[i - 1] == c || Str[i - 1] == '\t')) {
			pt[j] = &Str[i];
			N++;
			break;
		    }
		    else continue;
		}
		else continue;
	    }
	    else {
		if (Str[i] != c && Str[i] != '\t') {
		    pt[j] = &Str[i];
		    N++;
		    break;
		}
		else continue;
	    }
	}
    }
}
int CMyTeMath::StrPtrP(char* Str, char c, char** pt) {
    //$$+
    //static N=0;
    if (Str == NULL) return -1;
    for (UINT j = 0, i = 0, N = 0;; j++, i++) {
	for (;; i++) {
	    if (Str[i] == '\0' || Str[i] == '\n')return N;
	    if (i != 0) {
		if (Str[i] != c && Str[i] != ' ') {		//\t
		    if ((Str[i - 1] == c || Str[i - 1] == ' ')) {//\t
			pt[j] = &Str[i];
			N++;
			break;
		    }
		    else continue;
		}
		else continue;
	    }
	    else {
		if (Str[i] != c && Str[i] != ' ') {//\t
		    pt[j] = &Str[i];
		    N++;
		    break;
		}
		else continue;
	    }
	}
    }
}
//Порядковый номер дня от 0 января 1950 года
int CMyTeMath::JDm1950(int year, int data)
{
    //     year - год полностью (1995)                         *
    //     data - ММДД
    year -= 1948;
    if ((data -= 300) <= 0) {
	data += 1200;
	--year;
    }
    return (year / 4 + data - ((data / 100) * 347 + 3357) / 5 + 365 * year);
}
//*************************************************************************
double CMyTeMath::SpSearch_d(char* SPEC)
{//$$+
    UINT c;
    UINT cc;
    //char SPEC[1];
    //char Scl[1];
    //char Sbr[1];
    //memcpy(SPEC,Sp[0],1);
    //memcpy(Scl,Sp[1],1);
    //memcpy(Sbr,Sp[2],1);
    //double cc;	
    sscanf_s(&SPEC[1], "%1d%1d", &c, &cc);//(const char*)
    //sscanf_s(&Sbr,"%lf",&cc);//(const char*)
    if (cc == 0) return 380;
    switch (SPEC[0]) {
    case 'O': return 10. + (double)c + 100 * (double)cc;
    case 'B': return 20. + (double)c + 100 * (double)cc;
    case 'A': return 30. + (double)c + 100 * (double)cc;
    case 'F': return 40. + (double)c + 100 * (double)cc;
    case 'G': return 50. + (double)c + 100 * (double)cc;
    case 'K': return 60. + (double)c + 100 * (double)cc;
    case 'M': return 70. + (double)c + 100 * (double)cc;
    default: return 99.0;
    }
}
int CMyTeMath::CountDataFile(FILE* inpf) {
    int Count_str = 0;
    if (inpf) {
	while (!feof(inpf)) {
	    if (fgets(InLin, len, inpf) == 0) {
		//AfxMessageBox(_T("End Reading data_total File"));
		break;
	    }
	    if (InLin[0] == '*') {
		continue;
	    }
	    else {
		Count_str++;
	    }
	}
    }
    fseek(inpf, 0L, SEEK_SET);
    return Count_str;
}
void CMyTeMath::InpData(char** data, FILE* inpf, int Count_str, const char* Message) {
    ///*char** */data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
    for (UINT i = 0; i < Count_str; ) {
	try {                      // TEST FOR EXCEPTIONS.
	    if (fgets(InLin, len, inpf) == 0) break;
	    if (InLin[0] == '*') {
		continue;
	    }
	    else {
		data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
		memset(data[i], 0, len);
		data[i][0] = 0;
		memcpy(data[i], InLin, strlen(InLin));
		i++;
	    }// 	
	}
	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
	       // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
	       //Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
		     //AfxMessageBox(Message);//_T("Error Reading File")
	    exit(-1);
	}
    }
    //fclose(inpf);
}

UINT CMyTeMath::SpSearch(char N, char P)
{//$$+
    UINT c;
    sscanf_s(&P, "%d", &c);//(const char*)
    switch (N) {
    case 'O': return 10 + c;
    case 'B': return 20 + c;
    case 'A': return 30 + c;
    case 'F': return 40 + c;
    case 'G': return 50 + c;
    case 'K': return 60 + c;
    case 'M': return 70 + c;
    default: return 99;
    }
}
void CMyTeMath::Search_Band(CMyTeBand* Band,double* lambdaBeg,double* lambdaEnd)
{
//double lambdaBeg=0.0;
//double lambdaEnd=0.0;
bool Lbegin=false;
bool Lend=false;
    for(UINT i=0; i < Band->Count - 1; i++){
        double BandFlux0 = Band->Flux[i];
        double BandFlux1 = Band->Flux[i + 1];
        if ((!Lbegin) && (BandFlux0 == 0) && (BandFlux1 != 0)){
	        (*lambdaBeg) = Band->Lambda[i];
	        Lbegin = true;
        }
        if ((!Lend) && (BandFlux0 != 0) && (BandFlux1== 0)){
	        (*lambdaEnd) = Band->Lambda[i+1];
	        Lend = true;
        }
        if(Lbegin && Lend) break;
    }
}
/*  Вычисление среднего звездного гринвичского времени в радианах на 0 часов UT
jd50 - число суток от 0 января 1950 г.  (см. п/п JDm1950) */
double CMyTeMath::Stims(int jd50) {
    double k, l;
    double s;
    jd50 -= 13048;
    k = jd50 / 365;
    l = jd50 - k * 365;
    s = -3.202388e-03 - 4.166396e-3 * k + 1.7202792e-02 * l;
    return((s < 0.0) ? s + 6.2831853 : s);
}
//double CMyTeMath::Z(double t, double Sinfi, double Cosfi, double d)
//{
//	double SCZ;
//  //	s-=a;
//	return(acos(Sinfi * sin(d) + Cosfi * cos(d) * cos(t));
//}	
double CMyTeMath::Mz(double t, double Sinfi, double Cosfi, double d, short zz)
{
    double SCZ;
    switch (zz) {
    case 0: return(acos(Sinfi * sin(d) + Cosfi * cos(d) * cos(t)));
    case 1:	SCZ = 1.0 / (Sinfi * sin(d) + Cosfi * cos(d) * cos(t));
	return(SCZ - 0.0018167 * (SCZ - 1)\
	    - 0.002875 * (SCZ - 1) * (SCZ - 1)\
	    - 0.0008083 * (SCZ - 1) * (SCZ - 1) * (SCZ - 1));
    case 2: return (asin(sin(t) * Cosfi / cos(d)));
    default: return -1;
    }
}
//#endif
double CMyTeMath::Aid(CMyTeBand* Band,CMyTeBand* Extint,double Mz){

	return 0;
}
void SolverLSQ::Init()
{
    for (int i = 0; i < N; i++) {
	d[i] = 0;
	for (int j = 0; j < N; j++) R[i][j] = 0;
    }
}
void SolverLSQ::Accumulate(/*const*/ double A[], double b)
{
    //
    // Variables
    //
    //UINT      i,j;
    long double   c, s, h;
    double* a = new double[N];


    for (UINT i = 0; i < N; i++) a[i] = A[i];   // Copy A

// Construct and apply Givens plane rotation.
    for (UINT i = 0; i < N; i++)
    {
	// Construct the rotation and apply it to
	// eliminate the i-th element of a.
	if (R[i][i] == 0.0 && a[i] == 0.0) {
	    c = 1.0; s = 0.0; R[i][i] = 0.0;
	}
	else {
	    h = sqrt(R[i][i] * R[i][i] + a[i] * a[i]);
	    if (R[i][i] < 0.0) h = -h;
	    c = R[i][i] / h;
	    s = a[i] / h;
	    R[i][i] = h;
	}

	a[i] = 0.0;

	// Apply the rotation to the remaining elements of a
	for (UINT j = i + 1; j < N; j++) {
	    h = +c * R[i][j] + s * a[j];
	    a[j] = -s * R[i][j] + c * a[j];
	    R[i][j] = h;
	}

	// Apply the rotation to the i-th element of d
	h = +c * d[i] + s * b;
	b = -s * d[i] + c * b;
	d[i] = h;
    }

    delete[] a;
}


//
// Solve the LSQ problem for vector x[] by backsubstitution
//
void SolverLSQ::Solve(double x[])
{
    //
    // Variables
    //
    //UINT i,j; i=j=0;
    //double Sum=0.0;


    // Check for singular matrix
    for (UINT i = 0; i < N; i++)
	if (R[i][i] == 0.0) {
	    AfxMessageBox(_T("ERROR: Singular matrix R in SolverLSQ::Solve()"), MB_RETRYCANCEL | MB_ICONSTOP);//0U);
 //      cerr << " ERROR: Singular matrix R in SolverLSQ::Solve()" << endl;
	    exit(1);
	}

    //  Solve Rx=d for x_n,...,x_1 by backsubstitution
    x[N - 1] = d[N - 1] / R[N - 1][N - 1];
    for (int i = N - 2; i >= 0; i--) {//UINT 
	double Sum = 0.0;
	for (UINT j = i + 1; j < N; j++)
	    Sum += R[i][j] * x[j];
	x[i] = (d[i] - Sum) / R[i][i];
    }
}
#ifdef MATERVASH
//#endif
void CMyTeMath::TESTING()
{
		//CMyTeBand* Band;
		long double Pi2=6.283185307179586476925286766559;
		//UINT BandCount=4;
		FILE *inpf;
		int len=2048;
		//int NCRV=4;
		char InLin[2048];
		//int SP_Cod[5][36]={{0,80,81,82,83,84,85,86,87,88,89,90,
		//									91,92,93,94,95,96,97,99,-1},
		//							{0,70,71,46,47,48,49,72,73,52,74,
		//									75,76,77,78,79,99,-1},
		//							{0,70,71,46,47,48,49,50,51,52,53,
		//									54,55,56,57,58,59,60,61,62,63,
		//                  64,65,66,67,68,69,99,-1},
		//							{0,70,71,46,47,34,35,36,37,38,39,
		//									40,41,42,43,44,45,60,65,-1},
		//							{0,1,2,3,4,5,6,7,8,9,10,11,12,13,
		//									14,15,16,17,18,19,20,21,22,23,
		//                  24,25,26,27,28,29,30,31,32,33,99,-1} };
		bool FlagYes=false;
		char FileInput[]="C:\\mycalculateprogramm\\MyTe\\DAT_WE_STAND_.txt";//hd_103945.txt";//D_S_S.txt";//
		char FileTypesC[]="C:\\mycalculateprogramm\\MyTe\\C2000_2902.txt";//_S
		char FileTypes[]="C:\\mycalculateprogramm\\MyTe\\CATALO_2000_2404.txt";//_S
		char FileWBVR[]="C:\\mycalculateprogramm\\MyTe\\CAT_WBVR.txt";
		//char FileInput[]="C:\\mycalculateprogramm\\MyTe\\D__STAND_1.txt";		//DAT_S_WE-FON_S.txt";
		//char FileTypes[]="C:\\mycalculateprogramm\\MyTe\\2000_C_MAG_S.txt";
		char FileSpect[]="C:\\mycalculateprogramm\\MyTe\\SP_SV_A.txt";
		//char FileExtin[]="C:\\mycalculateprogramm\\MyTe\\EXTINCT.txt";
		char ErrFile[]="C:\\mycalculateprogramm\\MyTe\\NOT_FOUND_.txt";
		char OutFile[]="C:\\mycalculateprogramm\\MyTe\\RES_ST_SS.txt";
		char OutNormFile[]="C:\\mycalculateprogramm\\MyTe\\RES_N_SS.txt";

		CMyTeBand Extint;// пользоваться как обычным бандом
		// задаём Имя файла
		Extint.FName=_T("C:\\mycalculateprogramm\\MyTe\\EXTINCT_AA.txt");

		if(Extint.FName!="") {
			// загружаем файл
			Extint.LoadFromFile(Extint.FName);	
		}


		//открываем входной файл данных по всем ночам
		//if (fopen_s(&inpf, FileInput, "r") !=0 ){
		//	AfxMessageBox(_T("Error Opening data_total File"));
		//	//      return;
		//}
		////загружаем ВСЕ строковые данные (включая управляющие символы типа 'W,E,M,I,{,}...)
		////в массив Date0, кроме помеченных '*' в начале строки 
		//UINT  Count_str=0;  // Подсчёт кол-ва строк в файле прототипов
		//if (inpf) {
		//	while (!feof(inpf)){
		//		if ( fgets(InLin,len,inpf)==0 ){
		//			//AfxMessageBox(_T("End Reading data_total File"));
		//			break;
		//		}
		//		if (InLin[0]=='*'){ continue;
		//		}else{
		//			Count_str++;
		//		}
		//	}
		//}
		//fseek(inpf, 0L, SEEK_SET);
		//char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
		//for (UINT i=0; i<Count_str; ){
		//	try {                      // TEST FOR EXCEPTIONS.
		//		if ( fgets(InLin,len,inpf)==0 ) break;
		//		if (InLin[0]=='*'){ continue;
		//		}else{
		//			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
		//			memset(data[i],0,len);
		//			data[i][0]=0;
		//			memcpy(data[i],InLin,strlen(InLin));
		//			i++;
		//		}// 	
		//	}
		//	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		//		// YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		//		//Application->MessageBox("Could not allocate. Bye ...",
		//		//"Error...", MB_ICONERROR);
		//		AfxMessageBox(_T("Error Reading File"));
		//		exit(-1);
		//	}
		//}
		//fclose(inpf);
		//открыли файл типов с координатами
		if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
			AfxMessageBox(_T("Error Opening Coord File"));
			exit(-1);
		}
		// Подсчёт кол-ва строк в файле координат
		UINT  Count_coord=0;  
		if (inpf) {
			while (!feof(inpf)){
				if ( fgets(InLin,len,inpf)==0 ){
					//AfxMessageBox(_T("End Reading Coord File"));
					break;
				}
				if (InLin[0]=='*'){ continue;
				}else{
					Count_coord++;
				}
			}
		}
		fseek(inpf, 0L, SEEK_SET);
		char** data1 = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
		for (UINT i=0; i<Count_coord; ){
			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
				data1[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
				data1[i][0]=0;
				memcpy(data1[i],InLin,strlen(InLin));
				i++;// 	
			}
		}
		fclose(inpf);
		//ввод старого каталога WBVR
		//if (fopen_s(&inpf, FileWBVR, "r") !=0 ){
		//	AfxMessageBox(_T("Error Opening WBVR File"));
		//}
		//// Подсчёт кол-ва строк в файле WBVR
		//UINT  Count_WBVR=0;  
		//if (inpf) {
		//	while (!feof(inpf)){
		//		if ( fgets(InLin,len,inpf)==0 ){
		//			//AfxMessageBox(_T("End Reading WBVR File"));
		//			break;
		//		}
		//		if (InLin[0]=='*'){ continue;
		//		}else{
		//			Count_WBVR++;
		//		}
		//	}
		//}
		//fseek(inpf, 0L, SEEK_SET);
		//char** Data2 = new char*[Count_WBVR];        // STEP 1: SET UP THE ROWS.
		//for (UINT i=0; i<Count_WBVR; ){
		//	//try {                      // TEST FOR EXCEPTIONS.
		//	if ( fgets(InLin,len,inpf)==0 ) break;
		//	if (InLin[0]=='*'){ continue;
		//	}else{
		//		Data2[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
		//		Data2[i][0]=0;
		//		memcpy(Data2[i],InLin,strlen(InLin));
		//		i++;// 	
		//	}
		//	//}
		//	//catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		//	// // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		//	// //Application->MessageBox("Could not allocate. Bye ...",
		//	// //"Error...", MB_ICONERROR);
		//	// AfxMessageBox(_T("Error Reading File WBVR"));
		//	// exit(-1);
		//	//}
		//}
		//fclose(inpf);

		if(fopen_s(&inpf, FileTypesC, "r") !=0 ){ 
			AfxMessageBox(_T("Error Opening Coord File"));
			exit(-1);//return -1.0;
		}
		// Подсчёт кол-ва строк в файле координат
		UINT  Count_coordC=0;  
		if (inpf) {
			while (!feof(inpf)){
				if ( fgets(InLin,len,inpf)==0 ){
					//AfxMessageBox(_T("End Reading Coord File"));
					break;
				}
				if (InLin[0]=='*'){ continue;
				}else{
					Count_coordC++;
				}
			}
		}
		fseek(inpf, 0L, SEEK_SET);
		char** data1C = new char*[Count_coordC];        // STEP 1: SET UP THE ROWS.
		for (UINT i=0; i<Count_coordC; ){
			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
				data1C[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
				data1C[i][0]=0;
				memcpy(data1C[i],InLin,strlen(InLin));
				i++;// 	
			}
		}
		fclose(inpf);

		//считаем число строк без управляющих символов Count_str
		//создаем массивы входных данных 
		typedef struct Date0 {
			bool FlagYes;
			char Name[14];
			char Spect[4];
			char DDMOYY[7];
			char HHMMSS[7];
			char KLAD[2];
			char STAND[2];
			double* SpED;
			double UT;
			double JD;
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double Ri[MaxBand];
			double RO[MaxBand];
			double RM[MaxBand];
			double* eR;
			double TimeAngle;
			double Mz;
			double SidTime;
		} Date0;
		typedef struct WBVR_O{
			char Name[12];
			double RO[MaxBand];
		}WBVR_O;
		typedef struct TYPES{
			char Name[12];
			char TyName[13];
			char TSp[4];
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double eRA;
			double eDE;
			double RT[2];
			double eRT[2];
		}TYPES;
		char** ptr=new char* [200];
		TYPES* TYP = new TYPES[Count_coord];
		TYPES* TYPC= new TYPES[Count_coord];
	
		if (fopen_s(&errf, ErrFile, "a") !=0 ){
			AfxMessageBox(_T("Error Opening Err File"));
		}
		for(UINT i=0;i<Count_coord;i++){
			if(StrPtr(data1[i],'|',ptr)!=0)
			{
				sscanf_s(ptr[3],"%lf",&TYP[i].RA);
				sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
				sscanf_s(ptr[4],"%lf",&TYP[i].DE);
				sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
				sscanf_s(ptr[7],"%lf",&TYP[i].RT[0]);
				sscanf_s(ptr[8],"%lf",&TYP[i].eRT[0]);
				sscanf_s(ptr[9],"%lf",&TYP[i].RT[1]);
				sscanf_s(ptr[10],"%lf",&TYP[i].eRT[1]);
				memcpy((void*)TYP[i].Name,(const void*)ptr[2],9);
				TYP[i].Name[9]=0;
				memcpy((void*)TYP[i].TyName,(const void*)ptr[1],12);
				TYP[i].TyName[12]=0;
				memcpy((void*)TYP[i].TSp,(void*)ptr[11],3);
				TYP[i].TSp[3]=0;
			}
		}
		for(UINT i=0;i<Count_coordC;i++){
			if(StrPtr(data1C[i],'|',ptr)!=0)
			{
				memcpy((void*)TYPC[i].Name,(const void*)ptr[2],9);
				TYPC[i].Name[9]=0;
				memcpy((void*)TYPC[i].TyName,(const void*)ptr[1],12);
				TYPC[i].TyName[12]=0;
				memcpy((void*)TYPC[i].TSp,(void*)ptr[11],3);
				TYPC[i].TSp[3]=0;
			}else {
				fprintf_s(errf,"%s|",TYPC[i].TyName);
				exit(-1);//return -1;
			}
		}
		bool FlagYes0=false;
		for(UINT i=0;i<Count_coord;i++){
			FlagYes=false;
			FlagYes0=false;
			for(UINT j=0;j<Count_coordC;j++){
				if(strncmp(TYP[i].Name,TYPC[j].Name,9)==0){
					if(strncmp(TYP[i].TSp,TYPC[j].TSp,3)!=0 ){
						memcpy((void*)TYP[i].TSp,TYPC[j].TSp,4);//TYPC[i].TSp=TYP[j].TSp;
						fprintf(errf,"%5d|%s|%s|\n",i+1,TYP[i].Name,TYP[i].TSp);
						FlagYes=true;
						break;
					}else FlagYes0=true;
				}else continue;
			}
			if(!FlagYes ){
				if(!FlagYes0) fprintf(errf,"%5d|%s|%s|\n",i+1,TYP[i].Name,TYP[i].TSp);//TYPC[i].TSp);
				else fprintf(errf,"%5d|%s|%s|\n",i+1,TYP[i].Name,TYP[i].TSp);
			}
		}
		fclose(errf);
		//delete [] DS;
		//delete [] DJ;
		//delete [] WbvrO;
		delete [] TYP;
		delete [] TYPC;
		//de_allocate(data,Count_str);
		de_allocate(data1,Count_coord);
		de_allocate(data1C,Count_coordC);
		//de_allocate(Data2,Count_WBVR);
		_fcloseall();
		//return 0.0;
	//}
}
#endif
#ifdef EMOE
void CMyTeMath::TEST()//ING
{
	FILE *inpf,*outf;
	int len=2048;
	char InLin[2048];
	char FileTypesC[]="AOUT.txt";//RES_TOT_DIFF.txt";//DATA_TOT_LAST.txt";//C2000_2902.txt";//_S_Q
	char FileTypes[]="AINP.txt";//STD_6000.txt";//CATALO_2000_2404.txt";//_S
	if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
		AfxMessageBox(_T("Error Opening Coord File"));
		exit(-1);//return -1.0;
	}
	UINT  Count_coord=0;  
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//AfxMessageBox(_T("End Reading Coord File"));
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_coord++;
			}
		}
	}
	fseek(inpf, 0L, SEEK_SET);
	char** data = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
	for (UINT i=0; i<Count_coord; ){
		if ( fgets(InLin,len,inpf)==0 ) break;
		if (InLin[0]=='*'){ continue;
		}else{
			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
			data[i][0]=0;
			memcpy(data[i],InLin,strlen(InLin));
			i++;// 	
		}
	}
	fclose(inpf);
	if(fopen_s(&outf, FileTypesC, "w") !=0 ){ 
		AfxMessageBox(_T("Error Opening CoordC File"));
		exit(-1);//return -1.0;
	}

	UINT N_Part=0;
	UINT N_Int=0;
	sscanf_s(data[0],"%d%d",&N_Part,&N_Int);
	double *JD = new double[N_Part];        // STEP 1: SET UP THE ROWS.
	double *DM = new double[N_Part];        // STEP 1: SET UP THE ROWS.
	double JDout;
	double DMout;

	for (UINT i=1; i<=N_Part;i++ ){
		sscanf_s(data[i],"%lf%lf",&JD[i],&DM[i]);
	}
	for (UINT i=1; i<=N_Part; ){
		double Sjd=0;
		double Sdm=0;
		for (UINT j=0; j<N_Int;j++ ){
			Sjd+=JD[i+j];
			Sdm+=DM[i+j];
		}
		JDout=Sjd/N_Int;
		DMout=Sdm/N_Int;
		fprintf(outf,"%14.6lf  %8.5lf\n",JDout,DMout);
		i+=N_Int;
	}

	fclose(outf);
	delete [] JD;
	delete [] DM;
	_fcloseall();
	//return 0.0;
	//}
}
#endif
#ifdef EMOE
//ПРОГРАММА ВЫБОРКИ ОШИБОЧНЫХ ВЕЛИЧИН критерий > 0,1
void CMyTeMath::TEST()
{
	char FileInput[]="D:\\mecalculateprogramm\\MyTe\\resTOT_8766_new.txt";//RES_TOTAL_8766_.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char OutFile[]="D:\\mecalculateprogramm\\MyTe\\RES_ERR_8766_new_up0.3.txt";
	char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File"));
 //      return;
	}
	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File"));
 //      return;
	}
	//if (fopen_s(&outf, ErrFile, "w") !=0 ){
	//	AfxMessageBox(_T("Error Opening Err File"));
 ////      return;
	//}
	char** ptr=new char* [200];
	UINT  Count_str=0;
	UINT  Count_err=0;
	double dW,dB,dV,dR;
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	 fseek(inpf, 0L, SEEK_SET);
	 char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
    			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
 				memset(data[i],0,len);
	   			data[i][0]=0;
    			memcpy(data[i],InLin,strlen(InLin));
				i++;
			}// 	
	   }
	   catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		  //Application->MessageBox("Could not allocate. Bye ...",
			   //"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
	   }
	 }
	 fclose(inpf);

	 for(UINT i=0;i<Count_str;i++){
		if(StrPtr(data[i],'|',ptr)!=0){
			sscanf_s(ptr[13],"%lf",&dW);
			sscanf_s(ptr[14],"%lf",&dB);
			sscanf_s(ptr[15],"%lf",&dV);
			sscanf_s(ptr[16],"%lf",&dR);
		}
		if((abs(dW)<1.0 && abs(dW)>=0.3) || (abs(dB)<1.0 && abs(dB)>=0.3) || (abs(dV)<1.0 && abs(dV)>=0.3) || (abs(dR)<1.0 && abs(dR)>=0.3)){
			fputs(data[i],outf);
			Count_err++;
			//if(Count_err==1168)
			//	int i=0;
		}
	}
	fcloseall();
}
	//}
#endif
#ifdef EMOE
void CMyTeMath::TEST(){
//#ifdef MAZAFAKA
	long double Pi2=6.283185307179586476925286766559;
	//UINT BandCount=4;
	//FILE *inpf;
	//int len=2048;
	//int NCRV=4;
	char InLin[len];
	bool FlagYes=false;   
	char FileInput[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND_W_LSQ.txt";//resTOT_8766_01_2014.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileTypes[]="D:\\mecalculateprogramm\\MyTe\\CAT_LAST.txt";//CATALO_2000_2404.txt";//_S_Q
	//char FileWBVR[]="D:\\mecalculateprogramm\\MyTe\\CAT_WBVR.txt";
	//char FileSpect[]="D:\\mecalculateprogramm\\MyTe\\SP_SV_A.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	char OutFile[]="D:\\mecalculateprogramm\\MyTe\\RES.txt";
	//char OutNormFile[]="D:\\mecalculateprogramm\\MyTe\\RES_200889.txt";//RES_N_6000.txt";//RES_Q_N_6000.txt";
	//char OutNormEndFile[]="D:\\mecalculateprogramm\\MyTe\\RES_200889_DIFF.txt";//RES_Q_DIFF_6000.txt";//RES_Q_DIFF_6000.txt";//RES6000_N_End.txt";
    CMyTeBand Extint;// пользоваться как обычным бандом
    // задаём Имя файла
    Extint.FName=_T("D:\\mecalculateprogramm\\MyTe\\EXTINCT_AA.txt");
    if(Extint.FName!="") {
        // загружаем файл
        Extint.LoadFromFile(Extint.FName);	
    }   

//открываем входной файл данных по всем ночам
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//загружаем ВСЕ строковые данные (включая управляющие символы типа 'W,E,M,I,{,}...)
	//в массив Date0, кроме помеченных '*' в начале строки 
	UINT  Count_str=0,IDPC=0;  // Подсчёт кол-ва строк в файле прототипов
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//AfxMessageBox(_T("End Reading data_total File"));
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	IDPC=Count_str;
	 fseek(inpf, 0L, SEEK_SET);
	 char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
    			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
 				memset(data[i],0,len);
	   			data[i][0]=0;
    			memcpy(data[i],InLin,strlen(InLin));
				i++;
			}// 	
	   }
	   catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		  //Application->MessageBox("Could not allocate. Bye ...",
			   //"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
	   }
	 }
	 fclose(inpf);
	char *ptr[len];

	int IStlb=StrPtr(data[0],'|',ptr);
	int PLSQ = IStlb-1;//COUNT_PART+IMi;// Count_DPCat-1  количество оцениваемых параметров без звезды hd 5015 (нуль-пункт)
	SolverLSQ MNK(PLSQ);
	double *B = new double [IDPC]; // правая часть уравнений МНК
	//for (UINT i=0;i<IDPC;i++)
	//	double *B[i]=new double [MaxBand];
	double **A = new double* [IDPC]; // левая часть уравнений МНК
	for (UINT i=0;i<IDPC;i++)
		A[i]=new double [PLSQ];
	//UINT IStlb=0;
	//double **Res = new double* [MaxBand]; // правая часть уравнений МНК
	double *Res = new double [PLSQ]; // правая часть уравнений МНК
	//double **C = new double* [IDPC]; //  расширенное уравнение МНК
	for(UINT i=0;i<IDPC;i++){
		if(StrPtr(data[i],'|',ptr)!=0){
			sscanf_s(ptr[0],"%lf",&B[i]);//[k]
			for(UINT j=0;j<PLSQ;j++){//IStlb
				sscanf_s(ptr[j+1],"%lf",&A[i][j]);
			}
		}
	}
	if(fopen_s(&outf, OutFile, "a") !=0 ){ 
		AfxMessageBox(_T("Error Opening Out File"));
		exit(-1);
	}
		for(UINT i=0;i<IDPC;i++){
			fprintf(outf,"%7.3lf|",B[i]);
			for(UINT j=0;j<PLSQ;j++)
				fprintf(outf,"%7.3lf|",A[i][j]);
			fprintf(outf,"\n");
		}
		//fclose(outf);
	//for (UINT k=0;k<MaxBand;k++)
	//	double *Res=new double [PLSQ];

	////for (UINT k=0;k<MaxBand;k++){
	//	for(UINT i=0;i<IDPC;i++){
	//		IStlb=StrPtr(data[i],'|',ptr);
	//		C[i]=new double [IStlb];//+1-MaxBand
	//		if(IStlb!=0){
	//			//sscanf_s(ptr[k],"%lf",&B[i]);//[k]
	//			//C[i][0]=B[i][k];
	//			for(UINT j=0;j<IStlb;j++){
	//				sscanf_s(ptr[j],"%lf",&C[i][j]);
	//				//C[i][j+1]=A[i][j];
	//			}
	//		}
	//	}
	//}
	//if(fopen_s(&outf, OutFile, "w") !=0 ){ 
	//	AfxMessageBox(_T("Error Opening Out File"));
	//	exit(-1);
	//}
	int NLSQ=IStlb;//+1-MaxBand;
//double **C = new double* [IDPC]; //  расширенное уравнение МНК
	//for(UINT i=0;i<IDPC;i++){
	//	C[i]=new double [IStr+1];

	//for (UINT i=0;i<IDPC;i++){
	//	C[i][0]=B[i][k];
	//for (UINT k=0;k<MaxBand;k++){

	//double *SUM=new double [NLSQ];
	//for (UINT jj=1;jj<NLSQ;jj++){
	//	for (UINT j=0;j<NLSQ;j++){
	//		SUM[j]=0;
	//		for(UINT i=0;i<IDPC;i++){
	//			SUM[j]+=C[i][j]*C[i][jj];
	//		}
	//		fprintf(outf,"%7.3lf|",SUM[j]);
	//	}
	//	fprintf(outf,"\n");
	//}
	//fclose(outf);


//#ifdef EMOE
		MNK.Init();
		for(UINT i=0;i<IDPC;i++){
  			MNK.Accumulate(A[i],B[i]);
		}
		MNK.Solve(Res);
	//}
	//if(fopen_s(&outf, OutFile, "w") !=0 ){ 
	//	AfxMessageBox(_T("Error Opening Out File"));
	//	exit(-1);
	//}
	//double **RC = new double* [IDPC]; // правая часть уравнений МНК
	//for (UINT i=0;i<IDPC;i++)
	//	double *RC=new double [MaxBand];
	double *RC = new double [IDPC]; // правая часть уравнений МНК
	//double *RC=new double [
		for(UINT i=0;i<IDPC;i++){
			//for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"%7.3lf|",B[i]);
				for(UINT j=0;j<PLSQ;j++){
					RC[i]=B[i]-Res[j];
					fprintf(outf,"     %9.5lf| %9.5lf| %9.5lf|",A[i][j],Res[j]*A[i][j],RC[i]*A[i][j]);

					//RC[i][k]=B[i][k]+Res[k][j];
					//fprintf(outf,"%7.3lf| %7.3lf| %7.3lf| %7.3lf|",B[i][k],A[i][j],Res[k][j],RC[i][k]);
				}
				fprintf(outf,"\n");

			//}
		}

		fclose(outf);


	fcloseall;
}
#endif	
#ifdef MAZAFAKA
	//открыли файл типов с координатами
	 if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
			AfxMessageBox(_T("Error Opening Coord File"));
		  return -1.0;
	 }
	// Подсчёт кол-ва строк в файле координат
	 UINT  Count_coord=0;  
	 if (inpf) {
	   while (!feof(inpf)){
    		if ( fgets(InLin,len,inpf)==0 ){
				//AfxMessageBox(_T("End Reading Coord File"));
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_coord++;
			}
		}
	  }
	 fseek(inpf, 0L, SEEK_SET);
	 char** data1 = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_coord; ){
		if ( fgets(InLin,len,inpf)==0 ) break;
		if (InLin[0]=='*'){ continue;
		}else{
			data1[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
			data1[i][0]=0;
			memcpy(data1[i],InLin,strlen(InLin));
			i++;// 	
		}
	 }
	 fclose(inpf);
 //ввод старого каталога WBVR
	 if (fopen_s(&inpf, FileWBVR, "r") !=0 ){
		 AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR
	 UINT  Count_WBVR=0;  
	 if (inpf) {
		 while (!feof(inpf)){
			 if ( fgets(InLin,len,inpf)==0 ){
				 //AfxMessageBox(_T("End Reading WBVR File"));
				 break;
			 }
			 if (InLin[0]=='*'){ continue;
			 }else{
				 Count_WBVR++;
			 }
		 }
	 }
	 fseek(inpf, 0L, SEEK_SET);
	 char** Data2 = new char*[Count_WBVR];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_WBVR; ){
		 //try {                      // TEST FOR EXCEPTIONS.
			 if ( fgets(InLin,len,inpf)==0 ) break;
			 if (InLin[0]=='*'){ continue;
			 }else{
				 Data2[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
				 Data2[i][0]=0;
				 memcpy(Data2[i],InLin,strlen(InLin));
				 i++;// 	
			 }
	 }
	 fclose(inpf);

//#endif
	//считаем число строк без управляющих символов Count_str
	//создаем массивы входных данных 
		typedef struct Date0 {
			bool FlagYes;
			char Name[14];
			char Spect[4];
			char DDMOYY[7];
			char HHMMSS[7];
			char KLAD[2];
			char STAND[2];
			double TC;
			double* SpED;
			double UT;
			double JD;
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double Ri[MaxBand];
			double RO[MaxBand];
			double RM[MaxBand];
			double RD[MaxBand];
			double* eR;
			double TimeAngle;
			double Mz;
			double SidTime;
		} Date0;
	//#ifdef ETBOE
		typedef struct WBVR_O{
			char Name[12];
			double RO[MaxBand];
		}WBVR_O;
		typedef struct TYPES{
			char Name[12];
			char TyName[13];
			char TSp[4];
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double eRA;
			double eDE;
			double RT[2];
			double eRT[2];
		}TYPES;
	char** ptr=new char* [200];
	TYPES* TYP = new TYPES[Count_coord];
	for(UINT i=0;i<Count_coord;i++){
		if(StrPtr(data1[i],'|',ptr)!=0)
		{
			sscanf_s(ptr[3],"%lf",&TYP[i].RA);
			sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
			sscanf_s(ptr[4],"%lf",&TYP[i].DE);
			sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
			sscanf_s(ptr[7],"%lf",&TYP[i].RT[0]);
			sscanf_s(ptr[8],"%lf",&TYP[i].eRT[0]);
			sscanf_s(ptr[9],"%lf",&TYP[i].RT[1]);
			sscanf_s(ptr[10],"%lf",&TYP[i].eRT[1]);
			memcpy((void*)TYP[i].Name,(const void*)ptr[2],9);
			TYP[i].Name[9]=0;
			memcpy((void*)TYP[i].TyName,(const void*)ptr[1],12);
			TYP[i].TyName[12]=0;
			memcpy((void*)TYP[i].TSp,(void*)ptr[11],3);
			TYP[i].TSp[3]=0;
		}
	}

//#ifdef ETBOE
//#ifdef DONETKNOW
//#endif

	//создаем массив структур=строк
	//W_O* WO = 0;
	WBVR_O* WbvrO = new WBVR_O[Count_WBVR];
	//WO = new W_O[Count_WBVR];
//#endif
//читаем дату из названия файла (DDMMYY.FEA.txt) как DD,MO,YY;
//либо из файла
	int DD,MO,YY,HH,MM,SS;
	int Year;
	int WBVR[MaxBand];
	for(UINT i=0;i<MaxBand;i++){
		wchar_t NC = Band[i].Name;
		switch(NC){
			case _T('W'):
			case _T('w'):
				WBVR[0] = i;				
				break;
			case _T('B'):
			case _T('b'):
                WBVR[1] = i;
				break;
			case _T('V'):
			case _T('v'):
                WBVR[2] = i;
				break;
			case _T('R'):
			case _T('r'):
                WBVR[3] = i;
				break;
			default:break;
		}
		Search_Band(&Band[i],&Band[i].LBeg,&Band[i].LEnd);
	}

//************ ВВод спектров *******************
//==============================================
	 if(fopen_s(&inpf, FileSpect, "r") !=0 ){ 
		AfxMessageBox(_T("Error Opening Spectra File"));
		//goto error_return;
	 }
	 fgets(InLin,len,inpf); //Name Spectra
	UINT NCS = 0;
	 NCS=StrPtr(&InLin[8],' ',ptr);
	//NCS; // Кол-во колонок в SpData без волн = число типов спектра
	char **NameSp=new char* [NCS];
		for(UINT i = 0; i < NCS; i++){
			NameSp[i]=new char [4];
			memset(NameSp[i],0,4);
		}
	//char buff[10];
	for(UINT i=0;i<NCS;i++){
		memcpy((void*)NameSp[i],(void*)ptr[i],3);
		NameSp[i][3]=0;
	}
	UINT DLRSP=0;
	while(1){
		if(fgets(InLin,len,inpf)==0) break;
	  ++DLRSP;
	}
	double *SpLambd = new double[DLRSP];
	double **SpSV = new double*[NCS];
	for(UINT i=0;i<NCS;i++){
		SpSV[i] = new double[DLRSP];  // STEP 2: SET UP THE COLUMNS
	}
	fseek(inpf, 0L, SEEK_SET);
	fgets(InLin,len,inpf);//съели заголовок
	for(UINT j=0;j<DLRSP;j++){
		fgets(InLin,len,inpf);
		sscanf_s(InLin,"%lf",&SpLambd[j]);
	  StrPtr(&InLin[8],' ',ptr);
	  for(UINT i=0;i<NCS;i++){
  		if(sscanf_s(ptr[i],"%lf",&SpSV[i][j])==NULL) {
    		SpSV[i][j]=99999.9;
		}// break;}
		//fprintf(errf,"%10.1lf ",SpSV[i][j]);
	  }
	  //fputs("\n",errf);
	}	
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Err File"));
	}
char SpBUFF[]="A05 "; 
SpBUFF[3]=0;
//вводим старые величины WBVR
double R[MaxBand];
for(UINT i=0;i<Count_WBVR;i++){
	bool FlagYes=false;
	if(StrPtr(Data2[i],'|',ptr)!=0){
		memcpy((void*)WbvrO[i].Name,(void*)ptr[0],11);
		WbvrO[i].Name[11] = '\0';
		for(UINT k=0;k<MaxBand;k++){
			sscanf_s(ptr[k+5],"%lf",&R[k]);
		}
	}//W=W_B+B_V+V
	WbvrO[i].RO[0]=R[1]+R[2]+R[0];
	WbvrO[i].RO[1]=R[2]+R[0];
	WbvrO[i].RO[2]=R[0];
	WbvrO[i].RO[3]=R[0]-R[3];
}
//#endif
//наполнение DS
#define	MaxTypSt 100
#define	MaxSt 300
int CountSt=0;//число стандартов в куске
int CountTypSt=0;//число типов стандартов куске
Date0* DS = new Date0[Count_str];
//Date0** DDST = new Date0*[MaxTypSt];
for (UINT i=0;i<MaxTypSt;i++){
	DDST[i]=new Date0 [MaxSt];
}
Date0* DDS = new Date0[MaxSt];
double DDD[MaxTypSt][MaxBand];
UINT NS[MaxTypSt];
double N;
UINT N_Part=0;
UINT Beg_Part=0;
UINT ENd_Part=0;
for(UINT I0=0;I0<Count_str;){//НАЧАЛО ГЛОБАЛЬНОГО ЦИКЛА ПО КУСКАМ
	N_Part=0;
	//if (data[I0][0]=='{') //НАЧАЛО СЧЕТА КУСКА
	{
		//Beg_Part=I0+1;
		//for (UINT j=0;;j++)
		//{		
		//	if (data[j+Beg_Part][0]=='}'){
		//		ENd_Part=j+Beg_Part;
		//		break;
		//	}else {
		//		N_Part++;
		//		continue;
		//	}
		//}
		//I0=Beg_Part;

		Date0* DS=new Date0[N_Part];


		for(UINT i=0;i<Сount_str;i++){// ЦИКЛ счет количества кусков
			bool FlagYes=false;
			if(StrPtr(data[i+I0],'|',ptr)!=0){
				memcpy((void*)&DS[i].Name,(void*)ptr[0],11);
				DS[i].Name[11] = '\0';
				DS[i].SpED=new double [DLRSP];
				DS[i].FlagYes=false;
				sscanf_s(ptr[1],"%lf",&DS[i].TC);
				memcpy((void*)&DS[i].KLAD,(void*)ptr[2],1);
				DS[i].KLAD[1]=0;
				memcpy((void*)&DS[i].STAND,(void*)ptr[3],1);
				DS[i].STAND[1]=0;
				for(UINT k=0;k<MaxBand;k++){
					sscanf_s(ptr[k+4],"%lf",&DS[i].Ri[k]);
					N=DS[i].Ri[k];
					if (N<=0.0){
						DS[i].Ri[k]=0.0;
					} else{
						DS[i].Ri[k]=-2.5*log10(N/(1-28e-9*N));
					}
				}
				memcpy((void*)&DS[i].DDMOYY,(void*)ptr[8],6);
				memcpy((void*)&DS[i].HHMMSS,(void*)ptr[9],6);
				DS[i].DDMOYY[6] = '\0';
				DS[i].HHMMSS[6] = '\0';

				sscanf_s(ptr[8],"%2d%2d%2d",&DD,&MO,&YY);
				sscanf_s(ptr[9],"%2d%2d%2d",&HH,&MM,&SS);
			}
			int Dt=MO*100+DD;
			if(YY<40) Year=2000+YY;
			else Year=1900+YY;
			DS[i].UT=(double)HH+(double)MM/60.0+(double)SS/3600.0;
		//вычисляем порядковый номер дня от 0 января 1950 года
			int JD50 = JDm1950(Year, Dt);
		//Вычисляем среднее звездное гринвичское время в радианах на 0 часов UT на данное число
		//JD50 - число суток от 0 января 1950 г.
			double S0=Stims(JD50); //S0 в радианах
			double JD= (double)JD50+DS[i].UT/24;
			DS[i].JD=JD;
			DS[i].SidTime=fmod( (double)(S0 + (double)(1.0027379*DS[i].UT*
							0.2617993877983 + 1.34334356423)),(double)Pi2 );
	//получение и рассылка координат звезд
			FlagYes=false;
			for(UINT j=0;j<Count_coord;j++){
				int a=strncmp(TYP[j].Name,DS[i].Name,9);
				if(a==0){
					//DS[i].Spect = Types_Sp[j];
					memcpy(DS[i].Spect,TYP[j].TSp,4);
					DS[i].Spect[3]=0;
					DS[i].RA=TYP[j].RA;
					DS[i].DE=TYP[j].DE;
					DS[i].pmRA=TYP[j].pmRA;
					DS[i].pmDE=TYP[j].pmDE;
					if((DS[i].TimeAngle = DS[i].SidTime - TYP[j].RA * Pi/180)<0)
						DS[i].TimeAngle += Pi2;//t=s-RA в радианах		
					DS[i].Mz=Mz(DS[i].TimeAngle, SINfi, COSfi, DS[i].DE*Pi/180);
					FlagYes=true;
					break;
				}else continue;			
			}
			if(!FlagYes){
				fprintf(errf,"%s\n",DS[i].Name);
				DS[i].TimeAngle=0;
				DS[i].DE=0;
				DS[i].Mz=1.0;
				memcpy(DS[i].Spect,SpBUFF,4);
				//DS[i].Spect=SpBUFF;
			}
			FlagYes=false;
			for(UINT j=0;j<Count_WBVR;j++){
				if(strncmp((const char*)DS[i].Name,(const char*)WbvrO[j].Name,9)==0){
					for(UINT k=0;k<MaxBand;k++){
						DS[i].RO[k]=WbvrO[j].RO[k];
					}
					FlagYes=true;
					break;
				}else continue;			
			}
			if(!FlagYes){
				//fprintf(errf,"%s\n",DS[i].Name);
				for(UINT k=0;k<MaxBand;k++){
					DS[i].RO[k]=0.0;
				}
			}
	//#ifdef EMOE
		//************ Выбор Типов Спектров ********************//
		//======================================================//
int ic=0;
UINT p1,p2,cc;
UINT m=0;
			cc=SpSearch(DS[i].Spect[0],DS[i].Spect[1]);
			if(DS[i].Spect[2]==' '){
				if (cc>58){ ic=2;
				} else	ic=4;
			}else {
				sscanf_s(&DS[i].Spect[2],"%1d", &ic);
				ic-=1; // Класс светимости
			}
			while(1){
  				if(SP_Cod[ic][m+1]<0){
    				for(UINT j=0;j<DLRSP;j++){
	    				DS[i].SpED[j]=SpSV[SP_Cod[ic][m]][j];
						//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",DS[i].Spect,SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
					}
					break;//return;
  				}else{
					p1=SpSearch(NameSp[SP_Cod[ic][m]][0],NameSp[SP_Cod[ic][m]][1]);
					p2=SpSearch(NameSp[SP_Cod[ic][m+1]][0],NameSp[SP_Cod[ic][m+1]][1]);
  					if( (cc>=p1)&&(cc<p2) ){
						for(UINT j=0;j<DLRSP;j++){
							DS[i].SpED[j]= LinInt(SpSV[SP_Cod[ic][m]][j],SpSV[SP_Cod[ic][m+1]][j],
								(double)p1,(double)p2,(double)cc);
						}
						break;//	      return;
					}else{ m++;}
				}
			}
		}//КОНЕЦ ЦИКЛА рассылки данных по куску
		fclose(errf);
//Вычисление Ai по куску
//Подготовка подинтегральных функций для вычисления Ai
double **SubintP = new double* [MaxBand];
double **Subint = new double* [MaxBand];
		for(UINT k=0;k<MaxBand;k++){
			SubintP[k] = new double [DLRSP];
			Subint[k] = new double [DLRSP];
		}
		for(UINT i=0;i<N_Part;i++){		
			for(UINT k=0;k<MaxBand;k++){
				for(UINT j=0;j<DLRSP;j++){
					double a=DS[i].SpED[j];
					double b=Band[WBVR[k]].GetFlux(SpLambd[j]);
					double c=Extint.GetFlux(SpLambd[j]);
					double cc=pow(c,DS[i].Mz);//Extint.GetFlux(SpLambd[j])
					SubintP[k][j] = a*b*cc;
					Subint[k][j] = a*b;
						//DS[i].SpED[j]*Band[WBVR[k]].GetFlux(SpLambd[j])*pow(Extint.GetFlux(SpLambd[j]),DS[i].Mz);// Sp*Band*p^Mz 		sscanf(InLin,"%lf%lf",&EXT_Lambd[i],&EXT_Flux[i]);
				}
				double d= Integral_M(SubintP[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
				double dd=Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd);
				double ddd=	-2.5*log10(d/dd);		//double dd=

				DS[i].Ri[k]-=ddd; //-2.5*log10(Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd));
			}
//		i=i;
		}	//Конец вычислений Ai по куску
		de_allocate(SubintP,MaxBand);
		de_allocate(Subint,MaxBand);

//#ifdef EMOE
// контрольная печать в файл ПРОБКИ
	//открыли выходной файл
		if (fopen_s(&outf, OutFile, "w") !=0 ){
			AfxMessageBox(_T("Error Opening result File"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|\n");

//#ifdef EMOE
		for(UINT i=0;i<N_Part;i++){
			fprintf(outf,"%s|  %s| %s|",DS[i].Name,DS[i].KLAD,DS[i].STAND);
			fprintf(outf,"  %7.3lf|  %12.8lf|  %12.8lf|  %8.4lf|",DS[i].TimeAngle/Pi*12,DS[i].RA,DS[i].DE,DS[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %7.3lf|",DS[i].Ri[k]);
			}
			fprintf(outf,"%11.5lf|",DS[i].JD);
			fprintf(outf,"%s|%s|%s",DS[i].Spect,DS[i].DDMOYY,DS[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DS[i].RO[k]);
			}
			fputs("\n",outf);
		}
		fclose(outf);
//#endif
// нормализация
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|\n");
CountSt=0;//число стандартов в куске
CountTypSt=0;//число типов стандартов куске

//#ifdef EMOE
for(UINT i=0;i<N_Part;i++){
			if(DS[i].STAND[0]=='S'){
				for(UINT k=0;k<MaxBand;k++)	
					DS[i].RM[k]=DS[i].RO[k]-DS[i].Ri[k];
				DDS[CountSt]=DS[i];//[si]
				CountSt++;
			}else 
				for(UINT k=0;k<MaxBand;k++)	
					DS[i].RM[k]=DS[i].Ri[k];
		}
		double RA=0;
		double DE=0;
		for(int jk=0,ii=0;jk<CountSt;jk++){ //перетряс стандартов куска
			if(!DDS[jk].FlagYes){
				RA=DDS[jk].RA;
				DE=DDS[jk].DE;
			}else continue;
			int ij=0;
			for (int k = 0; k<MaxBand; k++){
				DDD[ii][k]=0;
			}
			for (int ik = 0; ik < CountSt; ik++){
				if(DDS[ik].RA==RA && DDS[ik].DE==DE){
					DDST[ii][ij]=DDS[ik];
					for(UINT k=0;k<MaxBand;k++){
						DDD[ii][k]+=DDST[ii][ij].RM[k];
					}
					DDS[ik].FlagYes=true;
					ij++;
					NS[ii]=ij;
				}else continue;
			}
			for(int k=0; k<MaxBand;k++){
				DDD[ii][k]/=ij;
			}
			ii++;//if(ij<kk)
		}
//#endif
	}else 	//конец куска
		AfxMessageBox(_T("Begin of part NOT FOUND"));
}
#endif
	// Проверка вычисления угловых расстояний
#ifdef EMOE1
void CMyTeMath::TESTING()
{
	
	char FileInput[]="D:\\mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char OutFile[]="D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
 //      return;
	}
	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File"));
 //      return;
	}
	//if (fopen_s(&outf, ErrFile, "w") !=0 ){
	//	AfxMessageBox(_T("Error Opening Err File"));
 ////      return;
	//}
	char** ptr=new char* [200];
	UINT  Count_str=0;
	//UINT  Count_err=0;
	//double dW,dB,dV,dR;
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	 fseek(inpf, 0L, SEEK_SET);
	 char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
   			if ( fgets(InLin,len,inpf)==0 ) break;
			if (InLin[0]=='*'){ continue;
			}else{
    			data[i] = new char[len];  // STEP 2: SET UP THE COLUMNS
 				memset(data[i],0,len);
	   			data[i][0]=0;
    			memcpy(data[i],InLin,strlen(InLin));
				i++;
			}// 	
	   }
	   catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		  //Application->MessageBox("Could not allocate. Bye ...",
			   //"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
	   }
	 }
	 fclose(inpf);
	 int PARA=0;
	 double RA1=0;
	 double DE1=0;
	 double RA2=0;
	 double DE2=0;
	 double Rho=0;
	 for(UINT i=0;i<Count_str;i++){
		if(StrPtr(data[i],'|',ptr)!=0){
			sscanf_s(ptr[0],"%d",&PARA);
			sscanf_s(ptr[1],"%lf",&RA1);
			sscanf_s(ptr[2],"%lf",&DE1);
			sscanf_s(ptr[3],"%lf",&RA2);
			sscanf_s(ptr[4],"%lf",&DE2);
		}
		double dX=cos(DE1*Pi/180.0)*cos(RA1*Pi/12.0)-cos(DE2*Pi/180.0)*cos(RA2*Pi/12.0);
		double dY=cos(DE1*Pi/180.0)*sin(RA1*Pi/12.0)-cos(DE2*Pi/180.0)*sin(RA2*Pi/12.0);
		double dZ=sin(DE1*Pi/180.0)-sin(DE2*Pi/180.0);
		Rho=2*asin(0.5*sqrt(dX*dX+dY*dY+dZ*dZ));
		fprintf(outf,"%3d %8.4lf %8.4lf   %8.4lf %8.4lf     %8.4lf\n",PARA,RA1,DE1,RA2,DE2, Rho*180/Pi);
	}
fcloseall();
}
#endif
#ifdef EMOE2
//Выдача нормированных спектров NGSL
void CMyTeMath::TESTING()
{
	FILE *inpf,*outf;	
	char FileInput[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char FileSpect[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX\\LSP.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	char** ptr=new char* [5000];
	UINT  Count_str=0;
	//UINT  Count_err=0;
	//double dW,dB,dV,dR;
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
 //      return;
	}
#define LLEN 100000
	 char INLIN[LLEN];
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(INLIN,LLEN,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (INLIN[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	 fseek(inpf, 0L, SEEK_SET);
	 char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	 for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
			memset(INLIN,'\0',LLEN);
   			if ( fgets(INLIN,LLEN,inpf)==0 ) break;
			if (INLIN[0]=='*'){ continue;
			}else{
    			data[i] = new char[LLEN];  // STEP 2: SET UP THE COLUMNS
 				memset(data[i],'\0',LLEN);
	   			data[i][0]='\0';
				int DST=strlen(INLIN);
    			memcpy(data[i],INLIN,DST);//strlen(INLIN)-1
				i++;
			}// 	
	   }
	   catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
		  // YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
		  //Application->MessageBox("Could not allocate. Bye ...",
			   //"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
	   }
	}
	fclose(inpf);
	UINT  Count_strSp=0;

	if (fopen_s(&inpf, FileSpect, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Out File"));
 //      return;
	}
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_strSp++;
			}
		}
	}
	fseek(inpf, 0L, SEEK_SET);
	//DocDataType* DATA=new DocDataType [Count_strSp];
	double *LamSt=new double [Count_strSp];
	if (inpf) {
		int i=0;
		while (!feof(inpf)){
	
			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*') continue;
			else {
				//if(StrPtr(InLin,'\t',ptr)!=0){
				sscanf_s(InLin, "%lf",&LamSt[i]);
				i++;
			}
		}
	}
	char NameFale[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX NORM\\";
	char namefale[]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	char NF[100];
	memset(NF,0,100);
double *Flux=new double[Count_strSp];
	for(UINT i=0;i<Count_str;i++){//Count_str
		UINT NUMSP=StrPtr(data[i],'|',ptr);
		memset(NF,0,100);
		memset(namefale,0,13);
		//int ST=strlen(ptr[0]);
		memcpy((void*)namefale,(void*)ptr[0],10);//strlen(data[i])
		memcpy(NF,NameFale,strlen(NameFale));
		strcat(NF,namefale);
		strcat(NF,".txt");
		//NF=_T(NameFale+namefale);
		//if(NUMSP!=0){
			for(UINT j=0;j<Count_strSp;j++){	//Count_strSp NUMSP-1
				sscanf_s(ptr[j+1], "%lf",&Flux[j]);//%lf
			}
		//}
		if (fopen_s(&outf, NF, "w") !=0 ){
			AfxMessageBox(_T("Error Opening Out File"));
	       return;
		}
		double FL5550=GetFlux(555.0,LamSt,Flux,Count_strSp);
		for(UINT j=0;j<Count_strSp;j++){
			fprintf(outf,"%6.1lf  %9.4lf\n",LamSt[j],Flux[j]/FL5550);
			//if(i==4681)
			//	i=i;
		}
		fclose(outf);
	}
_fcloseall;
}
#endif
#ifdef EMOE3
void CMyTeMath::TESTING()
{//Для вывода export- файла | в виде форматированного .txt
	FILE *inpf,*outf,*errf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3Double_2Cod.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\BC_090215_3DBL_2Cod.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char FileInput[]="D:\\MOSH\\SOURCE\\Redness coefficients.txt";//ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BOARD_CATALOG4_090215_3_SUM_GI_.txt";//SOURCE\\КАТАЛОГ  ГЕОФИЗИКЕ_3.txt";//ИСХОДНЫЙ КАТАЛОГ  ГЕОФИЗИКЕ.txt";//EXCESS.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\SOURCE\\Redness coefficients_End.txt";//ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BOARD_CATALOG4_090215_3_SUM_GI.txt";//SOURCE\\КАТАЛОГ  ГЕОФИЗИКЕ 3.txt";//ИСХОДНЫЙ КАТАЛОГ  ГЕОФИЗИКЕ_.txt";//EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\SOURCE\\Redness coefficients_err.txt";//ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\E_BOARD_CATALOG4_090215_3_SUM_E.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 46
	// ВВод файла исходных величин
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	for(UINT i=0;i<Count_str;i++){
		UINT NOut=StrPtr(Data0[i],'|',ptr);
		if(NOut!=NOUT0) fprintf(errf,"%6s|",ptr[0]);

		for(UINT j=0;j<NOut-1;j++){
			char buff[2048];
			int N=ptr[j+1]-ptr[j]-1;
			memcpy(buff,ptr[j],N);
			buff[N]=0;
			//sscanf_s(ptr[0], "%s",buff);

			fprintf(outf,"%6s|",buff);
		}
		fputs("\n",outf);
	}
	fclose(outf);
	_fcloseall();

	//}
}
#endif
#ifdef EMOE
void CMyTeMath::TESTING()
{//Для вычисления поглощений для вычисления избытков цвета
	FILE *inpf,*outf,*errf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3Double_2Cod.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\BC_090215_3DBL_2Cod.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char FileInput[]="D:\\MOSH\\SOURCE\\EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 31
	//typedef struct Date {
	//	//bool FlagYes;
	//	char Name[14];
	//	double Ri[MaxBand];
	//	double RO[MaxBand];
	//} DSP;
	// ВВод файла исходных величин
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	UINT NOut=StrPtr(Data0[0],'|',ptr);
	double *DA =new double [NOut];//Count_str
	double *DA1=new double [NOut];//Count_str
	double A,A1;
	/*for (UINT i=0,j=0;i<Count_str;i++)
	{
		DA[i]=new double [NOut];
	}*/
	//DA[i]=new double [NOut];
	fprintf(outf,"File_Name     |          R|		AR|          B|       AB|        Bjb|      ABjb|         Bp|        ABp|         Bt|        ABt|         GI|        AGI|\
		GII|       AGII|       GIII|		AGIII|        GIV|       AGIV|          H|         AH|        HJB|       AHJB|         Hp|        AHp|         Ic|        AIc|        Ijb|       AIjb|\
		J|         AJ|        JJB|       AJJB|          K|         AK|        KJB|       AKJB|         MT|        AMT|          R|         AR|         Rc|        ARc|        Rjb|       ARjb|\
		Ujb|       AUjb|          V|        AV|         Vc|         AVc|        Vjb|      AVjb|         Vt|         AVt|          W|         AW\n");
	for(UINT i=1;i<Count_str;i+=2){
		StrPtr(Data0[i],'|',ptr);
		StrPtr(Data0[i+1],'|',ptr1);
		memcpy(buff,ptr[0],14);
//		if(NOut!=NOUT0) 
		fprintf(outf,"%14s|",buff);
		for (UINT j=2;j<NOut;j++)
		{
			sscanf_s(ptr[j], "%lf",&DA[j]);
			sscanf_s(ptr1[j], "%lf",&DA1[j]);
			double d=DA1[j]-DA[j];
			fprintf(outf,"%11.4lf|%11.4lf|",DA[j],d);
		}
		fputs("\n",outf);
	}			
			//
			//for(UINT j=0;j<NOut-1;j++){
			//char buff[2048];
			//int N=ptr[j+1]-ptr[j]-1;
			//memcpy(buff,ptr[j],N);
			//buff[N]=0;
			//sscanf_s(ptr[0], "%s",buff);

		//	fprintf(outf,"%20s|",buff);
		//}
		//fputs("\n",outf);
	//}
	fclose(outf);
	_fcloseall();

	//}
}
#endif
#ifdef ETWOE1
//Вычисление величин из файла Пиклса с модельными спектрами по TYC-2MASS
void CMyTeMath::TESTING()
{
	FILE *inpf,*outf,*errf,*spcf;	
	char FileInput[]="D:\\MOSH\\SOURCE\\TABLE15-pcls_S.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char SpFile[]="D:\\MOSH\\SOURCE\\SP_PCLS_GEO.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\T15_PCLS_GEO.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char** ptr=new char* [5000];
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	//char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	//InpData(Data0,inpf, Count_str, NULL); 
	//fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&spcf, SpFile, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	UINT Count_str_Sp=CountDataFile(spcf);
	char** Data0 = new char*[Count_str_Sp];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,spcf, Count_str_Sp, NULL); 
	fclose(spcf);
	double 	V=0;
	double 	Vt=0;
	double 	eVt=0;
	//double 	Vc=0;
	double 	W_B=0;
	double 	B_V=0;
	//double 	B_Vh=0;
	//double 	eB_Vh=0;
	double  V_R=0;
	//double  V_Ih=0;
	//double  eV_Ih=0;
	//double  Vc_Rc=0;
	//double  Rc_Ic=0;
	//double  Vc_Ic=0;
	//double 	Bt=0;		
	//double 	eBt=0;		
/*	double 	J=0;		
	double 	eJ=0;		
	double 	H=0;		
	double 	eH=0;		
	double 	K=0;
	double  eK=0;
	*///double 	Hp=0;		
	//double 	eHp=0;		
	//UINT    pp;
	typedef struct Date {
		//bool FlagYes;
		char NameSp[16];
		double GI;
		double GII;
		double GIII;
		double GIV;
		double Vt;
	} DSP;
	DSP *PCLSp=new DSP[Count_str_Sp];
	//char FNameSp[7];
// номера колонок для файла SP_PCLS_GEO.txt; 
	for (UINT k=1;k<Count_str_Sp;k++){
		StrPtr(Data0[k],'|',ptr);
		memcpy(PCLSp[k].NameSp,ptr[0],6);					
		PCLSp[k].NameSp[6]=0;
		sscanf_s(ptr[6],"%lf",&PCLSp[k].Vt);
		sscanf_s(ptr[7],"%lf",&PCLSp[k].GI);
		sscanf_s(ptr[8],"%lf",&PCLSp[k].GII);
		sscanf_s(ptr[9],"%lf",&PCLSp[k].GIII);
		sscanf_s(ptr[10],"%lf",&PCLSp[k].GIV);
	}

	fgets(InLin,len,inpf);// Съели ЗАГОЛОВОК
	//fprintf(outf,"%s",InLin);//ЗАГОЛОВОК => outf
	UINT NColumn=	StrPtr(InLin,'|',ptr);//подсчёт колонок в исходном файле

	for (UINT i=1;i<Count_str;i++){ // начало цикла по файлy
		fgets(InLin,len,inpf);		//съели строку ПОСЛЕ ЗАГОЛОВКА
		//if(InLin[0]=='*') continue;
		StrPtr(InLin,'|',ptr);//StrPtr(Data0[i],'|',ptr);
		char NameSp[7];
		memcpy(NameSp,ptr[17],6);					
		NameSp[6]=0;
	//fprintf(outf,"%s|",PCLSp[k].NameSp);
	//memcpy(Qflag,ptr[40],4);
	//Qflag[3]=0;
	//ВВОД и печать TYC-2MASS
// номера колонок для файла TABLE15-pcls.txt; 
		double 	GI=0;
		double 	GII=0;
		double 	GIII=0;
		double 	GIV=0;
		sscanf_s(ptr[6],"%lf",&Vt);
		for (UINT k=1;k<Count_str_Sp;k++)	{
			if(strncmp(NameSp,PCLSp[k].NameSp,6)==0){
				GI=Vt+(PCLSp[k].GI-PCLSp[k].Vt);
				GII=Vt+(PCLSp[k].GII-PCLSp[k].Vt);
				GIII=Vt+(PCLSp[k].GIII-PCLSp[k].Vt);
				GIV=Vt+(PCLSp[k].GIV-PCLSp[k].Vt);
				break;
			}
		}
		GI=GI;
		fprintf(outf,"%8d|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",i,GI,GII,GIII,GIV);//  
	}
	fclose(outf);
	_fcloseall();
}
#endif
#ifdef ETWOE3
//Формирование файла с искусственным покраснением
void CMyTeMath::TESTING()
{
	FILE *inpf,*outf,*errf,*spcf;	
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\W_UIc_SS.txt";//DATASHIT\\BOARD_GEO_Hp_W_TYC_JHK_HM.txt";//BOARD_WORK_RED_0S1_W_Hp_HM.txt";//BOARD_WORK_RED_0S1_VcRcIc.txt";//BOARD_WORK_RED_0S1_W_Hp_HM.txt";//2.txt";//EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\GEO_W_VcRcIc_TYC_2MASS_Hp_mag.txt";//GEO_W_VcRcIc_FQ.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\SOURCE\\GI_FQ_Hp_.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char SpFile[]="D:\\MOSH\\SOURCE\\EXCESS_Awjbctjhk.txt";//LIST_SP_RED.txt";//RED_FAQTOR.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
	typedef struct Date {
		//bool FlagYes;
		char Name[16];
		char SpCod[4];
		double RedX;
		double 	AV;
		double 	AB;
		double 	AW;
		double  AR;
		double 	ABt;		
		double 	AVt;		
		double 	AVh;
		double 	ABh;
		//double 	AVh;
		double 	AHp;
		double 	AIh;
		double 	AVc;
		double 	AIc;
		double 	ARc;
		double 	AJ;
		double 	AH;
		double 	AK;
		//double 	AUp;
		double  AGI;
		double  AGII;
		double  AGIII;
		double  AGIV;
	} DSP;
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);
	if (fopen_s(&spcf, SpFile, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str_Sp=CountDataFile(spcf);
	char** Data1 = new char*[Count_str_Sp];        // STEP 1: SET UP THE ROWS.
	InpData(Data1,spcf, Count_str_Sp, NULL); 
	fclose(spcf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	char Name[17];
	UINT NColumn=	StrPtr(Data0[0],'|',ptr);
	//for (UINT i=1;i<Count_str;i++){ // начало цикла по 
	//	//double *DDA = new double[NColumn];
	//	StrPtr(Data0[i],'|',ptr);
	//	memcpy(Name,ptr[2],16);	//TYC
	//	Name[16]=0;
	//Номера для	BOARD_Hp_W_TYC_JHK_HM.txt		BOARD_WORK_RED_0S1_W_VcRcIc.txt
	double 	V=0;
	double 	W_B=0;
	double 	B_V=0;
	double  V_R=0;
	double 	Bt=0;		
	double 	Vt=0;		
	double 	Vt_J=0;		
	double 	Bt_Vt=0;
	double 	J=0;
	double 	H=0;
	double 	K=0;
	double 	J_H=0;		
	double 	H_K=0;		
	double 	J_K=0;
	double 	Vh=0;
	double 	B_Vh=0;
	double 	V_Ih=0;
	double 	Hp=0;
	double 	Hp_Ih=0;
	double 	Vc=0;
	double 	Bc_Vc=0;
	double 	Vc_Rc=0;
	double 	Vc_Ic=0;
	double 	Rc_Ic=0;
	double 	Rc=0;
	double 	Vjb=0;
	double 	U_Bjb=0;
	double 	B_Vjb=0;
	//double 	Vc_Rc=0;
	double  GI=0;
	double  GII=0;
	double  GIII=0;
	double  GIV=0;
	//double 	R_Ibs=0;GI,GII,GIII,GIV,V,W,B,R,Bt,Vt,J,H,K,Vh,Bh,Ih,Hp
	fprintf(outf,"    pp|             TYC|SpC| RAND|      GI|     GII|    GIII|     GIV|       W|       B|       V|       R|      Bt|      Vt|       J|       H|       K|      Bh|      Vh|      Ih|      Hp|      Vc|      Rc|      Ic|\n");
	fprintf(outf,"pp|TYC|SpC|RAND|GI|GII|GIII|GIV|W|B|V|R|Bt|Vt|J|H|K|Vh|Bh|Ih|Hp|Vc|Rc|Ic\n");
//		fprintf(outf,"pp|TYC|SpC|RAND|W_B|B_V|V_R|B_R|Bt_Vt|Vt_J|J_H|H_K|J_K|B_Vh|V_Ih|Hp_Ih\n");
	for (UINT i=1;i<Count_str;i++){ // начало цикла по 
		StrPtr(Data0[i],'|',ptr);
		memcpy(Name,ptr[4],16);	//TYC
		Name[16]=0;
		char SpCod[4];
		memcpy(	SpCod,ptr[1],3);
		SpCod[3]=0;
		if(SpCod[2]==' ') SpCod[2]='3';
		//Номера для	W_UIc_SS.txt		BOARD_Hp_W_TYC_JHK_HM.txt		BOARD_WORK_RED_0S1_W_VcRcIc.txt
		if(sscanf_s(ptr[13],"%lf",&V)==NULL) {
			AfxMessageBox(_T("Error Reaning V")); 
			break;
		}
		sscanf_s(ptr[14],"%lf",&W_B);
		sscanf_s(ptr[15],"%lf",&B_V);
		sscanf_s(ptr[16],"%lf",&V_R);
		sscanf_s(ptr[6],"%lf",&Bt);
		sscanf_s(ptr[7],"%lf",&Vt);
		//sscanf_s(ptr[19],"%lf",&Bt_Vt);
		sscanf_s(ptr[38],"%lf",&Vt_J);
		sscanf_s(ptr[33],"%lf",&J);
		sscanf_s(ptr[34],"%lf",&H);
		sscanf_s(ptr[35],"%lf",&K);
		sscanf_s(ptr[8],"%lf",&Vh);
		sscanf_s(ptr[9],"%lf",&B_Vh);
		sscanf_s(ptr[10],"%lf",&V_Ih);
		sscanf_s(ptr[11],"%lf",&Hp);
		//sscanf_s(ptr[9],"%lf",&Hp_Ih);
		sscanf_s(ptr[41],"%lf",&GI);
		sscanf_s(ptr[42],"%lf",&GII);
		sscanf_s(ptr[40],"%lf",&GIII);
		sscanf_s(ptr[39],"%lf",&GIV);
		sscanf_s(ptr[24],"%lf",&Vc);
		//sscanf_s(ptr[33],"%lf",&Bc_Vc);
		sscanf_s(ptr[27],"%lf",&Vc_Rc);
		sscanf_s(ptr[28],"%lf",&Rc_Ic);
		sscanf_s(ptr[29],"%lf",&Vc_Ic);
		sscanf_s(ptr[30],"%lf",&Rc);
		double B=B_V+V;
		double W=W_B+B;
		double R=V-V_R;
		double Bh=Vh+B_Vh;
		double Ih=Vh-V_Ih;
		double J=Vt-Vt_J;
		//double H=J-J_H;
		//double K=H-H_K;
		double Ic=Vc-Vc_Ic;
//Выдача исходных данных
		fprintf(outf,"%6d|%s|%s|0.000|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",i,Name,SpCod,GI,GII,GIII,GIV,W,B,V,R,Bt,Vt,J,H,K,Bh,Vh,Ih,Hp,Vc,Rc,Ic);//%8.4lf|%8.4lf|Vc_Rc,Rc_Ic,  Vc,Rc,Ic
		//fprintf(outf,"%6d|%s|0.000|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",i,Name,W_B,B_V,V_R,B_R,R,Vc,Vc_Rc,Rc_Ic,Vc_Ic,Rc);//%8.4lf|%8.4lf|Vc_Rc,Rc_Ic,
		}	

		//fprintf(outf,"\n");
		//fprintf(outf,"    pp|             TYC|0.000|     W_B|     B_V|     V_R|     B_R|   Bt_Vt|    Vt_J|     J_H|     H_K|     J_K|    B_Vh|    V_Ih|   Hp_Ih\n");
		//fprintf(outf,"pp|TYC|0.000|W_B|B_V|V_R|B_R|Bt_Vt|Vt_J|J_H|H_K|J_K|B_Vh|V_Ih|Hp_Ih\n");
		//fprintf(outf,"      TYC       |  RA_2000   |  DE_2000   |   pmRA|   pmDE|5 Bt  |e_Bt |7 Vt  |e_Vt |   HD_corr|Sp|        2Mass_id |12J   | e_J |14H   | e_H |16K   | e_K |Qf |Cf|Xf|Af|  HD_wbvr |23V   |24W_B |25B_V |26V_R | Sp_wbvr  |  UCAU4   | Ucac4_id| fmag | a_mag|err  |\n");
		//fprintf(outf,"    pp|             TYC| RAND|     W_B|     B_V|     V_R|     B_R|       R|      Vc|   Vc_Rc|   Rc_Ic|   Vc_Ic|\n");
		//fprintf(outf,"pp|TYC|RAND|W_B|B_V|V_R|B_R|R|Vc|Vc_Rc|Rc_Ic|Vc_Ic\n");
	DSP *FQ=new DSP[Count_str_Sp];
	for(UINT k=0;k<Count_str_Sp;k++){
		StrPtr(Data1[k],'|',ptr);
		memcpy(	FQ[k].SpCod,ptr[1],3);
		FQ[k].SpCod[3]=0;
		sscanf_s(ptr[2],"%lf",&FQ[k].RedX);
		sscanf_s(ptr[10],"%lf",&FQ[k].AV);
		sscanf_s(ptr[8],"%lf",&FQ[k].AW);
		sscanf_s(ptr[9],"%lf",&FQ[k].AB);
		sscanf_s(ptr[11],"%lf",&FQ[k].AR);
		sscanf_s(ptr[21],"%lf",&FQ[k].ABt);
		sscanf_s(ptr[22],"%lf",&FQ[k].AVt);
		sscanf_s(ptr[23],"%lf",&FQ[k].AJ);
		sscanf_s(ptr[24],"%lf",&FQ[k].AH);
		sscanf_s(ptr[25],"%lf",&FQ[k].AK);
		sscanf_s(ptr[15],"%lf",&FQ[k].AVh);
		sscanf_s(ptr[14],"%lf",&FQ[k].ABh);
		sscanf_s(ptr[17],"%lf",&FQ[k].AIh);
		sscanf_s(ptr[26],"%lf",&FQ[k].AHp);
		sscanf_s(ptr[18],"%lf",&FQ[k].AVc);
		sscanf_s(ptr[19],"%lf",&FQ[k].ARc);
		sscanf_s(ptr[20],"%lf",&FQ[k].AIc);
		sscanf_s(ptr[4],"%lf",&FQ[k].AGI);
		sscanf_s(ptr[5],"%lf",&FQ[k].AGII);
		sscanf_s(ptr[6],"%lf",&FQ[k].AGIII);
		sscanf_s(ptr[7],"%lf",&FQ[k].AGIV);
	}

	for (UINT i=1;i<Count_str;i++){ // начало цикла по Redding
		//double *DDA = new double[NColumn];
		StrPtr(Data0[i],'|',ptr);
		memcpy(Name,ptr[4],16);//TYC
		Name[16]=0;
		//memcpy(Qfl,ptr[45],3);//TYC
		//Qfl[3]=0;
		char SPCOD[4];
		memcpy(SPCOD,ptr[1],3);
		SPCOD[3]=0;
		if(SPCOD[2]==' ') SPCOD[2]='3';
		//fprintf(outf,"%6d|%s|%s|",i,Name,SPCOD);
		if(sscanf_s(ptr[13],"%lf",&V)==NULL) {
			AfxMessageBox(_T("Error Reaning V")); 
			break;
		}
		sscanf_s(ptr[14],"%lf",&W_B);
		sscanf_s(ptr[15],"%lf",&B_V);
		sscanf_s(ptr[16],"%lf",&V_R);
		sscanf_s(ptr[6],"%lf",&Bt);
		sscanf_s(ptr[7],"%lf",&Vt);
		//sscanf_s(ptr[19],"%lf",&Bt_Vt);
		sscanf_s(ptr[38],"%lf",&Vt_J);
		sscanf_s(ptr[33],"%lf",&J);
		sscanf_s(ptr[34],"%lf",&H);
		sscanf_s(ptr[35],"%lf",&K);
		sscanf_s(ptr[8],"%lf",&Vh);
		sscanf_s(ptr[9],"%lf",&B_Vh);
		sscanf_s(ptr[10],"%lf",&V_Ih);
		sscanf_s(ptr[11],"%lf",&Hp);
		//sscanf_s(ptr[9],"%lf",&Hp_Ih);
		sscanf_s(ptr[41],"%lf",&GI);
		sscanf_s(ptr[42],"%lf",&GII);
		sscanf_s(ptr[40],"%lf",&GIII);
		sscanf_s(ptr[39],"%lf",&GIV);
		sscanf_s(ptr[24],"%lf",&Vc);
		//sscanf_s(ptr[33],"%lf",&Bc_Vc);
		sscanf_s(ptr[27],"%lf",&Vc_Rc);
		sscanf_s(ptr[28],"%lf",&Rc_Ic);
		sscanf_s(ptr[29],"%lf",&Vc_Ic);
		sscanf_s(ptr[30],"%lf",&Rc);
		double B=B_V+V;
		double W=W_B+B;
		double R=V-V_R;
		double Bh=Vh+B_Vh;
		double Ih=Vh-V_Ih;
		double J=Vt-Vt_J;
		//double H=J-J_H;
		//double K=H-H_K;
		double Ic=Vc-Vc_Ic;

		double 	RN=0;
		//Выдача красных данных
		bool FlYes=false;
		for(UINT r=0;r<Count_str_Sp;r++){
			if(strncmp(FQ[r].SpCod,SPCOD,3)==0 ){
				RN=FQ[r].RedX;	// /2
				FlYes=true;
				break;
			}else {
				FlYes=false;
				continue;
			}
		}
		if(FlYes)
			RN*=RANDisex()*0.3;
		else RN=0;		//double a=W_B+RN*(0.74016+0.02414*B_R+0.018725*pow(B_R,2)-0.00426*pow(B_R,3));
		//fprintf(outf,"%5.3lf|",RN);
		for(UINT r=0;r<Count_str_Sp;r++){
			if(strncmp(FQ[r].SpCod,SPCOD,3)==0 ){
				GI+=RN*FQ[r].AGI;
				GII+=RN*FQ[r].AGII;
				GIII+=RN*FQ[r].AGIII;
				GIV+=RN*FQ[r].AGIV;
				W+=RN*FQ[r].AW;B+=RN*FQ[r].AB;V+=RN*FQ[r].AV;R+=RN*FQ[r].AR;
				Bt+=RN*FQ[r].ABt;Vt+=RN*FQ[r].AVt;J+=RN*FQ[r].AJ;H+=RN*FQ[r].AH;K+=RN*FQ[r].AK;
				Bh+=RN*FQ[r].ABh;Vh+=RN*FQ[r].AVh;Ih+=RN*FQ[r].AIh;Hp+=RN*FQ[r].AHp;
				Vc+=RN*FQ[r].AVc;Rc+=RN*FQ[r].ARc;Ic+=RN*FQ[r].AIc;
				FlYes=true;
				break;
			}else {
				FlYes=false;
				continue;
			}
		}
		fprintf(outf,"%6d|%s|%s|%5.3lf|",i,Name,SPCOD,RN);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|",GI,GII,GIII,GIV);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|",W,B,V,R);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|",Bt,Vt,J,H,K);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|",Bh,Vh,Ih,Hp);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|\n",Vc,Rc,Ic);
	}
	fclose(outf);
		_fcloseall();
}
//#ifdef ETWOE3
void CMyTeMath::TESTING()
{//Для вычисления покраснённых цветов по модельным избыткам цвета 
	FILE *inpf,*outf,*errf,*spcf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3Double_2Cod.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\BC_090215_3DBL_2Cod.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BOARD_Hp_W_TYC_JHK_HM.txt";//BOARD_WORK_RED_0S1_W_Hp_HM.txt";//BOARD_WORK_RED_0S1_VcRcIc.txt";//BOARD_WORK_RED_0S1_W_Hp_HM.txt";//2.txt";//EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\GEO_W_TYC_2MASS_mag.txt";//GEO_W_VcRcIc_FQ.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\SOURCE\\GI_FQ_Hp_.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char SpFile[]="D:\\MOSH\\SOURCE\\LIST_SP_RED.txt";//RED_FAQTOR.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\reportNOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 31
	typedef struct Date {
		//bool FlagYes;
		char Name[10];
		char SpCod[4];
		double RedX;
	} DSP;
	// ВВод файла исходных величин
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);
	if (fopen_s(&spcf, SpFile, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str_Sp=CountDataFile(spcf);
	char** Data1 = new char*[Count_str_Sp];        // STEP 1: SET UP THE ROWS.
	InpData(Data1,spcf, Count_str_Sp, NULL); 
	fclose(spcf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
UINT NColumn=	StrPtr(Data0[0],'|',ptr);
//double *DDA = new double[NColumn];
//for (UINT )
//{
//}
fprintf(outf,"    pp|             TYC| RAND|     W_B|     B_V|     V_R|     B_R|       R|   Bt_Vt|      Vt|    Vt_J|     J_H|     H_K|     J_K|    B_Vh|    V_Ih|   Hp_Ih|      Hp|\n");
fprintf(outf,"pp|TYC|RAND|W_B|B_V|V_R|B_R|Bt_Vt|Vt_J|J_H|H_K|J_K|B_Vh|V_Ih|Hp_Ih\n");
//fprintf(outf,"    pp|             TYC| RAND|     W_B|     B_V|     V_R|     B_R|       R|      Vc|   Vc_Rc|   Rc_Ic|   Vc_Ic|\n");
//fprintf(outf,"pp|TYC|RAND|W_B|B_V|V_R|B_R|R|Vc|Vc_Rc|Rc_Ic|Vc_Ic\n");
char Name[17];
char Qfl[4];
	double 	V=0;
	double 	W_B=0;
	double 	B_V=0;
	double  V_R=0;
	double 	Bt=0;		
	double 	Vt=0;		
	double 	Vt_J=0;		
	double 	Bt_Vt=0;
	double 	J_H=0;		
	double 	H_K=0;		
	double 	J_K=0;
	double 	Vh=0;
	double 	B_Vh=0;
	double 	V_Ih=0;
	double 	Hp=0;
	double 	Hp_Ih=0;
	//double 	Vc=0;
	//double 	Bc_Vc=0;
	//double 	Vc_Rc=0;
	//double 	Vc_Ic=0;
	//double 	Rc_Ic=0;
	//double 	Rc=0;
	//double 	Vds=0;
	//double 	U_Bbs=0;
	//double 	B_Vbs=0;
	//double 	Vc_Rc=0;
	//double 	R_Ibs=0;


	for (UINT i=1;i<Count_str;i++){ // начало цикла по 
		//double *DDA = new double[NColumn];
		StrPtr(Data0[i],'|',ptr);
		memcpy(Name,ptr[2],16);	//TYC
		Name[16]=0;
		//memcpy(Qfl,ptr[45],3);//TYC
		//Qfl[3]=0;
			//fprintf(outf,"%s|",Name);
		//double 	Bt=0;
		if(i==10)
			i=i;
//Номера для	BOARD_Hp_W_TYC_JHK_HM.txt		BOARD_WORK_RED_0S1_W_VcRcIc.txt
	if(sscanf_s(ptr[11],"%lf",&V)==NULL) {
		AfxMessageBox(_T("Error Reaning V")); 
		break;
	}
	sscanf_s(ptr[12],"%lf",&W_B);
	sscanf_s(ptr[13],"%lf",&B_V);
	sscanf_s(ptr[14],"%lf",&V_R);
	sscanf_s(ptr[17],"%lf",&Bt);
	sscanf_s(ptr[18],"%lf",&Vt);
	sscanf_s(ptr[19],"%lf",&Bt_Vt);
	sscanf_s(ptr[24],"%lf",&Vt_J);
	sscanf_s(ptr[21],"%lf",&J_H);
	sscanf_s(ptr[22],"%lf",&H_K);
	sscanf_s(ptr[23],"%lf",&J_K);
	sscanf_s(ptr[5],"%lf",&Vh);
	sscanf_s(ptr[6],"%lf",&B_Vh);
	sscanf_s(ptr[7],"%lf",&V_Ih);
	sscanf_s(ptr[8],"%lf",&Hp);
	sscanf_s(ptr[9],"%lf",&Hp_Ih);
	//sscanf_s(ptr[13],"%lf",&Vc);
	//sscanf_s(ptr[33],"%lf",&Bc_Vc);
	//sscanf_s(ptr[16],"%lf",&Vc_Rc);
	//sscanf_s(ptr[17],"%lf",&Rc_Ic);
	//sscanf_s(ptr[18],"%lf",&Vc_Ic);
	//sscanf_s(ptr[19],"%lf",&Rc);
	
	//for (UINT j=0;j<NColumn;j++){
	//	//switch (j)
	//	//{
	//double 	Bt_Vt=Bt-Vt; //t_Vt
	//double 	J_H=J-H;
	//double 	J_K=J-K;
	//double 	H_K=H-K;
	//double 	Vt_J=Vt-J;//pp|TYC|0.000|W_B|B_V|V_R|B_R|Bt_Vt|Vt_J|J_H|H_K|J_K|B_Vh|V_Ih|Hp_Ih\n
	double 	B_R=B_V+V_R; 
	double  R=V-V_R;
	fprintf(outf,"%6d|%s|0.000|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",i,Name,W_B,B_V,V_R,B_R,R,Bt_Vt,Vt,Vt_J,J_H,H_K,J_K,B_Vh,V_Ih,Hp_Ih,Hp);//%8.4lf|%8.4lf|Vc_Rc,Rc_Ic,  Vc_Rc,Rc_Ic,Vc_Ic,Rc
	//fprintf(outf,"%6d|%s|0.000|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",i,Name,W_B,B_V,V_R,B_R,R,Vc,Vc_Rc,Rc_Ic,Vc_Ic,Rc);//%8.4lf|%8.4lf|Vc_Rc,Rc_Ic,
	}	//Выдача исходных данных

	//fprintf(outf,"\n");
	//fprintf(outf,"    pp|             TYC|0.000|     W_B|     B_V|     V_R|     B_R|   Bt_Vt|    Vt_J|     J_H|     H_K|     J_K|    B_Vh|    V_Ih|   Hp_Ih\n");
	//fprintf(outf,"pp|TYC|0.000|W_B|B_V|V_R|B_R|Bt_Vt|Vt_J|J_H|H_K|J_K|B_Vh|V_Ih|Hp_Ih\n");
	//fprintf(outf,"      TYC       |  RA_2000   |  DE_2000   |   pmRA|   pmDE|5 Bt  |e_Bt |7 Vt  |e_Vt |   HD_corr|Sp|        2Mass_id |12J   | e_J |14H   | e_H |16K   | e_K |Qf |Cf|Xf|Af|  HD_wbvr |23V   |24W_B |25B_V |26V_R | Sp_wbvr  |  UCAU4   | Ucac4_id| fmag | a_mag|err  |\n");
	//fprintf(outf,"    pp|             TYC| RAND|     W_B|     B_V|     V_R|     B_R|       R|      Vc|   Vc_Rc|   Rc_Ic|   Vc_Ic|\n");
	//fprintf(outf,"pp|TYC|RAND|W_B|B_V|V_R|B_R|R|Vc|Vc_Rc|Rc_Ic|Vc_Ic\n");
	DSP *FQ=new DSP[Count_str_Sp];
	for(UINT k=0;k<Count_str_Sp;k++){
		StrPtr(Data1[k],'|',ptr);
		memcpy(	FQ[k].SpCod,ptr[1],3);
		FQ[k].SpCod[3]=0;
		sscanf_s(ptr[2],"%lf",&FQ[k].RedX);
	}

	for (UINT i=1;i<Count_str;i++){ // начало цикла по Redding
		//double *DDA = new double[NColumn];
		StrPtr(Data0[i],'|',ptr);
		memcpy(Name,ptr[2],16);//TYC
		Name[16]=0;
		//memcpy(Qfl,ptr[45],3);//TYC
		//Qfl[3]=0;
		char SPCOD[4];
		memcpy(SPCOD,ptr[1],3);
		SPCOD[3]=0;
		if(SPCOD[2]==' ') SPCOD[2]='3';
		fprintf(outf,"%6d|%s|",i,Name);
		//double 	Bt=0;		 

		//if(sscanf_s(ptr[6],"%lf",&V)==NULL) {
		//	AfxMessageBox(_T("Error Reaning V")); 
		//	break;
		//}
		//sscanf_s(ptr[7],"%lf",&W_B);
		//sscanf_s(ptr[8],"%lf",&B_V);
		//sscanf_s(ptr[9],"%lf",&V_R);
		//sscanf_s(ptr[10],"%lf",&Bt);
		//sscanf_s(ptr[11],"%lf",&Vt);
		//sscanf_s(ptr[39],"%lf",&J);
		//sscanf_s(ptr[41],"%lf",&H);
		//sscanf_s(ptr[43],"%lf",&K);
		//sscanf_s(ptr[12],"%lf",&Vh);
		//sscanf_s(ptr[13],"%lf",&B_Vh);
		//sscanf_s(ptr[14],"%lf",&V_Ih);
		//sscanf_s(ptr[15],"%lf",&Hp);
		//sscanf_s(ptr[16],"%lf",&Hp_Ih);
		//sscanf_s(ptr[13],"%lf",&Vc);
		////sscanf_s(ptr[33],"%lf",&Bc_Vc);
		//sscanf_s(ptr[16],"%lf",&Vc_Rc);
		//sscanf_s(ptr[17],"%lf",&Rc_Ic);
		//sscanf_s(ptr[18],"%lf",&Vc_Ic);
		//sscanf_s(ptr[19],"%lf",&Rc);
	
		if(sscanf_s(ptr[11],"%lf",&V)==NULL) {
			AfxMessageBox(_T("Error Reaning V")); 
			break;
		}
		sscanf_s(ptr[12],"%lf",&W_B);
		sscanf_s(ptr[13],"%lf",&B_V);
		sscanf_s(ptr[14],"%lf",&V_R);
		sscanf_s(ptr[17],"%lf",&Bt);
		sscanf_s(ptr[18],"%lf",&Vt);
		sscanf_s(ptr[19],"%lf",&Bt_Vt);
		sscanf_s(ptr[24],"%lf",&Vt_J);
		sscanf_s(ptr[21],"%lf",&J_H);
		sscanf_s(ptr[22],"%lf",&H_K);
		sscanf_s(ptr[23],"%lf",&J_K);
		sscanf_s(ptr[5],"%lf",&Vh);
		sscanf_s(ptr[6],"%lf",&B_Vh);
		sscanf_s(ptr[7],"%lf",&V_Ih);
		sscanf_s(ptr[8],"%lf",&Hp);
		sscanf_s(ptr[9],"%lf",&Hp_Ih);
	//for (UINT j=0;j<NColumn;j++){
	//	//switch (j)
	//	//{
	//double 	Bt_Vt=Bt-Vt; //t_Vt
	//double 	J_H=J-H;
	//double 	J_K=J-K;
	//double 	H_K=H-K;
	//double 	Vt_J=Vt-J;
	double 	B_R=B_V+V_R; 
	double  R=V-V_R;
		//fprintf(outf,"%s|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",Name,V,W_B,B_V,V_R,B_R,Vt,Bt_Vt,Vt_J,J_H,H_K,J_K)
	//}	
	double 	RN=0;
	//Выдача красных данных
	//UINT NCol_Sp=StrPtr(Data1[0],'|',ptr);
	//double *RedX=new double[Count_str_Sp];
	//char **SpCod=new char*[Count_str_Sp];
	//for(UINT r=0;r<Count_str_Sp;r++){
	//	StrPtr(Data1[0],'|',ptr);
	//	memcpy(SpCod[r],ptr[1],3);
	//	SpCod[r][3]=0;
	//	sscanf_s(ptr[2],"%lf",&RedX[r]);
	//}
	bool FlYes=false;
	for(UINT r=0;r<Count_str_Sp;r++){
		if(strncmp(FQ[r].SpCod,SPCOD,3)==0 ){
			RN=FQ[r].RedX;	// /2
			FlYes=true;
			break;
		}else continue;
	}
	if(FlYes)
		RN*=RANDisex()*0.3;
	else RN=RANDisex();		//double a=W_B+RN*(0.74016+0.02414*B_R+0.018725*pow(B_R,2)-0.00426*pow(B_R,3));
	fprintf(outf,"%5.3lf|",RN);	//RN|
//WBVR
	double a=0;
	if(B_R<=0.0) a=0.73-0.03*B_R;	//W_B
	else{
		if(B_R<=2.25) a=0.73+0.062*B_R;
		else a=1.05-0.08*B_R;
	}
	W_B+=RN*a;
//fscanf_s(inpf,"%lf",DDA[i][j]);
		//fprintf(outf," ); //W_B|

		if(B_R<=2.5) a=0.93-0.007*B_R;//V_R
		else a=(0.89+0.08*B_R);
		V_R+=RN*a;

		if(B_R<=1.0) a=1.965-0.025*B_R; //B_R
		else{
			if(B_R<=2.5) a=1.985-0.067*B_R;
			else a=1.5325+0.18*(B_R)	;
		}

//double a=B_V+R*(1.04-0.026*B_R)
		B_V+=RN*(1.04-0.026*B_R);
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|",W_B,B_V,V_R,B_R+RN*a,R); //W_B,B_V,V_R,B_R
//#ifdef CUZYA
////CUZinS		//Er_ic_1=0.72-0.03*Vc_Ic;Er_ic_2=0.748-0.051*Vc_Ic;
//		//Er_ic_1=0.722-0.017*B_R;Er_ic_2=0.81-0.054*B_R;
		if(B_R<2.5) a=0.72-0.016*B_R;
		else a=1.0-0.124*B_R;
		Rc_Ic+=RN*a;
		//Vc_Ic+=RN*a;
		//Ev_rc_1=0.595-0.012*B_R;Ev_rc_2=-0.01+0.22*B_R; //Erc_ic_1=0.72-0.016*B_R;Erc_ic_2=1.0-0.124*B_R;
		if(B_R<2.5) a=0.595-0.012*B_R;
		else a=-0.01+0.22*B_R;
		Vc_Rc+=RN*a;
		Vc_Ic=Vc_Rc-Rc_Ic;
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",Vc,Vc_Rc,Rc_Ic,Vc_Ic,Rc);	//Vc_Rc,Rc_Ic
//#endif
//TYC-2MASS
		if(B_R<0.0) a=1.035-0.035*B_R;	//Bt_Vt
		else{
			if(B_R<=1.0) a=1.035+0.01*B_R;
			else {
				if(B_R<=2.0)a=1.065-0.018*B_R;
				else a=1.03+0.008*B_R;
			}
			//if(Bt_Vt<=2) a=1.05-0.02*B_R;
		}
		Bt_Vt += RN*a;

		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|",Bt_Vt,Vt,Vt_J + RN*(2.645-0.041*B_R));


		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|",J_H+RN*(0.415-0.004*B_R),H_K+RN*0.22,J_K+RN*(0.628-0.005*B_R));//%s|%8.4lf|%8.4lf|
		//Hp    Eb_vh_1=1.01-0.043*B_R;Eb_vh_2=1.04-0.07*B_R;
		if(B_R<1.0) a=1.01-0.043*B_R;	//B_Vh
		else{
			if(B_R>=1.0 && B_R<2.5) a=1.04-0.07*B_R;
			else a=0.85;
		}
		B_Vh+=RN*a;
		//Ev_ih_1=1.391-0.025*B_R;Ev_ih_2=1.0+0.12*B_R;Ev_ih_3=1.392-0.012*B_R;
		if(B_R<0.0) a=1.392-0.012*B_R;	//V_Ih
		else{
			if(B_R>=0.0 && B_R<2.5) 1.391-0.025*B_R;
			else a=1.0+0.12*B_R;
		}
		V_Ih+=RN*a;
		//Hp_Ih Ehp_ih_1=1.539-0.196*B_R;Ehp_ih_2=1.54-0.186*B_R;
		if(B_R<1.5) a=1.539-0.196*B_R;
		else a=1.54-0.186*B_R;
		Hp_Ih+=RN*a;
		//if(Bt_Vt<=2) a=1.05-0.02*B_R;
		fprintf(outf,"%8.4lf|%8.4lf|%8.4lf|%8.4lf|\n",B_Vh,V_Ih,Hp_Ih,Hp);
//J_H

	} // КОНЕЦ ЦИКЛА ПО

	fclose(outf);
	_fcloseall();

	//}
}
#endif
#ifdef EMOE
void CMyTeMath::TESTING()
{//Для вычисления по полиномам для больших файлов 
	FILE *inpf,*outf,*errf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3Double_2Cod.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\BC_090215_3DBL_2Cod.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK.txt";//\\BOARD_CAT_m7.txt";//DATASHIT\\BC_13_WORK_2.txt";//EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_GI.txt";//BOARD_CAT_m7_W_TJHK.txt";//DATASHIT\\BC_13_WORK_2E.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_REV.txt";//\\BOARD_CAT_b6.75_REV.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 31
	//typedef struct Date {
	//	//bool FlagYes;
	//	char Name[14];
	//	double Ri[MaxBand];
	//	double RO[MaxBand];
	//} DSP;
	// ВВод файла исходных величин BCK7m+HYP+C+TYC+JHK.txt"; 
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	//char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	//InpData(Data0,inpf, Count_str, NULL); 
	//fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	//char* Data0[len];// = new char[Count_str];
//	fgets(InLin,len,inpf);// Съели ЗАГОЛОВОК
//	//fprintf(outf,"%s",InLin);
//	UINT NColumn=	StrPtr(InLin,'|',ptr);//подсчёт колонок в исходном файле
////						0		1		2		3		4		5	6		7		8	9		10
																																																													
//	fprintf(outf,"*    TYC        |    V   |    W_B |    B_V |   V_R  |   B_R  |     R  |    Vt  |  Bt_Vt |   Vt_J |   J    |     H  |   K    |Qf |   J_H  |  H_K   |  J_K   |\
//GIwbvrR|GIwbvrRmin|GIwbvrRmax|   GImVRIc1|GImVRIc1min|GImVRIc1max|     GIhmHp|  GIhmHpmin|  GIhmHpmax|  GItjhkVt|GItjhkVtmin|GItjhkVtmax|     pp|\n");
//	fprintf(outf,"*TYC|V|W_B|B_V|V_R|B_R|R|Vt|Bt_Vt|Vt_J|J|H|K|Qf|J_H|H_K|J_K|GIwbvrR|GIwbvrRmin|GIwbvrRmax|GImVRIc1|\
//				 GImVRIc1min|GImVRIc1max|GIhmHp|GIhmHpmin|GIhmHpmax|GItjhkmVt|GItjhkmVtmin|GItjhkmVtmax|GImtjhk|GImtjhkmin|GImtjhkmax|pp|\n");
//	fprintf(outf,"%s|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\
				 %s|%8.4lf|%8.4lf|%8.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%10.4lf|%7d|\n",\
				 Name,V,W_B,B_V,V_R,B_R,R,Vt,Bt_Vt,Vt_J,J,H,K,Qflag,J_H,H_K,J_K,GIwbvrR,GIwbvrRmin,GIwbvrRmax,GImVRIc1,\
				 GImVRIc1min,GImVRIc1max,GIhmHp,GIhmHpmin,GIhmHpmax,GItjhkmVt,GItjhkmVtmin,GItjhkmVtmax,GImtjhk,GImtjhkmin,GImtjhkmax,pp);

	char Name[17];
	char Qflag[4];
	//nan 	
	double 	V=0;
	double 	Vt=0;
	double 	eVt=0;
	double 	Vc=0;
	double 	W_B=0;
	double 	B_V=0;
	double 	B_Vh=0;
	double 	eB_Vh=0;
	double  V_R=0;
	double 	Vh=0;
	double  V_Ih=0;
	double  eV_Ih=0;
	double  Vc_Rc=0;
	double  Rc_Ic=0;
	double  Vc_Ic=0;
	double 	Bt=0;		
	double 	eBt=0;		
	double 	J=0;		
	double 	eJ=0;		
	double 	H=0;		
	double 	eH=0;		
	double 	K=0;
	double  eK=0;
	double 	Hp=0;		
	double 	eHp=0;		
	UINT    pp;
	fgets(InLin,len,inpf);// Съели ЗАГОЛОВОК
	//fprintf(outf,"%s",InLin);
	UINT NColumn=	StrPtr(InLin,'|',ptr);//подсчёт колонок в исходном файле
	//	напечатали ВЫХОДНОЙ ЗАГОЛОВОК
	fprintf(outf,"*    TYC        |     V  |   W_B |   B_V |  V_R  |  B_R  |    R  |      Vc|  Vc_Rc|  Rc_Ic|  Vc_Ic|     Rc|     Vh|   B_Vh|  eB_Vh|  V_Ih| eV_Ih|      Hp|     eHp|  Hp_Ih|      Bt|    eBt|     Vt|    eVt| Bt_Vt |  J    |     eJ|    H  |     eH|  K    |     eK|Qf |   Vt_J|   Vt_H|   Vt_K|    J_H|    H_K|    J_K|     GIwbvrR| GIwbvrRmin| GIwbvrRmax|    GImVRIc1|GImVRIc1min|GImVRIc1max|      GIhmHp|  GIhmHpmin|  GIhmHpmax|    GItjhkVt|GItjhkVtmin|GItjhkVtmax|     GImtjhk| GImtjhkmin| GImtjhkmax|     pp|\n");
	fprintf(outf,"*TYC|V|W_B|B_V|V_R|B_R|R|Vc|Vc_Rc|Rc_Ic|Vc_Ic|Rc|Vh|B_Vh|eB_Vh|V_Ih|eV_Ih|Hp|eHp|Hp_Ih|Bt|eBt|Vt|eVt|Bt_Vt|J|eJ|H|eH|K|eK|Qf |Vt_J|Vt_H|Vt_K|J_H|H_K|J_K|GIwbvrR|GIwbvrRmin|GIwbvrRmax|GImVRIc1|GImVRIc1min|GImVRIc1max|GIhmHp|GIhmHpmin|GIhmHpmax|GItjhkVt|GItjhkVtmin|GItjhkVtmax|GImtjhk|GImtjhkmin|GImtjhkmax|pp|\n");
	for (UINT i=1;i<Count_str;i++){ // начало цикла по 
		fgets(InLin,len,inpf);		//съели строку ПОСЛЕ ЗАГОЛОВКА
		//if(InLin[0]=='*') continue;
		StrPtr(InLin,'|',ptr);//StrPtr(Data0[i],'|',ptr);
		bool FlagQW[5]={true,true,true,true,true};  //V 14|  W_B |  B_V | V_R  
		bool FlagQc[4]={true,true,true,true};//Vc| Vc_Rc| Rc_Ic| Vc_Ic
		bool FlagQh[7]={true,true,true,true,true,true,true};//Vh|  B_Vh| eBVh|V_Ih|eV_I|26 Hp  | e_Hp
		bool FlagQt[10]={true,true,true,true,true,true,true,true,true,true};//
		//  Bt 7|e_Bt8|  Vt 9|e_Vt |J   | e_J |  H   | e_H |  K   | e_K |
		//for(int i=0;i<4;i++){
		//	FlagQ[i]=new bool [10];
		//	for (int j=0;j<NColumn;j++){
		//		FlagQ[i][j]=true;
		//	}
		//}
		memcpy(Name,ptr[1],16);					// номера колонок для файла BCK7m+HYP+C+TYC+JHK.txt"; 
		Name[16]=0;
		fprintf(outf,"%s|",Name);
		memcpy(Qflag,ptr[40],4);
		Qflag[3]=0;
//ВВОД и печать WBVR
		if(sscanf_s(ptr[14],"%lf",&V)==NULL) {
			FlagQW[0]=false;
			fprintf(outf,"       M|");
		}else{
			FlagQW[0]=true;
			fprintf(outf,"%8.3lf|",V);
		}
		if(sscanf_s(ptr[15],"%lf",&W_B)==NULL){
			FlagQW[1]=false;
			fprintf(outf,"      M|");
		}else{
			FlagQW[1]=true;
			fprintf(outf,"%7.3lf|",W_B);
		}

		if(sscanf_s(ptr[16],"%lf",&B_V)==NULL){
			FlagQW[2]=false;
			fprintf(outf,"      M|");
		}else{
			FlagQW[2]=true;
			fprintf(outf,"%7.3lf|",B_V);
		}
		if(sscanf_s(ptr[17],"%lf",&V_R)==NULL){
			FlagQW[3]=false;
			fprintf(outf,"      M|");
		}else{
			FlagQW[3]=true;
			fprintf(outf,"%7.3lf|",V_R);
		}
		double 	B_R=0;
		double  R=0;
		if(FlagQW[0] && FlagQW[2] && FlagQW[3]) {
			B_R=B_V+V_R; 
			R=V-V_R;
			fprintf(outf,"%7.3lf|%7.3lf|",B_R,R);
		}else{
			fprintf(outf,"      M|      M|");

		}
//ВВОД и печать Кузинса
		if(sscanf_s(ptr[29],"%lf",&Vc)==NULL){
			FlagQc[0]=false;
			fprintf(outf,"       M|");
		}else{
			FlagQc[0]=true;
			fprintf(outf,"%8.3lf|",Vc);
		}
		if(sscanf_s(ptr[30],"%lf",&Vc_Rc)==NULL){
			FlagQc[1]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQc[1]=true;
			fprintf(outf,"%7.3lf|",Vc_Rc);
		}
		if(sscanf_s(ptr[31],"%lf",&Rc_Ic)==NULL){
			FlagQc[2]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQc[2]=true;
			fprintf(outf,"%7.3lf|",Rc_Ic);
		}
		if(sscanf_s(ptr[32],"%lf",&Vc_Ic)==NULL){
			FlagQc[3]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQc[3]=true;
			fprintf(outf,"%7.3lf|",Vc_Ic);
		}
		double  Rc=0;
		if(FlagQc[0] && FlagQc[2] ){
			Rc=Vc-Vc_Rc;
			fprintf(outf,"%7.3lf|",Rc);
		}else{
			fprintf(outf,"      M|");
		}
//ВВОД и печать HIP
		if(sscanf_s(ptr[21],"%lf",&Vh)==NULL){
			FlagQh[0]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQh[0]=true;
			fprintf(outf,"%7.2lf|",Vh);
		}
		if(sscanf_s(ptr[22],"%lf",&B_Vh)==NULL){
			FlagQh[1]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQh[1]=true;
			fprintf(outf,"%7.3lf|",B_Vh);
		}
		if(sscanf_s(ptr[23],"%lf",&eB_Vh)==NULL){
			FlagQh[2]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQh[2]=true;
			fprintf(outf,"%7.3lf|",eB_Vh);
		}
		if(sscanf_s(ptr[24],"%lf",&V_Ih)==NULL){
			FlagQh[3]=false;//K=0;
			fprintf(outf,"     M|");
		}else{
			FlagQh[3]=true;
			fprintf(outf,"%6.2lf|",V_Ih);
		}
		if(sscanf_s(ptr[25],"%lf",&eV_Ih)==NULL){
			FlagQh[4]=false;//K=0;
			fprintf(outf,"     M|");
		}else{
			FlagQh[4]=true;
			fprintf(outf,"%6.2lf|",eV_Ih);
		}
		if(sscanf_s(ptr[26],"%lf",&Hp)==NULL){
			FlagQh[5]=false;//K=0;
			fprintf(outf,"       M|");
		}else{
			FlagQh[5]=true;
			fprintf(outf,"%8.4lf|",Hp);
		}
		if(sscanf_s(ptr[27],"%lf",&eHp)==NULL){
			FlagQh[6]=false;//K=0;
			fprintf(outf,"       M|");
		}else{
			FlagQh[6]=true;
			fprintf(outf,"%8.4lf|",eHp);
		}
		double  Hp_Ih=0;
		if(FlagQh[5] && FlagQh[4] && FlagQh[0] ){
			Hp_Ih=Hp-Vh+V_Ih;
			fprintf(outf,"%7.3lf|",Hp_Ih);
		}else{
			fprintf(outf,"      M|");
		}

//ВВОД и печать TYC-2MASS
		if(sscanf_s(ptr[7],"%lf",&Bt)==NULL) {
			FlagQt[0]=false;
			fprintf(outf,"       M|");
		}else{
			FlagQt[0]=true;
			fprintf(outf,"%8.3lf|",Bt);
		}

		if(sscanf_s(ptr[8],"%lf",&eBt)==NULL){
			//eBt=0.025;
			FlagQt[1]=false;
			fprintf(outf,"      M|");
		}else{
			FlagQt[1]=true;
			fprintf(outf,"%7.3lf|",eBt);
		}

		if(sscanf_s(ptr[9],"%lf",&Vt)==NULL){
			FlagQt[2]=false;
			fprintf(outf,"      M|");
		}else{
			FlagQt[2]=true;
			fprintf(outf,"%7.3lf|",Vt);
		}
		if(sscanf_s(ptr[10],"%lf",&eVt)==NULL){
			//eVt=0.025;
			FlagQt[3]=false;		
			fprintf(outf,"      M|");
		}else{
			FlagQt[3]=true;
			fprintf(outf,"%7.3lf|",eVt);
		}
		double Bt_Vt=0;
		if(FlagQt[0] && FlagQt[2]) {
			Bt_Vt=Bt-Vt; 
			fprintf(outf,"%7.3lf|",Bt_Vt);
		}else{
			fprintf(outf,"      M|");
		}

		if(sscanf_s(ptr[34],"%lf",&J)==NULL){
			FlagQt[4]=false;//J=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[4]=true;
			fprintf(outf,"%7.3lf|",J);
		}
		if(sscanf_s(ptr[35],"%lf",&eJ)==NULL){
			FlagQt[5]=false;//J=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[5]=true;
			fprintf(outf,"%7.3lf|",eJ);
		}

		if(sscanf_s(ptr[36],"%lf",&H)==NULL){
			FlagQt[6]=false;//H=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[6]=true;
			fprintf(outf,"%7.3lf|",H);
		}
		if(sscanf_s(ptr[37],"%lf",&eH)==NULL){
			FlagQt[7]=false;//H=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[7]=true;
			fprintf(outf,"%7.3lf|",eH);
		}
		if(sscanf_s(ptr[38],"%lf",&K)==NULL){
			FlagQt[8]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[8]=true;
			fprintf(outf,"%7.3lf|",K);
		}
		if(sscanf_s(ptr[39],"%lf",&eK)==NULL){
			FlagQt[9]=false;//K=0;
			fprintf(outf,"      M|");
		}else{
			FlagQt[9]=true;
			fprintf(outf,"%7.3lf|",eK);
		}

		fprintf(outf,"%s|",Qflag);

		double Vt_J=0;
		double Vt_H=0;
		double Vt_K=0;
		if(FlagQt[2] && FlagQt[4]) {
			Vt_J=Vt-J;
			fprintf(outf,"%7.3lf|",Vt_J);
		}else{
			fprintf(outf,"      M|");
		}
		if(FlagQt[2] && FlagQt[6]) {
			Vt_H=Vt-H;
			fprintf(outf,"%7.3lf|",Vt_H);
		}else{
			fprintf(outf,"      M|");
		}
		if(FlagQt[2] && FlagQt[8]) {
			Vt_K=Vt-K;
			fprintf(outf,"%7.3lf|",Vt_K);
		}else{
			fprintf(outf,"      M|");
		}
		//double 	Bt_Vt=Bt-Vt; //t_Vt
		double 	J_H=J-H;
		double 	J_K=J-K;
		double 	H_K=H-K;
		//double 	Vt_J=Vt-J;

		if(FlagQt[4] && FlagQt[6]) {
			J_H=J-H;
			fprintf(outf,"%7.3lf|",J_H);
		}else{
			fprintf(outf,"      M|");
		}
		if(FlagQt[6] && FlagQt[8]) {
			H_K=H-K;
			fprintf(outf,"%7.3lf|",H_K);
		}else{
			fprintf(outf,"      M|");
		}
		if(FlagQt[4] && FlagQt[8]) {
			J_K=J-K;
			fprintf(outf,"%7.3lf|",J_K);
		}else{
			fprintf(outf,"      M|");
		}

// порядковый номер строк в исходном файле
		sscanf_s(ptr[0],"%d",&pp);

		//for (UINT j=0;j<NColumn;j++){
		//	//switch (j)
		//	//{
		double a0=0,a1=0,a2=0,a3=0,a4=0,b1=0,b2=0,b3=0,b4=0,b5=0,b6=0,b7=0,b8=0,b9=0,b10=0,c1=0,c2=0,c3=0,c4=0,c5=0,\
			c6=0,c7=0,c8=0,c9=0,c10=0,c11=0,c12=0,c13=0,c14=0,c15=0,c16=0,c17=0,c18=0,c19=0,c20=0;
//Вычисление цветов для полиномов
		//double  Hp_Ih=Hp-Vh+V_Ih;
		double Ssys=0;
		double Sigm=0;
		
		//  Полином WBVR

		double  GIwbvrR=0;
		bool FlagYES=false;
		double GIwbvrRmin=0;
		double GIwbvrRmax=0;

		for (int j=0;j<5;j++){
			if (FlagQW[j]) FlagYES=true;
			else{
				FlagYES=false;
				break;
			}
		}
	if(FlagYES){
		a0=	0.0062279039362599419;
		a2=	0.083288897690114383;
		a3=	0.4445024115037593;
		b2=	-0.17848286374168193;
		b3=	-0.53653929014548551;
		b4=	-0.069739449528567934;
		b6=	0.60761449204233675;
		c2=	-0.33538308380658244;
		c3=	0.17299683137218355;
		c4=	-0.14246692204333999;
		c5=	0.11144541146388555;
		c6=	0.50268437931841037;
		c7=	0.58771263999441525;
		c8=	0.23120633107321012;
		c9=	-0.47621395573217401;
		c10=-0.64345246709062442;
		//GIwbvrR=R+0.006227+/*a1*W_B+*/0.083288*B_V+0.44450*V_R-0.17848*B_V^^2-0.536539*V_R^^2+b4*W_B*B_V+b6*B_V*V_R+\
		//	c2*B_V^^3+c3*V_R^^3+c4*W_B^^2*B_V+c5*W_B^^2*V_R+c6*W_B*B_V^^2+c7*B_V^^2*V_R+c8*W_B*V_R^^2+c9*B_V*V_R^^2+c10*W_B*B_V*V_R;
	////  Полином WBVR
	//for (int j;j<NColumn;j++){
	//	if (FlagQW[j]) FlagYES=true;
	//	else FlagYES=false;
	//}
			GIwbvrR=R+a0+/*a1*W_B+*/a2*B_V+a3*V_R+/*b1*W_B^^2+*/b2*pow(B_V,2)+b3*pow(V_R,2)+b4*W_B*B_V+\
		/*b5*W_B*V_R+*/b6*B_V*V_R+/*c1*W_B^^3+*/c2*pow(B_V,3)+c3*pow(V_R,3)+c4*pow(W_B,2)*B_V+c5*pow(W_B,2)*V_R\
		+c6*W_B*pow(B_V,2)+c7*pow(B_V,2)*V_R+c8*W_B*pow(V_R,2)+c9*B_V*pow(V_R,2)+c10*W_B*B_V*V_R;
		if(B_R>=-1.0 && B_R<1.5) {Ssys=0;Sigm=0.0025;}
		if(B_R>=1.5 && B_R<2.5) {Ssys=0.003;Sigm=0.008;}
		if(B_R>=2.5) {Ssys=0;Sigm=0.008;}
		GIwbvrRmin=GIwbvrR+Ssys-Sigm; 
		GIwbvrRmax=GIwbvrR+Ssys+Sigm;
		fprintf(outf,"%12.4lf|%11.4lf|%11.4lf|",GIwbvrR,GIwbvrRmin,GIwbvrRmax);
	}else{		
		fprintf(outf,"           M|          M|          M|");
	}
	// Кузинс
	double GImVRIc1=0;
	//for (int j;j<4;j++){
	//	if (FlagQW[j]) FlagYES=true;
	//	else FlagYES=false;
	//}
	for (int j=0;j<4;j++){
		if (FlagQc[j]) 
			FlagYES=true;
		else{
			FlagYES=false;
			break;
		}
	}
	if(FlagYES){
		a0=	-0.0029562812787334337;
		a1=	0.34983081172692582;
		b1=	0.76303971745513277;
		b3=	-1.0117679691698382;
		c1=	-1.0159989799035787;
		c2=	1.0687627563600661;
		c4=	-0.1597727395571684;
		GImVRIc1=Rc+a0+a1*Vc_Rc+b1*pow(Vc_Rc,2)+b3*Vc_Rc*Rc_Ic+c1*pow(Vc_Rc,3)+\
			c2*pow(Vc_Rc,2)*Rc_Ic+c4*pow(Rc_Ic,3);
		Ssys = 0; Sigm=0.0113;//sqrt (2*0.008*0.008);
		double GImVRIc1min=GImVRIc1+Ssys+Sigm; 
		double GImVRIc1max=GImVRIc1+Ssys-Sigm;
		fprintf(outf,"%12.4lf|%11.4lf|%11.4lf|",GImVRIc1,GImVRIc1min,GImVRIc1max);	
	}else 		fprintf(outf,"           M|          M|          M|");

	//HIP короткий полином но малые ошибки коэффициентов
	double	GIhmHp=0;
	//for (int j;j<7;j++){
	if (FlagQh[0]&&FlagQh[1]&&FlagQh[3]&&FlagQh[5]&&FlagQh[6]) FlagYES=true;
	else FlagYES=false;
	//}
	if(FlagYES){
		//	a0=	-0.003612077328646885;
		//	a1=	-0.24964097669635363;
		//	a2=	-0.27197252342021133;
		//	b1=	0.030055155449994195;
		//	b3=	-0.21342722499256084;
		//	b6=	0.13502975676539394;
		//	c1=	0.063563482207012967;
		//	c2=	-0.09723000515618920;
		//	c4=	-0.14543222168417969;
		//	c6=	0.19272857909282501;
		//	GIhmHp=Hp+a1*Hp_Ih+a2*B_Vh+b1*pow(Hp_Ih,2)+b3*pow(V_Ih,2)+b6*B_Vh*V_Ih+c1*pow(Hp_Ih,3)+c2*pow(B_Vh,3)+c4*pow(Hp_Ih,2)*B_Vh+6*Hp_Ih*pow(B_Vh,2);
		//}else GIhmHp=999.0;
		//if(Hp_Ih<0.5)				{Ssys=0.003;Sigm=sqrt (0.015*0.015+eHp*eHp);}
		//if(Hp_Ih>=0.5 && Hp_Ih<1.0)	{Ssys=0.0;Sigm=sqrt (0.015*0.015+eHp*eHp);}
		//if(Hp_Ih>=1.0)	{Ssys=0.0;Sigm=sqrt (0.025*0.025+eHp*eHp);}
		//double GIhmHpmin=GIhmHp+Ssys+Sigm; 
		//double GIhmHpmax=GIhmHp+Ssys-Sigm;
		a0=	-0.0042239000153461836;
		a1=	-0.22709980626122844;
		a2=	-0.29939479651072703;
		b1=	2.5986863345795705;
		b2=	-0.6846711940516454;
		b3=	3.0090224394822527;
		b5=	-6.1079013001964508;
		b6=	1.1836674208870206;
		c1=	-0.8383370869189626;
		c2=	0.61728811767388769;
		c4=	1.4760156285328951;
		c5=	0.71462269229292019;
		c6=	-1.2288804790583805;
		c9=	-0.75717050264696029;
		GIhmHp=Hp+a0+a1*Hp_Ih+a2*B_Vh+/*a3*V_Ih+*/b1*pow(Hp_Ih,2)+b2*pow(B_Vh,2)+b3*pow(V_Ih,2)+\
			/*b4*Hp_Ih*B_Vh+*/b5*Hp_Ih*V_Ih+b6*B_Vh*V_Ih+c1*pow(Hp_Ih,3)+c2*pow(B_Vh,3)+/*c3*V_Ih^^3+*/
			c4*pow(Hp_Ih,2)*B_Vh+c5*pow(Hp_Ih,2)*V_Ih+c6*Hp_Ih*pow(B_Vh,2)+/*c7*B_Vh^^2*V_Ih+c8*Hp_Ih*V_Ih^^2+*/
			c9*B_Vh*pow(V_Ih,2)/*+c10*Hp_Ih*B_Vh*V_Ih*/;
		if(Hp_Ih<0.5)				{Ssys=0.003;Sigm=sqrt (0.000225+eHp*eHp);}//0.015*0.015
		if(Hp_Ih>=0.5 && Hp_Ih<1.0)	{Ssys=0.0;Sigm=sqrt (0.000225+eHp*eHp);}
		if(Hp_Ih>=1.0)	{Ssys=0.0;Sigm=sqrt (0.000625+eHp*eHp);}//0.025*0.025
		double GIhmHpmin=GIhmHp+Ssys+Sigm; 
		double GIhmHpmax=GIhmHp+Ssys-Sigm;
		fprintf(outf,"%12.4lf|%11.4lf|%11.4lf|",GIhmHp,GIhmHpmin,GIhmHpmax);	
	}else 		fprintf(outf,"           M|          M|          M|");

//TYC_2MASS  Полином длинный, сигма 0.06, но большие ошибки коэффициентов
	//  Полином TYC_2MASS
	double  GItjhkmVt=0;
	//for (int j;j<5;j++){
	//	if (FlagQt[j]) FlagYES=true;
	//	else FlagYES=false;
	//}
	if ( FlagQt[0] && FlagQt[2] && FlagQt[3] && FlagQt[4] && FlagQt[6] && FlagQt[8]) FlagYES=true;
	else FlagYES=false;

	if(FlagYES){
		a1=	-0.5068654072339065;
		a3=	-0.12887947275399511;
		a4=	0.37694985050684393;
		b1=	4.481303075420457;
		b2=	1.6085810858521463;
		b3=	-0.3699473101563994;
		b4=	-3.3921221054676423;
		b5=	-5.5263951568409952;
		b7=	-3.4317510657667087;
		b8=	2.7117266237835045;
		b9=	0.34747571878112637;
		b10=	0.82850677049010302;
		c1=	3.0268492401572016;
		c2=	-0.4894507335669356;
		c3=	0.27686919046112257;
		c4=	1.4368281583650886;
		c5=	-4.6153531693153447;
		c6=	-4.4115182537286133;
		c7=	-5.3459285805101393;
		c8=	2.5473350034204461;
		c9=	-1.3953088304204098;
		c10=	-1.4989264415115384;
		c12=	3.5775331242975095;
		c13=	-0.25949029060680545;
		c14=	7.453659404803691;
		c15=	-3.842870589384556;
		c16=	2.4316173254474927;
		c17=	5.0385560051811513;
		c18=	5.5451933519996741;
		c19=	-3.4132670647808521;
		GItjhkmVt=Vt+/*a0+*/a1*Bt_Vt+/*a2*Vt_J+*/a3*J_H+a4*H_K+b1*pow(Bt_Vt,2)+b2*pow(Vt_J,2)+b3*pow(J_H,2)+b4*pow(H_K,2)+b5*Bt_Vt*Vt_J+\
		/*b6*Bt_Vt*J_H+*/b7*Bt_Vt*H_K+b8*Vt_J*H_K+b9*Vt_J*J_H+b10*J_H*H_K+c1*pow(Bt_Vt,3)+c2*pow(Vt_J,3)+c3*pow(J_H,2)+c4*pow(H_K,3)+\
		c5*pow(Bt_Vt,2)*Vt_J+c6*pow(Bt_Vt,2)*J_H+c7*pow(Bt_Vt,2)*H_K+c8*pow(Vt_J,2)*Bt_Vt+c9*pow(Vt_J,2)*J_H+c10*pow(Vt_J,2)*H_K+\
		/*c11*J_H^^2*Bt_Vt+*/c12*pow(J_H,2)*H_K+c13*pow(J_H,2)*Vt_J+c14*pow(H_K,2)*Bt_Vt+c15*pow(H_K,2)*Vt_J+c16*pow(H_K,2)*J_H+\
		c17*Bt_Vt*Vt_J*J_H+c18*Bt_Vt*Vt_J*H_K+c19*Bt_Vt*J_H*H_K/*+c20*Vt_J*J_H*H_K*/;
		if(Vt_J<0.5)				{Ssys=0.005;Sigm=sqrt (0.018*0.018+eVt*eVt);}
		if(Vt_J>=0.5 && Vt_J<1.5)	{Ssys=-0.005;Sigm=sqrt (0.04*0.04+eVt*eVt);}
		if(Vt_J>=1.5 && Vt_J<2.5)	{Ssys=0.05;Sigm=sqrt (0.05*0.05+eVt*eVt);}
		if(Vt_J>=2.5 && Vt_J<3.5)	{Ssys=-0.04;Sigm=sqrt (0.06*0.06+eVt*eVt);}
		if(Vt_J>=3.5)				{Ssys=0.05;Sigm=sqrt (0.08*0.08+eVt*eVt);}
		double GItjhkmVtmin=GItjhkmVt+Ssys-Sigm; 
		double GItjhkmVtmax=GItjhkmVt+Ssys+Sigm;
		fprintf(outf,"%12.4lf|%11.4lf|%11.4lf|",GItjhkmVt,GItjhkmVtmin,GItjhkmVtmax);	
	}else 		fprintf(outf,"           M|          M|          M|");


//TYC_2MASS  Полином короткий, сигма 0.075, но малые ошибки коэффициентов
	 double GImtjhk=	0;
	if(FlagYES){
		a1=	-0.50877505040086368;
		a3=	-0.080240792109694384;
		a4=	0.42713588100222732;
		b1=	-1.0446180570560697;
		b2=	0.37265633825988359;
		b3=	0.87497403493448378;
		b8=	0.4211000472540346;
		b9=	-0.36318183074321392;
		c3=	-0.36278744651974981;
		c4=	-0.55314736774094209;
		c5=	0.55832946537747752;
		c8=	-0.29853312235414498;
		c9=	-0.20973575632364369;
		c10=	0.26435231058134356;
		c15=	0.098307773193833492;
		c16=	0.50948715322684246;
		c17=	0.46011991188887341;
		c18=	-0.90670072083951714;
		c19=	0.47879028936685736;
		GImtjhk=Vt+a1*Bt_Vt+a3*J_H+a4*H_K+b1*pow(Bt_Vt,2)+b2*pow(Vt_J,2)+b3*pow(J_H,2)+b8*Vt_J*H_K+b9*Vt_J*J_H+c3*pow(J_H,2)+c4*pow(H_K,3)+
			c5*pow(Bt_Vt,2)*Vt_J+c8*pow(Vt_J,2)*Bt_Vt+c9*pow(Vt_J,2)*J_H+c10*pow(Vt_J,2)*H_K+
			c16*pow(H_K,2)*J_H+	c17*Bt_Vt*Vt_J*J_H+c18*Bt_Vt*Vt_J*H_K+c19*Bt_Vt*J_H*H_K;
		if(Vt_J<0.5)				{Ssys=0.005;Sigm=sqrt (0.018*0.018+eVt*eVt);}
		if(Vt_J>=0.5 && Vt_J<1.5)	{Ssys=-0.005;Sigm=sqrt (0.04*0.04+eVt*eVt);}
		if(Vt_J>=1.5 && Vt_J<2.5)	{Ssys=0.05;Sigm=sqrt (0.05*0.05+eVt*eVt);}
		if(Vt_J>=2.5 && Vt_J<3.5)	{Ssys=-0.04;Sigm=sqrt (0.06*0.06+eVt*eVt);}
		if(Vt_J>=3.5)				{Ssys=0.05;Sigm=sqrt (0.08*0.08+eVt*eVt);}
		double GImtjhkmin=GImtjhk+Ssys-Sigm; 
		double GImtjhkmax=GImtjhk+Ssys+Sigm;
		fprintf(outf,"%12.4lf|%11.4lf|%11.4lf|",GImtjhk,GImtjhkmin,GImtjhkmax);	
	}else 		fprintf(outf,"           M|          M|          M|");
	fprintf(outf,"%7d|\n",pp);
}

	//TYC       |  V   |  W_B |  B_V | V_R  | B_R  |   R  |  Vt  |Bt_Vt | Vt_J |  J   |  H   |  K   |Qf |  GIwbvrR|  GItjhkVt|     pp|\n\
//	*TYC|Bt|Vt|V|W_B|B_V|V_R|J|H|K|Qf|GIwbvrR|GItjhkVt|pp|\n");
	//if(GIwbvrR<=6.8 && GItjhkmVt<=6.8 && Vt<=6.8 )
//	i=i;
//		fprintf(outf,"%s|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|\
//%s|%8.4lf|%8.4lf|%8.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%11.4lf|%7d|\n",\
//Name,V,W_B,B_V,V_R,B_R,R,Vt,Bt_Vt,Vt_J,J,H,K,Qflag,J_H,H_K,J_K,GIwbvrR,GIwbvrRmin,GIwbvrRmax,GImVRIc1,\
//GImVRIc1min,GImVRIc1max,GIhmHp,GIhmHpmin,GIhmHpmax,GItjhkmVt,GItjhkmVtmin,GItjhkmVtmax,GImtjhk,GImtjhkmin,GImtjhkmax,pp);
	//else
	//	fprintf(errf,"%s|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%8.4lf|%s|%8.4lf|%8.4lf|%8.4lf|%10.4lf|%10.4lf|%7d|\n",\
	//	Name,V,W_B,B_V,V_R,B_R,R,Vt,Bt_Vt,Vt_J,J,H,K,Qflag,J_H,H_K,J_K,GIwbvrR,GItjhkmVt,pp);

//if(GIwbvrR)

	//fprintf(outf,"\n");
	//fprintf(outf,"      TYC       |  RA_2000   |  DE_2000   |   pmRA|   pmDE|5 Bt  |e_Bt |7 Vt  |e_Vt |   HD_corr|Sp|        2Mass_id |12J   | e_J |14H   | e_H |16K   | e_K |Qf |Cf|Xf|Af|  HD_wbvr |23V   |24W_B |25B_V |26V_R | Sp_wbvr  |  UCAU4   | Ucac4_id| fmag | a_mag|err  |\n");
	//fprintf(outf,"             TYC| RAND|       V|     W_B|     B_V|     V_R|     B_R|       R|      Vt|   Bt_Vt|    Vt_J|     J_H|     H_K|     J_K|\n");
	fclose(outf);
	_fcloseall();

	//}
}
#endif
//#endif
	//double RO1=DS[i].Ri[0]-CorrColorSp(DS[i].SpV,Count_strSp, J_H,J_K,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Ks J-J_K 

//#endif	//}
	//for(UINT i=0;i<Count_str;i++){
	//	if(StrPtr(data[i],'|',ptr)!=0){
	//		sscanf_s(ptr[], "%lf",&J);
	//		sscanf_s(ptr[], "%lf",&H);
	//		sscanf_s(ptr[], "%lf",&K);
	//		memcpy(Name,ptr[],18);//Номер Тихо
	//		Name[18]=0;
	//		memcpy(SP,ptr[],3);
	//		Sp[3]=0;
	//		memcpy(QFlag,ptr[],3);
	//		QFlag[3]=0;
	//	}
	//	Js=Found_Value(J,SP,QFlag,data1);
	//	Hs=Found_Value(H,SP,QFlag,data1);
	//	Ks=Found_Value(K,SP,QFlag,data1);
	//	fprintf(OutFile,"%s|%10.5lf|%10.5lf|%10.5lf|",Name,Js,Hs,Ks);
//#endif
//double Found_Value(double M,char *SP,char *QFlag,char** data1){
//#ifdef EMOE//
//}
#ifdef CorrCoord
void CMyTeMath::TESTING()//Редукция координат из 1991.25 в J2000
{//Для вычисления средне взвешаных координат//B_C_090215_3DBL_2Cod
//#ifdef CorrCoord
	FILE *inpf,*outf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\BOARD_CAT_COORD.txt";//BC_NEW_DBL-COORD.txt";//COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3DBL_2HM_6m.txt";//ASU_Not_Found_T-2 BC_NEW_DBL_COORD_1 BOARD_CAT_COORD_HIP.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3DBL_2HM_6mE.txt";//ASU_Not_Found_T_2.txt";//BC_NEW_DBL_COORD_HIP1//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char OutStr[2048];
	char buff[2048];
	typedef struct Date {
			//bool FlagYes;
			char Name[14];
			char Spect[4];
			char QFlag[4];
			//char QFlagOut[4];
			double RA;
			double pmRA;
			double DE;
			double pmDE;
			double pmRAs;
			double pmDEs;
			double RAs;
			double DEs;
			double MAG;
			double MAGs;
			double Dmag;
		} DSP;
// ВВод файла исходных величин
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
 //      return;
	}
	UINT Count_str=CountDataFile(inpf);
	 char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	 fclose(inpf);

	 if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
 //      return;
	}
//#ifdef MAZAFAKA
double dT=0.0875; // dT(2000-1995.25) в юлианских столетиях JED=2451545.0+(JDep-2000)*365.25; 
					//										JED(JDep = 1991.25) = 2448349.0625;
	 fprintf(outf,"pp|TYC|RA0|DE0|pmRA0|pmDE0|RAs|DEs|pmRAs|pmDEs|\n");
	 DSP *DES=new DSP[Count_str];
	 //double *RA0=new double[Count_str];
	 for(UINT i=0;i<Count_str;i++){
		 UINT NOut=StrPtr(Data0[i],'|',ptr);
		memcpy(DES[i].Name,ptr[0],16);
			DES[i].Name[16]=0;
		if(sscanf_s(ptr[12], "%lf",&DES[i].RA)!=NULL){ //n [10]
			//continue;
			//}
			sscanf_s(ptr[13], "%lf",&DES[i].DE); //n [11]
			sscanf_s(ptr[14], "%lf",&DES[i].pmRA);// mas/y n [12]
			DES[i].pmRAs=DES[i].pmRA/(10*AsRad);// mas/y=>rad/j100

			//DES[i].RA=DES[i].RA*Pi/180;
			sscanf_s(ptr[15], "%lf",&DES[i].pmDE); // mas/y n [13]
			DES[i].pmDEs=DES[i].pmDE/(10*AsRad); // mas/y=>rad/j100
			//sscanf_s(ptr[], "%lf",DES[i].Dmag);
			//sscanf_s(ptr[], "%lf",DES[i].MAG);
			if(DES[i].DE>89.9) DES[i].pmRAs=0;
			else DES[i].pmRAs/=cos(DES[i].DE*Pi/180);
		 //}
 		// for(UINT i=0;i<Count_str;i++){
			 double X=cos(DES[i].RA*Pi/180)*cos(DES[i].DE*Pi/180);
			 double Y=sin(DES[i].RA*Pi/180)*cos(DES[i].DE*Pi/180);
			 double Z=sin(DES[i].DE*Pi/180);
			 double vX=-DES[i].pmRAs*Y-Z*cos(DES[i].RA*Pi/180)*DES[i].pmDEs;
			 double vY= DES[i].pmRAs*X-Z*sin(DES[i].RA*Pi/180)*DES[i].pmDEs;
			 double vZ=cos(DES[i].DE*Pi/180)*DES[i].pmDEs;
			 X+=dT*vX;
			 Y+=dT*vY;
			 Z+=dT*vZ;
			 double R2=X*X+Y*Y+Z*Z;
			 double R=sqrt(R2);
			 double Rxy2=X*X+Y*Y;
			 double Rxy=sqrt(Rxy2);
			 if(Rxy2!=0){ 
				DES[i].RAs=atan2(Y,X);
				if(DES[i].RAs<0) DES[i].RAs+=Pi2;
				DES[i].RAs*=180/Pi;//deg
				DES[i].DEs=atan2(Z,Rxy);
				DES[i].pmRAs=(X*vY-Y*vX)/Rxy2;
				DES[i].pmRAs*=cos(DES[i].DEs)*2062648.062470964;//AsRad/Pi;// ms rad 100year
				DES[i].DEs*=180/Pi; //deg
				DES[i].pmDEs=(Rxy2*vZ-Z*(X*vX+Y*vY))/(Rxy*R2);
				DES[i].pmDEs*=2062648.062470964; // ms/y
			 }else DES[i].RAs=0;
  			 fprintf(outf,"%5d|%s| %13.8lf|%13.8lf|%8.2lf|%8.2lf| %13.8lf|%13.8lf|%8.2lf|%8.2lf|\n",
				 i+1,DES[i].Name,DES[i].RA,DES[i].DE,DES[i].pmRA,DES[i].pmDE,DES[i].RAs,DES[i].DEs,DES[i].pmRAs,DES[i].pmDEs);
		}else 	fprintf(outf,"%5d|%s|\n",i+1,DES[i].Name);


	 }
//#endif
	 DSP *DES=new DSP[Count_str];
	 for(UINT i=0;i<Count_str;i++){
		UINT NOut=StrPtr(Data0[i],'|',ptr);
		memcpy(DES[i].Name,ptr[0],16);
			DES[i].Name[16]=0;
		//if(sscanf_s(ptr[12], "%lf",&DES[i].RA)!=NULL){ //n [10]
			//continue;
			//}
			sscanf_s(ptr[13], "%lf",&DES[i].RA);
			sscanf_s(ptr[14], "%lf",&DES[i].DE); //n [11]
			//sscanf_s(ptr[14], "%lf",&DES[i].pmRA);// mas/y n [12]
			//DES[i].pmRAs=DES[i].pmRA/(10*AsRad);// mas/y=>rad/j100

			//DES[i].RA=DES[i].RA*Pi/180;
			//sscanf_s(ptr[15], "%lf",&DES[i].pmDE); // mas/y n [13]
			//DES[i].pmDEs=DES[i].pmDE/(10*AsRad); // mas/y=>rad/j100
			sscanf_s(ptr[7], "%lf",&DES[i].Dmag);
			sscanf_s(ptr[10], "%lf",&DES[i].MAG);
	 }
	 fprintf(outf,"pp|TYC|SMag|RAEs|DEEs|DCod|NearBe|\n");

//#ifdef CorrCoord
	 for(UINT i=0;i<Count_str;i+=2){//1,0857362047581295691278222972915
		 double d=DES[i].MAG-DES[i+1].MAG;
		 double a=0;
		 double W1=0;
		 double W2=0;
		 if(d==0) a=exp(DES[i].Dmag/EConst);
		 else a=exp(d/EConst);

		 W1=1/(1+a);
		 W2=a/(1+a);
			DES[i].RAs=W1*DES[i].RA+W2*DES[i+1].RA;
			DES[i].DEs=W1*DES[i].DE+W2*DES[i+1].DE;
		 if(d==0){
			DES[i].MAGs=DES[i].MAG;
		 }else
			DES[i].MAGs=-2.5*log10(exp(DES[i].MAG/EConst)+exp(DES[i+1].MAG/EConst));
		char DCod='D';

		fprintf(outf,"%5d|%s|%7.3lf|%13.8lf|%13.8lf|%c|%s|\n",i+1,DES[i].Name,DES[i].MAGs,DES[i].RAs,DES[i].DEs,DCod,DES[i+1].Name);
		//Для случая тройной звезды


		// //}else{
		//	DES[i].RAs=W1*DES[i+1].RA+W2*DES[i].RA;
		//	DES[i].DEs=W1*DES[i+1].DE+W2*DES[i].DE;
		//	fprintf(outf,"%s|%13.8lf|%13.8lf\n",DES[i].Name,DES[i].RAs,DES[i].DEs);
		//}
	 }	 
//#endif
	fclose(outf);
_fcloseall();

	//}
}
#endif
#ifdef CorrCoord
void CMyTeMath::TESTING()
//Для чистки вылетов по фильтрующим полинома //J_KUp=0.32+0.26*Vt_J;J_KDn1=-0.22+0.25*Vt_J;J_KDn2=0.5+0.08*Vt_J;
void CMyTeMath::TESTING()
{	FILE *inpf,*outf,*errf;	
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BC_13_WORK_2E_AXUM_0.txt";//BCK7m+HYP+C+TYC+JHK.txt";//\\BOARD_CAT_m7.txt";//DATASHIT\\BC_13_WORK_2.txt";//EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BC_13_WORK_2E_AXUM_REV.txt";//GI_BCK7m+HYP+C+TYC+JHK.txt";//BOARD_CAT_m7_W_TJHK.txt";//DATASHIT\\BC_13_WORK_2E.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\\\BOARD_CAT_REV.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	//char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 31
	//typedef struct Date {
	//	//bool FlagYes;
	//	char Name[14];
	//	double Ri[MaxBand];
	//	double RO[MaxBand];
	//} DSP;
	// ВВод файла исходных величин BC_13_WORK_2E_AXUM	/*_S_2709.txt */
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	if (fopen_s(&errf, ErrFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	double Vt_J=0;
	double J_K=0;
	char Name[17];
//	fgets(InLin,len,inpf);	//	ЗАГОЛОВОК
	UINT NColumn=	StrPtr(Data0[0],'|',ptr); //число столбцов
	//UINT Count_str=CountDataFile(inpf);
	//bool *FlagOUT=new bool[Count_str];
	//for(UINT i=2;i<Count_str;i++)
	//	FlagOUT[i]=true;

	for(UINT i=1;i<Count_str/2;i++){
		//fgets(InLin,len,inpf);
		StrPtr(Data0[i],'|',ptr);
		//memcpy(Name,ptr[0],16);
		//Name[16]=0;
		sscanf_s(ptr[11], "%lf",&Vt_J);
		sscanf_s(ptr[14], "%lf",&J_K);
		// J_KUp=0.32+0.26*Vt_J;J_KDn1=-0.22+0.25*Vt_J;J_KDn2=0.44+0.08*Vt_J;
		//bool FLAGQ1=false;//TRUE
		//bool FLAGQ2=false;//TRUE
		//bool FLAGQ3=false;//TRUE 
		//if(J_K<0.32+0.26*Vt_J)  FLAGQ1=true;
		//else FLAGQ1=false;  J_KUp=0.25+0.29*Vt_J;J_KUp1=0.35+0.25*Vt_J;J_KDn1=-0.22+0.25*Vt_J;J_KDn2=0.2+0.11*Vt_J;
		//J_KUp=0.08+0.3*Vt_J;J_KUp1=0.35+0.25*Vt_J;J_KDn=-0.12+0.25*Vt_J;J_KDn1=0.2+0.11*Vt_J;
		double J_KUp=0.08+0.3*Vt_J;
		double J_KDn=-0.12+0.25*Vt_J;
		//double J_KUp1=0.35+0.25*Vt_J;
		//double J_KDn1=0.2+0.11*Vt_J;
//		if(Vt_J<3.0){
			if(J_K>J_KDn && J_K<J_KUp) {
				fprintf(outf,"%s",Data0[i]);
				fprintf(outf,"%s",Data0[i+4452]);
			}else {
				fprintf(errf,"%s",Data0[i]);
				fprintf(errf,"%s",Data0[i+4452]);
			}
		//}else{
		//	if(J_K>J_KDn1 && J_K<J_KUp1) {
		//		fprintf(outf,"%s",Data0[i]);
		//		fprintf(outf,"%s",Data0[i+4452]);
		//	}else {
		//		fprintf(errf,"%s",Data0[i]);
		//		fprintf(errf,"%s",Data0[i+4452]);
		//	}
		//}
	//	if(Vt_J>=3.88 && J_K>0.44+0.08*Vt_J && J_K<0.32+0.26*Vt_J){
	//		fprintf(outf,"%s",Data0[i]);
	//		fprintf(outf,"%s",Data0[i+4452]);
	//	}else {
	//		fprintf(errf,"%s",Data0[i]);
	//		fprintf(errf,"%s",Data0[i+4452]);
	//	}
	}

	//	//if(FLAGQ2 || FLAGQ3){
	//for(UINT i=2;i<Count_str;i++){
	//	if(FlagOUT[i])
	//		fprintf(outf,"%s",Data0[i]);
	//		//fputs("\n",outf);
	//	else
	//		fprintf(errf,"%s",Data0[i]);
	//}
	fclose(outf);
	_fcloseall();

	//}
}
#endif
#ifdef EMOE4
//Выдача нормированных спектров M-GIANTS
void CMyTeMath::TESTING()
{
	FILE *inpf,*outf;	
	char FileInput[]="D:\\MOSH\\SOURCE\\M-GIANT\\FIG6_M-G_MK.txt";//FIG3_M-G_Case.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
//	char FileSpect[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX\\LSP.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\SOURCE\\M-GIANT\\FIG6_M-G_MK_.txt";//mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	char** ptr=new char* [5000];
	//UINT  Count_str=0;
	//UINT  Count_err=0;
	//double dW,dB,dV,dR;
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
		//      return;
	}
	//if (fopen_s(&errf, ErrFile, "w") !=0 ){
	//	AfxMessageBox(_T("Error Opening ErrFile")); 
	//	//      return;
	//}
#define LLEN 2048
char InLin1[LLEN];
	//lam M0 M0c M1 M1c M2 M2c M3 M3c M4 M4c M5 M5c M6 M6c M7 M7c M8 M8c M9 M9c M10 M10c 

	//char INLIN[LLEN];
	UINT Count_str=CountDataFile(inpf);
	//char** Data0 = new char*[Count_str];        
	//InpData(Data0,inpf, Count_str, NULL); // Ввод файла со спектрами
	//fclose(inpf);
	double *Lambda=new double[Count_str];
	char NameFale[]="D:\\MOSH\\SOURCE\\M-GIANT\\M-GIANT FLUX MK NORM\\";
	char NF[100];
	fgets(InLin,len,inpf);//Заголовок
	UINT NUMSP=StrPtr(InLin,'|',ptr)-1;//число Flux
	double *Flux=new double[Count_str];
	char **namefale=new char*[NUMSP];
	for(UINT j=0;j<NUMSP;j++){
		namefale[j]=new char[7];
		memset(namefale[j],'\0',7);
		//Flux[j]=new double[Count_str];
	}
	for(UINT j=0;j<NUMSP;j++){
		memcpy(namefale[j],ptr[j+1],6);//strlen(data[i])(void*)
		namefale[j][6]=0;
	}
	//for(UINT i=1;i<Count_str;i++){//Count_str
		//memset(NF,'\0',100);
		////memset(NF,0,100);
		//memset(namefale,0,13);
		//int ST=strlen(ptr[0]);
		//memcpy((void*)namefale,(void*)ptr[1],10);//strlen(data[i])
	for(UINT j=0;j<NUMSP;j+=2){
		memset(NF,'\0',100);
		memcpy(NF,NameFale,strlen(NameFale));
		strcat(NF,namefale[j]);
		strcat(NF,".txt");
		//NF=_T(NameFale+namefale);
		//if(NUMSP!=0){
		fseek(inpf, 0L, SEEK_SET);
		fgets(InLin,len,inpf);
		for(UINT i=0;i<Count_str-1;i++){
			fgets(InLin,len,inpf);
			//if(i==5000){
			//	i=i;
			//}
			if (fopen_s(&errf, ErrFile, "a") !=0 ){
				AfxMessageBox(_T("Error Opening ErrFile")); 
				//      return;
			}
			fprintf(errf,"%s",InLin);
			fclose(errf);
			StrPtr(InLin,'|',ptr);
			sscanf_s(ptr[0], "%lf",&Lambda[i]);//%lf
			sscanf_s(ptr[j+1], "%lf",&Flux[i]);//%lf
		}
		//} ВЫдача файла спектров
		if (fopen_s(&outf, NF, "w") !=0 ){
			AfxMessageBox(_T("Error Opening Out File"));
			return;
		}
//Нормировка на 555нм
		double FL5550=	Integral_M(Flux,Lambda,Count_str-1,505.0,605.0);//505.0,605.0);
		//double FL5550=GetFlux(555.0,LamSt,Flux,Count_strSp);
		for(UINT i=0;i<Count_str-1;i++){
			fprintf(outf,"%7.1lf  %9.4e\n",Lambda[i],Flux[i]*100*80/FL5550);
			//if(i==4681)
			//	i=i;
		}
		fclose(outf);
	}
	delete [] Lambda;
	//de_allocate(Data0,Count_str);
	delete [] Flux;	
	de_allocate(namefale,NUMSP);
	_fcloseall;
}
#endif
#ifdef EMOE2
//Выдача нормированных спектров NGSL
void CMyTeMath::TESTING()
{
	FILE *inpf,*outf;	
	char FileInput[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char FileSpect[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX\\LSP.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	char** ptr=new char* [5000];
	UINT  Count_str=0;
	//UINT  Count_err=0;
	//double dW,dB,dV,dR;
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
		//      return;
	}
#define LLEN 100000
	char INLIN[LLEN];
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(INLIN,LLEN,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (INLIN[0]=='*'){ continue;
			}else{
				Count_str++;
			}
		}
	}
	fseek(inpf, 0L, SEEK_SET);
	char** data = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	for (UINT i=0; i<Count_str; ){
		try {                      // TEST FOR EXCEPTIONS.
			memset(INLIN,'\0',LLEN);
			if ( fgets(INLIN,LLEN,inpf)==0 ) break;
			if (INLIN[0]=='*'){ continue;
			}else{
				data[i] = new char[LLEN];  // STEP 2: SET UP THE COLUMNS
				memset(data[i],'\0',LLEN);
				data[i][0]='\0';
				int DST=strlen(INLIN);
				memcpy(data[i],INLIN,DST);//strlen(INLIN)-1
				i++;
			}// 	
		}
		catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF bad_alloc IS THROWN.
			// YOU COULD REQUEST OTHER ACTIONS BEFORE TERMINATING
			//Application->MessageBox("Could not allocate. Bye ...",
			//"Error...", MB_ICONERROR);
			AfxMessageBox(_T("Error Reading File"));
			exit(-1);
		}
	}
	fclose(inpf);
	UINT  Count_strSp=0;

	if (fopen_s(&inpf, FileSpect, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Out File"));
		//      return;
	}
	if (inpf) {
		while (!feof(inpf)){
			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*'){ continue;
			}else{
				Count_strSp++;
			}
		}
	}
	fseek(inpf, 0L, SEEK_SET);
	//DocDataType* DATA=new DocDataType [Count_strSp];
	double *LamSt=new double [Count_strSp];
	if (inpf) {
		int i=0;
		while (!feof(inpf)){

			if ( fgets(InLin,len,inpf)==0 ){
				//fputs(InLin,errf);
				//AfxMessageBox(_T("Error Reading Input File"));
				//fclose(errf);
				break;
			}
			if (InLin[0]=='*') continue;
			else {
				//if(StrPtr(InLin,'\t',ptr)!=0){
				sscanf_s(InLin, "%lf",&LamSt[i]);
				i++;
			}
		}
	}
	char NameFale[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX NORM\\";
	char namefale[]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	char NF[100];
	memset(NF,0,100);
	double *Flux=new double[Count_strSp];
	for(UINT i=0;i<Count_str;i++){//Count_str
		UINT NUMSP=StrPtr(data[i],'|',ptr);
		memset(NF,0,100);
		memset(namefale,0,13);
		//int ST=strlen(ptr[0]);
		memcpy((void*)namefale,(void*)ptr[0],10);//strlen(data[i])
		memcpy(NF,NameFale,strlen(NameFale));
		strcat(NF,namefale);
		strcat(NF,".txt");
		//NF=_T(NameFale+namefale);
		//if(NUMSP!=0){
		for(UINT j=0;j<Count_strSp;j++){	//Count_strSp NUMSP-1
			sscanf_s(ptr[j+1], "%lf",&Flux[j]);//%lf
		}
		//}
		if (fopen_s(&outf, NF, "w") !=0 ){
			AfxMessageBox(_T("Error Opening Out File"));
			return;
		}
		double FL5550=GetFlux(555.0,LamSt,Flux,Count_strSp);
		for(UINT j=0;j<Count_strSp;j++){
			fprintf(outf,"%6.1lf  %9.4lf\n",LamSt[j],Flux[j]/FL5550);
			//if(i==4681)
			//	i=i;
		}
		fclose(outf);
	}
	_fcloseall;
}
//#endif
void CMyTeMath::TESTING()
	//Вывод спектров Пиклса, нормированных на полосу 505нм - 605нм (555нм)
{
	FILE *inpf,*outf,*errf,*spcf;	
	char FileInput[]="D:\\MOSH\\SOURCE\\uvk_pic_spc_.txt";//спектры Pickles__.txt";//M-GIANT\\FIG3_M-G_Case.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char FileSpect[]="D:\\MOSH\\SOURCE\\LamSpPicles.txt";//NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\SOURCE\\NGSL\\NGSL FLUX\\LSP.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\SOURCE\\M-GIANT\\FIG3_M-G_Case_.txt";//mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_

	char** ptr=new char* [5000];
	//UINT  Count_str=0;
	//UINT  Count_err=0;
	//double dW,dB,dV,dR;
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
		//      return;
	}
	if (fopen_s(&spcf, FileSpect, "r") !=0 ){
		AfxMessageBox(_T("Error Opening Input File")); 
		//      return;
	}
	//if (fopen_s(&errf, ErrFile, "w") !=0 ){
	//	AfxMessageBox(_T("Error Opening ErrFile")); 
	//	//      return;
	//}
#define LLEN 100000 //2048
	char InLin1[LLEN];
	//lam M0 M0c M1 M1c M2 M2c M3 M3c M4 M4c M5 M5c M6 M6c M7 M7c M8 M8c M9 M9c M10 M10c 

	//char INLIN[LLEN];
	UINT Count_str=CountDataFile(inpf);
	UINT Count_str_Lam=CountDataFile(spcf);
	//char** Data0 = new char*[Count_str];        
	//InpData(Data0,inpf, Count_str, NULL); // Ввод файла со спектрами
	//fclose(inpf);
	//double *Lambda=new double[Count_str];
	char NameFale[]="D:\\MOSH\\SOURCE\\PICLES FLUX\\PICLES FLUX NORM\\";//M-GIANT\\M-GIANT FLUX NORM\\";
	char NF[200];
	fgets(InLin1,LLEN,inpf);//Заголовок
	double *Lambda=new double[Count_str_Lam];
	//char **namefale=new char*[NUMSP];
	char namefale[11];//=new char [Count_str];
	UINT NUMFlux=0;
	for(UINT i=0;i<Count_str_Lam;i++){
		fgets(InLin1,LLEN,spcf);	//fgets(InLin,len,inpf);//Заголовок
		sscanf_s(InLin1, "%lf",&Lambda[i]);//%lf
	}
	for(UINT i=0;i<Count_str;i++){ //НАЧАЛО цикла по строкам спектров
		fgets(InLin1,LLEN,inpf);
		NUMFlux=StrPtr(InLin1,'|',ptr)-1;//число Flux без имени спектра в строке InLin
		double *Flux=new double[NUMFlux];
	//}
//#ifdef YIGOGO
	//for(UINT i=0;i<Count_str;i++){
		for(UINT j=0;j<NUMFlux;j++){
			sscanf_s(ptr[j+1], "%lf",&Flux[j]);
		}
		//namefale=new char[10];
		memset(namefale,'\0',11);
		int nn=ptr[1]-ptr[0]-1;
		memcpy(namefale,ptr[0],ptr[1]-ptr[0]);		
		namefale[nn]=0;
		memset(NF,'\0',100);
		memcpy(NF,NameFale,strlen(NameFale));
		strcat(NF,namefale);
		if (fopen_s(&outf, NF, "w") !=0 ){
			AfxMessageBox(_T("Error Opening Out File"));
			return;
		}
		//Нормировка на 555нм+-50
		double FL5550=	Integral_M(Flux,Lambda,NUMFlux,505.0,605.0);//505.0,605.0);
		for(UINT j=0;j<NUMFlux;j++){
			fprintf(outf,"%7.1lf  %9.4e\n",Lambda[j],Flux[j]*100/FL5550);
		}
		fclose(outf);
		delete [] Flux;	
	}
//	}
	//for(UINT j=0;j<NUMSP;j++){
	//	memcpy(namefale[j],ptr[j+1],6);//strlen(data[i])(void*)
	//	namefale[j][6]=0;
	//}
	//for(UINT i=1;i<Count_str;i++){//Count_str
	//memset(NF,'\0',100);
	////memset(NF,0,100);
	//memset(namefale,0,13);
	//int ST=strlen(ptr[0]);
	//memcpy((void*)namefale,(void*)ptr[1],10);//strlen(data[i])
	//for(UINT j=0;j<NUMSP;j+=2){
	//	memset(NF,'\0',100);
	//	memcpy(NF,NameFale,strlen(NameFale));
	//	strcat(NF,namefale[j]);
	//	strcat(NF,".txt");
	//	//NF=_T(NameFale+namefale);
	//	//if(NUMSP!=0){
	//	//fseek(inpf, 0L, SEEK_SET);
	//	//fgets(InLin,len,inpf);
	//	//for(UINT i=0;i<Count_str-1;i++){
	//	//	fgets(InLin,len,inpf);
	//	//	//if(i==5000){
	//	//	//	i=i;
	//	//	//}
	//	//	if (fopen_s(&errf, ErrFile, "a") !=0 ){
	//	//		AfxMessageBox(_T("Error Opening ErrFile")); 
	//	//		//      return;
	//	//	}
	//	//	fprintf(errf,"%s",InLin);
	//	//	fclose(errf);
	//	//	StrPtr(InLin,'|',ptr);
	//	//	sscanf_s(ptr[0], "%lf",&Lambda[i]);//%lf
	//	//	sscanf_s(ptr[j+1], "%lf",&Flux[i]);//%lf
	//	//}
	//	//} ВЫдача файла спектров
	//	if (fopen_s(&outf, NF, "w") !=0 ){
	//		AfxMessageBox(_T("Error Opening Out File"));
	//		return;
	//	}
	//	//Нормировка на 555нм
	//	double FL5550=	Integral_M(Flux,Lambda,Count_str,505.0,605.0);//505.0,605.0);
	//	//double FL5550=GetFlux(555.0,LamSt,Flux,Count_strSp);
	//	for(UINT i=0;i<Count_str-1;i++){
	//		fprintf(outf,"%7.1lf  %9.4e\n",Lambda[i],Flux[i]*100*80/FL5550);
	//		//if(i==4681)
	//		//	i=i;
	//	}
	//	fclose(outf);
	//}
	//delete [] Lambda;
	////de_allocate(Data0,Count_str);
	//delete [] Flux;	
	////de_allocate(namefale,NUMSP);
//#endif
	delete [] Lambda;
	_fcloseall;
}
#endif
#ifdef EMOE6
void CMyTeMath::TESTING()
{//Для выборки из больших файлов 
	FILE *inpf,*outf,*errf;	
	//char FileSpect[]="D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	//char FileInput[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\B_C_090215_3Double_2Cod.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	//char OutFile[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\BC_090215_3DBL_2Cod.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BOARD_13+W+HIP+BS+2MASS+U.txt";//BCK7m+HYP+C+TYC+JHK.txt";//\\BOARD_CAT_m7.txt";//DATASHIT\\BC_13_WORK_2.txt";//EXCESS_1.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_1.txt";//B_C_090215_3DBL_2HM_6m_CODMT.txt";//BOARD_CATALOG4_090215_3_D_6m.txt";//BC_NEW_DBL_COORD_2s_COD_.txt";//B_C_090215_3DBL_2Cod.txt";//B_C_090215_3Double.txt";//BOARD_hd_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J.txt";//LAJA2_pm08_W_TycJHK.txt";//LAJA1_W_TycJHK.txt";//TestAUE.txt";//"D:\\MOSH\\STAR_TRACK\\MT\\MT\\DATASHIT\\DS1_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_GI.txt";//BOARD_CAT_m7_W_TJHK.txt";//DATASHIT\\BC_13_WORK_2E.txt";//SOURCE\\EXCESS_A.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\MT_COD_NEW_.txt";//BOARD_CAT_NEW_TYC_COORD_27_2_Esort_RA_2.txt";//B_C_090215_3DBL_2HM_6m_CODMT__.txt";//BOARD_CATALOG4_090215_3_D_6mE.txt";//BC_090215_3DBL__2sCOD.txt";//BOARD_hd1_CATALOG4_090215_1.txt";//BSC5_CAT_RI_J1.txt";//LAJA3_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q_.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	char ErrFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_REV.txt";//\\BOARD_CAT_b6.75_REV.txt";//STAR TRACK\\MT\\MT\\DATASHIT\\NOT_FOUND.txt";//Q_
	char** ptr=new char* [5000];
	char** ptr1=new char* [5000];
	char OutStr[2048];
	char buff[2048];
#define NOUT0 31
	//typedef struct Date {
	//	//bool FlagYes;
	//	char Name[14];
	//	double Ri[MaxBand];
	//	double RO[MaxBand];
	//} DSP;
	// ВВод файла исходных величин BCK7m+HYP+C+TYC+JHK.txt"; 
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	//char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	//InpData(Data0,inpf, Count_str, NULL); 
	//fclose(inpf);

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out w File")); 
		//      return;
	}
	//if (fopen_s(&errf, ErrFile, "w") !=0 ){
	//	AfxMessageBox(_T("Error Opening Out File")); 
	//	//      return;
	//}
	//char* Data0[len];// = new char[Count_str];
	//	fgets(InLin,len,inpf);// Съели ЗАГОЛОВОК
	//	//fprintf(outf,"%s",InLin);
	//	UINT NColumn=	StrPtr(InLin,'|',ptr);//подсчёт колонок в исходном файле
	////						0		1		2		3		4		5	6		7		8	9		10
	fgets(InLin,len,inpf);// Съели ЗАГОЛОВОК
	//fprintf(outf,"%s",InLin);
	char QFlag[4];
	UINT MinF=100000;
	UINT A[3];
	UINT NColumn=	StrPtr(InLin,'|',ptr);//подсчёт колонок в исходном файле
	//	напечатали ВЫХОДНОЙ ЗАГОЛОВОК
	fprintf(outf,"%s",InLin);
	char* OutStr1=_AWAY(InLin);
	fprintf(outf,"%s",OutStr1);
	for (UINT i=1;i<Count_str;i++){ // начало цикла по 
		fgets(InLin,len,inpf);		//съели строку ПОСЛЕ ЗАГОЛОВКА
		//if(InLin[0]=='*') continue;
		StrPtr(InLin,'|',ptr);//StrPtr(Data0[i],'|',ptr);	sscanf_s(ptr[0], "%lf",&Lambda[i]);
		memcpy(QFlag,ptr[41],4);
		QFlag[3]=0;
		//for(UINT i=0;i<Count_str;i++){//Начало цикла вычислений
		for(UINT k=0;k<3;k++){
			A[k]=QFlag[k];
		//	//MinF=MIN(MinF,A[k]);
		//}
		//for(UINT k=0;k<3;k++){
			UINT d=A[k]-'A';
			if(d!=0){
				fputs(InLin,outf);
				break;
			}
				
		}
		fclose(outf);
		if (fopen_s(&outf, OutFile, "a") !=0 ){
			AfxMessageBox(_T("Error Opening Out a File")); 
			//      return;
		}


		//if(sscanf(ptr[40],"%d",&QFlag)!=NULL){
		//	//if(QFlag!="AAA")
		//	fputs(InLin,outf);
		////bool FlagQW[5]={true,true,true,true,true};  //V 14|  W_B |  B_V | V_R  
		//bool FlagQc[4]={true,true,true,true};//Vc| Vc_Rc| Rc_Ic| Vc_Ic
		//bool FlagQh[7]={true,true,true,true,true,true,true};//Vh|  B_Vh| eBVh|V_Ih|eV_I|26 Hp  | e_Hp
		//bool FlagQt[10]={true,true,true,true,true,true,true,true,true,true};//
	}
	fcloseall();
	exit(1);
}
#endif
char* CMyTeMath::_AWAY(char* NameStr) {
    char Outstr[16384];
    //int NStr_len=0;
    int NStr_len = strlen(NameStr);
    int j = 0;
    for (int i = 0; i < NStr_len; i++) {
	if (NameStr[i] == ' ' || NameStr[i] == '*') continue;
	else {
	    Outstr[j] = NameStr[i];
	    j++;
	}
    }
    Outstr[j] = 0;
    return Outstr;
}
double CMyTeMath::CorrColorSp(double Sp, UINT NRC, UINT NCln, UINT NOut, double InColor, double** RedCol/*,bool Flg0*/) {
    double X = 0;
    double ColOut = 0;
    for (UINT i = 0; i < NRC;)//i++
    {
	double dSp1 = Sp - RedCol[i][0];
	double dSp2 = Sp - RedCol[i + 10][0];
	if (dSp1 > 100.0) {
	    i += 10;
	    continue;
	}
	else {
	    if (dSp1 > 10 && dSp2 > 10) {
		i += 10;
		continue;
	    }
	    else {
		if (dSp1 >= 0 && dSp2 < 10) {}
	    }

	}
	// && dSp1>10.0
	//while (dSp1>100.0){
	//	dSp1=Sp-RedCol[i][0];
	//	i+=10;
	//}
	//}else( 
	if (dSp1 <= 10.0 && dSp1 >= 1.0) {
	    for (int r = 0; r < 10; r++) {
		if (InColor > RedCol[i + r][NCln] && InColor > RedCol[i + r + 1][NCln])continue;
		if (InColor >= RedCol[i + r][NCln] && InColor < RedCol[i + r + 1][NCln]) {
		    X = LinInt(0.1 * r, 0.1 * (r + 1), RedCol[i + r][NCln], RedCol[i + r + 1][NCln], InColor);
		    ColOut = LinInt(RedCol[i + r][NOut], RedCol[i + r + 1][NOut], 0.1 * r, 0.1 * (r + 1), X);
		}
	    }

	}
    }
    return ColOut;
}
#ifdef MAZAFAKA
void CMyTeMath::TESTING()
{	//Вычисление исправленных JHK => JHKs
	FILE *inpf,*outf;	
	char FileSpect[]="D:\\MOSH\\STAR TRACK\\MT\\MT\\DATASHIT\\COLOR1_JHK_RED_0_1.txt";//NGSL\\NGSL_Spectra.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\спектры Pickles.txt";//uvk_pic_spc.txt";//mecalculateprogramm\\MyTe\\RES_Rho.txt";//DATA_200889_SHORT.txt";//DAT_WE-fon.txt";//DATA_TOT_Fcorr.txt";//DAT_6000.txt";
	char FileInput[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_GI_Not_AAA_S.txt";//BOARD_CATALOG4_090215_3_SUM_GI_ABSEND.txt";//LAJA4_pm08_W_TycJHK.txt";//DSF_Sp_JHKs_Q.txt";//DS1_JHK_Q.txt";//BOARD_CATALOG_170115_1.txt";//SOURCE\\NGSL\\NGSL_Lambda.txt";//STAR TRACK\\МАЛЫЙ ДАТЧИК\\LamSt.txt";//SOURCE\\спектры Pickles.txt";////uvk_pic_grd.txt";
	char OutFile[]="D:\\MOSH\\ДОГОВОР ГЕОФИЗИКА\\DATASHIT\\BCK7m+HYP+C+TYC+JHK_GI_Not_AAA_CORR.txt";//BOARD_CATALOG4_090215_3_SUM_GI_ABSEND_JHKs.txt";//DS1_AAA.txt";//D:\\mecalculateprogramm\\MyTe\\RES_Rho_OUT.txt";
	//char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	typedef struct Date {
		//bool FlagYes;
		char Name[14];
		char Spect[4];
		char QFlag[4];
		//char QFlagOut[4];
		double SpV;
		double Red;
		double J,H,K;
		double Ri[MaxBand];//J_H,H_K,J_K
		double RO[MaxBand];//JO,HO,KO;//
	} DSP;

	char** ptr=new char* [5000];
	//#define LLEN 100000
	//	 char INLIN[LLEN];
	// ВВод файла исходных величин
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	}
	UINT Count_str=CountDataFile(inpf);
	char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	fclose(inpf);
	// ВВод файла нормальных покрасненных цветов
	if (fopen_s(&inpf, FileSpect, "r") !=0 ){
		AfxMessageBox(_T("Error Opening SP File"));
		//      return;
	}
	UINT Count_strSp=CountDataFile(inpf);
	char** Data1 = new char*[Count_strSp];        // STEP 1: SET UP THE ROWS.
	InpData(Data1,inpf, Count_strSp, NULL); 
	fclose(inpf);

	//if(Data1[1]>Data1[2]){}
	//double J,H,K,X;///*J_H,H_K,J_K*/,
	char SP[4];
	char QFlag[4];
	char Name[19];

	if (fopen_s(&outf, OutFile, "w") !=0 ){
		AfxMessageBox(_T("Error Opening Out File")); 
		//      return;
	}
	DSP* DSpec=new DSP [Count_strSp];//НОМЕРА колонок для файла D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt
	for(UINT i=0;i<Count_strSp;i++){
		if(StrPtr(Data1[i],'|',ptr)!=0){
			memcpy(DSpec[i].Spect,ptr[0],3);
			DSpec[i].Spect[3]=0;
			sscanf_s(ptr[1], "%lf",&DSpec[i].Red);
			for(UINT j=0;j<3;j++){
				sscanf_s(ptr[j+2], "%lf",&DSpec[i].Ri[j]);//J_H, H_K,J_K
			}
		}
	}
	//SpSearch(N , P)
	DSP* DS=new DSP [Count_str];
	//НОМЕРА колонок для файла BCK7m+HYP+C+TYC+JHK_GI_Not_AAA.txt   \\D:\\MOSH\\STAR_TRACK\\MT\\MT\\DSF_Sp_JHKs_Q.txt
	//	LAJA4_pm08_W_TycJHK.txt
	for(UINT i=0;i<Count_str;i++){
		if(StrPtr(Data0[i],'|',ptr)!=0){
			//sscanf_s(ptr[1], "%lf",&DS[i].Red);
			//memcpy(DS[i].Spect,ptr[21],3);
			memcpy(DS[i].Spect,ptr[13],3);
			DS[i].Spect[3]=0;
			if(DS[i].Spect[0]==' '){DS[i].Spect[0]='A';DS[i].Spect[1]='5';}
			if(DS[i].Spect[2]==' ')DS[i].Spect[2]='3';
			DS[i].SpV=SpSearch_d(DS[i].Spect);
			memcpy(DS[i].Name,ptr[0],16);
			DS[i].Name[16]=0;
			//memcpy(DS[i].QFlag,ptr[21],3);
			memcpy(DS[i].QFlag,ptr[42],3);
			DS[i].QFlag[3]=0;
			for(UINT j=0,k=0;j<3;j++,k++){
				//sscanf_s(ptr[j+8+k], "%lf",&DS[i].Ri[j]);
				f_sscans(ptr[36+2*j], "%lf",&DS[i].Ri[j]);
				DS[i].RO[j]=DS[i].Ri[j];
			}
		}
	}
	double **RedCol=new double* [Count_strSp];
	for(UINT i=0;i<Count_strSp;i++)
		RedCol[i]=new double [5];
	for(UINT i=0;i<Count_strSp;i++){
		RedCol[i][0]=SpSearch_d(DSpec[i].Spect);
		//double cc=0;
		//sscanf_s(&DSpec[i].Spect[2],"%lf",cc);
		//RedCol[i][0]=100*cc+c;
		RedCol[i][1]=DSpec[i].Red;
		for(UINT k=0;k<3;k++)
			RedCol[i][k+2]=DSpec[i].Ri[k];
	}
	int A[3];
	int MinF=11111; 
	fprintf(outf,"Name|QFl|J|H|K|Js|Hs|Ks|\n");
	for(UINT i=0;i<Count_str;i++){//Начало цикла вычислений
		for(UINT k=0;k<3;k++){
			A[k]=DS[i].QFlag[k];
			MinF=MIN(MinF,A[k]);
		}
		for(UINT k=0;k<3;k++){
			A[k]-=MinF;
		}
		//typedef {DS[i].Ri[0]-DS[i].Ri[2]}
		//if(A[0]==0 && A[1]==0 && A[2]==0)//AAA
		//{ double CMyTeMath::CorrColorSp(char* Sp, UINT NRC,UINT NCln,UINT NOut,double InCol, double ** RedCol){
#define J_H 2
#define H_K 3
#define J_K 4
		if(A[0]<=1 && A[1]<=1 && A[2]<=1){//AAA DDD BBB CCC EEE FFF UUU [ABB CDD UUF EDU?]
			for(UINT k=0;k<3;k++) 
				DS[i].RO[k]=DS[i].Ri[k];
			//continue;
		}
		if(A[0]>1 && A[1]>1 && A[2]==0){//DEA 'K' + цвета по  Х=0 для данного Sp
			DS[i].RO[0]=DS[i].Ri[2]+CorrColorSp(DS[i].SpV,Count_strSp, H_K,J_K,999, RedCol);//Js K+J_K
			DS[i].RO[1]=DS[i].Ri[2]+CorrColorSp(DS[i].SpV,Count_strSp, J_K,H_K,999, RedCol);//Hs K+H_K
		}
		if(A[0]==0 && A[1]>1 && A[2]>1){//ADE 'J' + цвета по  Х=0 для данного Sp
			DS[i].RO[1]=DS[i].Ri[0]-CorrColorSp(DS[i].SpV,Count_strSp, H_K,J_H,999, RedCol);//Hs J-J_H
			DS[i].RO[2]=DS[i].Ri[0]-CorrColorSp(DS[i].SpV,Count_strSp, J_K,J_K,999, RedCol);//Ks J-J_K
		}

		if(A[0]>1 && A[1]==0 && A[2]>1){//DAE 'H' + цвета по  Х=0 для данного Sp
			DS[i].RO[0]=DS[i].Ri[1]+CorrColorSp(DS[i].SpV,Count_strSp, H_K,J_H,999, RedCol);//Js H+J_H
			DS[i].RO[2]=DS[i].Ri[1]-CorrColorSp(DS[i].SpV,Count_strSp, J_K,H_K,999, RedCol);//Ks H-H_K
		}

		//CorrColorSp(char* Sp, UINT NCln,UINT NOut,double InCol, double ** RedCol)
		if(A[0]>1 && A[1]<=1 && A[2]<=1){//EAA
			//DS[i].RO[]=CorrColorSp(DS[i].Spect,Count_strSp, 4,2,DS[i].Ri[1]-DS[i].Ri[2], RedCol);//H_K
			double RO1=DS[i].Ri[2]+CorrColorSp(DS[i].SpV,Count_strSp, H_K,J_K,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Js K+J_K
			double RO2=DS[i].Ri[1]+CorrColorSp(DS[i].SpV,Count_strSp, H_K,J_H,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Js H+J_H
			DS[i].RO[0]=(RO1+RO2)/2;//Js
		}
		if(A[0]<=1 && A[1]>1 && A[2]<=1){//AEA
			double RO1=DS[i].Ri[0]-CorrColorSp(DS[i].SpV,Count_strSp, J_K,J_H,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Hs J-J_H
			double RO2=DS[i].Ri[2]+CorrColorSp(DS[i].SpV,Count_strSp, J_K,H_K,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Hs K+H_K
			DS[i].RO[1]=(RO1+RO2)/2;//Hs
		}
		if(A[0]<=1 && A[1]==0 && A[2]>1){//BAE EDU
			//DS[i].RO[]=CorrColorSp(DS[i].Spect,Count_strSp, 3,2,DS[i].Ri[0]-DS[i].Ri[1], RedCol);//J_H
			double RO1=DS[i].Ri[0]-CorrColorSp(DS[i].SpV,Count_strSp, J_H,J_K,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Ks J-J_K 
			double RO2=DS[i].Ri[1]-CorrColorSp(DS[i].SpV,Count_strSp, J_H,H_K,DS[i].Ri[0]-DS[i].Ri[2], RedCol);//Ks H-H_K
			DS[i].RO[2]=(RO1+RO2)/2;//Ks
		}
		//}

		//for(UINT i=0;i<Count_str;i++){
		//	//if(DS[i].QFlag=="AAA"){
		//	//for(UINT j=0;j<3;j++){
		//	if((DS[i].QFlag[0]==DS[i].QFlag[1])&&(DS[i].QFlag[0]==DS[i].QFlag[2])) {
		//		for(UINT k=0;k<3;k++){
		//			DS[i].RO[k]=DS[i].Ri[k];
		//		}
		//	}

		//int CG=0;
		//for(UINT k=0;k<3;k++){
		//	char NC = DS[i].QFlag[k];
		//	switch(NC){
		//		case 'A':
		//		case 'B':
		//		case 'C':
		//		case 'D':
		//		case 'E':
		//		case 'F':
		//		case 'U':
		//			CG++;
		//			//DS[i].RO[k]=DS[i].Ri[k];				
		//			break;
		//		default: DS[i].RO[k]=0;
		//			break;
		//	}
		//}	
		//if(CG==3){
		//	for(UINT k=0;k<3;k++) DS[i].RO[k]=DS[i].Ri[k];
		//} 
		fprintf(outf,"%s|%s| ",DS[i].Name,DS[i].QFlag);
		for(UINT k=0;k<3;k++){
			fprintf(outf,"%10.5lf| ",DS[i].Ri[k]);
		}
		for(UINT k=0;k<3;k++){
			fprintf(outf,"%10.5lf|",DS[i].RO[k]);
		}
		fputs("\n",outf);
	}
	#ifdef EMOE
	int ic=0;
	UINT p1,p2,cc;
	UINT m=0;
	cc=SpSearch(DS[i].Spect[0],DS[i].Spect[1]);
	if(DS[i].Spect[2]==' '){
		if (cc>58){ ic=2;
		} else	ic=4;
	}else {
		sscanf_s(&DS[i].Spect[2],"%1d", &ic);
		if(ic<=0) 
			ic=ic;
		ic-=1; // Класс светимости
	}
	while(1){
		if(SP_Cod[ic][m+1]<0){
			for(UINT j=0;j<DLRSP;j++){
				DS[i].SpED[j]=SpSV[SP_Cod[ic][m]][j];
				//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",DS[i].Spect,SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
			}
			break;//return;
		}else{
			p1=SpSearch(NameSp[SP_Cod[ic][m]][0],NameSp[SP_Cod[ic][m]][1]);
			p2=SpSearch(NameSp[SP_Cod[ic][m+1]][0],NameSp[SP_Cod[ic][m+1]][1]);
			if( (cc>=p1)&&(cc<p2) ){
				for(UINT j=0;j<DLRSP;j++){
					DS[i].SpED[j]= LinInt(SpSV[SP_Cod[ic][m]][j],SpSV[SP_Cod[ic][m+1]][j],
						(double)p1,(double)p2,(double)cc);
				}
				break;//	      return;
			}else{ m++;}
		}
	}



	//#ifdef EMOE
	switch(DS[i].QFlag){
		case "AAA"
			//Кусок для вычисления цветов JHK из файла D:\\MOSH\\SOURCE\\Value_for_JHK_RED_0.1.txt  
			//с выдачей в D:\\MOSH\\SOURCE\\COLOR_JHK_RED_0.1.txt
			fprintf(outf,"SP|X|J_H|H_K|J_K|\n");
			for(UINT i=0;i<Count_strSp;i++){
				if(StrPtr(Data0[i],'|',ptr)!=0){
					sscanf_s(ptr[1], "%lf",&X);
					memcpy(SP,ptr[0],3);
					SP[3]=0;
					sscanf_s(ptr[2], "%lf",&J);
					sscanf_s(ptr[3], "%lf",&H);
					sscanf_s(ptr[4], "%lf",&K);
				}
				//J_H=J-H;H_K=H-K;J_K=J-K;				
				fprintf(outf,"%s|%6.3lf|%10.5lf|%10.5lf|%10.5lf|\n",SP,X,J-H,H-K,J-K);
			}
#endif
			fclose(outf);
			_fcloseall();

	}
#endif
	void CMyTeMath::TESTING()
	{	//Транспонирование матрицы T=>T(-1)
	    FILE* inpf, * outf;
	    //	char FileSpect[]="
	    //	char FileInput[]="D:\\MOSH\\SOURCE\\Sp_Pickles220517.txt";//
	    char OutFile[] = "D:\\MOSH\\SOURCE\\Sp_Pickles_T.txt";//

	    char** ptr = new char* [200];//131 Pickles
	    //#define LLEN 100000
	    //	 char INLIN[LLEN];
#ifdef EMOE
	// ВВод файла исходных величин
	    if (fopen_s(&inpf, FileInput, "r") != 0) {
		AfxMessageBox(_T("Error Opening CAT File"));
		//      return;
	    }
	    UINT Count_str = CountDataFile(inpf);//131 строк по 4772 числа
	    char** Data0 = new char* [Count_str];        // Входная матрица ||O5V| 123.23|132.33|..| ||
	    InpData(Data0, inpf, Count_str, NULL);
	    fclose(inpf);
	    UINT Count_column = StrPtr(Data0[0], '|', ptr);// 4772 колонок 
	    double** SubDat = new double* [Count_column]; //n Sp 131  Выходная матрица  ||O5V___|B01___|...||
	    for (UINT i = 0; i < Count_column; i++) { //N lambda //4772						 ||123.23|112.33|...||
		SubDat[i] = new double[Count_str];
	    }
	    char** SDatName = new char* [Count_str]; // 
	    for (UINT i = 0; i < Count_str; i++) { //4772					
		SDatName[i] = new char[7];
	    }
	    for (UINT i = 0; i < Count_str; i++) {//131
		if (StrPtr(Data0[i], '|', ptr) != 0) {
		    memcpy(&SDatName[i], ptr[0], 6);
		    SDatName[i][6] = 0;
		    for (UINT j = 1; j < Count_column - 1; j++) {//4772
			sscanf_s(ptr[j], "%lf", &SubDat[i][j]);//4772
		    }
		}
	    }
	    for (UINT i = 0; i < Count_str; i++) {//131
		fprintf(outf, "%11s|", SDatName[i]);
		fputs("\n", outf);
	    }
#endif
	    if (fopen_s(&outf, OutFile, "w") != 0) {
		AfxMessageBox(_T("Error Opening Out File"));
		//      return;
	    }
	    for (UINT j = 0; j < 4772; j++) {//4772
    //		for(UINT i=0;i<Count_str;i++)//131
		fprintf(outf, "a%d|", j);
	    }
	    fputs("\n", outf);
	    fclose(outf);

	    _fcloseall();

	}
// ПРОГРАММА ВЫНЕСЕНИЯ ЗА АТМОСФЕРУ "SIRIUS" OH-8
double CMyTeMath::PROBKA(vector <CMyTeBand> Band)		//CMyTeBand* Band
{
//#ifdef ETBOE
	//CMyTeBand* Band;
//	long double Pi2=6.283185307179586476925286766559;
	//UINT BandCount=4;

	//FILE *inpf;
	//int len=2048;
	//int NCRV=4;
	char InLin[len];
	int SP_Cod[5][36]={{0,80,81,82,83,84,85,86,87,88,89,90,
										91,92,93,94,95,96,97,99,-1},
								{0,70,71,46,47,48,49,72,73,52,74,
										75,76,77,78,79,99,-1},
								{0,70,71,46,47,48,49,50,51,52,53,
										54,55,56,57,58,59,60,61,62,63,
	                  64,65,66,67,68,69,99,-1},
								{0,70,71,46,47,34,35,36,37,38,39,
										40,41,42,43,44,45,60,65,-1},
								{0,1,2,3,4,5,6,7,8,9,10,11,12,13,
										14,15,16,17,18,19,20,21,22,23,
	                  24,25,26,27,28,29,30,31,32,33,99,-1} };
	bool FlagYes=false; 
	//Исходный файл с наблюдениями по ночам:
	//char FileInput[]="D:\\mecalculateprogramm\\MyTe\\DATA_TOTAL_140627.txt";//DATA_TOTAL_140623.txt";//DATA_TOTAL_140513  DATA_050187_ALL.txt";//DATA_050187_ALL.txt";//DATA_050187.txt";////DATA_TOTAL_140224_LSQ.txt";//DATA_TOTAL_140402_.txt";//DATA_TOTAL_140224.txt";//DATA_TOTAL_Fcorr.txt";//
	char FileInput[]="D:\\MOSH\\СИРИУС NEW\\ОБРАБОТКА\\DATA SHIT\\DATA_TOTAL_140627.txt";//DATA_TOTAL_140623.txt";//DATA_TOTAL_140513  DATA_050187_ALL.txt";//DATA_050187_ALL.txt";//DATA_050187.txt";////DATA_TOTAL_140224_LSQ.txt";//DATA_TOTAL_140402_.txt";//DATA_TOTAL_140224.txt";//DATA_TOTAL_Fcorr.txt";//
	//Файл типов звезд:
	char FileTypes[]="D:\\mecalculateprogramm\\MyTe\\CAT_LAST_.txt";//CATALO_2000_2404.txt";//_S_Q
	//файл величин из старого каталога:
	char FileWBVR[]="D:\\mecalculateprogramm\\MyTe\\CAT_WBVR.txt";
	//Файл спектров из Свидерскене
	char FileSpect[]="D:\\mecalculateprogramm\\MyTe\\SP_SV_nm.txt";
	// Список первоочередных звезд
	char FileListDP[]="D:\\mecalculateprogramm\\MyTe\\LIST_CAT_LAST_.txt";//LIST_6485_hd_LSQ.txt";//LIST_6485_hd.txt";////LIST_WBVR8766_hd.txt";
	//вспомогательные файлы для промежуточного контроля
	char ErrFile[]="D:\\mecalculateprogramm\\MyTe\\NOT_FOUND.txt";//Q_
	char OutFile[]="D:\\mecalculateprogramm\\MyTe\\RES.txt";
	char OutNormFile[]="D:\\mecalculateprogramm\\MyTe\\RES_140627_.txt";//050187_LSQ.txt";//140513 RES_200889.txt";//RES_N_6000.txt";//RES_Q_N_6000.txt";
	//файл конечной обработки всего каталога:
	char OutNormEndFile[]="D:\\mecalculateprogramm\\MyTe\\RES_TOTAL_140627_.txt";//RES_050187_LSQ.txt";//RES_200889_DIFF.txt";//RES_Q_DIFF_6000.txt";//RES_Q_DIFF_6000.txt";//RES6000_N_End.txt";
    CMyTeBand Extint;// пользоваться как обычным бандом
    // задаём Имя файла средней экстинкции
    Extint.FName=_T("D:\\mecalculateprogramm\\MyTe\\EXTINCT_nm.txt");
    if(Extint.FName.size()) {
        // загружаем файл
        Extint.LoadFromFile(Extint.FName);	
    }   
//БЛОК №1 ВВОД ДАННЫХ
//_____________________
//создаем массивы входных данных 
//Структура строки наблюдений
		typedef struct Date0 {
			bool FlagYes;
			char Name[14];
			char Spect[4];
			char DDMOYY[7];
			char HHMMSS[7];
			char KLAD[2];
			char STAND[2];
			int  N_P;
			double TC;
			double* SpED;
			double UT;
			double JD;
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double Bt;
			double Vt;
			double Ri[MaxBand];
			double RO[MaxBand];
			double RM[MaxBand];
			double RD[MaxBand];
			double RC[MaxBand];
			double* eR;
			double TimeAngle;
			double Mz;
			double Z;
			double PA;
			double SidTime;
		} Date0;
		typedef struct WBVR_O{
			char Name[12];
			double RO[MaxBand];
		}WBVR_O;
//Структура типов звезд
		typedef struct TYPES{
			char Name[12];
			char TyName[13];
			char TSp[4];
			double RA;
			double DE;
			double pmRA;
			double pmDE;
			double eRA;
			double eDE;
			double Bt;
			double Vt;
			double eBt;
			double eVt;
		}TYPES;
//***********************************
// $ 1.1 ВВод данных по всем ночам 
//***********************************
//открываем входной файл 
	if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
		exit(1);//0U);
		//AfxMessageBox(_T("Error Opening data_total File"));
 //      return;
	}
	//загружаем ВСЕ строковые данные (включая управляющие символы типа {,},...)
	//в массив Date0, кроме помеченных '*' в начале строки 
// Подсчёт кол-ва строк в файле прототипов
	UINT Count_str=CountDataFile(inpf);
	 char** Data0 = new char*[Count_str];        // STEP 1: SET UP THE ROWS.
	InpData(Data0,inpf, Count_str, NULL); 
	 fclose(inpf);
// $ 1.2 ВВод данных типов звезд
//***********************************
	//открыли файл типов с координатами
	 if(fopen_s(&inpf, FileTypes, "r") !=0 ){ 
		AfxMessageBox(_T("TYPES NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
			//AfxMessageBox(_T("Error Opening Coord File"));
		  return -1.0;
	 }
	// Подсчёт кол-ва строк в файле координат
	 //UINT  Count_coord=0;  
	UINT Count_coord=CountDataFile(inpf);
	 char** Data1 = new char*[Count_coord];        // STEP 1: SET UP THE ROWS.
	InpData(Data1,inpf, Count_coord, NULL); 
	 fclose(inpf);
// $ 1.3 ВВод данных спектров 
//*******************====================
	 if(fopen_s(&inpf, FileSpect, "r") !=0 ){ 
		AfxMessageBox(_T("SPECTRA NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		//AfxMessageBox(_T("Error Opening Spectra File"));
		//goto error_return;
	 }
	 fgets(InLin,len,inpf); //Name Spectra
	UINT NCS = 0;
	char *ptrS[len];
	 NCS=StrPtr(&InLin[8],' ',ptrS);
	//NCS; // Кол-во колонок в SpData без волн = число типов спектра
	char **NameSp=new char* [NCS];
		for(UINT i = 0; i < NCS; i++){
			NameSp[i]=new char [4];
			memset(NameSp[i],0,4);
		}
	//char buff[10];
	for(UINT i=0;i<NCS;i++){
		memcpy((void*)NameSp[i],(void*)ptrS[i],2);
		//NameSp[i][3]=0;
	}
	UINT DLRSP=0;
	while(1){
		if(fgets(InLin,len,inpf)==0) break;
	  ++DLRSP;
	}
	double *SpLambd = new double[DLRSP];
	double **SpSV = new double*[NCS];
	for(UINT i=0;i<NCS;i++){
		SpSV[i] = new double[DLRSP];  // STEP 2: SET UP THE COLUMNS
	}
	fseek(inpf, 0L, SEEK_SET);
	fgets(InLin,len,inpf);//съели заголовок
	for(UINT j=0;j<DLRSP;j++){
		fgets(InLin,len,inpf);
		sscanf_s(InLin,"%lf",&SpLambd[j]);
	  StrPtr(&InLin[8],' ',ptrS);
	  for(UINT i=0;i<NCS;i++){
  		if(sscanf_s(ptrS[i],"%lf",&SpSV[i][j])==NULL) {
    		SpSV[i][j]=99999.9;
		}// break;}
		//fprintf(errf,"%10.1lf ",SpSV[i][j]);
	  }
	  //fputs("\n",errf);
	}	
	if (fopen_s(&errf, ErrFile, "a") !=0 ){
			AfxMessageBox(_T("ERR NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		//AfxMessageBox(_T("Error Opening Err File"));
	}
//char SpBUFF[]="A05 "; 
//SpBUFF[3]=0;

// $ 1.4 ВВод старого каталога WBVR
 //***********************************
	 if (fopen_s(&inpf, FileWBVR, "r") !=0 ){
		AfxMessageBox(_T("WBVR NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		 //AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR
	 //UINT  Count_WBVR=0;  
	UINT Count_WBVR=CountDataFile(inpf);
	 char** Data2 = new char*[Count_WBVR];        // STEP 1: SET UP THE ROWS.
	InpData(Data2,inpf, Count_WBVR, NULL); 
	 fclose(inpf);
// $ 1.5 ВВод списка первоочередных звезд
 //***********************************
	 if (fopen_s(&inpf, FileListDP, "r") !=0 ){
		AfxMessageBox(_T("ListDP NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
		 //AfxMessageBox(_T("Error Opening WBVR File"));
	 }
	 // Подсчёт кол-ва строк в файле WBVR8766
	 //UINT  Count_WBVR=0;  
	UINT Count_DPCat=CountDataFile(inpf);
	 char** Data3 = new char*[Count_DPCat];        // STEP 1: SET UP THE ROWS.
	InpData(Data3,inpf, Count_DPCat, NULL); 
	 fclose(inpf);
//конец БЛОК №1
//***********************************

//БЛОК № 2 РАССЫЛКА ВСПОМОГАТЕЛЬНЫХ ДАННЫХ 
//***********************************
// $ 2.1 Рассылка типов
//***********************************
	char** ptr=new char* [200];
	TYPES* TYP = new TYPES[Count_coord];

	for(UINT i=0;i<Count_coord;i++){
		if(StrPtr(Data1[i],'|',ptr)!=0)
		{
			sscanf_s(ptr[3],"%lf",&TYP[i].RA);
			sscanf_s(ptr[5],"%lf",&TYP[i].pmRA);
			sscanf_s(ptr[4],"%lf",&TYP[i].DE);
			sscanf_s(ptr[6],"%lf",&TYP[i].pmDE);
			if(sscanf_s(ptr[7],"%lf",&TYP[i].Bt)==0) {
				TYP[i].Bt=0;
			}
			if(sscanf_s(ptr[8],"%lf",&TYP[i].eBt)==0) TYP[i].eBt=0;
			if(sscanf_s(ptr[9],"%lf",&TYP[i].Vt)==0) TYP[i].Vt=0;
			if(sscanf_s(ptr[10],"%lf",&TYP[i].eVt)==0) TYP[i].eVt=0;
			//sscanf_s(ptr[8],"%lf",&TYP[i].eBt);
			//sscanf_s(ptr[9],"%lf",&TYP[i].Vt);
			//sscanf_s(ptr[10],"%lf",&TYP[i].eVt);
			memcpy((void*)TYP[i].Name,(const void*)ptr[0],9);
			TYP[i].Name[9]=0;
			memcpy((void*)TYP[i].TyName,(const void*)ptr[2],12);
			TYP[i].TyName[12]=0;
			memcpy((void*)TYP[i].TSp,(void*)ptr[11],3);
			TYP[i].TSp[3]=0;
		}
	}
//#ifdef ETBOE
//#ifdef ETBOE
//#ifdef DONETKNOW
//#endif
// $ 2.2 Рассылка WBVR
//***********************************
	//создаем массив структур=строк WBVR
	WBVR_O* WbvrO = new WBVR_O[Count_WBVR];
//#endif
// Упорядочивание кривых реакции WBVR
	int WBVR[MaxBand];
	for(UINT i=0;i<MaxBand;i++){
		wchar_t NC = Band[i].Name.at(0);
		switch(NC){
			case _T('W'):
			case _T('w'):
				WBVR[0] = i;				
				break;
			case _T('B'):
			case _T('b'):
                WBVR[1] = i;
				break;
			case _T('V'):
			case _T('v'):
                WBVR[2] = i;
				break;
			case _T('R'):
			case _T('r'):
                WBVR[3] = i;
				break;
			default:break;
		}
		Search_Band(&Band[i],&Band[i].LBeg,&Band[i].LEnd);
	}
//вводим старые величины WBVR
double R[MaxBand];
for(UINT i=0;i<Count_WBVR;i++){
	bool FlagYes=false;
	if(StrPtr(Data2[i],'|',ptr)!=0){
		memcpy((void*)WbvrO[i].Name,(void*)ptr[0],11);
		WbvrO[i].Name[11] = '\0';
		for(UINT k=0;k<MaxBand;k++){
			sscanf_s(ptr[k+5],"%lf",&R[k]);
		}
	}//W=W_B+B_V+V
	WbvrO[i].RO[0]=R[1]+R[2]+R[0];
	WbvrO[i].RO[1]=R[2]+R[0];
	WbvrO[i].RO[2]=R[0];
	WbvrO[i].RO[3]=R[0]-R[3];
}
// $ 2.2 Рассылка номеров дополнительного каталога
//***********************************
char **NameDPCat=new char* [Count_DPCat];
for (UINT i=0;i<Count_DPCat;i++){
	NameDPCat[i]=new char [11];
}
for(UINT i=0;i<Count_DPCat;i++){
	if(StrPtr(Data3[i],'|',ptr)!=0){
		memcpy((void*)NameDPCat[i],(void*)ptr[0],9);
		NameDPCat[i][9]=0;
		fprintf(errf,"%s\n",NameDPCat[i]);
	}
}		//sscanf_s(Data3[k+5],"%lf",&R[k]);

// БЛОК № 3 ОБРАБОТКА ДАННЫХ
//***********************************
//наполнение DS
#define	MaxTypSt 200 // максимальное количество типов стандартов
#define	MaxSt 300	// максимальное количество наблюдений стандартов в куске
int CountSt=0;//число стандартов в куске
int CountTypSt=0;//число типов стандартов куске

UINT IOUT=0; // счетчик выходного массива обработанных строк 
Date0* DS1=0;//new Date0[Count_str]
Date0* DSI = new Date0[Count_str];// вспомогательный массив для обработки
Date0** DDST = new Date0*[MaxTypSt];// вспомогательный массив для стандартов в куске
for (UINT i=0;i<MaxTypSt;i++){
	DDST[i]=new Date0 [MaxSt];
}
Date0* DDS = new Date0[MaxSt];// вспомогательный массив для обработки
double DDD[MaxTypSt][MaxBand];// вспомогательный массив для обработки
UINT NS[MaxTypSt];
double N;
UINT N_Part=0;//количество строк в куске
UINT COUNT_PART=0;//номер куска
UINT IDPC=0; //  количество наблюдений звезд в выборке по ДоП. каталогу

UINT Beg_Part=0;// адрес начала куска
UINT ENd_Part=0;// адрес конца куска
int DD,MO,YY;
int HH,MM,SS;
int Year;
char SpBUFF[]="A05 ";
// $ 3.1 Обработка по кускам
//***********************************

for(UINT I0=0;I0<Count_str;){//НАЧАЛО ГЛОБАЛЬНОГО ЦИКЛА ПО КУСКАМ
	N_Part=0;
	if (Data0[I0][0]=='{') //НАЧАЛО СЧЕТА КУСКА
	{
		Beg_Part=I0+1;
		for (UINT j=0;;j++)
		{		
			if (Data0[j+Beg_Part][0]=='}'){
				ENd_Part=j+Beg_Part;
				break;
			}else {
				N_Part++;	//считаем число строк без управляющих символов Count_str
				continue;
			}
		}
		I0=Beg_Part;

		DS1=new Date0[N_Part];//Date0* 
		
	for(UINT i=0;i<N_Part;i++){// ЦИКЛ рассылки данных по куску
		bool FlagYes=false;
		if(StrPtr(Data0[i+I0],'|',ptr)!=0){
			memcpy((void*)&DS1[i].Name,(void*)ptr[0],11);
			DS1[i].Name[11] = '\0';
			DS1[i].SpED=new double [DLRSP];
//#ifdef MOXU
			DS1[i].FlagYes=false;
			sscanf_s(ptr[1],"%lf",&DS1[i].TC);
			memcpy((void*)&DS1[i].KLAD,(void*)ptr[2],1);
			DS1[i].KLAD[1]=0;
			memcpy((void*)&DS1[i].STAND,(void*)ptr[3],1);
			DS1[i].STAND[1]=0;
			double tau=28E-9;

			for(UINT k=0;k<MaxBand;k++){
				sscanf_s(ptr[k+4],"%lf",&DS1[i].Ri[k]);
				N=DS1[i].Ri[k];
				if (N<=0.0){
					DS1[i].Ri[k]=0.0;
				} else{
					nonlinear(DS1[i].Ri[k]);
					//do{double n=
					//}while();
					DS1[i].Ri[k]=-2.5*log10(N/(1-tau*N));// exp(N*exp(
				}
			}
			memcpy((void*)&DS1[i].DDMOYY,(void*)ptr[8],6);
			memcpy((void*)&DS1[i].HHMMSS,(void*)ptr[9],6);
			DS1[i].DDMOYY[6] = '\0';
			DS1[i].HHMMSS[6] = '\0';

			sscanf_s(ptr[8],"%2d%2d%2d",&DD,&MO,&YY);
			sscanf_s(ptr[9],"%2d%2d%2d",&HH,&MM,&SS);
			DS1[i].N_P=COUNT_PART; //  для каждой строки запоминается номер куска наблюдения
		}
		int Dt=MO*100+DD;
		if(YY<40) Year=2000+YY;
		else Year=1900+YY;
		DS1[i].UT=(double)HH+(double)MM/60.0+(double)SS/3600.0;
	//вычисляем порядковый номер дня от 0 января 1950 года
		int JD50 = JDm1950(Year, Dt);
	//Вычисляем среднее звездное гринвичское время в радианах на 0 часов UT на данное число
	//JD50 - число суток от 0 января 1950 г.
		double S0=Stims(JD50); //S0 в радианах
		double JD= (double)JD50+DS1[i].UT/24;
		DS1[i].JD=JD;
		DS1[i].SidTime=fmod( (double)(S0 + (double)(1.0027379*DS1[i].UT*
						0.2617993877983 + 1.34334356423)),(double)Pi2 );
//получение и рассылка координат звезд
		FlagYes=false;
		for(UINT j=0;j<Count_coord;j++){
			int a=strncmp(TYP[j].Name,DS1[i].Name,9);
			if(a==0){
				//DS[i].Spect = Types_Sp[j];
				memcpy(DS1[i].Spect,TYP[j].TSp,4);
				DS1[i].Spect[3]=0;
				DS1[i].RA=TYP[j].RA;
				DS1[i].DE=TYP[j].DE;
				DS1[i].pmRA=TYP[j].pmRA;
				DS1[i].pmDE=TYP[j].pmDE;
				if((DS1[i].TimeAngle = DS1[i].SidTime - TYP[j].RA * Pi/180)<0)
					DS1[i].TimeAngle += Pi2;//t=s-RA в радианах		
				DS1[i].Z =Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,0 );
				DS1[i].Mz=Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,1);
				DS1[i].PA=Mz(DS1[i].TimeAngle, SINfi, COSfi, DS1[i].DE*Pi/180,2);
				DS1[i].Bt=TYP[j].Bt;
				DS1[i].Vt=TYP[j].Vt;
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			fprintf(errf,"%s\n",DS1[i].Name);
			DS1[i].TimeAngle=0;
			DS1[i].DE=0;
			DS1[i].Mz=1.0;
			memcpy(DS1[i].Spect,SpBUFF,4);
			//DS[i].Spect=SpBUFF;
		}
		FlagYes=false;
		for(UINT j=0;j<Count_WBVR;j++){
			if(strncmp((const char*)DS1[i].Name,(const char*)WbvrO[j].Name,9)==0){
				for(UINT k=0;k<MaxBand;k++){
					DS1[i].RO[k]=WbvrO[j].RO[k];
				}
				FlagYes=true;
				break;
			}else continue;			
		}
		if(!FlagYes){
			//fprintf(errf,"%s\n",DS[i].Name);
			for(UINT k=0;k<MaxBand;k++){
				DS1[i].RO[k]=0.0;
			}
		}
//#ifdef EMOE
	//************ Выбор Типов Спектров ********************//
	//======================================================//
int ic=0;
UINT p1,p2,cc;
UINT m=0;
if(i==69){
	i=i;}
		cc=SpSearch(DS1[i].Spect[0],DS1[i].Spect[1]);
		if(DS1[i].Spect[2]==' '){
			if (cc>58){ ic=2;
			} else	ic=4;
		}else {
			sscanf_s(&DS1[i].Spect[2],"%1d", &ic);
if(ic<=0) 
	ic=ic;
			ic-=1; // Класс светимости
		}
		while(1){
  			if(SP_Cod[ic][m+1]<0){
    			for(UINT j=0;j<DLRSP;j++){
	    			DS1[i].SpED[j]=SpSV[SP_Cod[ic][m]][j];
					//fprintf_s(errf,"%s %6.1lf  %6.1lf\n",DS[i].Spect,SpLambd[j],SpSV[SP_Cod[ic][m]][j]);
				}
				break;//return;
  			}else{
				p1=SpSearch(NameSp[SP_Cod[ic][m]][0],NameSp[SP_Cod[ic][m]][1]);
				p2=SpSearch(NameSp[SP_Cod[ic][m+1]][0],NameSp[SP_Cod[ic][m+1]][1]);
  				if( (cc>=p1)&&(cc<p2) ){
					for(UINT j=0;j<DLRSP;j++){
						DS1[i].SpED[j]= LinInt(SpSV[SP_Cod[ic][m]][j],SpSV[SP_Cod[ic][m+1]][j],
							(double)p1,(double)p2,(double)cc);
					}
					break;//	      return;
				}else{ m++;}
			}
		}
//#endif
	}//КОНЕЦ ЦИКЛА рассылки данных по куску
		//fclose(errf);
//Вычисление Ai по куску
//Подготовка подинтегральных функций для вычисления Ai
double **SubintP = new double* [MaxBand];
double **Subint = new double* [MaxBand];
	for(UINT k=0;k<MaxBand;k++){
		SubintP[k] = new double [DLRSP];
		Subint[k] = new double [DLRSP];
	}
	for (UINT i = 0; i < N_Part; i++) {
	    for (UINT k = 0; k < MaxBand; k++) {
		for (UINT j = 0; j < DLRSP; j++) {
		    double a = DS1[i].SpED[j];
		    double b = Band[WBVR[k]].GetFlux(SpLambd[j]);
		    double c = Extint.GetFlux(SpLambd[j]);
		    double cc = pow(c, DS1[i].Mz);//Extint.GetFlux(SpLambd[j])
		    SubintP[k][j] = a * b * cc;
		    Subint[k][j] = a * b;
		    //DS[i].SpED[j]*Band[WBVR[k]].GetFlux(SpLambd[j])*pow(Extint.GetFlux(SpLambd[j]),DS[i].Mz);// Sp*Band*p^Mz 		sscanf(InLin,"%lf%lf",&EXT_Lambd[i],&EXT_Flux[i]);
		}
		double d = Integral_M(SubintP[k], SpLambd, DLRSP, Band[WBVR[k]].LBeg, Band[WBVR[k]].LEnd);
		double dd = Integral_M(Subint[k], SpLambd, DLRSP, Band[WBVR[k]].LBeg, Band[WBVR[k]].LEnd);
		double ddd = -2.5 * log10(d / dd);		//double dd=

		DS1[i].Ri[k] -= ddd; //-2.5*log10(Integral_M(Subint[k],SpLambd,DLRSP,Band[WBVR[k]].LBeg,Band[WBVR[k]].LEnd));
	    }
	    //		i=i;
	}	//Конец вычислений Ai по куску
		//de_allocate(SubintP,MaxBand);
		//de_allocate(Subint,MaxBand);

//#ifdef EMOE
// контрольная печать в файл ПРОБКИ
	//открыли выходной файл печати ПРОБКИ
		if (fopen_s(&outf, OutFile, "w") !=0 ){
			AfxMessageBox(_T("OUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
			//AfxMessageBox(_T("Error Opening result File"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");

//#ifdef EMOE
		for(UINT i=0;i<N_Part;i++){
			//if(strncmp((const char*)DS1[i].DDMOYY,"050187",6)==0){
			fprintf(outf,"%s|  %s| %s|",DS1[i].Name,DS1[i].KLAD,DS1[i].STAND);
			fprintf(outf,"  %7.3lf|  %12.8lf|  %12.8lf|  %8.4lf|",DS1[i].TimeAngle/Pi*12,DS1[i].RA,DS1[i].DE,DS1[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %7.3lf|",DS1[i].Ri[k]);
			}
			fprintf(outf,"%11.5lf|",DS1[i].JD);
			fprintf(outf,"%s|%s|%s",DS1[i].Spect,DS1[i].DDMOYY,DS1[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DS1[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DS1[i].Bt);
			fprintf(outf,"  %7.3lf|",DS1[i].Vt);
			fputs("\n",outf);
			//}
		}
		//fclose(outf);
//#endif
// нормализация
//		if (fopen_s(&outf, OutFile, "w") !=0 ){
//			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
////AfxMessageBox(_T("Error Opening result NormFile"));
//		}
//		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
CountSt=0;//число стандартов в куске
CountTypSt=0;//число типов стандартов куске

//#ifdef EMOE вычисление нормализованных величин для стандартов и .RM для других  
	for(UINT i=0;i<N_Part;i++){
		if(DS1[i].STAND[0]=='S'){
			for(UINT k=0;k<MaxBand;k++)	
				DS1[i].RM[k]=DS1[i].RO[k]-DS1[i].Ri[k];
			DDS[CountSt]=DS1[i];//[si]
			CountSt++;
		}else 
			for(UINT k=0;k<MaxBand;k++)	
				DS1[i].RM[k]=DS1[i].Ri[k];
	}
	double RA=0;
	double DE=0;
	for(int jk=0;jk<CountSt;jk++){ //перетряс стандартов куска
		if(!DDS[jk].FlagYes){
			RA=DDS[jk].RA;
			DE=DDS[jk].DE;
		}else continue;
		int ij=0;
		for (int k = 0; k<MaxBand; k++){
			DDD[CountTypSt][k]=0;
		}
		for (int ik = 0; ik < CountSt; ik++){
			if(DDS[ik].RA==RA && DDS[ik].DE==DE){
				DDST[CountTypSt][ij]=DDS[ik];
				for(UINT k=0;k<MaxBand;k++){
					DDD[CountTypSt][k]+=DDST[CountTypSt][ij].RM[k];
				}
				DDS[ik].FlagYes=true;
				ij++;
				NS[CountTypSt]=ij;
			}else continue;
		}
		for(int k=0; k<MaxBand;k++){
			DDD[CountTypSt][k]/=ij;
		}
		CountTypSt++;
		//if(ij<kk)
	}//конец перетряски стандартов
//#endif
	//}else 	{exit(1);}//конец куска
	//	AfxMessageBox(_T("Begin of part NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);

//}
//#ifdef EMOE
//#ifdef ETBOE
	for(UINT i=0;i<N_Part;i++){//нормализация куска
		//if(strncmp((const char*)DSI[i].DDMOYY,"050187",6)==0 &&
		//	strncmp((const char*)DSI[i].HHMMSS,"133154",6)==0){
	  //if(DS1[i].DDMOYY=="050187" && DS1[i].HHMMSS=="133154"){	
		if (DS1[i].STAND[0]=='S'){		//
			for(UINT ii=0;ii<CountTypSt;ii++){	//MaxTypSt
				if (DS1[i].RA == DDST[ii][0].RA && DS1[i].DE == DDST[ii][0].DE){
					for(UINT k=0;k<MaxBand;k++) {
						DS1[i].RM[k]=DDD[ii][k]+DS1[i].Ri[k];
					}
					break;
				}else{ continue;}
			}
		} else{	// не стандарты
			bool FlagYes=false;
			double Ro=0;
			for(UINT ii=0;ii<CountTypSt;ii++){
				double dX=cos(DS1[i].DE)*cos(DS1[i].RA)-cos(DDST[ii][0].DE)*cos(DDST[ii][0].RA);
				double dY=cos(DS1[i].DE)*sin(DS1[i].RA)-cos(DDST[ii][0].DE)*sin(DDST[ii][0].RA);
				double dZ=sin(DS1[i].DE)-sin(DDST[ii][0].DE);//RA???!!!
				double dMz=abs(DS1[i].Mz - DDST[ii][0].Mz);
				//if (/*(abs(DS[i].Mz - DDST[ii][0].Mz)<0.15)&& (abs(DS[i].RA - DDST[ii][0].RA)<20) && (abs(DS[i].DE - DDST[ii][0].DE)<15 ))*/
				Ro=2*asin(0.5*sqrt(dX*dX+dY*dY+dZ*dZ));
				if ( Ro<0.4 && dMz<0.15){//>0.5???
					if( DS1[i].JD<DDST[ii][0].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDST[ii][0].RM[k];
						}
						FlagYes=true;
						break;
					}
					if( DS1[i].JD>DDST[ii][NS[ii]-1].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDST[ii][NS[ii]-1].RM[k];
						}
						FlagYes=true;
						break;
					}
					for(int ij=0;ij<CountSt;ij++){
						if( (DS1[i].JD>DDST[ii][ij].JD) && (DS1[i].JD<DDST[ii][ij+1].JD) )/* &&*/ {
							for(int k=0;k<MaxBand;k++){
								double b=LinInt(DDST[ii][ij].RM[k],DDST[ii][ij+1].RM[k],DDST[ii][ij].JD,DDST[ii][ij+1].JD,DS1[i].JD);
								DS1[i].RM[k]+=b;
							}
							FlagYes=true;
							break;
						}else continue;//ij++;
					}
				}else continue;
				break;
			}
			if (!FlagYes){	//если не найдены подходящие стандарты - любые соседние
				for(int ij=0;ij<CountSt;ij++){
					if( (DS1[i].JD>DDS[ij].JD) && (DS1[i].JD<DDS[ij+1].JD) ){
						for(int k=0;k<MaxBand;k++){
							double b=LinInt(DDS[ij].RM[k],DDS[ij+1].RM[k],DDS[ij].JD,DDS[ij+1].JD,DS1[i].JD);
							DS1[i].RM[k]+=b;
						}
						break;
					}
					if( DS1[i].JD<DDS[0].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDS[0].RM[k];
						}
						break;
					}
					if( DS1[i].JD>DDS[CountSt-1].JD ){
						for(int k=0;k<MaxBand;k++){
							DS1[i].RM[k]+=DDS[CountSt-1].RM[k];
						}
						break;
					}
				}
			}
		}
	  //}
//i=I+KK;
	}//конец нормализации куска 
//формирование выходного массива
		//for(UINT i=0;i<N_Part;i++,IOUT++){
		//	DSI[IOUT]=DS[i];
		//	//delete DS;
		//}
		//delete DS;
	for(UINT j=0/*,ij=0*/;j<Count_DPCat;j++){ // j-по звездам ДПК ij- по количеству звезд в наблюдениях из ДПК
		for(UINT i=0;i<N_Part;i++){							//  ii- по числу mi кроме нормстар hd 5015
			if(strncmp((const char*)DS1[i].Name,(const char*)NameDPCat[j],9)==0){
				DSI[IDPC]=DS1[i];
				//ij++;
				IDPC++;
			}else continue;
		}
	}
//	контрольная выдача величин
//		if (fopen_s(&outf, OutFile, "a") !=0 ){
//			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
////AfxMessageBox(_T("Error Opening result NormFile"));
//		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
			for(UINT i=0;i<N_Part;i++){
			//if(strncmp((const char*)DS1[i].DDMOYY,"050187",6)==0){

			fprintf(outf,"%s|%s|%s|",DS1[i].Name,DS1[i].KLAD,DS1[i].STAND);
			fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DS1[i].TimeAngle/Pi*12,DS1[i].RA,DS1[i].DE,DS1[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DS1[i].RM[k]);
			}
			fprintf(outf,"%11.5lf|",DS1[i].JD);
			fprintf(outf,"%s|%s|%s",DS1[i].Spect,DS1[i].DDMOYY,DS1[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DS1[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DS1[i].Bt);
			fprintf(outf," %7.3lf|",DS1[i].Vt);
			fputs("\n",outf);
		  }
		//}
		delete DS1;

	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
	}
	COUNT_PART++;
	I0=ENd_Part+1;
	fprintf(errf,"%d\n",COUNT_PART);
}//КОНЕЦ ГЛОБАЛЬНОГО ЦИКЛА ПО КУСКАМ
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|TA|RA|DE|Mz|Rw|Rb|Rv|Rr|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
			fprintf(outf,"%s|%s|%s|",DSI[i].Name,DSI[i].KLAD,DSI[i].STAND);
			fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSI[i].TimeAngle/Pi*12,DSI[i].RA,DSI[i].DE,DSI[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].Ri[k]);
			}
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].RM[k]);
			}
			fprintf(outf,"%11.5lf|",DSI[i].JD);
			fprintf(outf,"%s|%s|%s|",DSI[i].Spect,DSI[i].DDMOYY,DSI[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"%7.3lf|",DSI[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSI[i].Bt);
			fprintf(outf," %7.3lf|",DSI[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
//fcloseall();
//	return 0.0;
//}
//#ifdef ETBOE
//#endif
//БЛОК  ВЫЧИСЛЕНИЕ уклонений от средних значений
	if (fopen_s(&outf, OutNormEndFile, "w") !=0 ){
		//AfxMessageBox(_T("Error Opening result NormEndFile"));
	}
	fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|W|B|V|R|dW|dB|dV|dR|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
	for(UINT i=0;i<IDPC;i++){
		DSI[i].FlagYes=false;
	}
	char buff[12];
	Date0* DSJ = new Date0[1000];
	for(UINT i=0;i<IDPC;i++){
		//Date0* DSJ = new Date0[1000];
		//fprintf(outf,"%7d|",i);
		if(!DSI[i].FlagYes)	
			memcpy(buff,DSI[i].Name,12);
		else continue;
		int kk=0;
		double Ddd[MaxBand]={0,0,0,0};
		for(UINT j=0,ij=0;j<IDPC;j++){//IOUT
			int a=strncmp(buff,DSI[j].Name,11);
			if(a==0){
				DSJ[ij]=DSI[j];
				DSI[j].FlagYes=true;
				for(UINT k=0;k<MaxBand;k++){
					Ddd[k]+=DSI[j].RM[k];
				}
				kk++;
				ij++;
			}else{continue;}
		}
		if(kk>1){
			for(UINT k=0;k<MaxBand;k++){	
				Ddd[k]/=(double)kk;
				for(UINT ij=0;ij<kk;ij++)	{
					DSJ[ij].RD[k]=DSJ[ij].RM[k]-Ddd[k];
					ij=ij;
				}
			}
			for(UINT ij=0;ij<kk;ij++){
				fprintf(outf,"%s|%s|%s|%4d|",DSJ[ij].Name,DSJ[ij].KLAD,DSJ[ij].STAND,DSJ[ij].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSJ[ij].TimeAngle/Pi*12,DSJ[ij].Z/Pi*180,DSJ[ij].PA/Pi*180,
					DSJ[ij].RA,DSJ[ij].DE,DSJ[ij].Mz);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DSJ[ij].RM[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"  %10.6lf|",DSJ[ij].RD[k]);
					//fprintf(outf,"  %7.3lf|",DS[ij].RM[k]);
				}
				fprintf(outf,"%11.5lf|",DSJ[ij].JD);
				fprintf(outf,"%s|%s|%s",DSJ[ij].Spect,DSJ[ij].DDMOYY,DSJ[ij].HHMMSS);
				for(UINT k=0;k<MaxBand;k++){
					fprintf(outf,"|%7.3lf",DSJ[ij].RO[k]);
				}
				fprintf(outf,"|  %7.3lf|",DSJ[ij].Bt);
				fprintf(outf," %7.3lf|",DSJ[ij].Vt);
				fputs("\n",outf);
			}
		} //else DJ[i].FlagYes=true; end IF(kk>1)
	//}
//#endif//endif
	} //КОНЕЦ ВЫЧИСЛЕНИЯ уклонений от средних значений 
	fclose(outf);
//delete DS;
//БЛОК ВЫБОРА ЗВЕЗД 
	//Date0* DSC = new Date0[IDPC];//IOUT, Count_DPCat //выборка звезд доп.каталога в порядке номеров
//UINT IDPC=0; //  количество наблюдений звезд в выборке по ДоП. каталогу
//	for(UINT i=0;i<IDPC;i++){							//  ii- по числу mi кроме нормстар hd 5015
//		DSC[i]=DSI[i];
//	}
//delete DSI;
#ifdef GYGYGY
UINT IMi=0;	//  количество столбцов величин, кроме hd 5015
short** IName_DC = new short*[IDPC];// вспомогательный массив для линейных уравнений по количеству звезд
for (UINT i=0;i<IDPC;i++){
	IName_DC[i]=new short [Count_DPCat];// длиной в число звезд в DP каталоге
	for(UINT j=0;j<Count_DPCat;j++){
		IName_DC[i][j]=0;
		//IMi=j;
	}
	IMi=i;
}
	IMi=IMi;
////double** IDate_DC = new double*[IOUT];// вспомогательный массив для линейных уравнений по кускам
////for (UINT i=0;i<IOUT;i++){
////	IDate_DC[i]=new double [COUNT_PART];//длиной в количество кусков
////	for(UINT j=0;j<COUNT_PART;j++)
////		IDate_DC[i][j]=0;
//}
//for(UINT j=0,ij=0;j<Count_DPCat;j++){ // j-по звездам ДПК ij- по количеству звезд в наблюдениях из ДПК
//}

		//for(UINT j=0;j<NN;j++){		//Count_str
		//	if (DS[j].FlagYes) continue;
		//	a=MIN(a,DS[j].JD);
		//}
//ПЕРЕБОР И УПОРЯДОЧИВАНИЕ КУСКОВ ПО ПОРЯДКУ
//#ifdef GYGYGY
char Buff[10];
UINT **NLIST=new UINT* [IDPC];
for (UINT i=0;i<IDPC;i++){
	NLIST[i]=new UINT [COUNT_PART];
	for(UINT j=0;j<COUNT_PART;j++)
		NLIST[i][j]=0;
}

for(UINT i=0;i<IDPC;i++){
	DSI[i].FlagYes=false;
}
UINT IP=0;
	for(int ii=IDPC;ii>0;IP++){
	double a=100000;
		for(UINT i=0;i<IDPC;i++){
			if (DSI[i].FlagYes) continue;
			a=MIN(a,DSI[i].N_P);
		}
		for(UINT i=0;i<IDPC;i++){
			if(DSI[i].N_P==a){
				DSI[i].FlagYes=true;
				NLIST[i][IP]=1;
				ii--;
			}
		}
	}
//
	for(UINT i=0,ii=0;i<IDPC;/*i++,ii++*/){
	//for(UINT ii=0;ii<Count_DPCat-1;/*ii++*/){
		//UINT i=0;
		memcpy(Buff,DSI[i].Name,9);
		Buff[9]=0;
	//if(strncmp((const char*)DSC[i].Name,Buff,9)==0){
//	//	IDate_DC[i][DSC[i].N_P]=1;
		if(strncmp((const char*)DSI[i].Name,"hd   5015",9)!=0){
			while(strncmp((const char*)DSI[i].Name,Buff,9)==0){
				IName_DC[i][ii]=1;
				//fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);
				i++;
			}
			ii++;
		}else{
			while(strncmp((const char*)DSI[i].Name,"hd   5015",9)==0){
			//continue;
				//fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);//IName_DC[i][ii]=0;
				i++;
			}
		}
		//ii++;
//	//		IDate_DC[i][DSC[i].N_P]=1;
	}
	//for(UINT i=0,ii=0;i<IDPC;i++/*,ii++*/){
	//	fprintf(errf,"%3.1lf\n",IName_DC[i][ii]);
	//}
	//fclose(errf);

//	//	IMi++;// считаем столбцы по числу mi кроме нормстар hd 5015
//	//}else {
//		while(strncmp((const char*)DSC[i].Name,Buff,9)==0){
//			IDate_DC[i][DSC[i].N_P]=1;
//			i++;
//		}
//	//}
//	//ij++;
//}
//}
			//if(strncmp((const char*)DSC[ij].Name,"hd   5015",9)!=0){
			//	IName_DC[ij][ii]=1;
			//	IDate_DC[ij][DSC[ij].N_P]=1;
		//}


		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
				fprintf(outf,"%s|%s|%s|%4d|",DSI[i].Name,DSI[i].KLAD,DSI[i].STAND,DSI[i].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSI[i].TimeAngle/Pi*12,DSI[i].Z/Pi*180,DSI[i].PA/Pi*180,
					DSI[i].RA,DSI[i].DE,DSI[i].Mz);
			//fprintf(outf,"%s|%s|%s|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND);
			//fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSI[i].Ri[k]);
			}
			for(UINT kj=0;kj<IMi;kj++){
				fprintf(outf,"  %7.3lf|",IName_DC[i][kj]);
			}
			for(UINT kj=0;kj<IP;kj++){
				fprintf(outf,"  %3d|",NLIST[i][kj]);
			}

			fprintf(outf,"%11.5lf|",DSI[i].JD);
			fprintf(outf,"%s|%s|%s",DSI[i].Spect,DSI[i].DDMOYY,DSI[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DSI[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSI[i].Bt);
			fprintf(outf," %7.3lf|",DSI[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);

//БЛОК СОСТАВЛЕНИЯ МАТРИЦ И РЕШЕНИЯ УРАВНЕНИЙ МНК
//#ifdef GYGYGY
int PLSQ = IP+Count_DPCat-1;//COUNT_PART+Count_DPCat;// Count_DPCat-1  количество оцениваемых параметров без звезды hd 5015 (нуль-пункт)
SolverLSQ MNK(PLSQ);
double **B = new double* [IDPC]; // правая часть уравнений МНК
for (UINT i=0;i<IDPC;i++)
	B[i]=new double [MaxBand];
double **A = new double* [IDPC]; // левая часть уравнений МНК
for (UINT i=0;i<IDPC;i++)
	A[i]=new double [PLSQ];

double **Res = new double* [MaxBand]; // правая часть уравнений МНК
for (UINT k=0;k<MaxBand;k++)
	Res[k]=new double [PLSQ];

// Cоставление и решение уравнений для WBVR
for(UINT k=0;k<MaxBand;k++){//
	for(UINT i=0;i<IDPC;i++){
		//for(UINT k=0;k<MaxBand;k++)
		if(strncmp((const char*)DSI[i].Name,"hd   5015",9)==0/* && IName_DC[i][j]!=0*/)
			B[i][k]=DSI[i].Ri[k]-DSI[i].RO[k];
		else B[i][k]=DSI[i].Ri[k];
	}
}
for(UINT i=0;i<IDPC;i++){
	UINT J=0;
	for(UINT j=0;j<Count_DPCat-1;j++){
		//if(strncmp((const char*)DSC[i].Name,"hd   5015",9)==0 /*&& IName_DC[i][j]!=0*/){
			//A[i][j]=0;//j--;
			//j++;
			//break;//continue;//J--;//
		//}else {
			A[i][j]=IName_DC[i][j];
			J++;
		//}
	}
	for(UINT j=0;j<IP;j++)//COUNT_PART
		A[i][j+J]=NLIST[i][j];
}
//fclose(errf);
for(UINT i=0;i<IDPC;i++){
	for(UINT k=0;k<MaxBand;k++)//
		fprintf(errf,"%7.3lf|",B[i][k]);
	for(UINT ij=0;ij<PLSQ;ij++){
		fprintf(errf,"%7.3lf|",A[i][ij]);
	}
	fputs("\n",errf);
}
fclose(errf);
//int PLSQ=2;//2;
//#ifdef GYGYGY
//#ifdef ETBOE
for(UINT k=0;k<MaxBand;k++){//
	MNK.Init();
	for(UINT i=0;i<IDPC;i++){
  		MNK.Accumulate(A[i],B[i][k]);//[k]
	}
	MNK.Solve(Res[k]);
}

	for(UINT i=0;i<IDPC;i++){
		for(UINT k=0;k<MaxBand;k++){
			for(UINT j=0;j<PLSQ;j++){
				DSC[i].RC[k]=DSC[i].Ri[k]-Res[k][j];
				fprintf(errf,"     %9.5lf| %9.5lf| %9.5lf|",A[i][j],Res[k][j]*A[i][j],DSC[i].RC[k]*A[i][j]);
			}
		}
		fprintf(errf,"\n");
	}
		//for(UINT i=0;i<IDPC;i++){
		//	for(UINT k=0;k<MaxBand;k++){
		//		fprintf(errf,"%7.3lf|",B[i][k]);
		//		for(UINT j=0;j<PLSQ;j++){
		//			DSC[i].RC[k]=B[i][k]-Res[k][j];
		//			fprintf(errf,"     %9.5lf| %9.5lf| %9.5lf|",A[i][j],Res[j]*A[i][j],RC[i]*A[i][j]);

		//			//RC[i][k]=B[i][k]+Res[k][j];
		//			//fprintf(outf,"%7.3lf| %7.3lf| %7.3lf| %7.3lf|",B[i][k],A[i][j],Res[k][j],RC[i][k]);
		//		}
		//		fprintf(errf,"\n");

		//	//}
		//}
	fclose(errf);
//#endif
		if (fopen_s(&outf, OutNormFile, "w") !=0 ){
			AfxMessageBox(_T("OUTnorm NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);//0U);
//AfxMessageBox(_T("Error Opening result NormFile"));
		}
		fprintf(outf,"NAME|KLAD|ST|N_P|TA|Z|PA|RA|DE|Mz|Wi|Bi|Vi|Ri|W|B|V|R|JD|Sp|Date|Time|WO|BO|VO|RO|Bt|Vt|\n");
		for(UINT i=0;i<IDPC;i++){
				fprintf(outf,"%s|%s|%s|%4d|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND,DSC[i].N_P);
				fprintf(outf,"  %7.3lf|  %5.1lf| %5.1lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].Z/Pi*180,DSC[i].PA/Pi*180,
					DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			//fprintf(outf,"%s|%s|%s|",DSC[i].Name,DSC[i].KLAD,DSC[i].STAND);
			//fprintf(outf,"  %7.3lf|  %13.8lf|  %13.8lf|  %9.5lf|",DSC[i].TimeAngle/Pi*12,DSC[i].RA,DSC[i].DE,DSC[i].Mz);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSC[i].Ri[k]);
			}
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"  %10.6lf|",DSC[i].RC[k]);
			}
			fprintf(outf,"%11.5lf|",DSC[i].JD);
			fprintf(outf,"%s|%s|%s",DSC[i].Spect,DSC[i].DDMOYY,DSC[i].HHMMSS);
			for(UINT k=0;k<MaxBand;k++){
				fprintf(outf,"|%7.3lf",DSC[i].RO[k]);
			}
			fprintf(outf,"  %7.3lf|",DSC[i].Bt);
			fprintf(outf," %7.3lf|",DSC[i].Vt);
			fputs("\n",outf);
		}
	//COUNT_PART++;
	//fprintf(outf,"%d",COUNT_PART);
		fclose(outf);
#endif
//#ifdef EMOE
		//else
		//{

		//}
//	delete [] DS;
	//delete [] DJ;
	delete [] WbvrO;
	delete [] TYP;
	//de_allocate(Data0,Count_str);
	//de_allocate(Data1,Count_coord);
	//de_allocate(Data2,Count_WBVR);

_fcloseall();
//#endif //endif
return 0.0;

}
double CMyTeMath::nonlinear(double N){
	double n=0;
	double b=0;
	do{
		n=N*exp(28e-9*N);
		b=n/N-1;
	}while(b>0.0001);
	return(n);
}
void Polinom(double *Coeff,/*double *RO,*/double W, double B,  double V, double R,double Mz,double lgG){//
	//double W_B=R0[0]-R0[1];
	//double B_V=R0[1]-R0[2];
	//double V_R=R0[2]-R0[3];
	//double B_R=R0[1]-R0[3];
	double W_B=W-B;
	double B_V=B-V;
	double V_R=V-R;
	double B_R=B-R;
	double Member[21];
	double Ai;
	//Member[0]=W_B;
	Member[1]=W_B;
	Member[2]=B_V;
	Member[3]=B_R;
	Member[4]=pow(W_B,2);
	Member[5]=pow(B_V,2);
	Member[6]=pow(B_R,2);
	Member[7]=W_B*B_V;
	Member[8]=W_B*B_R;
	Member[9]=B_V*B_R;
	Member[10]=pow(W_B,3);
	Member[11]=pow(B_V,3);
	Member[12]=pow(B_R,3);
	Member[13]=pow(W_B,2)*B_V;
	Member[14]=pow(W_B,2)*B_R;
	Member[15]=W_B*pow(B_V,2);
	Member[16]=pow(B_V,2)*B_R;
	Member[17]=W_B*pow(B_R,2);
	Member[18]=B_V*pow(B_R,2);
	Member[19]=B_V;
	Member[20]=B_V*W_B*B_R;
	//Member[2]=B_V;
	//Member[2]=B_V;
	//Member[10]=B_V;
	//Member[11]=B_V;
//a0+a1*W_B+a2*B_V+a3*B_R+/*b1*W_B^^2+*/b2*B_V^^2+
//	/*b3*B_R^^2+b4*W_B*B_V+b5*W_B*B_R+*/b6*B_V*B_R+/*c1*W_B^^3+*/
//	c2*B_V^^3+c3*B_R^^3+c4*W_B^^2*B_V+/*c5*W_B^^2*B_R+*/
//	c6*W_B*B_V^^2+c7*B_V^^2*B_R+c8*W_B*B_R^^2+c9*B_V*B_R^^2+c10*W_B*B_V*B_R;
		for(UINT j=1;j<20;j++){
			Ai=Coeff[0];
			Ai+=Coeff[j]*Member[j];
		}
		return;// (Ai);

}
void ReadPolinom(FILE *outf,char *FileInput){//
char *ptr[16384];
FILE *inpf;
		if (fopen_s(&inpf, FileInput, "r") !=0 ){
		AfxMessageBox(_T("INPUT NOT FOUND"),MB_RETRYCANCEL | MB_ICONSTOP);
exit(1);//0U);
//AfxMessageBox(_T("Error Opening data_total File"));
//      return;
	}
	char** Data0 = new char* [500];        // STEP 1: SET UP THE ROWS.
	double R = 0.0;
	//	char* Coeff = new char[20];        // STEP 1: SET UP THE ROWS.
	char* Coeff0[20] = { "a0","a1","a2","a3","b1","b2","b3","b4","b5","b6","c1","c2","c3","c4","c5","c6","c7","c8","c9","c10" };
	double CoVal[20];

	UINT Count = CMyTeMath::CountDataFile(inpf);
	char** Data = new char* [Count];        // STEP 1: SET UP THE ROWS.
	CMyTeMath::InpData(Data, inpf, Count, NULL);
	fclose(inpf);
	typedef struct Pol
	{
	    int Count;
	    char Coeff[3];
	    double CoVal;
	}DD;
	DD* Pol = new DD[Count];
	bool flag = false;
	double* Coeff = new double[20];
	for (UINT j = 1; j < Count - 1; j++) {
	    CMyTeMath::StrPtr(Data[j], '\t', ptr);
	    memcpy(ptr[0], Pol[j].Coeff, 2);
	    sscanf(ptr[1], "%lf", &Pol[j].CoVal);
	}
	for (UINT i = 0; i < 20; i++) {
	    //if ( fgets(InLin,len,inpf)==0 ) break;
	    //if(Data[0]=="C") continue;
	    //for(UINT j=0;j<20;j++){
	    //CMyTeMath::StrPtr(Data[i],'\t',ptr);
	    //char c[2];
	    //memcpy(ptr[0],c,2);
	    //c[2]=0;
	    bool flag = false;
	    for (UINT j = 0; j < Count - 2; j++) {
		if (strncmp(Pol[j].Coeff, Coeff0[i], 2) == 0) {
		    //sscanf(ptr[1],"%lf",&CoVal[j]);
		    Coeff[i] = Pol[j].CoVal;
		    flag = true;
		    break;
		}
		else continue;
		//continue;
	    }
	    if (!flag)
		Coeff[i] = 0.0;
	}
	for (UINT i = 0; i < 20; i++) {
	    fprintf(outf, "%8.4lf ", Coeff[i]);
	    fputs("\n", outf);
	}
	fcloseall();
}


//	for(UINT j=i;j<20;){
//		if(strncmp(c,Coeff[j],2)==0){		
//		}
//		else {

//		}
//}
//for(UINT i=0;i<20;i++){
//	R+=CoVal[i]


//		a0=	-0.07456320327679613;
//a1=	-0.1761008232315179;
//a2=	2.2227005002629157;
//a3=	-2.1656464664468404;
//
//b2=	2.0746003378608391;
//
//b6=	-1.0784999310911307;
//
//c2=	-8.2891367399317204;
//c3=	0.39088934306812295;
//c4=	-0.1007839697650172;
//
//c6=	4.9367011958975082;
//c7=	9.2661162285199552;
//c8=	1.1972701505456878;
//c9=	-3.3628339119443944;
//c10=	-4.7448527860659695;
//J_WBVR=V+a0+a1*W_B+a2*B_V+a3*B_R+/*b1*W_B^^2+*/b2*B_V^^2+
//	/*b3*B_R^^2+b4*W_B*B_V+b5*W_B*B_R+*/b6*B_V*B_R+/*c1*W_B^^3+*/
//	c2*B_V^^3+c3*B_R^^3+c4*W_B^^2*B_V+/*c5*W_B^^2*B_R+*/
//	c6*W_B*B_V^^2+c7*B_V^^2*B_R+c8*W_B*B_R^^2+c9*B_V*B_R^^2+c10*W_B*B_V*B_R;
//
//}
//if(strcmp(C[i],"a0")==0)
//	C[i]=="a0";





int CMyTeMath::Ai(DocDataType& Data,
    vector <CMyTeBand>& BandArray,
    CMyTeBand& Redden,
    CMyTeBand& Extint,
    vector <CMyTeBand>& AdvFilters, //фильтры для SBand_LG
    vector <double>& VegaArray,
    VectorArray& OutData)
{
    double RX = 0;
    bool FlagYes = false;
    double X = 0.0;
    double Mz = 1.0;
    int Nred = 6;//0;//0;//0;//5;//5;	//number of random i.s.ext. amount (thickness, depth) 
    const size_t BandCount = BandArray.size();

    for (int jj = 0; jj < Nred; jj++) {

	if (jj == 0) {
	    X = 0.0;
	}
	else {
	    X = CMyTeMath::RANDisex() * 4.0;
	}

	for (UINT ij = 0; ij < 6; ij++) {//Mz
	    vector <double> Result;
	    Result.push_back(X);
	    if (ij == 0) Mz = 0.0;
	    else {
		Mz = 1.0 + CMyTeMath::RANDisex() * 2.2;
	    }
	    Result.push_back(Mz);
	    for (UINT j = 0; j < BandCount; j++) {
		Result.push_back(CMyTeMath::SBand_4(&Data, &BandArray[j], VegaArray[j], &Redden, X, Mz, &Extint));
	    }
	    for (UINT j = 0; j < BandCount; j++) {
		Result.push_back(CMyTeMath::SBand_3(&Data, &BandArray[j], 0, &Redden, X, Mz, &Extint));
	    }
	    for (UINT j = 0; j < BandCount; j++) {
		Result.push_back(1.086 * (CMyTeMath::SBand_L(&Data, &BandArray[j], 0, &Redden, X, Mz, &Extint) -
		    CMyTeMath::SBand_L(&Data, &BandArray[j], 0, &Redden, X, 0.0, &Extint)));
	    }
	    for (UINT j = 0; j < BandCount; j++) {
		Result.push_back(CMyTeMath::SBand_LG(&Data, &BandArray[j], &AdvFilters[j], 0.0, &Redden, X, Mz, &Extint));
	    }
	    OutData.push_back(Result);
	}
    }
    return 0;
}
double CMyTeMath::SBand_LG(DocDataType* data, CMyTeBand* Band, CMyTeBand* LGBand,double Vega, CMyTeBand* Redden, double RedThik, double Mz, CMyTeBand* EXTIN)

{
	if (data == NULL) return -1.0;
	if (Band == NULL) return -1.0;
	DocDataType Subint = *data;
	DocDataType Subint0 = *data;

	long double Res = 0.0;
	long double Res0 = 0.0;
	double red = 1.0;
	const double lambdaBeg = Band->LBeg;
	const double lambdaEnd = Band->LEnd;

	for (UINT i = 0; i < data->Count - 1; i++)
	{
		if (Redden != NULL)
		{
			red = Redden->GetFlux(data->Lambda[i]);
			red = pow(red, RedThik);
		}
		const double BandFlux = Band->GetFlux(data->Lambda[i]);
		double EXT = EXTIN->GetFlux(data->Lambda[i]);
		double LGF = LGBand->GetFlux(data->Lambda[i]);
		EXT = pow(EXT, Mz);
		Subint.Flux[i] = data->Flux[i] * LGF * red * BandFlux * EXT;
		Subint0.Flux[i] = data->Flux[i] * red * BandFlux * EXT;
	}

	Res = Integral_1(&Subint, Band, lambdaBeg, lambdaEnd);
	Res0 = Integral_1(&Subint0, Band, lambdaBeg, lambdaEnd);
	Res =/*2.5*log10l*/(Res / Res0);

	return Res;
}
