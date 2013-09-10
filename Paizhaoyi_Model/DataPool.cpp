#include "DataPool.h"
#include "../Paizhaoyi_Log/Log.h"
#include "../Paizhaoyi_CommonFuncs/CommonFuncs.h"
#include "../Paizhaoyi_Network/Network.h"
#include "../Paizhaoyi_NetProtocal/NetProtocal.h"
#include <cstring>
#include <map>
#include <sstream>
#include <bitset>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <sys/wait.h>
using std::bitset;
using std::map;
using std::ostringstream;
using std::vector;
using std::string;
using std::ifstream;

#define MAX_SIZE 100

static pthread_mutex_t g_mtxPic;
static pthread_mutex_t g_mtxAct;

static vector<string> g_vecField;
struct dumy
{
	dumy()
	{

		g_vecField.push_back("print");
		g_vecField.push_back("qrcode");
		g_vecField.push_back("print_qrcode");
	}
} Dah;//////////////////////////////////////////////////////////////////////////

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
	return NULL;
}

void *ThSendQRCode(void *buf)
{
	DBHead *pstDBHead = (DBHead *)buf;
	int nSock = pstDBHead->m_sockFD;
	unsigned long nID = pstDBHead->m_nID;
	mysqlpp::Connection *pConn = pstDBHead->m_pCurConn;
	int nChild = pstDBHead->m_nChild;
	//sleep(1);//等待生成二维码
	waitpid(nChild, NULL, 0);
	mysqlpp::Query query = pConn->query();
	//vector<mysqlpp::Row> vecRow;
	query << "select qrcode_src, guid from qrtest where id=" << nID;
	//query.storein(vecRow);
	mysqlpp::StoreQueryResult res = query.store();
	if(res.num_rows() != 1)
	{

		//TODO
	}
	unsigned long uFileSize = GetFileSize(res[0]["qrcode_src"].c_str());
	ifstream ifs(res[0]["qrcode_src"].c_str());
	if(!ifs)
	{
		Head tmp;
		strcpy(tmp.m_pszPicName, "");
		tmp.m_nDataLength = 0;
		Network::SendData(pstDBHead->m_sockFD, &tmp, sizeof(Head));
		return NULL;
	}

	Head stRetHead;
	stRetHead.m_nDataLength = uFileSize;
	strcpy(stRetHead.m_pszPicName, res[0]["qrcode_src"].c_str());
	char *pBuf = new char[uFileSize + sizeof(Head)];
	memcpy(pBuf, &stRetHead, sizeof(Head));
	ifs.read(pBuf + sizeof(Head), uFileSize);
	Network::SendData(pstDBHead->m_sockFD, pBuf, uFileSize + sizeof(Head));
	delete[] pBuf;




	/*
	ostringstream oss;
	oss << "select qr_code , uuid from test where id=" << nID - 1;
	try
	{
		RecordSet res = pConn->Execute(oss.str());
		res.FetchOneRow();
		Log::Info("before 1st");
		Log::Info(res.FieldName(0));
		ifstream ifs(res["qr_code"].c_str());
		Log::Info("first qrcode");
		if(!ifs)
		{
			return NULL;
		}
		unsigned long uSize = GetFileSize(res["qr_code"].c_str());
		Log::Info(res["qr_code"]);
		Log::Info("2nd qr_code");
		Head stRetHead;
		char *pBuf = new char[uSize + sizeof(stRetHead)];
		ifs.read(pBuf + sizeof(stRetHead), uSize);
		ifs.close();
		strcpy(stRetHead.m_pszPicName, res["uuid"].c_str());
		Log::Info(res["uuid"]);
		stRetHead.m_nDataLength = uSize;
		memcpy(pBuf, &stRetHead, sizeof(stRetHead));
		Network::SendData(pstDBHead->m_sockFD, pBuf, uSize + sizeof(stRetHead));

		delete[] pBuf;
		
		
	}
	catch(string &strError)
	{
	  Log::Info(strError);
	  return NULL;
	}
	*/
}

DataPool::DataPool(int nPoolSize, mysqlpp::Connection *conn)
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
	
	pthread_cond_signal(&this->m_stCanSave);
	pthread_mutex_unlock(&this->m_stMutex);

	if(0 == stDBHead.m_uActionBitFlag)
	{
		this->DumpPicPath(&stDBHead);
	}
	else
	{
		this->DumpAction(&stDBHead);
	}
	return true;
}

