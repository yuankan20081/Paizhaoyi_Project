#pragma once
#include "mysql_connection_yk.h"
#include <string>
using std::string;

class DB
{
public:
	virtual bool Save() = 0;
	virtual bool Fetch() = 0;
private:
	Connection *conn;
	string m_strDBName;
	string m_strHost;
	string m_strUserName;
	string m_strPasswd;


};