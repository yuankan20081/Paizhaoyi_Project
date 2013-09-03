#pragma once

typedef struct NetProtocal
{
	bool m_bReqRQCode;
	unsigned long m__strMachineID;
	int m_nDataLength;
	int m_sockFD;
	char m_pszPicName[128];
} Head, *PHead;