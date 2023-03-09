#pragma once
#include <Windows.h>

class DocDataType
{
public:
	double* Lambda;//������, ������� ������(X) ������ � �������� Count
	double* Flux;	//����������, ������� ������(Y) ������ � �������� Count
	double* ErrFlux;//������ ������� ������ ������ � �������� Count
	double* NormErrFlux;//���������� ������, ������� ������ ������ � �������� Count
	UINT Count;//������ ������� ��������
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
};
typedef DocDataType DDT;

