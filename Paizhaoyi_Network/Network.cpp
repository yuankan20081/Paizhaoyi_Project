#include "Network.h"
#include "../Paizhaoyi_Log/Log.h"
#include "../Paizhaoyi_NetProtocal/NetProtocal.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>


void Network::Start(const string &host, const int port)
{
	Network net(host, port);

	net.Init();
	net.Run();
}

int Network::SendData(void *buf, size_t size)
{
	//return net.SendRaw(buf, size);
	Head *pstHead = (Head *)buf;
	char *pData = (char *)buf + sizeof(Head);
	return send(pstHead->m_sockFD, pData, pstHead->m_nDataLength, 0);
}

int Network::SendRaw(void *buf, size_t size)
{
	
}

Network::Network(const string &host, const int port)
	:m_strHost(host), m_nPort(port)
{

}
Network::~Network()
{

}

void *ThreadFunc(void *data)
{
	Head *stHead = (Head *)data;
	std::shared_ptr<char> buf(new char[stHead->m_nDataLength]);
	int ret = recv(stHead->m_sockFD, (void *)buf.get(), stHead->m_nDataLength, 0);

	//TODO
	//写入数据库
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
	stSvrAddr.sin_addr.s_addr = inet_addr(m_strHost.c_str());

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
				if(!HandleClientConn(fd))
				{
					FD_CLR(fd, &m_stMainSet);
					Log::Info("one left");
				}
			}
		}
		

	}
}



