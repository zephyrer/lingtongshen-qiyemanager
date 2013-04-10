#pragma once
#include "../SQLDriver/SQLDriver.h"
#include "../Global/Global.h"
using namespace std;

class CGridView:public CView
{
public:
	CGridView(
		CWnd*   pWnd,
		CRect   Rect,
		int     MaxRow,
		int		MaxCol,
		int		FixedRow,
		int		FixedCol,
		int     TableIndex);

	virtual ~CGridView();

//callback
public:
	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);

//methods for overwrite/load
public:
	virtual BOOL InitGrid();
	virtual BOOL SetTitle();
	virtual BOOL SetContent();
	virtual BOOL InsertGridWithOneRow(LPCTSTR str);
	virtual BOOL DeleteGridWithOneRow();
	virtual BOOL InsertGridWithOneColumn(LPCTSTR str);
	virtual void UpdateGridData();	
	virtual void SetCellText(int row, int col, LPCTSTR str);
	virtual void OnDraw(CDC* pDC); 
	//print
	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnGridViewSize(CRect rect);
	virtual BOOL OnGridCMDMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnGridToggleReadonly();
	virtual BOOL OnGridLoadComplete();
	virtual void SetGridViewCallback(GRIDVIEWCALLBACK pCallback){pfnGridViewCallback=pCallback;}
	virtual void SetGridViewData(vector<wstring> vstr);
	virtual void SetGVClickCallbackFunc();
	virtual int  GetTableIndex();
	virtual BOOL GetRowOrderInfo(int row,ORDER_INFO* pInfo);


public:
	GRIDVIEWCALLBACK pfnGridViewCallback;
	vector<vector<wstring> >m_GridViewData;
	int m_ListNo;

protected:	
	CGridCtrl* m_pGridCtrl;
	

private:	
	CWnd*   m_pWnd;
	CSQLDriver* m_pSQLDriver;
    CRect   m_Rect;
	int     m_MaxRow;
	int		m_MaxCol;
	int		m_FixedRow;
	int		m_FixedCol;
	int     m_TableIndex;
	
	
};