#include "stdafx.h"
#include "SQLDriver.h"
#include "../ConfigParser/ConfigParser.h"
#include "../OrderManager/GridView.h"
#include "../ConfigParser/IniReader.h"
#include "../Global/Global.h"

using namespace log4cplus;
using namespace sql;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CSQLDriver::CSQLDriver()
{
	m_pSQLPool = NULL;
	m_pState=NULL;
	m_pResult=NULL;
	m_pCon_meta =NULL;
	m_pConn=NULL;
	m_TableKeyName[0]=DB_TABLE_ORDER_KEY;
	m_TableKeyName[1]=DB_TABLE_SAMPLE_KEY;
	m_TableKeyName[2]=DB_TABLE_CUSTOMER_KEY1;
	m_TableKeyName[2]=m_TableKeyName[2]+L","+DB_TABLE_CUSTOMER_KEY2;
	m_TableKeyName[3]=DB_TABLE_ORDER_DETAIL_KEY;

}

CSQLDriver::~CSQLDriver()
{

}


void CSQLDriver::CreateDataBase()
{
	S_TRACE_FUNCTION;
	wchar_t command[255];
	m_pState->execute("set names gbk");	
	swprintf(command,255,L"create database %s",g_dbp.sqlDBName.c_str());
	m_pState->execute(CUtil::ws2s(command));
}
void CSQLDriver::CreateTables()
{
	S_TRACE_FUNCTION;
	wstring szStr;
	map<wstring,wstring>::iterator iter;
	map<wstring,wstring> mapTemp;
	for(int index=0;index<DB_TABLE_NUM;index++)
	{
		mapTemp=CGlobal::GetDBTableMap(index);
		szStr=L"create table ";
		szStr =szStr+g_dbTableName[index]+L"(";
		//skip head
		iter=mapTemp.begin();
		iter++;
		while(iter!=mapTemp.end())
		{
			szStr=szStr+iter->second +L" varchar(255), ";
			iter++;		
		}
		szStr =szStr+L"primary key("+m_TableKeyName[index]+L"))";
		m_pState->execute(CUtil::ws2s(szStr.c_str()));	
	}
}

void CSQLDriver::UseDataBase()
{
	S_TRACE_FUNCTION;
	wchar_t command[255];
	m_pState->execute("set names gbk");	
	swprintf(command,255,L"use %s",g_dbp.sqlDBName.c_str());
	m_pState->execute(CUtil::ws2s(command));
}

void CSQLDriver::QueryTable(int records,int table_index)
{
	S_TRACE_FUNCTION;
	wstring szQuery=L"select * from ";
	szQuery =szQuery+g_dbTableName[table_index]+L" order by "+g_dbTableKeyList[table_index]+L" desc limit 0,";
	wchar_t sztmpRecordNum[255];
	swprintf(sztmpRecordNum,255,L"%d",records);
	szQuery +=sztmpRecordNum;
	m_pResult = m_pState->executeQuery(CUtil::ws2s(szQuery.c_str())); 
}

void CSQLDriver::QueryTableWithCondition(int table_index,LPCTSTR szCondition)
{
	S_TRACE_FUNCTION;
	wstring szQuery=L"select * from ";
	szQuery=szQuery+g_dbTableName[table_index]+L" where "+szCondition+L" order by "+g_dbTableKeyList[table_index]+L" desc";
	m_pResult = m_pState->executeQuery(CUtil::ws2s(szQuery.c_str())); 

}

void CSQLDriver::QueryTableSegmentWithCondition(int table_index,LPCTSTR szQueryCondition,LPCTSTR szLimitCondition)
{
	S_TRACE_FUNCTION;
	wstring szQuery=L"select * from ";
	szQuery=szQuery+g_dbTableName[table_index]+L" where "+szQueryCondition+L" order by "+g_dbTableKeyList[table_index]+L" desc limit "+szLimitCondition;
	m_pResult = m_pState->executeQuery(CUtil::ws2s(szQuery.c_str())); 

}


