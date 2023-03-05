#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
//#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include "MyFile.h"
#include "stdafx.h"
#pragma warning(suppress : 4996)
// ----------------------------------------------------------------------------
// FileName - строка 
int ReadData(wstring FileName, VectorArray& Data)
{
	ifstream in(FileName);
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

int WriteDataHead(wstring FileName, VectorArray Data, vector <wstring> Header) {
	wofstream out (FileName, ios::trunc); // open renew
	if (out.is_open()) {
		for (size_t i = 0; i < Header.size(); i++) {
			out << Header[i];
			out << "\n";
		}
		for (size_t i = 0; i < Data.size(); i++) {
			for (size_t j = 0; j < Data[i].size(); j++) {
				out << Data[i][j];
				out << "  ";
			}
			out << "\n";
		}
	}
	out.close();     // закрываем файл
	return 0;
}

int WriteData(wstring FileName, VectorArray Data) {
	vector <wstring> Header;
	return WriteDataHead(FileName, Data, Header);
}
// ‘ункци€ читает папку и помещает файлы, удовлетвор€ющие маске в массив Paths
int ReadFolder(wstring Folder, wstring Mask, PathsArray& Paths) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	wstring szPath(Folder + TEXT("\\") + Mask);
		
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
std::string format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::vector<char> v(1024);
	while (true)
	{
		va_list args2;
		va_copy(args2, args);
		int res = vsnprintf(v.data(), v.size(), fmt, args2);
		if ((res >= 0) && (res < static_cast<int>(v.size())))
		{
			va_end(args);
			va_end(args2);
			return std::string(v.data());
		}
		size_t size;
		if (res < 0)
			size = v.size() * 2;
		else
			size = static_cast<size_t>(res) + 1;
		v.clear();
		v.resize(size);
		va_end(args2);
	}
}
std::wstring wformat(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::vector<wchar_t> v(1024);
	while (true)
	{
		va_list args2;
		va_copy(args2, args);
		int res = _snwprintf(v.data(), v.size(), fmt, args2);
		if ((res >= 0) && (res < static_cast<int>(v.size())))
		{
			va_end(args);
			va_end(args2);
			return std::wstring(v.data());
		}
		size_t size;
		if (res < 0)
			size = v.size() * 2;
		else
			size = static_cast<size_t>(res) + 1;
		v.clear();
		v.resize(size);
		va_end(args2);
	}
}

