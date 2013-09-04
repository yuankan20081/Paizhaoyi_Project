#pragma once
#include <string>
using std::string;

typedef struct DBProtocal
{
	string m_strPath;
	string m_strMachineID;
	int m_sockFD;
	void *pParent;

	DBProtocal(const string &strPath, const string &nID, int nSockFD, void *pDah = NULL)
		:m_strPath(strPath), m_strMachineID(nID), m_sockFD(nSockFD), pParent(pDah)
	{

	}
} DBHead, *PDBHead;