#pragma once
#include <string>
#include <sys/select.h>
using std::string;

class Network
{
public:
	static void Start(const string &host, const int port);
	~Network();
	static int SendData(int sockFD, void *buf, size_t size);
private:
	Network(const string &host, const int port);
	bool Init();
	void Run();
	//int SendRaw(void *buf, size_t size);
	void SetNonBlock();
private:
	int m_nListenFD;
	string m_strHost;
	int m_nPort;
	fd_set m_stMainSet, m_stTmpSet;
	int m_nMaxFD;
	//
	//static Network	net;
	
};