#include"../sokoban/stdafx.h"
#include"../sokoban/tools.h"
int Renamemaps()
{
	static int i = 0;
	char buf[20];
	char buf1[32] = {0};
	std::vector<std::string> maps;
	wchar_t savepath[] = L".\\test";
	cris::findXSBs(savepath, maps);
	std::ifstream fin;
	std::ofstream fout;
	int m=5;
	for (auto it = maps.begin(); it != maps.end(); it++)
	{
		fin.open(std::string(".\\test\\") + *it);
		sprintf_s(buf, ".\\test\\%d.xsbs", i);
		fout.open(buf);
		std::string line;
		int x = 0;
		while (!fin.eof())
		{
			std::getline(fin, line);
			fout << line<<std::endl;
			if(line.size()==0)
				++x;
		}
		if (x < m)
			m = x;
		fin.clear();
		fin.close();
		//remove((std::string(".\\test\\") + *it).c_str());
		fout.clear();
		fout.close();
		i++;
	}
	return m;
}

class MapStream
{
	std::ifstream fin;

	bool first;
public:
	MapStream(std::string mappath)
	{
		first = true;
		fin.open(mappath);

	}
	

	void operator >> (char *buf)
	{
		memset(buf, 0, 5);
		if (first)
			buf[0] = 'd';
		fin.read(buf+1,4);
		if (fin.eof()) 
		{
			int i = fin.gcount();
			buf[i] = ';';
		}
	}
	~MapStream() 
	{
		fin.close();
	}

};

int main() 
{
	MapStream map = { ".\\test\\screen.1" };

	char buf[5];
	map >> buf;
	map >> buf;
	return 0;
}