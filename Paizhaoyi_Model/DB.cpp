#include "DB.h"
#include <pthread.h>
#include "../Paizhaoyi_CommonFuncs/CommonFuncs.h"

DB::DB(const string &strDbName, const string &strHost, const string &strUserName, const string &strPasswd /* = "" */)
	:m_strDBName(strDbName), m_strHost(strHost), m_strUserName(strUserName), m_strPasswd(strPasswd)
{
	//m_sqlConn = new Connection(m_strUserName, m_strPasswd, m_strDBName, m_strHost);
	//m_sqlConn = new mysqlpp::Connection(false);
	//m_sqlConn->connect(strDbName.c_str(), strHost.c_str(), strUserName.c_str(), strPasswd.c_str()); //��Ҫ�ж�
	m_sqlConn = new mysqlpp::Connection(strDbName.c_str(), strHost.c_str(), strUserName.c_str(), strPasswd.c_str());
	m_dpPool = new DataPool(100, m_sqlConn);
}

DB::~DB()
{
	delete m_sqlConn;

	delete m_dpPool;
}


bool DB::Save(const DBHead &stDBHead)
{
	//pthread_t stTID;

	//pthread_create(&stTID, NULL, dumy, (void *)&DBHead(stDBHead.m_strPath, stDBHead.m_strMachineID, stDBHead.m_sockFD, this));
	//m_dpPool->Save(stDBHead);

	return m_dpPool->Save(stDBHead);
}

//global obj
//DB mysqlDB("pzy", "127.0.0.1", "root");
DB mysqlDB(GetConfByName("db"), GetConfByName("host"), GetConfByName("user"), GetConfByName("pwd"));

/*
void *dumy(void *buf)
{
	DBHead *dbhHead = (DBHead *)buf;
	dbhHead->pParent->m_dpPool->Save(*dbhHead);
	return NULL;

}
*/

