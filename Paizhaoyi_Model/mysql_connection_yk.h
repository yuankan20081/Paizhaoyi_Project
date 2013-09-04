#pragma once
#include <string>
#include "mysql_recordset_yk.h"

class Connection
{
	using std::string;
public:
	Connection(const string &_UserName, cont string &_Password, const string &HostName = "localhost");
	~Connection();

	RecordSet Execute(const string &_sqlstr);

private:
	MYSQL m_mysql;
	MYSQL_RES *m_sqlResultSet;


	string m_strHostName;
	string m_strUserName;
	string m_strPassword;
	unsigned int m_nPort;
};