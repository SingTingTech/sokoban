#pragma once
#include"stdafx.h"

namespace cris {
	void findSaves(wchar_t* lpPath, std::vector<std::string> &fileList)
	{
		wchar_t szFind[MAX_PATH];
		WIN32_FIND_DATA FindFileData;
		wcscpy_s(szFind, lpPath);
		wcscpy_s(szFind, lpPath);
		wcscat_s(szFind, L"\\*.save");

		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)    return;

		while (true)
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{		
				char c[520];
				//std::cout << FindFileData.cFileName << std::endl;  
				WideCharToMultiByte(CP_ACP, 0, FindFileData.cFileName, 260, c, 520, NULL, NULL);
				fileList.push_back(c);
			}
			if (!FindNextFile(hFind, &FindFileData))    break;
		}
		FindClose(hFind);
	}
	void findXSBs(wchar_t* lpPath, std::vector<std::string> &fileList)
	{
		wchar_t szFind[MAX_PATH];
		WIN32_FIND_DATA FindFileData;
		wcscpy_s(szFind, lpPath);
		wcscpy_s(szFind, lpPath);
		wcscat_s(szFind, L"\\*.xsbs");

		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)    return;

		while (true)
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char c[520];
				//std::cout << FindFileData.cFileName << std::endl;  
				WideCharToMultiByte(CP_ACP, 0, FindFileData.cFileName, 260, c, 520, NULL, NULL);
				fileList.push_back(c);
			}
			if (!FindNextFile(hFind, &FindFileData))    break;
		}
		FindClose(hFind);
	}
	bool mystrcmp(std::string a, std::string b)
	{
#define cmp <
		if (a.size() != b.size())
			return a.size() cmp b.size();
		auto ita = a.begin();
		auto itb = b.begin();
		while ( ita != a.end() && itb != b.end()&&(*ita) - (*itb) == 0 )
		{
			++ita;
			++itb;
		}
		if (ita == a.end())
		{
			if (itb == b.end())
			{
				return *ita cmp *itb;
			}
			else
			{
				return 0 cmp 1;
			}
		}
		else
		{
			if (itb == b.end())
			{
				return 1 cmp 0;
			}
			else
			{
				return *ita cmp *itb;
			}
		}
	}

}