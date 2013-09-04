#include "DB.h"
#include <pthread.h>

DB::DB(const string &strDbName, const string &strHost, const string &strUserName, const string &strPasswd /* = "" */)
	:m_strDBName(strDbName), m_strHost(strHost), m_strUserName(strUserName), m_strPasswd(strPasswd)
{
	m_sqlConn = new Connection(m_strUserName, m_strPasswd, m_strDBName, m_strHost);
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
DB mysqlDB("pzy", "127.0.0.1", "root");

/*
void *dumy(void *buf)
{
	DBHead *dbhHead = (DBHead *)buf;
	dbhHead->pParent->m_dpPool->Save(*dbhHead);
	return NULL;

}
*/

