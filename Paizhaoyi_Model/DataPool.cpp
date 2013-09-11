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



static pthread_mutex_t g_mtxQuery;
static pthread_cond_t g_condCanCreate;

static vector<string> g_vecField;
static vector<string> g_vecFieldPrice;
struct dumy
{
	dumy()
	{

		g_vecField.push_back("print");
		g_vecField.push_back("qrcode");
		g_vecField.push_back("print_qrcode");

		g_vecFieldPrice.push_back("print_price");
		g_vecFieldPrice.push_back("qrcode_price");
		g_vecFieldPrice.push_back("takeawayall_price");
	}
} Dah;//////////////////////////////////////////////////////////////////////////





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
	pthread_cond_signal(&g_condCanCreate);//已经保存数据，通知取数据线程
	
	//sleep(1);//等待生成二维码
	waitpid(nChild, NULL, 0);
	pthread_mutex_lock(&g_mtxQuery);
	mysqlpp::Query query = pConn->query();
	//vector<mysqlpp::Row> vecRow;
	query << "select qrcode_src, guid from qrtest where id=" << nID;
	//query.storein(vecRow);
	mysqlpp::StoreQueryResult res = query.store();
	pthread_mutex_unlock(&g_mtxQuery);
	if(res.num_rows() != 1)
	{

		//TODO
	}
	unsigned long uFileSize = GetFileSize(res[0]["qrcode_src"].c_str());
	if(0 == uFileSize)
	{
		Head tmp;
		strcpy(tmp.m_pszPicName, "");
		tmp.m_nDataLength = 0;
		Network::SendData(nSock, &tmp, sizeof(Head));
		return NULL;
	}
	ifstream ifs(res[0]["qrcode_src"].c_str());
	if(!ifs)
	{
		Head tmp;
		strcpy(tmp.m_pszPicName, "");
		tmp.m_nDataLength = 0;
		Network::SendData(nSock, &tmp, sizeof(Head));
		return NULL;
	}

	Head stRetHead;
	stRetHead.m_nDataLength = uFileSize;
	strcpy(stRetHead.m_pszPicName, res[0]["qrcode_src"].c_str());
	char *pBuf = new char[uFileSize + sizeof(Head)];
	memcpy(pBuf, &stRetHead, sizeof(Head));
	ifs.read(pBuf + sizeof(Head), uFileSize);
	Network::SendData(nSock, pBuf, uFileSize + sizeof(Head));
	delete[] pBuf;

	return NULL;

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
	pthread_mutex_lock(&g_mtxQuery);

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
		//DBHead stTmpHead(*pstDBHead);
		pthread_t tid;
		pthread_create(&tid, NULL, ThSendQRCode, (void *)pstDBHead);
		pthread_cond_wait(&g_condCanCreate, &g_mtxQuery);
		pthread_mutex_unlock(&g_mtxQuery);
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
	pthread_mutex_lock(&g_mtxQuery);
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
		mysqlpp::StoreQueryResult res_id = query.store();
		query << "select " << g_vecFieldPrice[i] << " from machine where machine=" << mysqlpp::quote_only << pstDBHead->m_strMachineID;
		mysqlpp::StoreQueryResult res_price = query.store();

		query << "insert into download_statistics(date, print_type_id, machineid, price) values(current_timestamp(), "
			<< res_id[0]["id"] << ", " << mysqlpp::quote_only << pstDBHead->m_strMachineID << ", " << mysqlpp::sql_decimal(res_price[0][g_vecFieldPrice[i].c_str()]) << ")";
		query.execute();
		
	}

	pthread_mutex_unlock(&g_mtxQuery);
	return false;
}
