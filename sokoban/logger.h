#pragma once
#include"stdafx.h"

const int LOGMAX = 20;

class logger {
	std::string logFileName;
	std::fstream file;


public:
	friend std::fstream &operator<<(logger&,std::string);
	logger(std::string path, std::string loggerName);
	~logger();
};
std::fstream &operator<<(logger&, std::string);