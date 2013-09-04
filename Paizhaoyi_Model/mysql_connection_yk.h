#pragma once
//#include <WinSock2.h>
#include <string>
#include <mysql.h>
#include "mysql_recordset_yk.h"
using namespace std;
class Connection
{
	
public:
	Connection(const string &_UserName, const string &_Password, const string &_dbname, const string &HostName = "localhost");
	~Connection();

	RecordSet Execute(const string &_sqlstr);

private:
	MYSQL m_mysql;
	MYSQL_RES *m_sqlResultSet;


	string m_strHostName;
	string m_strUserName;
	string m_strPassword;
	string m_strDBName;
	unsigned int m_nPort;
	
	bool m_bValidConn;


};