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
#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

#pragma comment(lib, "User32.lib")

#define C_ALL(X) cbegin(X), cend(X)
using namespace std;

typedef vector < vector<double>> VectorArray;
typedef vector <wstring> PathsArray;
typedef vector <VectorArray> VectorCube;

int ReadData(wstring Folder, wstring FileName, VectorArray & Data);
int WriteData(wstring Folder, wstring FileName, VectorArray& Data);
int ReadFolder(wstring Folder, wstring Mask, PathsArray& Paths);
int ProcessData(VectorArray Data, VectorCube Filters, VectorArray & Out);
// --data .\Data --mask *.* --filter .\Filters --out .\ 
int _tmain(int argc, TCHAR* argv[])

{
	wstring Dir;
	wstring Mask;
	wstring FiltersDir;
	wstring OutDir;
	PathsArray DataFiles;
	PathsArray FilterPaths;
	vector <VectorArray> Data;
	vector <VectorArray> Filters;
	VectorArray Out;
	int rez = 0;

	struct optparse_long longopts[] = {
		{TEXT("data"), 'd', OPTPARSE_REQUIRED},
		{TEXT("filter"), 'f', OPTPARSE_REQUIRED},
		{TEXT("mask"), 'm', OPTPARSE_REQUIRED},
		{TEXT("out"), 'o', OPTPARSE_REQUIRED},
		{TEXT("help"), '?', OPTPARSE_NONE},
		{TEXT("help"), 'h', OPTPARSE_NONE},
		{0}
	};

	int option;
	struct optparse options;
	bool d = 0;
	bool f = 0;
	bool m = 0;
	bool o = 0;

	(void)argc;
	optparse_init(&options, argv);
	while ((option = optparse_long(&options, longopts, NULL)) != -1) {
		switch (option) {
		case 'd':
			Dir = options.option_arg;
			d = 1;
			break;
		case 'f':
			FiltersDir = options.option_arg;
			f = 1;
			break;
		case 'm':
			Mask = options.option_arg;
			m = 1;
			break;
		case 'o':
			OutDir = options.option_arg;
			o = 1;
			break;
		case 'h':
		case '?':
			fwprintf(stderr, TEXT("Usage: \n %s --data .\\Data --mask *.* --filter .\\Filters --out .\\ \n"), argv[0]);
			return(EXIT_FAILURE);
		default:
			fwprintf(stderr, TEXT("%s: %s\n"), argv[0], options.errmsg);
			return(EXIT_FAILURE);
		}
	}

	if (!(f && d && m && o)) {
		fwprintf(stderr, TEXT("Usage: \n %s --data .\\Data --mask *.* --filter .\\Filters --out .\\ \n"), argv[0]);
		return EXIT_FAILURE;
	}
	// Read filters folder
	if (ReadFolder(FiltersDir, TEXT("*.*"), FilterPaths)) {
		return 1;
	}

	for (int i = 0; i < FilterPaths.size(); i++) {
		VectorArray Data1;
		cout << "Read file [";
		wcout << FilterPaths[i];
		cout << "] ... ";
		if (ReadData(FiltersDir, FilterPaths[i], Data1)) {
			cout << " OK.\n";
		}
		else {
			cout << " Error.\n";
		}
		Filters.push_back(Data1);
	}

	// Read data folder
	if (ReadFolder(Dir, Mask, DataFiles)) {
		return 1; 
	}
	
	for (int i = 0; i < DataFiles.size(); i++) {
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
	for (int i = 0; i < Data.size(); i++) {
		cout << "Processing file [";
		wcout << DataFiles[i];
		cout << "] ...";
		ProcessData( Data[i],  Filters,  Out);
		cout << " OK.\n";
		cout << "Saving result file [";
		wcout << DataFiles[i];
		cout << ".out] ...";
		WriteData(OutDir, DataFiles[i], Out);
		cout << " OK.\n";
	}
} 

int ReadData(wstring Folder, wstring FileName, VectorArray & Data)
{
	wstring path = Folder + TEXT("\\") + FileName;
	ifstream in(path);
	if (in.is_open())
	{
		string s_row;
		while (getline(in, s_row))
		{
			double var;
			istringstream st(s_row);
			if (st.bad() || st.eof() || s_row == "") {
				continue;
			}
			vector <double> v_row;
			while (st.good())
			{
				st >> std::dec >> var;
				if (st.good() || st.eof()) {
					v_row.push_back(var);
				}
				if (st.eof()) {
					Data.push_back(v_row);
				}
				if (st.fail() || st.eof()) {
					break;
				}
			}
		}
	}
	in.close();     // закрываем файл
	if (Data.empty()) {
		return 0;
	}
	else {
		return 1;
	}
}
int WriteData(wstring Folder, wstring FileName, VectorArray& Data) {
	wstring path = Folder + TEXT("\\") + FileName + TEXT(".out");
	ofstream out(path);
	if (out.is_open()) {
		for (int i = 0; i < Data.size();i++) {
			for (int j = 0; j < Data[i].size(); j++) {
				out << Data[i][j];
				out << "  ";
			}
			out << "\n";
		}
	}
	out.close();     // закрываем файл
	return 0;
}
// Функция читает папку и помещает файлы, удовлетворяющие маске в массив Paths
int ReadFolder(wstring Folder, wstring Mask, PathsArray & Paths) {

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	wstring szPath(Folder + TEXT("\\") + Mask);

	// Find the first file in the directory.

	hFind = FindFirstFile(&szPath[0], &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		return dwError;
	}
	
	do {
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			Paths.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		return dwError;
	}

	FindClose(hFind);
	return 0;
}

int ProcessData(VectorArray Data, VectorCube Filters, VectorArray& Out) {
	//Data[0][1] первый строчка, второй столбец в исходном файле данных
	//Filters[0][0][0] первый номер фильтра второй строчка третий столбец
	//Out[0][0] первый строчка второй столбец результатов
	
	for(int i=0;i<Data.size();i++){
		vector <double> data1;
		Data[i][1]/=10.0;
		data1.push_back(Data[i][1]);
		data1.push_back(Data[i][8]);
		Out.push_back(data1); 
	}
	return 0;
}
