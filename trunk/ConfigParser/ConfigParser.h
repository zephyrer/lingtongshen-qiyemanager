//ConfigParser.h
#include "../Global/Global.h"

#pragma once

class CConfigParser
{
public:
	CConfigParser();
public:
	virtual ~CConfigParser();
public:
	BOOL parseConfigFile();
	int getLogLevel();	
	std::wstring getSQLDBName();
	std::wstring getSQLUrl();
	std::wstring getSQLUserName();
	std::wstring getSQLPwd();
};