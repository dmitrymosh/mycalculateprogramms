#pragma once

#include<stdlib.h>
#include <windows.h>
#include <string>
#include "MyTeBand.h"
#include "MyFile.h"
#include "DocDataType.h"
struct SData_option
{
	double Vega; //�������� ����
	std::wstring VegaFile;//���� � ����� ����
	double* Lambda_Beg;//�������� ����������. ������ � ������� ������ ���������
	double* Lambda_End;//�������� ����������. ������ � ������� ����� ���������
	double* VegaFlux;//�������� ����������. ������ �� ��������� ���� � ���� ���������
	UINT NumberBand;//�������� ����������. ���������� ����������
	double StepLmkm;//�������� ������. ������ ����
	double LmkmBeg;//�������� ������. ������ ���������
	double LmkmEnd;//�������� ������. ����� ���������
	//ULONG32 OutCount;//�������� ������. ������ ��������
	double GraphBeg;//���������� ��������.������ ��������� 
	double GraphEnd;//���������� ��������.����� ���������
	double Coeff;//
};

class CMyTeMath
{
public:
	CMyTeMath(void);
	~CMyTeMath(void);
	static double RANDisex();
	static double RANDisexN(double N);
	static double swscanfd(std::wstring str);
	static double MyTe_scanf(BYTE* buffer,ULONG size, ULONG& pos, ULONG& beg, ULONG& end);
	static UINT SkipNotNumber(BYTE* szBuffer, UINT begin,UINT bufsize);
	static UINT CopyStrNumber(BYTE* szBuffer, UINT begin, UINT bufsize, wchar_t* dest,UINT dsize);
	static int MagnK(DocDataType* data,SData_option option);
	static double Integral(double* SubInt,ULONG32 MaxCount,double* lambda,double lambdaBeg,double lambdaEnd,double LBREAK,bool ERRF);
	static int Energy_Distribution(DocDataType* data,SData_option option);
	static int Energy_Distribution1_0(DocDataType* data,SData_option option);	
	static int Energy_Distribution1_1(DocDataType* data,SData_option option);	
	static int Energy_Distribution1_2(DocDataType* data,SData_option option);	
	static int Energy_Distribution2(DocDataType* data,SData_option option,CMyTeBand* Band);	
	static int Energy_Distribution1(DocDataType* data,SData_option option);	

	static double* Vec_Mult(DocDataType* data,double* a);
	static void SubIntPre(DocDataType* data,double* SubInt);
	static void SubIntPre0(DocDataType* data,double* SubInt,double*  DeDeTe);
	//static void CMyTeMath::SubIntPre0(DocDataType* data,double* SubInt,DocDataType* DDT);

	//static double GetFlux(DocDataType* data,double lambda);

	static ULONG32 JtoWm_2mkm_1(DocDataType* data,double* DataFlux,double* DataNormErr,double* DL,
		/*ULONG32 Count,*/SData_option option);
	static int CountDataFile(FILE *inpf);
	static void InpData(char** data,FILE *inpf,int Count_str, const char* Message); 
	static double Integral(double* SubInt,DocDataType* data,SData_option option);
	static double Integral_1(DocDataType* data,CMyTeBand* Band,double lambdaBeg,double lambdaEnd);
	static double Integral_M(double* Flux,double* Lambda,UINT Count,double lambdaBeg,double lambdaEnd);
	static int EnergyCount( DocDataType* data, SData_option option);
	static int EnergyCountIntegrBand( DocDataType* data, SData_option option);
	static double LinInt(double a,double b,double ta,double tb,double t);	
	static float LinInt(float a,float b,float ta,float tb,float t);
	static int WriteResult(std::wstring fname, std::wstring rfname, DocDataType* data,bool Over);
	static int WriteResult1(std::wstring fname, std::wstring data,bool Over);
	static int SaveChangedFile(std::wstring fname, DocDataType* data,SData_option option,bool Over);
	static int SaveFile(std::wstring fname, DocDataType* data,SData_option option,bool Over);
	// ���������� ������� � ����
	static int SaveEnergy(DocDataType* data, SData_option option,std::wstring rfname,bool Over);
	static int Norm5500(DocDataType* data, SData_option option);
	static double SBand(DocDataType* data, CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik);
	static double SBand_2(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik);
	static double SBand_3(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik,double Mz,CMyTeBand*EXTIN);
	static double SBand_4(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik,double Mz,CMyTeBand*EXTIN);
	static double SBand_L(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik,double Mz,CMyTeBand*EXTIN);
	static double SBand_L2(DocDataType* data,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik,double Mz,CMyTeBand*EXTIN);
	static int JDm1950(int year,int data);
	static double PROBKA_1(CMyTeBand* Band);
	static double PROBKA(CMyTeBand* Band);
	//static double Z(double t, double Sinfi, double Cosfi, double d);
	static double Mz(double t, double Sinfi, double Cosfi, double d, short zz);
	static double Stims(int jd50);
	//static double SBand(DocDataType* data, SData_option option,CMyTeBand* Band,double Vega,CMyTeBand* Redden,double RedThik);
	//static void ProcessCatalog(std::wstring CatInFName, std::wstring CatOutFName, std::wstring AdvDataFName, SData_option option , CMyTeBand Bands);
	static double GetFlux(double lambda,double* Lambda,double* Flux, ULONG32 Count);
	static UINT SpSearch(char N ,char P);
	static double SpSearch_d(char *SPEC);
	static double Aid(CMyTeBand* Band,CMyTeBand* Extint,double Mz);
	static int StrPtr(char Str[],char c,char** pt);
	static int StrPtrP(char Str[],char c,char** pt);
	static void Search_Band(CMyTeBand* Band,double* lambdaBeg,double* lambdaEnd);
	//static void Search_Band(CMyTeBand* Band,double lambdaBeg,double lambdaEnd);
	//double SIRIUS(DocDataType* data, SData_option option,CMyTeBand* Band,CMyTeBand* Extint,double Mz);
	static void TESTING();
	static char* _AWAY(char*NameStr);
	static void TEST();
	static double CorrColorSp(double Sp,UINT NRC,UINT NCln,UINT NOut,double InColor, double ** RedCol/*,bool Flg0*/);
	static double nonlinear(double N);
	static double Polinom( double DS,double N);
};
class SolverLSQ
{
  public:

    // Constructor, destructor
    SolverLSQ(int n) // n: Number of estimation parameters
    :N(n)
    {
      d=new double[N];
      R=new double*[N];
      for(UINT  i=0;i<N;i++) R[i]=new double[N];
    	Init();
    }

    ~SolverLSQ()
    {
		delete d;
		for(UINT  i=0;i<N;i++) delete R[i];
		delete R;
    }

    // Member functions

    // Reset to solve a new problem
    void Init();

    // Insert a data equation of form Ax=b to a least squares estimation
    // (performs a row-wise QR transformation using Givens rotations)
    void Accumulate (/*const*/ double A[], double b);

    // Solve the LSQ problem for vector x[] by backsubstitution
    void Solve (double x[]);

  private:

    UINT      N;         // Number of estimation parameters
    double   *d;        // Right hand side of transformed equations
    double   **R;       // Upper right triangular matrix

};
