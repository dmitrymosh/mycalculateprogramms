#include <tchar.h> 
#include <string>
#include <vector>
#include "MyTeBand.h"
#include <windows.h>
#include "MyFile.h"

CMyTeBand::CMyTeBand(void)
{
	Count=0;
	LBeg=0;
	LEnd=0;
}

CMyTeBand::~CMyTeBand(void)
{
	
}
void CMyTeBand::PutLambda(UINT i, double L) {
	FluxA[0][i] = L;
}
double CMyTeBand::GetLambda(UINT i) {
	return FluxA[0][i];
}
void CMyTeBand::PutFlux(UINT i, double L) {
	FluxA[1][i] = L;
}
double CMyTeBand::GetFlux(UINT i) {
	return FluxA[1][i];
}
//типа ввода
#ifdef EMOE		
int _strptr(char Str[],char c,char** pt){
	//$$+
	//static UINT N=0;
	for(UINT j=0,i=0,N=0;;j++,i++){
		for(;;i++){
			if(Str[i]=='\0' || Str[i]=='\n')return N;
			if(i!=0){
				if((Str[i]!=c && Str[i]!='\t')){
					if((Str[i-1]==c || Str[i-1]=='\t')){
						pt[j]=&Str[i];
						N++;
						break;
					}else continue;
				}else continue;
			}else {
				if(Str[i]!=c && Str[i]!='\t'){
					pt[j]=&Str[i];
					N++;
					break;
				}else continue;
			}
		}
	}
}
//************ ВВод спектров *******************
//==============================================
spcf = fopen(FileSpect, "r");//spectSv
fgets(InLin,len,spcf); //Name Spectra
NCS=_strptr(&InLin[8],' ',ptr);
//#ifdef VISCON
//	Look3->Lines->Append(NameSp);
//Look3->Lines->Append(IntToStr(NCS));//распределение  энергии в спектре
//#endif
//NCS; // Кол-во колонок в SpData без волн = число типов спектра
for(UINT i=0;i<NCS;i++){
	sscanf(ptr[i],"%s",buff);//NameSp[i]);
	memcpy(NameSp[i],buff,strlen(buff)+1);
//#ifdef VISCON
	//Look3->Lines->Append(NameSp[i]);
//#endif
}
while(1){
	if(fgets(InLin,len,spcf)==0) break;
	++DLRSP;
}
sp = new float*[NCS];
for(UINT i=0;i<NCS;i++){
	sp[i] = new float[DLRSP];  // STEP 2: SET UP THE COLUMNS
}
fseek(spcf, 0L, SEEK_SET);
fgets(InLin,len,spcf);
for(UINT j=0;j<DLRSP;j++){
	fgets(InLin,len,spcf);
	_strptr(&InLin[8],' ',ptr);
	for(UINT i=0;i<NCS;i++){
		if(sscanf(ptr[i],"%f",&sp[i][j])==NULL) {
			sp[i][j]=99999.9;
		}// break;}
	}
}

class VECTOR 
#endif
void CMyTeBand::LoadFromFile(std::wstring folder, std::wstring fname)
{
	ReadData(folder, fname, FluxA);
}

// вычисляет значение кривой реакции для данного значения лямбды
double CMyTeBand::GetFlux(double lambda)
{
	//исключаем значения вне кривой
	if((lambda< Lambda[0])||(lambda> Lambda[Count-1]))
		return 0.0;
	for(UINT i=0;i<Count-1;i++)
	{
		if((Lambda[i]<=lambda)&&(Lambda[UINT(i+1)]>lambda))
		{
			return LinInt(Flux[i], Flux[UINT(i + 1)], Lambda[i], Lambda[UINT(i + 1)],lambda);
		}
	}
	return 0;
}

void CMyTeBand::NormMax(void)
{
	double MaxFlux=0.0;
	for(ULONG32 i=0;i<Count;i++)
	{
		if(MaxFlux< Flux[i]) MaxFlux= Flux[i];
	}
	for(ULONG32 i=0;i<Count;i++)
	{
		Flux[i]/=MaxFlux;
	}	
}

void CMyTeBand::ConvertLnToFlux(void)
{
	for(ULONG32 i=0;i<Count;i++)
	{
		double X= Flux[i]/E_CONST;
		Flux[i]=exp(X);
		Lambda[i]*=10.0;
	}
}
double CMyTeBand::LinInt(double a, double b, double ta, double tb, double t)
//$$+							y1		y2		x1		x2		x
{
	if (ta != tb) {
		return a + (b - a) * (t - ta) / (tb - ta);
	}
	else return a;
}
float CMyTeBand::LinInt(float a, float b, float ta, float tb, float t)
//$$+
{
	if (ta != tb) {
		return a + (b - a) * (t - ta) / (tb - ta);
	}
	else return a;
}