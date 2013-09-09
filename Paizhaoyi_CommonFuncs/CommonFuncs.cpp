#include "CommonFuncs.h"
#include <sys/socket.h>
#include <time.h>


size_t GetFileSize(const string &strPath)
{
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

