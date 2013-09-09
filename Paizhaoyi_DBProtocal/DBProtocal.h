#pragma once
#include <string>
using std::string;

namespace mysqlpp{class Connection;}
typedef struct DBProtocal
{
	string m_strPath;
	string m_strMachineID;
	int m_sockFD;
	unsigned long m_uActionBitFlag;
	void *pParent;
	unsigned long m_nID;
	mysqlpp::Connection *m_pCurConn;

	DBProtocal(const string &strPath, const string &nID, int nSockFD, unsigned long uBitFlag = 0, void *pDah = NULL)
		:m_strPath(strPath), m_strMachineID(nID), m_sockFD(nSockFD), m_uActionBitFlag(uBitFlag), pParent(pDah)
	{

	}
} DBHead, *PDBHead;