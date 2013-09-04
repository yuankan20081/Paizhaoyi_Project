#pragma once

typedef struct NetProtocal
{
	bool m_bReqRQCode;
	char m_pszMachineID[20];
	int m_nDataLength;
	int m_sockFD;
	char m_pszPicName[128];
} Head, *PHead;