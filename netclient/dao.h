#pragma once
#include<windows.h>
#include<sql.h>
#include<sqltypes.h>
#include<sqlext.h>
#include<odbcss.h>
#include<iostream>
#include<vector>
#include"md5.h"


class MapInfo 
{
public:
	std::string mappath;
	std::string solutionpath;
	bool isprivate;
	MapInfo(std::string mappath, std::string solutionpath, bool isprivate) 
	{
		this->mappath = mappath;
		this->isprivate = isprivate;
		this->solutionpath = solutionpath;
	}
	MapInfo() = default;
};
class Dao
{

	UCHAR DataSourceName[15] = "sokoban";
	UCHAR userName[15] = "root";
	UCHAR pwd[15] = "root";

	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;


public:



	void ConnectInit();
	void ExcuteUpdate(SQLCHAR * x, int size);
	int Register(char * name, char * passwd, SQL_DATE_STRUCT regdate, bool isadmin);
	void Query();
	void GenerateTables();
	void Disconnect();
	bool Login(char* username,char*password);
	int upload(char* mapPath, char* soluPath, bool isPrivate,char*uploader);
	void getMaps(char*user, std::vector<MapInfo> &mapInfos);
};
