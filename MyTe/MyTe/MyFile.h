#pragma once
#include <string>
#include <vector>
using namespace std;

typedef vector < vector<double>> VectorArray;
typedef vector <wstring> PathsArray;
typedef vector <VectorArray> VectorCube;

int ReadData(wstring FileName, VectorArray& Data);
int WriteData(wstring FileName, VectorArray Data);
int WriteDataHead(wstring FileName, VectorArray Data, vector <wstring> Header);
int ReadFolder(wstring Folder, PathsArray& Paths);
std::string format(const char* fmt, ...);
std::wstring wformat(const wchar_t* fmt, ...);