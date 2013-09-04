#pragma once
#include <string>
#include <list>

class RecordSet
{
	using std::string;
	
public:
	RecordSet(const MYSQL_RES *_RecordSet);
	~RecordSet();

	bool IsValid();
public:
	size_t FiledCount();
	string FieldName(int _index);

	size_t RowCount();
	std::list<string> FetchOneRow();
private:
	MYSQL_RES *m_sqlRecordSet;
	MYSQL_ROW m_sqlRow;
	MYSQL_FIELD *m_sqlField;

	size_t m_nFieldCount;
	siez_t m_nRowCount;


};