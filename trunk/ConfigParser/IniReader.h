#ifndef INIREADER_H
#define INIREADER_H
#include "../Global/Global.h"

using namespace std;

class CIniReader
{
public:
	 CIniReader(LPCTSTR szFileName); 
	 int ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue);
	 float ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue);
	 bool ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue);
	 LPTSTR ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue);
	 void ReadAllKeysInSection(int configType);
	 void GetConfigKeyValue(int configType,TCHAR*, TCHAR* );
	 void ParseInitFile();
	 void InsertHead();
private:
	  TCHAR m_szFileName[255];
	  wstring m_szconfigItem[ITEM_COUNT];

};



#endif//INIREADER_H