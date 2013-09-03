#include "Log.h"

Log *Log::log = NULL;

void Log::Start(const string &strFilePath/* ="" */)
{
	Log::log = new Log(strFilePath);	
}

void Log::Info(const string &strMsg)
{
	if(log->m_nLogLvl < Log::INFO)
	{
		return;
	}
	(log->m_bUseStd ? std::cout : log->m_os) << strMsg << std::endl;
}

void Log::Warning(const string &strMsg)
{
	if(log->m_nLogLvl < Log::WARNING)
	{
		return;
	}
	(log->m_bUseStd ? std::cout : log->m_os) << strMsg << std::endl;
}

void Log::Error(const string &strMsg)
{
	(log->m_bUseStd ? std::cout : log->m_os) << strMsg << std::endl;
}

void Log::Critical(const string &strMsg)
{
	(log->m_bUseStd ? std::cout : log->m_os) << strMsg << std::endl;
}

void Log::SetLevel(int lvl)
{
	Log::log->m_nLogLvl = lvl;
}

Log::Log(const string &strFilepath)
	:m_strLogFileName(strFilepath), m_nLogLvl(Log::INFO)
{
	if(m_strLogFileName.empty()) //写到标准输出
	{
		m_bUseStd = true;
	}
	else				  //写到指定文件
	{
		m_bUseStd = false;
		m_os.open(strFilepath.c_str());
	}
}