#pragma once
//#include "mysql_connection_yk.h"
#include "../Paizhaoyi_DBProtocal/DBProtocal.h"
#include <pthread.h>
#include <queue>
#include <string>
#include <pthread.h>
#include <mysql++.h>
using std::queue;
using std::string;


class DataPool
{
	//using namespace mysqlpp;

public:
	DataPool(int nPoolSize, mysqlpp::Connection *conn);
	~DataPool();
	bool Save(const DBHead &stDBHead);
	bool DumpToDB();
	//
	bool DumpPicPath(DBHead *pstDBHead);
	bool DumpAction(DBHead *pstDBHead);
private:
	queue<DBHead> m_qDataPool;
	pthread_mutex_t m_stMutex;
	pthread_cond_t m_stCanSave, m_stCanRead;
	
	int m_nPoolSize;
private:
	mysqlpp::Connection *m_sqlConn;


};