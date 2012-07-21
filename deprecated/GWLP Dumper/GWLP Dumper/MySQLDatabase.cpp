#include "MySQLDatabase.h"

MySQLQueryResult::MySQLQueryResult( MYSQL_RES* res,UINT32 FieldCount,UINT32 RowCount )
:m_Result(res),m_FieldCount(FieldCount),m_RowCount(RowCount)
{
	m_CurrentRow = new Field[FieldCount];
}

MySQLQueryResult::~MySQLQueryResult()
{
	mysql_free_result(m_Result);
	delete[] m_CurrentRow;
}

bool MySQLQueryResult::NextRow()
{
	MYSQL_ROW row = mysql_fetch_row(m_Result);
	if(row == NULL)
		return false;

	for(UINT32 i = 0; i < m_FieldCount; i++)
		m_CurrentRow[i].SetValue(row[i]);

	return true;
}

MySQLDatabase::MySQLDatabase( const char* host,const char* db,const char* user,const char* pw,int port )
{
	MYSQL* mysql;
	mysql = mysql_init(NULL);
	bool my_true = true;
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(mysql, MYSQL_OPT_RECONNECT, &my_true);
	m_Connection = mysql_real_connect(mysql,host,user,pw,db,port,0,0);
	if(m_Connection == NULL)
		throw mysql_error(mysql);
}


MySQLDatabase::~MySQLDatabase()
{
	mysql_close(m_Connection);
}

MySQLQueryResult* MySQLDatabase::Query( const char* query,... )
{
	static char sql[250000];
	va_list vlist;
	va_start(vlist,query);
	vsnprintf(sql,250000,query,vlist);
	va_end(vlist);

	int query_state = mysql_query(m_Connection,sql);
	if(query_state != 0)
		return NULL;

	MySQLQueryResult* res = NULL;
	MYSQL_RES* pRes = mysql_store_result(m_Connection);
	UINT32 uRows = (UINT32)mysql_affected_rows(m_Connection);
	UINT32 uFields = (UINT32)mysql_field_count(m_Connection);

	if(uRows == 0 || uFields == 0 || pRes == 0)
	{
		if(pRes != NULL)
			mysql_free_result(pRes);
		return NULL;
	}
	res = new MySQLQueryResult(pRes,uFields,uRows);
	res->NextRow();

	return res;
}

bool MySQLDatabase::Execute( const char* query,... )
{
	static char sql[250000];
	va_list vlist;
	va_start(vlist,query);
	vsnprintf(sql,250000,query,vlist);
	va_end(vlist);

	int query_state = mysql_query(m_Connection,sql);

	return ( *LastError() == '\0' );
}

const char* MySQLDatabase::LastError()
{
	return mysql_error(m_Connection);
}

std::string MySQLDatabase::EscapeString( char* str,int len )
{
	static char escaped[250000];
	mysql_real_escape_string(m_Connection,escaped,str,len);
	return std::string(escaped);
}