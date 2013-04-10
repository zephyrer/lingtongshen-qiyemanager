//Global.h
#include "../Logger/MyLogger.h"
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <boost/regex.hpp>

#include "GridCtrl.h"
#include "ObjectTemplate.h"
#include "Util.h"

#include "mysql_public_iface.h"
#include <mysql_connection.h>  
#include <mysql_driver.h>  

#include <list> 
#include <vector>
#include <map>
#include <string>
#include <process.h>
#include <windows.h>
#include <iostream>
#include <fstream> 



#pragma once
using namespace std;

//configuration
#define  CFGNAME L"config\\config.ini"
#define ITEM_COUNT      6
#define ORDER_ITEM		0
#define SAMPLE_ITEM		1
#define CUSTOMER_ITEM	2
#define ORDER_DETAIL    3
#define LOG_ITEM		4
#define SQL_ITEM		5

struct ConfigFileItem
{
	 map<wstring, wstring> mapOrder;
     map<wstring, wstring> mapSample;
     map<wstring, wstring> mapCustomer;
	 map<wstring, wstring> mapOrderDetail;
     map<wstring, wstring> mapLog;
	 map<wstring, wstring> mapSQL;
};
extern struct ConfigFileItem cfgFileItem;
struct DataBasePara
{
	wstring sqlDBName;
	wstring sqlURL;
	wstring sqlUserName;
	wstring sqlPWD;

};
extern struct DataBasePara g_dbp;

//grid
#define MAX_COLUMN 50
#define MAX_ROW    60
#define GRID_VIRTUAL_MODE 1
#define ORDER_CLICKABLE_COL  9
#define ORDER_NO_OFFSET  1
typedef BOOL (CALLBACK* GRIDVIEWCALLBACK)(GV_DISPINFO*,LPARAM);
typedef struct _ORDER_INFO{
	wstring orderNo;
	wstring orderCount;
	wstring orderSendCount;
	wstring orderLeftCount;
}ORDER_INFO;


//Database
#define DB_TABLE_NUM    4
#define SORT_NAME_KEY    L"col0"
#define DB_TABLE_SORT_NAME L"���"
#define MAX_RECORDS_NUM		60
#define DB_TABLE_ORDER_KEY      L"������"
#define DB_TABLE_SAMPLE_KEY     L"������"
#define DB_TABLE_CUSTOMER_KEY1   L"��˾����"
#define DB_TABLE_CUSTOMER_KEY2   L"��ϵ��"
#define DB_TABLE_ORDER_DETAIL_KEY L"��ϸ��"
#define DB_TABLE_ORDER_COUNT	L"��������"
#define DB_TABLE_ORDER_SENT_COUNT L"��������"
#define DB_TABLE_ORDER_LEFT_COUNT L"ʣ������"
#define FUZZY_MATCH   0
#define WORD_MATCH    1
extern int g_CurTableIndex;
extern const wstring g_dbTableName[DB_TABLE_NUM];
extern const wstring g_dbTableKeyList[DB_TABLE_NUM];





//UI
#define WM_USER_MSG WM_USER + 1001
#define WM_USER_MSG_REFRESH WM_USER+1002
#define WM_USER_MSG_LOADVIEW WM_USER+1003
#define SET_PB_POS(hWnd,pos)  ::PostMessage(hWnd, WM_USER_MSG,WPARAM(pos),LPARAM(0));
#define SET_PB_LOAD_POS(pView,pos)   pView->SendMessage(WM_USER_MSG_LOADVIEW,WPARAM(pos),LPARAM(0));

#define SELECT_ORDER      L"��������"
#define SELECT_SAMPLE     L"��������"
#define SELECT_CUSTOMER   L"�ͻ�����"
#define SELECT_ORDER_DETAIL L"������ϸ"
#define SW_TITLE_NAME     L"��ү�������"

#define ADD_NEW_ITEM_ORDER_DLG_TITLE	L"¼�붩����Ϣ"
#define ADD_NEW_ITEM_SAMPLE_DLG_TITLE   L"¼��������Ϣ"
#define ADD_NEW_ITEM_CUSTOMER_DLG_TITLE L"¼��ͻ���Ϣ"
#define ORDER_SEND_DETAIL_TITLE			L"����������ϸ"
#define SEARCH_TITLE                    L"����"
#define SPLASH_SCREEN_TIMEOUT			5000



//Thread
extern CCriticalSectionLock g_spLock;



class CGlobal
{
public:
	CGlobal();
	~CGlobal();

public:
	static map<wstring,wstring> GetDBTableMap(int tableindex);
	static BOOL GetKeywordOffset(int tableindex,LPCTSTR szKeyword, int* pOffset);
	static void SetControlTextFont(CWnd* pWnd,CFont* pFont,int height);


};