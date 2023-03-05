#include "stdafx.h"

void DocDataType::Init(UINT strcount, UINT NumBand)
{
	
	NumberBand = NumBand;

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
	
	this->MyData = Source.MyData;
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

size_t DocDataType::GetCount_p() {
	return this->MyData.size();
}

double DocDataType::GetLambda_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[0].size() > i)) {
		return this->MyData[0][i];
	}
	return -1.0;
}

double DocDataType::GetFlux_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[1].size() > i)) {
		return this->MyData[1][i];
	}
	return -1.0;
}
double DocDataType::GetErrFlux_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[2].size() > i)) {
		return this->MyData[2][i];
	}
	return -1.0;
}
double DocDataType::GetNormErrFlux_p(size_t i) {
	if ((this->MyData.size() > 0) && (this->MyData[3].size() > i)) {
		return this->MyData[3][i];
	}
	return -1.0;
}
void DocDataType::PutLambda_p(size_t i, double b){

}
void DocDataType::PutFlux_p(size_t i, double b) {

}
void DocDataType::PutErrFlux_p(size_t i, double b) {

}
void DocDataType::PutNormErrFlux_p(size_t i, double b) {

}