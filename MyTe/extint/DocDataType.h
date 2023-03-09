#pragma once
#include <Windows.h>

class DocDataType
{
public:
	double* Lambda;//Лямбда, входные данные(X) массив с размером Count
	double* Flux;	//Светимость, входные данные(Y) массив с размером Count
	double* ErrFlux;//Ошибка входные данные массив с размером Count
	double* NormErrFlux;//Нормальная ошибка, входные данные массив с размером Count
	UINT Count;//размер входных массивов
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
};
typedef DocDataType DDT;