int CSQLDriver::QueryRecordsCountWithCondition(int table_index,LPCTSTR szCondition)
{
	S_TRACE_FUNCTION;
	int recordsNum=0;
	wstring szQuery=L"select count(*) from ";
	szQuery=szQuery+g_dbTableName[table_index]+L" where "+szCondition;
	m_pResult = m_pState->executeQuery(CUtil::ws2s(szQuery.c_str())); 
	while(m_pResult->next())
	{
		 recordsNum=m_pResult->getInt(1);
		 break;		
	}
	return recordsNum;

} 

void CSQLDriver::HandleResults(int colnum,vector<vector<wstring> > *pData)
{
	S_TRACE_FUNCTION;
	vector<wstring> vTemp;
	TCHAR szListNo[255];
	wstring szListSeq;

	int rows=(int)m_pResult->rowsCount();
	while(m_pResult->next())
	{
		//set row index to head
		_itow_s(rows,szListNo,10);
		szListSeq=szListNo;
		vTemp.push_back(szListSeq);

		for(int index=1; index<colnum;index++)
		{
			vTemp.push_back(CUtil::s2ws(m_pResult->getString(index).c_str()));			
		}
		pData->push_back(vTemp);
		vTemp.clear();
		rows--;
	}

}

BOOL CSQLDriver::ReadRecordsCountWithCondition(int table_index,LPCTSTR szCondition,int* pCount)
{
	S_TRACE_FUNCTION;
	if(!InitConnection()) return FALSE;	
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();
		*pCount=QueryRecordsCountWithCondition(table_index,szCondition);
	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;


}

BOOL CSQLDriver::ReadTableWithCondition(int table_index,LPCTSTR szCondition,int colnum,vector<vector<wstring> > *pData)
{
	S_TRACE_FUNCTION;
	if(!InitConnection()) return FALSE;	
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();
		QueryTableWithCondition(table_index,szCondition);
		HandleResults(colnum,pData);
	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;
}

BOOL CSQLDriver::ReadTableSegmentWithCondition(int table_index,LPCTSTR szQueryCondition,LPCTSTR szLimitCondition,int colnum,vector<vector<wstring> > *pData)
{
	S_TRACE_FUNCTION;
	if(!InitConnection()) return FALSE;	
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();
		QueryTableSegmentWithCondition(table_index,szQueryCondition,szLimitCondition);
		HandleResults(colnum,pData);
	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;
}


BOOL CSQLDriver::ReadFromDataBase(int records,int table_index,int colnum,vector<vector<wstring> > *pData)
{
	S_TRACE_FUNCTION;
	m_pState->execute("set names gbk");
	UseDataBase();
	QueryTable(records,table_index);
	HandleResults(colnum,pData);
	return TRUE;
}


void CSQLDriver::CreateSQLDatabase()
{
	S_TRACE_FUNCTION;
	CreateDataBase();
    UseDataBase();
    CreateTables();
}

BOOL  CSQLDriver::InitConnection()
{
	m_pSQLPool=CSQLPool::GetInstance();
	m_pConn=m_pSQLPool->GetConnection();
	if(!m_pConn) return FALSE;	
	return TRUE;

}
void CSQLDriver::ReleaseSQLConnection()
{
	if(m_pConn) {m_pSQLPool->ReleaseConnection(m_pConn);m_pConn=NULL;}
	if(m_pState) {delete m_pState;m_pState=NULL;}
	if(m_pResult) {delete m_pResult;m_pResult=NULL;}
}

BOOL CSQLDriver::UpdateOneRecordInDataBase(int tableindex, LPCTSTR szContent, LPCTSTR szCondition)
{
	S_TRACE_FUNCTION;
	wstring szStr;
	if(!InitConnection()) return FALSE;
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();
		szStr=L"update "+g_dbTableName[tableindex]+L" set "+szContent+L" where "+szCondition;
		m_pState->execute(CUtil::ws2s(szStr.c_str()));		

	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;
}

BOOL CSQLDriver::DeleteOneRecordWithCondition(int tableindex,LPCTSTR szCondition)
{
	S_TRACE_FUNCTION;
	wstring szStr;
	if(!InitConnection()) return FALSE;
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();
		szStr=L"delete from "+g_dbTableName[tableindex]+ L" where "+szCondition;
		m_pState->execute(CUtil::ws2s(szStr.c_str()));		

	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;
}


