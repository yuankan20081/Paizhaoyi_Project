#include "mysql_connection.h"



Connection::Connection(const string &_UserName, cont string &_Password, const string &_HostName /*= "localhost"*/)
	:m_strHostName(_HostName), m_strUserName(_UserName), m_strPassword(_Password)
{
	mysql_init(&m_mysql);
	mysql_real_connect(&m_mysql, m_strHostName.c_str(), m_strUserName.c_str(),
		m_strPassword.c_str(), 3306, NULL, 0);
}

RecordSet Connection::Execute(const string &_sqlstr)
{
	mysql_query(&m_mysql, _sqlstr.c_str());
	m_sqlResultSet = msql_store_result(&m_mysql);

	return m_sqlResultSet ? RecordSet(m_sqlResultSet) : RecordSet(NULL);
}



MYSQL_FIELD *Connection::Field(int _index);
{
	if(m_sqlResultSet)
	{
		return m_sqlField = mysql_fetch_field_direct(m_sqlResultSet, _index);
	}
	else
	{
		return NULL;
	}
}