// OrderManager.h : OrderManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COrderManagerApp:
// �йش����ʵ�֣������ OrderManager.cpp
//



class COrderManagerApp : public CWinApp
{
public:
	COrderManagerApp();


// ��д
public:
	virtual BOOL InitInstance();
	void OnNewItemInsert();
	void OnInfoSearch();
	BOOL GetProgState();

// ʵ��
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_ProgStarted;
	
};

extern COrderManagerApp theApp;