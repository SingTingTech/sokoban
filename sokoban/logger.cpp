#include "logger.h"

std::fstream & operator<<(logger &log, std::string msg)
{
	log.file << msg << std::endl;
	return log.file;
}

logger::logger(std::string path, std::string loggerName)
{
	int i = 1;
	logFileName = path + loggerName + ".log";
	file.open(logFileName, std::ios::in | std::ios::out | std::ios::app);
	if (!file) {
		file.open(logFileName, std::ios::in | std::ios::out | std::ios::_Noreplace);
	}
	std::cout << 8;
}


logger::~logger()
{
	file.close();
}
