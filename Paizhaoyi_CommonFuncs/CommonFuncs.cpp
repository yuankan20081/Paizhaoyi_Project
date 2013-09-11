#include "CommonFuncs.h"
#include <sys/socket.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
using std::ifstream;
using std::cerr;
using std::map;

size_t GetFileSize(const string &strPath)
{
	if(0 == strPath.size())
	{
		return 0;
	}
	struct stat st;
	stat(strPath.c_str(), &st);
	return st.st_size;
}

void SendRaw(int sockFD, const char *pBuf, size_t uBufSize)
{
	const char *pTmp = pBuf;
	size_t uLeft = uBufSize;

	while(uLeft)
	{
		int ret = send(sockFD, pTmp, uLeft, 0);
		pTmp += ret;
		uLeft -= ret;
	}

}



static void SplitString(const string &strKV, map<string, string> &mapConf)
{
	string strKey, strVal;
	unsigned int nDeliPos = strKV.find("=");
	strKey.assign(strKV.c_str(), nDeliPos);
	strVal.assign(strKV.c_str(), nDeliPos + 1, strKV.size() - nDeliPos - 1);
	g_mapConf.insert(std::pair<string, string>(strKey, strVal));

}

string GetConfByName(const string &strKeyName)
{
	static map<string, string> mapConf;
	if(g_mapConf.size() != 0)
	{
		return g_mapConf.find(strKeyName)->second;
	}
	ifstream ifs("pzy_conf");
	if(!ifs)
	{
		cerr << "config file: pzy_conf not found\n" ; 
		exit(1);		
	}
	string strKV;
	while(ifs >> strKV)
	{
		SplitString(strKV, mapConf);
	}
	ifs.close();
	return g_mapConf[strKeyName];


}

