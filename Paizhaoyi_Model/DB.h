#pragma once
#include "mysql_connection_yk.h"
#include "../Paizhaoyi_DBProtocal/DBProtocal.h"
#include "DataPool.h"
#include <string>
using std::string;

class DB
{
public:
	bool Save(const DBHead &stDBHead);
	//virtual bool Fetch();
public:
	DB(const string &strDbName, const string &strHost, const string &strUserName,
		const string &strPasswd = "");
	~DB();
private:
	Connection *m_sqlConn;
	string m_strDBName;
	string m_strHost;
	string m_strUserName;
	string m_strPasswd;
private:
	DataPool *m_dpPool;

	//friend void *dumy(void *buf);

};