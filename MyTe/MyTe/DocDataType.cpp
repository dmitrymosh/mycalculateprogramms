#include "stdafx.h"

void DocDataType::Init(UINT strcount, UINT NumBand)
{
	
	NumberBand = NumBand;

}
void DocDataType::InitOut(UINT _OutCount)
{
	
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
	
}
void DocDataType::operator=(DocDataType& Source)
{

	Init(Source.Count, Source.NumberBand);
	InitOut(Source.OutCount);
	
	this->MyData = Source.MyData;
	this->OutData = Source.OutData;
	this->SummData = Source.SummData;
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
	return this->MyData.at(i).at(0);
}

double DocDataType::GetFlux_p(size_t i) {
	return this->MyData.at(i).at(1);
}
double DocDataType::GetErrFlux_p(size_t i) {
	return this->MyData.at(i).at(2);
}
double DocDataType::GetNormErrFlux_p(size_t i) {
	return this->MyData.at(i).at(3);
}
void DocDataType::PutLambda_p(size_t i, double b) {
	this->MyData.at(i).at(0) = b;	
}
void DocDataType::PutFlux_p(size_t i, double b) {
	this->MyData.at(i).at(1) = b;
}
void DocDataType::PutErrFlux_p(size_t i, double b) {
	this->MyData.at(i).at(2) = b;
}
void DocDataType::PutNormErrFlux_p(size_t i, double b) {
	this->MyData.at(i).at(3) = b;
}

int DocDataType::LoadFromFile(wstring FileName) {
    wstring t = FileName;
    size_t l = t.find_last_of(_T("\\"));
    this->FileName = t.substr(l+1);

    this->FilePath = FileName;
    return ReadData( FileName,  this->MyData);

}