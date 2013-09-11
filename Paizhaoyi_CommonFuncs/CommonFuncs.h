#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using std::string;

size_t GetFileSize(const string	&strPath);
void SendRaw(int sockFD, const char *pBuf, size_t uBufSize);

time_t GetDayBegin();

string GetConfByName(const string &strKeyName);
