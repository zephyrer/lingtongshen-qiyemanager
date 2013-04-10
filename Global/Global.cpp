#include "stdafx.h"
#include "Global.h"



//Configuration
struct ConfigFileItem cfgFileItem;


//Database

const std::wstring g_dbTableName[DB_TABLE_NUM]={L"orderitem",L"sampleitem",L"customeritem",L"orderdetail"};
const std::wstring g_dbTableKeyList[DB_TABLE_NUM]={DB_TABLE_ORDER_KEY,DB_TABLE_SAMPLE_KEY,DB_TABLE_CUSTOMER_KEY2,DB_TABLE_ORDER_DETAIL_KEY};
struct DataBasePara g_dbp;
int g_CurTableIndex=0;

//Threads&UI
CCriticalSectionLock g_spLock;

CGlobal::CGlobal()
{
}

CGlobal::~CGlobal()
{

}
map<wstring,wstring> CGlobal::GetDBTableMap(int tableindex)
{
	switch (tableindex)
	{
		case ORDER_ITEM:return cfgFileItem.mapOrder;
		case SAMPLE_ITEM:return cfgFileItem.mapSample;
		case CUSTOMER_ITEM:return cfgFileItem.mapCustomer;
		case ORDER_DETAIL:return cfgFileItem.mapOrderDetail;
		default: return cfgFileItem.mapOrder;	
	}
}


BOOL CGlobal::GetKeywordOffset(int tableindex,LPCTSTR szKeyword, int* pOffset)
{
	map<wstring,wstring>tmpMap=GetDBTableMap(tableindex);
	map<wstring,wstring>::iterator iter;
	int index=0;
	for(iter=tmpMap.begin();iter!=tmpMap.end();iter++)
	{
		if(0==_wcsicmp(szKeyword,iter->second.c_str()))
		{
			*pOffset=index;
			return TRUE;	
		}
		index++;
	}
	return FALSE;
}


void CGlobal::SetControlTextFont(CWnd* pWnd,CFont* pFont,int height)
{
	LOGFONT stFont;
	//CFont font;
    memset(&stFont,0,sizeof(LOGFONT));
	stFont.lfHeight=height;
    stFont.lfWeight=FW_DONTCARE;
    stFont.lfCharSet=GB2312_CHARSET;
	pFont->CreateFontIndirect(&stFont);
	pWnd->SetFont(pFont);
}











