#include "Save.h"


std::fstream & cris::Save::operator<<(std::string str)
{
	file << str;
	return this->file;
}

cris::Save::Save(std::string path, std::string saveName)
{
	int i = 1;
	saveFileName = path + saveName + ".save";
	file.open(saveFileName, std::ios::out);



}

cris::Save::~Save()
{
	file.close();
}

