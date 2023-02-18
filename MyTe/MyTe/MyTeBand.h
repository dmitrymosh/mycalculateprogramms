#pragma once

class CMyTeBand
{
public:
	double* Lambda;
	double* Flux;
	ULONG32 Count;
	double LBeg;
	double LEnd;
	CString FName;
	COLORREF GraphColor;
    wchar_t Name;
public:
	CMyTeBand(void);
	~CMyTeBand(void);

	void LoadFromFile(CString fname);
	// вычисляет значение фильтра для данного значения лямбды
	double GetFlux(double lambda);
	void NormMax(void);
	void ConvertLnToFlux(void);
};
