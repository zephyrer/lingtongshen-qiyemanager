#include "stdafx.h"
#include "IniReader.h"
#include "../SQLDriver/SQLDriver.h"



CIniReader::CIniReader(LPCTSTR szFileName)
{
	 memset(m_szFileName, 0x00, sizeof(m_szFileName));
	 memcpy(m_szFileName, szFileName, _tcslen(szFileName)*sizeof(TCHAR));
	 m_szconfigItem[0]=g_dbTableName[0];
	 m_szconfigItem[1]=g_dbTableName[1];
	 m_szconfigItem[2]=g_dbTableName[2];
	 m_szconfigItem[3]=g_dbTableName[3];
	 m_szconfigItem[4]=L"log";
	 m_szconfigItem[5]=L"SQL";
	
}

int CIniReader::ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue)
{
	 int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName);
	 return iResult;
}

float CIniReader::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue)
{
	 TCHAR szResult[255];
	 TCHAR szDefault[255];
	 float fltResult;
	 _stprintf_s(szDefault, 255, TEXT("%f"),fltDefaultValue);
	 GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName);
	 fltResult =  (float)_tstof(szResult);
	 return fltResult;
}

bool CIniReader::ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue)
{
	 TCHAR szResult[255];
	 TCHAR szDefault[255];
	 bool bolResult;
	 _stprintf_s(szDefault, 255, TEXT("%s"), bolDefaultValue? TEXT("True") : TEXT("False"));
	 GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	 bolResult =  (_tcscmp(szResult, TEXT("True")) == 0 ||
			_tcscmp(szResult, TEXT("true")) == 0) ? true : false;
	 return bolResult;
}

LPTSTR CIniReader::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue)
{
	 LPTSTR szResult = new TCHAR[255];
	 memset(szResult, 0x00, sizeof(szResult));
	 if(0==GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName))
	 {
		 S_LOG_ERROR(L"GetPrivateProfileString error, error code:"<< GetLastError());	
	 }
	 return szResult;
}




void CIniReader::GetConfigKeyValue(int configType,TCHAR* szKey,TCHAR* szValue )
{
	switch(configType)
	{
	case ORDER_ITEM:
		cfgFileItem.mapOrder.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	case SAMPLE_ITEM:
		cfgFileItem.mapSample.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	case CUSTOMER_ITEM:
		cfgFileItem.mapCustomer.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	case LOG_ITEM:
		cfgFileItem.mapLog.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	case SQL_ITEM:
		cfgFileItem.mapSQL.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	case ORDER_DETAIL:
		cfgFileItem.mapOrderDetail.insert(pair<wstring,wstring>(szKey,szValue));
		break;
	default:
		break;	
	}
}


 void CIniReader::ReadAllKeysInSection(int configType)
 {
	 TCHAR szResult[1024];
	 TCHAR szKey[25];
	 TCHAR szValue[100];
	 int key_start_index=0;
	 int value_start_index=0;
	 int equal_start_index=0;
	 LPCTSTR szSection=m_szconfigItem[configType].c_str();
	 memset(szResult,'\0',sizeof(szResult));
	 memset(szKey,'\0',sizeof(szKey));
	 memset(szValue,'\0',sizeof(szValue));

	 DWORD dw=GetPrivateProfileSection(szSection,szResult,1024,m_szFileName);
	 for(DWORD index=0,pos=0;index<dw;index++,pos++)
	 {
		 if(szResult[index]=='=')
		 {
			 key_start_index=index-pos;
			 wcsncpy_s(szKey,&szResult[key_start_index],pos);
			 equal_start_index=index;
		 }			 	 
		 if(szResult[index]=='\0')
		 {
			 value_start_index=equal_start_index+1;
			 wcsncpy_s(szValue,&szResult[value_start_index],index-value_start_index);
			 GetConfigKeyValue(configType,szKey,szValue);
			 pos=-1;	 
		 }		 
	 } 
	 if(0==dw)
	 {
		S_LOG_ERROR(L"ReadAllKeysInSection error, error code:"<< GetLastError());	
	 }

 }


 void CIniReader::ParseInitFile()
 {
	 InsertHead();
	 for(int configindex=0;configindex<ITEM_COUNT;configindex++)
	 {
		 ReadAllKeysInSection(configindex);  
	 }
 }

 void CIniReader::InsertHead()
 {
 	cfgFileItem.mapOrder.insert(pair<wstring,wstring>(SORT_NAME_KEY,DB_TABLE_SORT_NAME));
	cfgFileItem.mapSample.insert(pair<wstring,wstring>(SORT_NAME_KEY,DB_TABLE_SORT_NAME));
	cfgFileItem.mapCustomer.insert(pair<wstring,wstring>(SORT_NAME_KEY,DB_TABLE_SORT_NAME));
	cfgFileItem.mapOrderDetail.insert(pair<wstring,wstring>(SORT_NAME_KEY,DB_TABLE_SORT_NAME));
 
 }

