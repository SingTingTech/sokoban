#pragma once
#include"stdafx.h"

// 每调用一次向缓冲区中送入下1024个数据
class MapStream
{
	std::ifstream fin;
	bool isPrivate;
	bool first;
public:
	MapStream(std::string mappath,bool isPrivate)
	{
		first = true;
		fin.open(mappath);
		this->isPrivate = isPrivate;
	}


	void operator >> (char *buf)
	{
		memset(buf, 0, 1024);
		if (first)
		{
			buf[0] = 'u';
			buf[1] = isPrivate ? 't' : 'f';
			fin.read(buf + 2, 1022);
		}
		else {
			buf[0] = 't';
			fin.read(buf + 1, 1023);
		}
	}
	~MapStream()
	{
		fin.close();
	}

};