void CSQLDriver::InsertRecord(int tableindex,vector<wstring> *pData)
{
	wstring szStr;
	int recordsNum=0;
	vector<wstring>::iterator iter;

#if 0
	szStr=L"select count(*) from "+g_dbTableName[tableindex];
	m_pResult=m_pState->executeQuery(CUtil::ws2s(szStr.c_str()));

	//update first column
	while(m_pResult->next())
	{
		 recordsNum=m_pResult->getInt(1);
		 break;		
	}
	int newrecordindex=1+recordsNum;
	wchar_t szNewRecordInx[255];
    swprintf(szNewRecordInx,255,L"%d",newrecordindex);
	iter=pData->begin();
	*iter=szNewRecordInx;
#endif
	//skip head
	iter=pData->begin();
	iter++;
	//get sql string
	szStr=L"insert into "+g_dbTableName[tableindex]+L" values(";	
	while(iter!=pData->end())
	{
		szStr =szStr+L"\""+ *iter +L"\"";
		iter++;
		if(iter!=pData->end())
			szStr +=L", ";
	}
	szStr +=L") on duplicate key update ";


	//handle duplicate record
	int posVec=0;
	if(!CGlobal::GetKeywordOffset(tableindex,g_dbTableKeyList[tableindex].c_str(),&posVec)) return;
	map<wstring,wstring>tmpMapTitle;
	vector<wstring> tempV;
	tmpMapTitle=CGlobal::GetDBTableMap(tableindex);
	map<wstring,wstring>::iterator miter;
	for(miter=tmpMapTitle.begin();miter!=tmpMapTitle.end();miter++)
	{
		tempV.push_back(miter->second.c_str());	
	};
	for(int index=posVec+1;index<(int)tempV.size();index++)
	{
		szStr=szStr+tempV[index]+L"=\""+pData->at(index)+L"\"";
		if(index==((int)tempV.size()-1))
			;
		else
			szStr+=L", ";
	}
	
	m_pState->execute(CUtil::ws2s(szStr.c_str()));

}

BOOL CSQLDriver::WriteOneRecordToDataBase(int tableindex,vector<wstring> *pData)
{
	S_TRACE_FUNCTION;

	if(!InitConnection()) return FALSE;	
	try{
		m_pState=m_pConn->createStatement();
		//use database
		UseDataBase();		
		//insert record
		InsertRecord(tableindex,pData);	
	
	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}

	ReleaseSQLConnection();
	return TRUE;

}

BOOL CSQLDriver::OpenSQL(int tableindex,vector<vector<wstring> > *pData)
{
	S_TRACE_FUNCTION;
	map<wstring,wstring>tmpMap=CGlobal::GetDBTableMap(tableindex);
	BOOL is_database_exist=FALSE;
	if(!InitConnection()) return FALSE;	
	try{
		//check if database exists?
		m_pState=m_pConn->createStatement();
		m_pCon_meta = m_pConn->getMetaData();
		m_pResult=m_pCon_meta->getSchemas();
		m_pResult->afterLast();
		while (m_pResult->previous()) {
			string stmp=m_pResult->getString("TABLE_SCHEM");
			wstring wstmp=CUtil::s2ws(stmp);
			if(0==_wcsicmp(g_dbp.sqlDBName.c_str(),wstmp.c_str()))
			{
				is_database_exist=TRUE;
				break;
			}
		}
		if(is_database_exist)
		{
			ReadFromDataBase(MAX_RECORDS_NUM,tableindex,(int)tmpMap.size(),pData);		
		}else{

			CreateSQLDatabase();				
		}
	
	}catch(sql::SQLException &e){
		S_LOG_FATAL(L"SQL exception:"<<e.what()<<L"ErrorCode:"<<e.getErrorCode());
		ReleaseSQLConnection();
		return FALSE;
	}catch(std::runtime_error &e){
		S_LOG_FATAL(L"Runtime exception:"<<e.what());
		ReleaseSQLConnection();
		return FALSE;
	}
	ReleaseSQLConnection();
	return TRUE;



}

