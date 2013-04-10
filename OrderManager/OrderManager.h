// OrderManager.h : OrderManager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// COrderManagerApp:
// 有关此类的实现，请参阅 OrderManager.cpp
//



class COrderManagerApp : public CWinApp
{
public:
	COrderManagerApp();


// 重写
public:
	virtual BOOL InitInstance();
	void OnNewItemInsert();
	void OnInfoSearch();
	BOOL GetProgState();

// 实现
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_ProgStarted;
	
};

extern COrderManagerApp theApp;