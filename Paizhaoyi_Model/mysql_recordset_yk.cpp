#include "mysql_recordset_yk.h"

RecordSet::RecordSet(MYSQL_RES *_RecordSet)
	:m_sqlRecordSet(_RecordSet)
{
}

RecordSet::~RecordSet()
{
}



bool RecordSet::IsValid()
{
	return m_sqlRecordSet ? true : false;
}

size_t RecordSet::FieldCount()
{
	if(m_sqlRecordSet)
	{
		return m_nFieldCount = mysql_num_fields(m_sqlRecordSet);
	}
	else
	{
		throw string("Invalid RecordSet!!");
	}
}

string RecordSet::FieldName(unsigned int _index) 
{
	if(_index < 0 || _index > (m_nFieldCount - 1))
	{
		throw string("Index Out Of Range!!");
	}
	if(!m_sqlRecordSet)
	{
		throw string("Invalid RecordSet!!");
	}

	string _FieldName(mysql_fetch_field_direct(m_sqlRecordSet, _index)->name);
	return _FieldName;
}

size_t RecordSet::RowCount()
{
	if(m_sqlRecordSet)
	{
		return m_nRowCount = mysql_num_rows(m_sqlRecordSet);
	}
	else
	{
		throw string("Invalid RecordSet!!");
	}
}

std::list<string> RecordSet::FetchOneRow()
{
	if(!m_sqlRecordSet)
	{
		throw string("Invalid RecordSet!!");
	}
	m_sqlRow = mysql_fetch_row(m_sqlRecordSet);

	this->FieldCount();
	std::list<string> rowitems;
	if(!RowCount() || !m_sqlRow)
	{
		return rowitems;
	}
	//m_mapBoom.clear();//
	for(unsigned int i = 0; i < m_nFieldCount; i++)
	{
		rowitems.push_back(m_sqlRow[i]);
		//m_mapBoom.insert(pair<string, string>(this->FieldName(i), m_sqlRow[i]));
		m_mapBoom[this->FieldName(i)] = m_sqlRow[i];
	}
	return rowitems;
}

string RecordSet::operator[](const string &_fieldname)
{
	map<string, string>::iterator it = m_mapBoom.find(_fieldname);
	if(it != m_mapBoom.end())
	{
		return it->second;
	}
	else
	{
		throw string(_fieldname + " Not Found!!");
	}
}
