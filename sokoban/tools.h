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
		wcscat_s(szFind, L"\\*.xsb");

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
}