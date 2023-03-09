#pragma once

#include "stdafx.h"


class DocDataType
{
public:
	__declspec(property(get = GetLambda_p, put = PutLambda_p)) double Lambda[]; //Лямбда, входные данные(X) массив с размером Count
	__declspec(property(get = GetFlux_p, put = PutFlux_p)) double Flux[];		//Светимость, входные данные(Y) массив с размером Count
	__declspec(property(get = GetErrFlux_p, put = PutErrFlux_p)) double ErrFlux[];//Ошибка входные данные массив с размером Count
	__declspec(property(get = GetNormErrFlux_p, put = PutNormErrFlux_p)) double NormErrFlux[];//Нормальная ошибка, входные данные массив с размером Count
	__declspec(property(get = GetCount_p, put = PutCount_p)) size_t Count; //размер входных массивов
	//__declspec(property(get = GetCount_p, put = PutCount_p)) size_t NumberBand; //размер входных массивов
	
	UINT  NumberBand;
	double* SummErrFlux;//сумма квадратов ошибок по диапазонам, массив с размером options->NumberBand
	double* SummNormErrFlux;//сумма нормальных ошибок , массив с размером options->NumberBand
	double* SummFlux;//сумма светимости , массив с размером options->NumberBand

	double* LambdaOut;//Лямбда, выходные данные массив с размером options->OutCount; 
	double* FluxOutW;//Светимость , выходные данные массив с размером options->OutCount;
	double* ErrOut;//ошибка,  выходные данные массив с размером options->OutCount;
	double* FotonCount;//Количество фотонов, выходной массив с размером options->NumberBand
	double* EmFluxWsm2A_7;//Поток в полосах в W/cm2/A, выходной массив с размером options->NumberBand
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

