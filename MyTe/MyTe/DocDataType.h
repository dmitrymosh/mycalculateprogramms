#pragma once

#include "stdafx.h"


class DocDataType
{
public:
	__declspec(property(get = GetLambda_p, put = PutLambda_p)) double Lambda[]; //������, ������� ������(X) ������ � �������� Count
	__declspec(property(get = GetFlux_p, put = PutFlux_p)) double Flux[];		//����������, ������� ������(Y) ������ � �������� Count
	__declspec(property(get = GetErrFlux_p, put = PutErrFlux_p)) double ErrFlux[];//������ ������� ������ ������ � �������� Count
	__declspec(property(get = GetNormErrFlux_p, put = PutNormErrFlux_p)) double NormErrFlux[];//���������� ������, ������� ������ ������ � �������� Count
	__declspec(property(get = GetCount_p, put = PutCount_p)) size_t Count; //������ ������� ��������
	//__declspec(property(get = GetCount_p, put = PutCount_p)) size_t NumberBand; //������ ������� ��������
	
	UINT  NumberBand;
	double* SummErrFlux;//����� ��������� ������ �� ����������, ������ � �������� options->NumberBand
	double* SummNormErrFlux;//����� ���������� ������ , ������ � �������� options->NumberBand
	double* SummFlux;//����� ���������� , ������ � �������� options->NumberBand

	double* LambdaOut;//������, �������� ������ ������ � �������� options->OutCount; 
	double* FluxOutW;//���������� , �������� ������ ������ � �������� options->OutCount;
	double* ErrOut;//������,  �������� ������ ������ � �������� options->OutCount;
	double* FotonCount;//���������� �������, �������� ������ � �������� options->NumberBand
	double* EmFluxWsm2A_7;//����� � ������� � W/cm2/A, �������� ������ � �������� options->NumberBand
	double LmBeg;
	double LmEnd;
	double LmStep;
	UINT OutCount;

	DocDataType(void);
	void Init(UINT strcount, UINT NumberBand);
	void InitOut(UINT _OutCount);
	~DocDataType();
	void operator=(DocDataType& Source);
	double GetFlux(DocDataType* data, double lambda);
	double LinInt(double a, double b, double ta, double tb, double t);
	float LinInt(float a, float b, float ta, float tb, float t);
	double GetLambda_p(size_t i);
	double GetFlux_p(size_t i);
	double GetErrFlux_p(size_t i);
	double GetNormErrFlux_p(size_t i);
	size_t GetCount_p();
	void PutLambda_p(size_t i, double b);
	void PutFlux_p(size_t i, double b);
	void PutErrFlux_p(size_t i, double b);
	void PutNormErrFlux_p(size_t i, double b);
	int LoadFromFile(wstring FileName);
	wstring FileName;
	wstring FilePath;
	VectorArray MyData;
	VectorArray SummData;
	VectorArray OutData;
};
//typedef DocDataType DDT;

