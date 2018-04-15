#pragma once
#include"user.h"
#include"stdafx.h"
namespace cris {
	class user
	{
	public:
		std::string username;
		std::string passwd;
		int current;
		std::string lurd;
		void save()
		{
			std::ofstream fout;
			fout.open((".\\save\\" + username + ".save").c_str(), std::ios::out | std::ios::_Noreplace);
			fout.write(username.c_str(), 20);
			fout.write(passwd.c_str(), 30);
			fout << current;
			fout << lurd;
			fout.close();
			fout.open(".\\save\\l", std::ios::out | std::ios::_Noreplace);
			fout << username;
		}

		bool getLogin()
		{
			std::string s;
			std::ifstream fin;
			fin.open(".\\save\\l", std::ios::in);
			if (!fin) return false;
			read(s);
			return true;
		}
		void read(std::string username)
		{
			char name[20];
			char passwd[30];
			std::ifstream fin;
			fin.open((".\\save\\" + username + ".save").c_str(), std::ios::in);
			if (!fin)
				return;
			this->username = name;
			this->passwd = passwd;
			fin.read(name, 20);
			fin.read(passwd, 30);
			fin >> current;
			fin >> lurd;

		}
	};
}