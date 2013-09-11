#include "Network.h"
#include "../Paizhaoyi_Log/Log.h"
#include "../Paizhaoyi_NetProtocal/NetProtocal.h"
#include "../Paizhaoyi_DBProtocal/DBProtocal.h"
#include "../Paizhaoyi_Model/DB.h"
#include "../Paizhaoyi_CommonFuncs/CommonFuncs.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <time.h>
#include <errno.h>
#define DEF_SAVE_DIR "./pics/"

//extern class DB;
extern DB mysqlDB;

static void SavePic(Head *stHead);
static void SaveActtion(Head *stHead);

static string MakePath(const char *pszFilename)
{
	std::ostringstream oss;
	//oss << DEF_SAVE_DIR << pszFilename;
	//oss << DEF_SAVE_DIR << time(0) << "-" << pszFilename;
	oss << GetConfByName("savepath") << time(0) << "-" << pszFilename;
	string strPath = oss.str();
	return strPath;
}

void Network::Start(const string &host, const int port)
{
	Network net(host, port);

	net.Init();
	net.Run();
}

int Network::SendData(int sockFD, void *buf, size_t size)
{
	//return net.SendRaw(buf, size);
	/*
	Head *pstHead = (Head *)buf;
	char *pData = (char *)buf + sizeof(Head);
	return send(pstHead->m_sockFD, pData, pstHead->m_nDataLength, 0);
	*/
	SendRaw(sockFD, (char *)buf, size);
	return 0;
}

/*
int Network::SendRaw(void *buf, size_t size)
{
	
}
*/



Network::Network(const string &host, const int port)
	:m_strHost(host), m_nPort(port)
{
	m_nPort = atoi(GetConfByName("svrport").c_str());
}
Network::~Network()
{

}

void *ThreadFunc(void *data)
{
	using std::ofstream;

	Head stHead = *((Head *)data);
	//std::shared_ptr<char> buf(new char[stHead->m_nDataLength]);
	
	if(stHead.m_bIsUpload)
	{
		SavePic(&stHead);
	}
	else
	{
		SaveActtion(&stHead);
	}
	return NULL;
}

// 函数返回true,表示接收数据并处理
// 函数返回false, 表示连接断开
static bool HandleClientConn(int fd)
{
	Head stHead;
	int ret = recv(fd, (void *)&stHead, sizeof(Head), 0);
	if(ret != sizeof(Head))
	{
		close(fd);
		return false;
	}

	stHead.m_sockFD = fd;
	pthread_t stThID;
	pthread_create(&stThID, NULL, ThreadFunc, (void *)&stHead);

	return true;
}

bool Network::Init()
{
	this->m_nListenFD = socket(AF_INET, SOCK_STREAM, 0);
	if(m_nListenFD < 0)
	{
		Log::Error("socket创建失败！");
		return false;
	}
	
	sockaddr_in stSvrAddr;
	stSvrAddr.sin_family = AF_INET;
	stSvrAddr.sin_port = htons(m_nPort);
	//stSvrAddr.sin_addr.s_addr = inet_addr(m_strHost.c_str());
	stSvrAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int n = 1;
	setsockopt(m_nListenFD, SOL_SOCKET, SO_REUSEADDR, (const void *)&n, sizeof(n));
	if(bind(m_nListenFD, (struct sockaddr *)&stSvrAddr, sizeof(stSvrAddr)) < 0)
	{
		Log::Error("bind指定IP和端口失败！");
		close(m_nListenFD);
		return false;
	}

	if(listen(m_nListenFD, 1024) < 0)
	{
		Log::Error("listen失败!");
		close(m_nListenFD);
		return false;
	}

	FD_ZERO(&m_stMainSet);
	FD_SET(m_nListenFD, &m_stMainSet);
	m_nMaxFD = m_nListenFD;
	Log::Info("网络初始化成功！");
	return true;

}


void Network::Run()
{
	while(1)
	{
		
		m_stTmpSet = m_stMainSet;
		int ret = select(m_nMaxFD + 1, &m_stTmpSet, NULL, NULL, NULL);
		if(ret < 0)
		{
			Log::Error("select函数错误！");
			return;
		}

		for(int fd = 0; fd <= m_nMaxFD; fd++)
		{
			if(!FD_ISSET(fd, &m_stTmpSet))
			{
				continue;
			}
			if(fd == m_nListenFD)
			{
				int nSockClient = accept(fd, NULL, NULL);
				FD_SET(nSockClient, &m_stMainSet);
				m_nMaxFD = m_nMaxFD > nSockClient ? m_nMaxFD : nSockClient;////
				Log::Info("xinlianjie");
			}
			else
			{
				SetNonBlock(fd);
				//客户端连接后，应该将set中的位去掉。
				//注意重置m_nMaxFD!
				FD_CLR(fd, &m_stMainSet);
				if(!HandleClientConn(fd))
				{
					//FD_CLR(fd, &m_stMainSet);
					Log::Info("connection not handled!");
				}
			}
		}
		

	}
}



void SavePic(Head *stHead)
{
	char *buf = new char[stHead->m_nDataLength];
	int nLeft = stHead->m_nDataLength;
	char *tmp = buf;
	int nSockFD = stHead->m_sockFD;
	while(nLeft)
	{
		int ret = recv(nSockFD, (void *)tmp, nLeft, 0);
		//ret <= 0
		if(ret == 0)
		{
			//TODO
			delete[] buf;
			close(stHead->m_sockFD);
			return;
		}
		else if(ret < 0)
		{
			if(errno == EAGAIN)
			{
				continue;
			}
			else
			{
				delete[] buf;
				close(stHead->m_sockFD);
				return;
			}
		}
		tmp += ret;
		nLeft -= ret;
	}


	//TODO
	//保存文件并写入数据库
	string strPath = MakePath(stHead->m_pszPicName);
	ofstream ofs(strPath.c_str());
	if(!ofs)
	{
		Log::Critical("failed to save photo");
		return;
	}
	ofs.write(buf, stHead->m_nDataLength);
	ofs.close();
	delete[] buf;

	mysqlDB.Save(DBHead(strPath, stHead->m_pszMachineID, nSockFD));
}

void SaveActtion(Head *stHead)
{
	mysqlDB.Save(DBHead("", stHead->m_pszMachineID, stHead->m_sockFD, stHead->m_uActionBitFlag));
}

void Network::SetNonBlock(int nSocketFD)
{
	//struct timeval timeout = {3, 0};
	fcntl(nSocketFD, F_SETFL, O_NONBLOCK);
	//setsockopt(nSocketFD, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	//setsockopt(nSocketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}