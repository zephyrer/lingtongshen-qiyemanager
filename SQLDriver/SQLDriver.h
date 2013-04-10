//SQLDriver
#include "../Global/Global.h"
#include "../SQLDriver/SQLPool.h"
#pragma once




class CSQLDriver
{
public:
	CSQLDriver();
public:
	virtual ~CSQLDriver();
public:
	BOOL OpenSQL(int tableindex, vector<vector<wstring> > *pData);
	BOOL InitConnection();
	void ReleaseSQLConnection();
	void CreateDataBase();
	void UseDataBase();
	void CreateTables();
	void QueryTable(int records,int table_index);
	void QueryTableWithCondition(int table_index,LPCTSTR szCondition);
	void QueryTableSegmentWithCondition(int table_index,LPCTSTR szQueryCondition,LPCTSTR szLimitCondition);
	int  QueryRecordsCountWithCondition(int table_index,LPCTSTR szCondition);
	void HandleResults(int colnum,vector<vector<wstring> > *pData);
	void CreateSQLDatabase();
	void InsertRecord(int tableindex,vector<wstring> *pData);
	BOOL DeleteOneRecordWithCondition(int tableindex,LPCTSTR szCondition);
	BOOL ReadFromDataBase(int records,int tableindex,int colnum,vector<vector<wstring> > *pData);	
	BOOL UpdateOneRecordInDataBase(int tableindex, LPCTSTR szContent, LPCTSTR szCondition);
	BOOL WriteOneRecordToDataBase(int tableindex,vector<wstring> *pData);
	BOOL ReadTableWithCondition(int tableindex,LPCTSTR szCondition,int colnum,vector<vector<wstring> > *pData);
	BOOL ReadRecordsCountWithCondition(int tableindex,LPCTSTR szCondition,int* pCount);
	BOOL ReadTableSegmentWithCondition(int tableindex,LPCTSTR szQueryCondition,LPCTSTR szLimitCondition,int colnum,vector<vector<wstring> > *pData);
	

private:
	CSQLPool* m_pSQLPool;
	Connection* m_pConn;
	Statement* m_pState;
	ResultSet* m_pResult;
	DatabaseMetaData* m_pCon_meta;
	std::wstring m_TableKeyName[DB_TABLE_NUM];

};

