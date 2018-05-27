#include"stdafx.h"
#include "dao.h"


void findXSBs(char* lpPath, std::vector<std::string> &fileList)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy_s(szFind, lpPath);
	strcat_s(szFind, "\\*.xsbs");

	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)   
		return;

	while (true)
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			fileList.push_back(FindFileData.cFileName);
		}
		if (!FindNextFile(hFind, &FindFileData))  
			break;
	}
	FindClose(hFind);
}
void upload(char *mapdir) 
{
	Dao d;
	d.ConnectInit();
	std::vector<std::string> filelist;
	findXSBs(mapdir, filelist);
	char mappath[20];
	char solpath[20] = {0};
	for (auto it = filelist.begin(); it != filelist.end(); ++it) 
	{
		strcpy_s(mappath, (".\\maps\\" + *it).c_str());
		d.upload(mappath, solpath, false, "admin");

	}
	d.Disconnect();
}
#ifdef TEST
int main() 
{
	upload(".\\maps");
}

#endif // !TEST