bool DataPool::DumpPicPath(DBHead *pstDBHead)
{
	pthread_mutex_lock(&g_mtxPic);

	mysqlpp::Query query = this->m_sqlConn->query();
	query << "insert into qrtest(images, machineid) values(" << mysqlpp::quote_only << pstDBHead->m_strPath << ", " 
		<< mysqlpp::quote_only << pstDBHead->m_strMachineID << ")";
	query.execute();

	//vector<Row> vecRow;
	query << "select id from qrtest where images=" << mysqlpp::quote_only << pstDBHead->m_strPath << "and machineid=" << mysqlpp::quote_only
		<< pstDBHead->m_strMachineID;
	//query.storein(vecRow);
	mysqlpp::StoreQueryResult res = query.store();
	if(res.num_rows() != 1)
	{
		//TODO
		return false;
	}
	unsigned long uID = atol(res[0]["id"].c_str());
	pstDBHead->m_nID = uID;
	pstDBHead->m_pCurConn = m_sqlConn;

	//TODO
	//guid, qrcode generate
	
	if((pstDBHead->m_nChild= fork()) == 0)
	{

		execl("/usr/bin/python", "usr/bin/python", "ReqQRCode.py", res[0]["id"].c_str(), NULL);
		return true;
	}
	else
	{
		DBHead stTmpHead(*pstDBHead);
		pthread_t tid;
		pthread_create(&tid, NULL, ThSendQRCode, (void *)&stTmpHead);

		pthread_mutex_unlock(&g_mtxPic);
		return true;
	}
	
	
	
	/*
	ostringstream oss;
	oss << "insert into test(pic_path, machine_id) values('" << pstDBHead->m_strPath << "', '" << pstDBHead->m_strMachineID << "')";
	string strSql = oss.str();
	try
	{
		m_sqlConn->Execute(strSql);
		oss.clear();
		oss.str("");
		oss << "select id from test where machine_id='" << pstDBHead->m_strMachineID << "' and pic_path='" << pstDBHead->m_strPath << "'";
		RecordSet res = m_sqlConn->Execute(oss.str());
		res.FetchOneRow();
		pstDBHead->m_nID = atoi(res["id"].c_str());
		pstDBHead->m_pCurConn = m_sqlConn;
		pthread_t id;
		pthread_create(&id, NULL, ThSendQRCode, (void *)pstDBHead);
		pthread_mutex_unlock(&g_mtxPic);
		
		return true;
	}
	catch(string &strError)
	{
	  Log::Info(strError);
	  
		pthread_mutex_unlock(&g_mtxPic);
		return false;
	}
	*/
}


bool DataPool::DumpAction(DBHead *pstDBHead)
{
	pthread_mutex_lock(&g_mtxAct);
	bitset<3> flag(pstDBHead->m_uActionBitFlag);
	for(int i = 0; i < 3; ++i)
	{
		if(!flag[i])
		{
			continue;
		}

		mysqlpp::Query query = m_sqlConn->query();
		/*
		query << "insert into download_statistics(date, " << g_vecField[i] << ", machineid) values(current_timestamp(), " 
			 << "1, " << mysqlpp::quote_only << pstDBHead->m_strMachineID << ")";
		*/
		query << "select id from print_type where description=" << mysqlpp::quote_only << g_vecField[i];
		mysqlpp::StoreQueryResult res = query.store();
		query << "insert into download_statistics(date, print_type_id, machineid) values(current_timestamp(), "
			<< res[0]["id"] << ", " << mysqlpp::quote_only << pstDBHead->m_strMachineID << ")";
		query.execute();
		
	}
	/*
	ostringstream oss;
	for(int i = 0; i < 3; i++)
	{
		if(flag[i])
		{
			oss << "select " << g_vecField[i] << " from count where machine_id='" << pstDBHead->m_strMachineID << "'";
			string strSqlGetVal = oss.str();
			try
			{
				RecordSet res = m_sqlConn->Execute(strSqlGetVal);
				res.FetchOneRow();
				int nOrigCount = atoi(res[g_vecField[i]].c_str());
				oss.clear();
				oss.str("");
				oss << "update count set " << g_vecField[i] << "=" << nOrigCount + 1 
					<< " where machine_id='" << pstDBHead->m_strMachineID << "'";
				string strSqlUpdateVal = oss.str();
				m_sqlConn->Execute(strSqlUpdateVal);
				
				pthread_mutex_unlock(&g_mtxAct);
				return true;
			}
			catch(string &strError)
			{
				pthread_mutex_unlock(&g_mtxAct);
				return false;
			}
		}
	}

	*/
	pthread_mutex_unlock(&g_mtxAct);
	return false;
}
