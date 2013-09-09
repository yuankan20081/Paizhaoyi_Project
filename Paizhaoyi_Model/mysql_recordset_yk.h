#pragma once
//#include <WinSock2.h>
#include <string>
#include <list>
#include <mysql.h>
#include <map>
using namespace std;


class RecordSet
{	
public:
	RecordSet(MYSQL_RES *_RecordSet);
	~RecordSet();

	bool IsValid();
public:
	size_t FieldCount();
	string FieldName(unsigned int _index);

	size_t RowCount();
	list<string> FetchOneRow();
	string operator[](const string &_fieldname); // get rowitem by fieldname
private:
	MYSQL_RES *m_sqlRecordSet;
	MYSQL_ROW m_sqlRow;
	MYSQL_FIELD *m_sqlField;

	size_t m_nFieldCount;
	size_t m_nRowCount;

	map<string, string> m_mapBoom;//


};