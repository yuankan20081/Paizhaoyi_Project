#include "DataPool.h"
#include <map>
#include <sstream>
using std::map;
using std::ostringstream;

#define MAX_SIZE 100

/*
void *ThFuncInSave(void *buf)
{
	DataPool *dp = (DataPool *)buf;
	pthread_mutex_lock(&dp->m_stMutex);

	while(dp->m_qDataPool.empty())
	{
		pthread_cond_wait(&dp->m_stCanSave, &dp->m_stMutex);
	}

	dp->m_qDataPool.push("");

	pthread_mutex_unlock(&dp->m_stMutex);
	return NULL;
}
*/

void *ThDump(void *buf)
{
	DataPool *pPool = (DataPool *)buf;
	while(1)
	{
		pPool->DumpToDB();
	}
}

DataPool::DataPool(int nPoolSize, Connection *conn)
	:m_nPoolSize(nPoolSize), m_sqlConn(conn)
{
	pthread_mutex_init(&m_stMutex, NULL);
	pthread_cond_init(&m_stCanRead, NULL);
	pthread_cond_init(&m_stCanSave, NULL);

	//start dump thread
	pthread_t id;
	pthread_create(&id, NULL, ThDump, (void *)this);
}

DataPool::~DataPool()
{
	pthread_mutex_destroy(&m_stMutex);
	pthread_cond_destroy(&m_stCanSave);
	pthread_cond_destroy(&m_stCanRead);
}

bool DataPool::Save(const DBHead &stDBHead)
{
	//int nID;
	//pthread_create(&nID, NULL, ThFuncInSave, (void *)this);

	pthread_mutex_lock(&this->m_stMutex);

	while(this->m_qDataPool.size() == MAX_SIZE)
	{
		pthread_cond_wait(&this->m_stCanSave, &this->m_stMutex);
	}

	this->m_qDataPool.push(stDBHead);
	pthread_cond_signal(&this->m_stCanRead);

	pthread_mutex_unlock(&this->m_stMutex);
	return true;
}

bool DataPool::DumpToDB()
{
	pthread_mutex_lock(&this->m_stMutex);

	while(this->m_qDataPool.empty())
	{
		pthread_cond_wait(&this->m_stCanRead, &this->m_stMutex);
	}

	DBHead stDBHead = m_qDataPool.front();
	m_qDataPool.pop();
	ostringstream oss;
	oss << "insert into test(pic_path, machine_id) values('" << stDBHead.m_strPath << "', '" << stDBHead.m_strMachineID << "')";
	string strSql = oss.str();
	try
	{
		m_sqlConn->Execute(strSql);
	}
	catch(string &strError)
	{

	}
	pthread_cond_signal(&this->m_stCanSave);
	pthread_mutex_unlock(&this->m_stMutex);
	return true;
}