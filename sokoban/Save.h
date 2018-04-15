#pragma once
#include"stdafx.h"
#include"map.h"
namespace cris {

	class Save {
	protected:
		std::string savepath;
		std::string saveFileName;
		std::fstream file;
	public:
		std::fstream & operator<<(std::string);
		Save(std::string path, std::string saveName);
		void createSave(std::string username) 
		{
			if (!file) {
				file.open(saveFileName, std::ios::out | std::ios::_Noreplace);
			}
		}

		~Save();
	};
	class MapSave:public Save 
	{
	public:
		std::fstream & operator<<(map m) 
		{
			 
		}
	};
	
}
