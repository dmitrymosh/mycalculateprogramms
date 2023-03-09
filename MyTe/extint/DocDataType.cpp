#include "DocDataType.h"
#include "MyFile.h"
#include <Windows.h>

void DocDataType::Init(UINT strcount, UINT NumBand)
{
	//создаём массивы
	Count = strcount;
	NumberBand = NumBand;
	Lambda = new double[strcount];
	Flux = new double[strcount];
	ErrFlux = new double[strcount];
	NormErrFlux = new double[strcount];
	SummErrFlux = new double[NumberBand];
	SummFlux = new double[NumberBand];
	EmFluxWsm2A_7 = new double[NumberBand];
	FotonCount = new double[NumberBand];
	SummNormErrFlux = new double[NumberBand];
	for (ULONG32 i = 0; i < NumberBand; i++)
	{
		SummErrFlux[i] = 0.0;
		SummFlux[i] = 0.0;
	}
}
void DocDataType::InitOut(UINT _OutCount)
{
	//создаём массивы
	OutCount = _OutCount;
	ErrOut = new double[OutCount];
	FluxOutW = new double[OutCount];
	LambdaOut = new double[OutCount];

	for (ULONG32 i = 0; i < OutCount; i++)
	{
		ErrOut[i] = 0.0;
		FluxOutW[i] = 0.0;
		LambdaOut[i] = 0.0;
	}
}
DocDataType::DocDataType(void)
	:Count(0)
	, Lambda(NULL)
	, Flux(NULL)
	, ErrFlux(NULL)
	, NormErrFlux(NULL)
	, SummErrFlux(NULL)
	, SummNormErrFlux(NULL)
	, SummFlux(NULL)
	, OutCount(0)
	, ErrOut(NULL)
	, FluxOutW(NULL)
	, LambdaOut(NULL)
	, EmFluxWsm2A_7(NULL)
	, FotonCount(NULL)
	, NumberBand(0)
	, LmBeg(0)
	, LmEnd(0)
	, LmStep(0)
{

}
double DocDataType::GetFlux(DocDataType* data, double lambda) {
	double a;
	if (lambda < data->Lambda[0]) a = data->Flux[0];
	if (lambda > data->Lambda[data->Count - 1]) a = data->Flux[data->Count - 1];
	for (ULONG32 i = 0; i < data->Count; i++) {
		if ((lambda >= data->Lambda[i]) && (lambda < data->Lambda[i + 1])) {
			a = LinInt(data->Flux[i], data->Flux[i + 1],
				data->Lambda[i], data->Lambda[i + 1], lambda);
			break;
		}
	}
	return a;
}
DocDataType::~DocDataType()
{
	if (Flux != NULL) delete Flux;
	if (Lambda != NULL)	delete Lambda;
	if (NormErrFlux != NULL)	delete NormErrFlux;
	if (ErrFlux != NULL)	delete ErrFlux;

	if (ErrOut != NULL)	delete ErrOut;
	if (FluxOutW != NULL)	delete FluxOutW;
	if (LambdaOut != NULL)	delete LambdaOut;

	if (EmFluxWsm2A_7 != NULL)	delete EmFluxWsm2A_7;
	if (FotonCount != NULL)	delete FotonCount;
	if (SummErrFlux != NULL)	delete SummErrFlux;
	if (SummNormErrFlux != NULL)	delete SummNormErrFlux;
	if (SummFlux != NULL)	delete SummFlux;
}
void DocDataType::operator=(DocDataType& Source)
{
	Init(Source.Count, Source.NumberBand);
	InitOut(Source.OutCount);
	for (ULONG32 j = 0; j < Count; j++)
	{
		Flux[j] = Source.Flux[j];
		Lambda[j] = Source.Lambda[j];
		NormErrFlux[j] = Source.NormErrFlux[j];
		ErrFlux[j] = Source.ErrFlux[j];
	}
	for (ULONG32 j = 0; j < OutCount; j++)
	{
		FluxOutW[j] = Source.FluxOutW[j];
		LambdaOut[j] = Source.LambdaOut[j];
		ErrOut[j] = Source.ErrOut[j];
	}
	for (ULONG32 j = 0; j < NumberBand; j++)
	{
		SummErrFlux[j] = Source.SummErrFlux[j];
		SummNormErrFlux[j] = Source.SummNormErrFlux[j];
		SummFlux[j] = Source.SummFlux[j];
		FotonCount[j] = Source.FotonCount[j];
		EmFluxWsm2A_7[j] = Source.EmFluxWsm2A_7[j];
	}
}

double DocDataType::LinInt(double a, double b, double ta, double tb, double t)
//$$+							y1		y2		x1		x2		x
{
	if (ta != tb) {
		return a + (b - a) * (t - ta) / (tb - ta);
	}
	else return a;
}
float DocDataType::LinInt(float a, float b, float ta, float tb, float t)
//$$+
{
	if (ta != tb) {
		return a + (b - a) * (t - ta) / (tb - ta);
	}
	else return a;
}