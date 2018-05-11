#pragma once
#include"user.h"
#include"stdafx.h"
namespace cris {
	class user
	{
		bool logined = false;
	public:
		std::string username;
		std::string passwd;
		int current = -1;
		std::string lurd;
		void save()
		{
			std::ofstream fout;
			fout.open((".\\save\\" + username + ".save").c_str(), std::ios::out);
			fout.write(username.c_str(), 20);
			fout.write(passwd.c_str(), 30);
			fout << current;
			fout << lurd;
			fout.close();
			fout.open(".\\save\\l", std::ios::out);
			fout << username;
		}

		bool getLogin()
		{
			if (!logined)
			{
				std::string s;
				std::ifstream fin;
				fin.open(".\\save\\l", std::ios::in);
				if (!fin) return false;
				fin >> s;
				read(s);
				logined = true;
				return true;

			}
			else
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
			fin.read(name, 20);
			fin.read(passwd, 30);
			this->username = name;
			this->passwd = passwd;

			fin >> current;
			fin >> lurd;

		}
		void login() 
		{
			std::ofstream fout;
			fout.open(".\\save\\l", std::ios::out);
			fout << username;
			fout.close();
		}
	};
}