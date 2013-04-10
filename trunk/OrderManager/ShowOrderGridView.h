#pragma once

#include "GridView.h"

class CShowOrderGridView:public CGridView
{
public:
	CShowOrderGridView(CWnd*   pWnd,CRect   Rect);
	~CShowOrderGridView();

//call back
public:
	static BOOL CALLBACK GridClickCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
	static BOOL CALLBACK OrderGridViewCallbackFun(GV_DISPINFO *pDispInfo, LPARAM lParam);

//overwrite
public:
	void SetGVClickCallbackFunc();

private:
	int m_OrderNoOff;
	int m_OrderCountOff;
	int m_OrderSentCountOff;
	int m_OrderLeftCountOff;

};