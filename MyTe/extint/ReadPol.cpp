// extint.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include "optparse.h"
#include "MyFile.h"



//#define C_ALL(X) cbegin(X), cend(X)

int ReadPolinome(VectorArray Data, VectorCube Filters, VectorArray& Out);
// --extint --data .\\Data --mask *.* --out .\ 
// --Ai --data .\\stelar_data --filter .\\Filters --extint_stellar \\extint --extint_atm \\extint --out .\\Out
// --readpol --data .\\polynoms --out .\\outresult
int ReadPol(int argc, TCHAR* argv[])

{
	wstring Dir;
	wstring Mask;
	wstring FiltersDir;
	wstring StellarDir;
	wstring AtmDir;
	wstring OutDir;
	PathsArray DataFiles;
	PathsArray FilterPaths;
	vector <VectorArray> Data;
	vector <VectorArray> Filters;
	VectorArray Out;
	int rez = 0;

	struct optparse_long longopts[] = {
		{TEXT("readpol"), 'p', OPTPARSE_NONE},
		{TEXT("data"), 'd', OPTPARSE_REQUIRED},
		{TEXT("mask"), 'm', OPTPARSE_REQUIRED},
		{TEXT("filter"), 'f', OPTPARSE_REQUIRED},
		{TEXT("extint_stellar"), 's', OPTPARSE_REQUIRED},
		{TEXT("extint_atm"), 'a', OPTPARSE_REQUIRED},
		{TEXT("out"), 'o', OPTPARSE_REQUIRED},
		{TEXT("help"), '?', OPTPARSE_NONE},
		{TEXT("help"), 'h', OPTPARSE_NONE},
		{0}
	};

	int option;
	struct optparse options;
	bool d = 0;
	bool m = 0;
	bool f = 0;
	bool s = 0;
	bool a = 0;
	bool o = 0;

	(void)argc;
	optparse_init(&options, argv);
	while ((option = optparse_long(&options, longopts, NULL)) != -1) {
		switch (option) {
		case 'p':break;
		case 'd':
			Dir = options.option_arg;
			d = 1;
			break;
		case 'm':
			Mask = options.option_arg;
			m = 1;
			break;
		case 'f':
			FiltersDir = options.option_arg;
			f = 1;
			break;
		case 's':
			StellarDir = options.option_arg;
			s = 1;
			break;
		case 'a':
			AtmDir = options.option_arg;
			a = 1;
			break;
		case 'o':
			OutDir = options.option_arg;
			o = 1;
			break;
		case 'h':
		case '?':
			fwprintf(stderr, TEXT("Usage: \n %s -p -d .\\Data -m *.* -f .\\Filter -s .\\Stellar -a .\\Atm -o .\\n"), argv[0]);
			fwprintf(stderr, TEXT("%s\n"), options.errmsg);
			return(EXIT_FAILURE);
		default:
			fwprintf(stderr, TEXT("%s\n"), options.errmsg);
			return(EXIT_FAILURE);
		}
	}

	if (!(d && m && f && s && a && o)) {
		fwprintf(stderr, TEXT("Usage: \n %s --extint --data .\\Data --mask *.*  --out .\\ \n"), argv[0]);
		return EXIT_FAILURE;
	}

	// Read data folder
	if (ReadFolder(Dir, Mask, DataFiles)) {
		return 1;
	}

	for (size_t i = 0; i < DataFiles.size(); i++) {
		VectorArray Data1;
		cout << "Read file [";
		wcout << DataFiles[i];
		cout << "] ... ";
		if (ReadData(Dir, DataFiles[i], Data1)) {
			cout << " OK.\n";
		}
		else {
			cout << " Error.\n";
		}
		Data.push_back(Data1);
	}
	for (size_t i = 0; i < Data.size(); i++) {
		cout << "Processing file [";
		wcout << DataFiles[i];
		cout << "] ...";
		ReadPolinome(Data[i], Filters, Out);
		cout << " OK.\n";
		cout << "Saving result file [";
		wstring fname = DataFiles[i];
		fname.replace(fname.size() - 3, 3, TEXT("txt"));
		wcout << fname;
		cout << "] ...";
		WriteData(OutDir, fname, Out);
		cout << " OK.\n";
	}
	return 0;
}

int ReadPolinome(VectorArray Data, VectorCube Filters, VectorArray& Out) {
	//Data[0][1] первая строчка, второй столбец в исходном файле данных
	//Filters[0][0][0] первый номер фильтра второй строчка третий столбец
	//Out[0][0] первый строчка второй столбец результатов

	for (size_t i = 0; i < Data.size(); i++) {
		vector <double> data1;
		Data[i][1] /= 10.0;
		data1.push_back(Data[i][1]);
		data1.push_back(Data[i][8]);
		Out.push_back(data1);
	}
	return 0;
}
