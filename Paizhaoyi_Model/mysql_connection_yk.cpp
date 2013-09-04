#include "mysql_connection_yk.h"



Connection::Connection(const string &_UserName, const string &_Password, const string &_dbname, const string &_HostName /*= "localhost"*/)
	:m_strHostName(_HostName), m_strUserName(_UserName), m_strPassword(_Password), m_strDBName(_dbname), m_bValidConn(false)
{
	mysql_init(&m_mysql);
	if(mysql_real_connect(&m_mysql, m_strHostName.c_str(), m_strUserName.c_str(),
		m_strPassword.c_str(), m_strDBName.c_str(), 3306, NULL, CLIENT_MULTI_STATEMENTS))
	{
		m_bValidConn = true;
	}
}

RecordSet Connection::Execute(const string &_sqlstr)
{
	if(!m_bValidConn)
	{
		throw string(mysql_error(&m_mysql));
	}
	mysql_query(&m_mysql, _sqlstr.c_str());
	m_sqlResultSet = mysql_store_result(&m_mysql);

	if(mysql_errno(&m_mysql))
	{
		throw string(mysql_error(&m_mysql));
	}
	return m_sqlResultSet ? RecordSet(m_sqlResultSet) : RecordSet(NULL);
}

Connection::~Connection()
{
	mysql_close(&m_mysql);
}


