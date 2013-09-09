#pragma once

typedef struct NetProtocal
{
	bool m_bIsUpload; //用来判断是否是照片上传请求
	unsigned long m_uActionBitFlag;//通过位来客户端判断动作
	char m_pszMachineID[20];
	int m_nDataLength;
	int m_sockFD;
	char m_pszPicName[128];
} Head, *PHead;