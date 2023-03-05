#pragma once
#include "stdafx.h"
class CMyTeBand
{
public:
	__declspec(property(get = GetLambda_p, put = PutLambda_p)) double Lambda[];
	__declspec(property(get = GetFlux_p, put = PutFlux_p)) double Flux[];
	__declspec(property(get = GetCount_p, put = PutCount_p)) size_t Count;
	//double* Lambda;
	//double* Flux;
	//ULONG32 Count;

	VectorArray MyData;

	double LBeg;
	double LEnd;
	wstring FName;
	COLORREF GraphColor;
    wchar_t Name;

public:
	CMyTeBand(void);
	~CMyTeBand(void);
	size_t GetCount_p();
	double GetLambda_p(size_t i);
	double GetFlux_p(size_t i);
	void PutLambda_p(size_t i, double b);
	void PutFlux_p(size_t i, double b);

	void LoadFromFile(wstring fname);
	// вычисляет значение фильтра для данного значения лямбды
	double GetFlux(double lambda);
	void NormMax(void);
	void ConvertLnToFlux(void);
};
