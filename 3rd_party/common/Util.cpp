#include "stdafx.h"
#include "Util.h"
#include <AtlConv.h>

std::wstring CUtil::s2ws(const std::string& s)
{
	USES_CONVERSION;
	std::wstring wsTemp(A2W_CP(s.c_str(),CP_ACP));
	return wsTemp;
			
	
}

std::string  CUtil::ws2s(const std::wstring& ws)
{
	USES_CONVERSION;

	std::string sTemp(W2A_CP(ws.c_str(),CP_ACP));
	return sTemp;
	
}