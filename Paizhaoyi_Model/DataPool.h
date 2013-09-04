#pragma once
#include "mysql_connection_yk.h"
#include "../Paizhaoyi_DBProtocal/DBProtocal.h"
#include <pthread.h>
#include <queue>
#include <string>
#include <pthread.h>
using std::queue;
using std::string;

class DataPool
{
public:
	DataPool(int nPoolSize, Connection *conn);
	~DataPool();
	bool Save(const DBHead &stDBHead);
	bool DumpToDB();
private:
	queue<DBHead> m_qDataPool;
	pthread_mutex_t m_stMutex;
	pthread_cond_t m_stCanSave, m_stCanRead;
	
	int m_nPoolSize;
private:
	Connection *m_sqlConn;


};