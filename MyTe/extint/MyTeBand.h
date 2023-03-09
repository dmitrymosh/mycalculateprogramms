#pragma once
#include <Windows.h>
#include "MyFile.h"
#define E_CONST -1.0857362047581295691278222972915

class CMyTeBand
{
public:
	__declspec(property(get = GetLambda, put = PutLambda)) double Lambda[];
	__declspec(property(get = GetFlux, put = PutFlux)) double Flux[];
	//VectorArray Lambda;
	
	ULONG32 Count;
	double LBeg;
	double LEnd;
	std::wstring FName;
	COLORREF GraphColor;
    wchar_t Name;
public:
	CMyTeBand(void);
	~CMyTeBand(void);
	void PutLambda(UINT i, double L);
	double GetLambda(UINT i);
	void PutFlux(UINT i, double L);
	double GetFlux(UINT i);
	void LoadFromFile(std::wstring folder, std::wstring fname);
	// вычисляет значение фильтра для данного значения лямбды
	double GetFlux(double lambda);
	void NormMax(void);
	void ConvertLnToFlux(void);
private:
	VectorArray FluxA; // Flux [0] - Lambda; Flux[1] - Flux
	double LinInt(double a, double b, double ta, double tb, double t);
	float LinInt(float a, float b, float ta, float tb, float t);
};
