#include "StdAfx.h"
//#include "MyTeBand.h"
//#include "MyTeMath.h"

CMyTeBand::CMyTeBand(void)
{
	
}

CMyTeBand::~CMyTeBand(void)
{
	this->MyData.clear();
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
void CMyTeBand::LoadFromFile(wstring fname) {
	size_t k = fname.find_last_of(_T('\\'));
	this->Name = fname[k+1];
	this->FName = fname;
	this->MyData.clear();
	ReadData(fname, this->MyData);
}

#ifdef EMOE
void CMyTeBand::LoadFromFile(CString fname)
{
	BYTE*	szBuffer; 
	ULONG32    nActual = 0; 	
	ULONG32 strcount=0;
	CFile* myFile;
	myFile=new CFile;
	CFileException ex;
	
	//if( !f.Open( pFileName, CFile::modeCreate | CFile::modeWrite, &e ) )

	if (!myFile->Open(fname,CFile::modeRead,&ex ))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
	}else
	{	
		//myFile->Open(fname,CFile::modeRead);
		UINT filesize = (UINT)myFile->GetLength();
		szBuffer= new BYTE [filesize];
		nActual = myFile->Read( szBuffer, filesize ); 
		myFile->Close();
		if(nActual<filesize) filesize=nActual;	
		
		/*CString color;
		int r,g,b;
		wchar_t s[200];
		{
			wchar_t temp[200];
			int i=0;
			int bi=0;
			do{			
				temp[i]=szBuffer[bi];
				bi++;i++;
			}while((szBuffer[bi]!='\n')&&(bi<filesize));
			swscanf_s((wchar_t*)&temp[6],_T("%d %d %d"),&r,&g,&b);
		}
		GraphColor=RGB(r,g,b);*/
		//пропускаем заголовок
		int head=CMyTeMath::SkipNotNumber(szBuffer,0,filesize);

		//подсчет строк
		for(ULONG32 i=head;i<filesize;i++)
		{
			if (szBuffer[i]==0x09)
			{
				szBuffer[i]=0x20;
			}
			/*
			if((szBuffer[i]==0x0A)&&(szBuffer[i+1]==0x20)&&(szBuffer[i-1]!=0x0D))
			{
			szBuffer[i]=0x0D;
			szBuffer[i+1]=0x0A;
			}*/
			if(/*((szBuffer[i]==0x0D)&&(szBuffer[i+1]==0x0A))||*/(szBuffer[i]==0x0A)) 
			{
				strcount++;
			}
		}
		//Data.Init(strcount,theApp.Options.NumberBand,strcount);	

		//создаём массивы
		Count=strcount;
		Lambda		=	new double[strcount];
		Flux		=	new double[strcount];
		FName		=	fname;
        Name        =   _T('\0');
        
        int k = fname.ReverseFind(_T('\\'));
        Name = fname[k+1];
           
        
		//загружаем данные в массивы	
		wchar_t temp[200];
		UINT bi=0;
		bi=CMyTeMath::SkipNotNumber(szBuffer,bi,filesize);	
		for(ULONG32 j=0;j<strcount;j++)
		{	
			int i=0;		
			while((bi<filesize)&&(szBuffer[bi]!=0x0D)&&(szBuffer[bi]!=0x0A))
			{			
				temp[i]=szBuffer[bi];
				bi++;i++;
				temp[i]=0x00;
			}
			while((szBuffer[bi]==0x0D)||(szBuffer[bi]==0x0A))
			{
				bi++;
			}
			swscanf_s((wchar_t*)&temp,_T("%lf%lf"),&Lambda[j],&Flux[j]);
		}
		delete[] szBuffer;
	}
}
#endif
// вычисляет значение кривой реакции для данного значения лямбды
double CMyTeBand::GetFlux(double lambda)
{
	//исключаем значения вне кривой
	if((lambda<Lambda[0])||(lambda>Lambda[Count-1])) 
		return 0.0;
	for (size_t i = 0; i < Count - 1; i++) {
		if ((Lambda[i] <= lambda) && (Lambda[i + 1] > lambda)) {
			return CMyTeMath::LinInt(Flux[i],Flux[i+1],Lambda[i],Lambda[i+1],lambda);
		}
	}
	return 0.0;
}

void CMyTeBand::NormMax(void)
{
	double MaxFlux=0.0;
	for(ULONG32 i=0;i<Count;i++)
	{
		if(MaxFlux<Flux[i]) MaxFlux=Flux[i];
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
		double X = Flux[i]/E_CONST;
		Flux[i]=exp(X);		
		Lambda[i]*=10.0;
	}
}
size_t CMyTeBand::GetCount_p() {
	return this->MyData.size();
}

double CMyTeBand::GetLambda_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[0].size() > i)) {
		return this->MyData[0][i];
	}
	return -1.0;
}

double CMyTeBand::GetFlux_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[1].size() > i)) {
		return this->MyData[1][i];
	}
	return -1.0;
}
void CMyTeBand::PutLambda_p(size_t i, double b) {
	if (this->MyData.size() == 0) {
		return;
	}
	if (this->MyData[0].size() <= i) {
		this->MyData[0].push_back(b);
	}	
}
void CMyTeBand::PutFlux_p(size_t i, double b) {

}