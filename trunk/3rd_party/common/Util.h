#ifndef _UTIL_H  
#define _UTIL_H
#include <string>

class CUtil
{
public:
	static std::wstring s2ws(const std::string& s);
	static std::string  ws2s(const std::wstring& ws);

};



#endif  