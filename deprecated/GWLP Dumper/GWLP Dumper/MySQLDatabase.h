#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <winsock.h>
#include <mysql/mysql.h>
#include <string>

#pragma comment(lib,"libmysql.lib")

class Field
{
public:
	inline void SetValue(char* value) { m_Value = value; }

	inline const char* GetString() {return m_Value;}
	inline float GetFloat() {return m_Value ? static_cast<float>(atof(m_Value)) : 0; }
	inline bool GetBool() {return m_Value ? atoi(m_Value) > 0 : false; }
	inline UINT8 GetUInt8() { return m_Value ? static_cast<UINT8>(atol(m_Value)) : 0; }
	inline INT8 GetInt8() { return m_Value ? static_cast<INT8>(atol(m_Value)) : 0; }
	inline UINT16 GetUInt16() { return m_Value ? static_cast<UINT16>(atol(m_Value)) : 0; }
	inline INT16 GetInt16() { return m_Value ? static_cast<INT16>(atol(m_Value)) : 0; }
	inline UINT32 GetUInt32() { return m_Value ? static_cast<UINT32>(atol(m_Value)) : 0; }
	inline INT32 GetInt32() { return m_Value ? static_cast<INT32>(atol(m_Value)) : 0; }

private:
	char* m_Value;
};

class MySQLQueryResult
{
public:
	MySQLQueryResult(MYSQL_RES* res,UINT32 FieldCount,UINT32 RowCount);
	~MySQLQueryResult();

	inline Field* Fetch(){ return m_CurrentRow; }
	inline UINT32 GetFieldCount(){ return m_FieldCount; }
	inline UINT32 GetRowCount() { return m_RowCount; }

	bool NextRow();
private:
	MYSQL_RES* m_Result;
	UINT32 m_FieldCount;
	UINT32 m_RowCount;
	Field* m_CurrentRow;
};

class MySQLDatabase
{
public:
	MySQLDatabase(const char* host,const char* db,const char* user,const char* pw,int port);
	~MySQLDatabase();

	MySQLQueryResult* Query(const char* query,...);
	bool Execute(const char* query,...);
	int GetInsertId(){return mysql_insert_id(m_Connection);}
	std::string EscapeString(char* str,int len);

	const char* LastError();
private:
	MYSQL* m_Connection;
};
