#pragma once

typedef struct NetProtocal
{
	bool m_bIsUpload; //�����ж��Ƿ�����Ƭ�ϴ�����
	unsigned long m_uActionBitFlag;//ͨ��λ���ͻ����ж϶���
	char m_pszMachineID[20];
	int m_nDataLength;
	int m_sockFD;
	char m_pszPicName[128];
} Head, *PHead;