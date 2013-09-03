#pragma once
#include <string>
#include <iostream>
#include <fstream>
using std::string;
using std::ofstream;


class Log
{
public:
	enum {CRITICAL=1, ERROR=2, WARNING=3, INFO=4};
	static void Start(const string &strFilePath="");
	static void Info(const string &strMsg);
	static void Warning(const string &strMsg);
	static void Error(const string &strMsg);
	static void Critical(const string &strMsg);

	static void SetLevel(int lvl);

private:
	Log(const string &strFilepath);
	~Log();
private:
	string m_strLogFileName;
	int m_nLogLvl;
	bool m_bUseStd;
	ofstream m_os;	
private:
	static Log *log;
	
	
};