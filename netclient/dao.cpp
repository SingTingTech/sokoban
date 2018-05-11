
#include"dao.h"

void Dao::ConnectInit()
{
	RETCODE ret;
	SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, SQL_INVALID_HANDLE, SQL_PARAM_DATA_AVAILABLE;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	ret = SQLConnect(hdbc, DataSourceName, SQL_NTS, userName, SQL_NTS, pwd, SQL_NTS);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO))
	{
		std::cout << "连接失败" << ret<<std::endl;
	}
	else
	{
		std::cout << "连接成功"<<std::endl;
	}
}


void Dao::ExcuteUpdate(SQLCHAR *x, int size)
{
	RETCODE ret;
	SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	ret = SQLExecDirect(hstmt, x, size);
	std::cout << ret;
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

int Dao::Register(char *name, char *pass, SQL_DATE_STRUCT regdate, bool isadmin)
{
	char passwd[33];
	md5 m;
	strcpy_s(passwd, (char*)m.getMD5(pass).c_str());
	/*
	char id[11];
	char name[21];

	tagDATE_STRUCT regdate;
	bool isadmin;
	*/
	UCHAR x[] = "INSERT INTO USER(NAME,passwd,regtime,isadmin) VALUES(?,?,?,?)";

	RETCODE ret;
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	SQLPrepare(hstmt, x, strlen((char*)x));
	ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, name, 0, 0);
	ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 33, 0, passwd, 0, 0);
	 ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_DATE, SQL_DATE, 0, 0, &regdate, 0, 0);
	ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &isadmin, 0, 0);

	ret = SQLExecute(hstmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if (!ret) 
	{
		std::cout << "注册成功！";
		return true;
	}
	std::cout << "注册失败！";

	return false;
}

void Dao::Query()
{

	char name[21];
	char passwd[33];
	SQL_DATE_STRUCT regdate;
	bool isadmin;



	UCHAR x[] = "select * from user;";
	SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	SQLExecDirect(hstmt, x, strlen((char*)x));
	SQLBindCol(hstmt, 2, SQL_C_CHAR, name, 21, 0);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, passwd, 33, 0);
	SQLBindCol(hstmt, 4, SQL_C_DATE, &regdate, sizeof regdate, 0);
	SQLBindCol(hstmt, 5, SQL_C_STINYINT, &isadmin, sizeof isadmin, 0);
	while (!SQLFetch(hstmt))
		std::cout << " name:" << name << " password:" << passwd << " isadmin:" << isadmin << std::endl;

}

void Dao::GenerateTables()
{
	UCHAR usersql[] = "CREATE TABLE USER(\
			NAME CHAR(20) NOT NULL,\
			PASSWD CHAR(32) NOT NULL,\
			REGTIME DATE NOT NULL,\
			ISADMIN TINYINT(1) NOT NULL,\
			PRIMARY KEY(NAME)\
			) AUTO_INCREMENT=0;";
	ExcuteUpdate(usersql, strlen((char*)usersql));
	UCHAR mapsql[] = "CREATE TABLE MAP(\
			MAPID INT(32) NOT NULL AUTO_INCREMENT,\
			MAPPATH VARCHAR(256) NOT NULL,\
			SOLUTIONPATH VARCHAR(256) NOT NULL,\
			PRIVATE TINYINT(1) NOT NULL,\
			USERID CHAR(20) NOT NULL,\
			PRIMARY KEY(MAPID),\
			FOREIGN KEY(USERID) REFERENCES USER(NAME)\
			) AUTO_INCREMENT=0;";
	ExcuteUpdate(mapsql, strlen((char*)mapsql));
}

void Dao::Disconnect()
{
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

bool Dao::Login(char * username, char * password)
{
	char passwd[33];
	md5 m;


	UCHAR x[] = "select * from user where name=?;";
	RETCODE ret;
	SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(username), 0, username, 0, 0);
	ret = SQLPrepare(hstmt, x, strlen((char*)x));
	ret = SQLExecute(hstmt);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, passwd, 33, 0);
	if(!SQLFetch(hstmt))
	{
		if(!strcmp(m.getMD5( password).c_str(), passwd))
			return true;
		
	}
	return false;
}

int Dao::upload(char * mapPath, char * soluPath, bool isPrivate, char * uploader)
{
	UCHAR x[] = "INSERT INTO MAP(mappath,solutionpath,private,userid) VALUES(?,?,?,?);";

	RETCODE ret;
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	SQLPrepare(hstmt, x, strlen((char*)x));
	ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, mapPath, 0, 0);
	ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, soluPath, 0, 0);
	ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &isPrivate, 0, 0);
	ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, uploader, 0, 0);
	ret = SQLExecute(hstmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if (!ret)
	{
		std::cout << "上传成功！";
		return 0;
	}
	std::cout << ret;

	std::cout << "上传失败！";
	return -1;
}

void Dao::getMaps(char * user,std::vector<MapInfo> &mapInfos)
{
	UCHAR mappath[256] = { 0 };
	UCHAR solutionpath[256] = { 0 };
	bool isprivate;
	UCHAR x[] = "SELECT MAPPATH,SOLUTIONPATH,PRIVATE FROM MAP WHERE USERID=? OR PRIVATE=TRUE ORDER BY MAPID;";
	RETCODE ret;
	SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	ret = SQLPrepare(hstmt, x, strlen((char*)x));
	ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, user, 0, 0);
	SQLBindCol(hstmt, 1, SQL_C_CHAR, mappath, 21, 0);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, solutionpath, 33, 0);
	SQLBindCol(hstmt, 3, SQL_C_STINYINT, &isprivate, sizeof isprivate, 0);
	SQLExecute(hstmt);
	while (!SQLFetch(hstmt))
	{
		//遍历操作
		mapInfos.push_back(MapInfo((char*)mappath, (char*)solutionpath, isprivate));
	}
}




#ifdef DEBUG_DAO

int main()
{
	/*/
	Dao d;

	d.ConnectInit();

	//d.GenerateTables();
	/*/


	//*/
	/*/
	SQL_DATE_STRUCT date;
	date.year = 2018;
	date.month = 03;
	date.day = 31;
	//.Register("cris", "hello", date, false);
	std::cout<<d.Login("cris", "hello");
	d.Query();
	d.Disconnect();

	//*/

	std::cin.get();
}
#endif // DEBUG



