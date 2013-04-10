#include "stdafx.h"
#include "ConfigParser.h"
#include "IniReader.h"



using namespace std;


CConfigParser::CConfigParser()
{
}

CConfigParser::~CConfigParser()
{

}

BOOL CConfigParser::parseConfigFile()
{
	S_TRACE_FUNCTION;
	TCHAR szCFGPath[MAX_PATH];
	ZeroMemory(szCFGPath, MAX_PATH);
	if(0<::GetModuleFileName(NULL,szCFGPath, _countof(szCFGPath)))
	{
		std::wstring wsFilePathName=szCFGPath;
		size_t pos=wsFilePathName.rfind(L"\\");
		if(0<pos)
		{
			wsFilePathName=wsFilePathName.substr(0,pos+1);
			wsFilePathName=wsFilePathName+CFGNAME;
			CIniReader iReader(wsFilePathName.c_str());
			//iReader.ReadAllKeysInSection(ORDER_ITEM);
			iReader.ParseInitFile();
			return TRUE;	
		}
	}
	S_LOG_ERROR(L"parse error, error code:"<< GetLastError());	 
	return FALSE;
}


int CConfigParser::getLogLevel()
{
	S_TRACE_FUNCTION;
	wstring loglevel=L"0";
	map<wstring,wstring>::iterator iter=cfgFileItem.mapLog.find(_T("loglevel"));
	if(iter!=cfgFileItem.mapLog.end())
	{
		loglevel=iter->second;	
	}
	return _wtoi(loglevel.c_str());
}


std::wstring CConfigParser::getSQLDBName()
{
	map<wstring, wstring>::iterator iter=cfgFileItem.mapSQL.find(L"dbname");
	if(iter==cfgFileItem.mapSQL.end()) return NULL;
	return cfgFileItem.mapSQL.find(L"dbname")->second;
}

std::wstring CConfigParser::getSQLUrl()
{
	map<wstring, wstring>::iterator iter_ip=cfgFileItem.mapSQL.find(L"ip");
	if(iter_ip==cfgFileItem.mapSQL.end()) return NULL;
	map<wstring, wstring>::iterator iter_port=cfgFileItem.mapSQL.find(L"port");
	if(iter_port==cfgFileItem.mapSQL.end()) return NULL;
	wstring wsTemp(L"tcp://"+iter_ip->second+L":"+iter_port->second);			
	return wsTemp;
}



std::wstring CConfigParser::getSQLUserName()
{
	map<wstring, wstring>::iterator iter=cfgFileItem.mapSQL.find(L"user");
	if(iter==cfgFileItem.mapSQL.end()) return NULL;
	return cfgFileItem.mapSQL.find(L"user")->second;

}
std::wstring CConfigParser::getSQLPwd()
{
	map<wstring, wstring>::iterator iter=cfgFileItem.mapSQL.find(L"password");
	if(iter==cfgFileItem.mapSQL.end()) return NULL;
	return cfgFileItem.mapSQL.find(L"password")->second;
}