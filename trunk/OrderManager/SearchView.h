#include "AddNewItem.h"
#include "Resource.h"
#pragma once

class CSearchView:public CAddNewItem
{
public:
	CSearchView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchView();


public:

	enum { IDD = IDD_DIALOG_ADD_NEW_ORDER };


public:
	void ReLayout();
	virtual void DoDataExchange(CDataExchange* pDX); 
private:
	BOOL GetControlInfo(CWnd* pWnd, CWnd** pSafeWnd, CRect* pRect);
	void UpdateControlPostion(CWnd* pWnd,CRect rect, LPARAM lParam);
	//BOOL SetDlgGridTitle(int table_index);
	BOOL SetDlgGridContent();
	void OnTableSelChange();
	void SetSearchInput();
	void OnSearch();
	void GetSearchCondition(map<wstring,wstring>* pMapCondition);
	static  unsigned int  SearchDataBase(LPVOID lParam);
	static  unsigned int  ClickPage(LPVOID lParam);
	void GetMeetRecordsCount(int* pCount,map<wstring,wstring>* pMapCondition);
	BOOL QueryShowRecords();
	void AddResultGrid();
	void DeleteContentRow();
	void InsertBlanRow();
	void OnPageClick();
	void OnSelectFirstPage();
	void OnSelectPrevPage();
	void OnSelectNextPage();
	void OnSelectLastPage();
	void SetPageState();
	void OnItemDelRow();
	void OnItemAddOk();
	void ResetResultIndex();


public:

private:
	CButton *pSearch,*pFirst,*pPrev,*pNext,*pLast,*pMatch;
	CEdit* pPageProg;
	CComboBox *pTableSelect;
	CFont m_FontNormal,m_FontSmall;
	int m_RecordsCount,m_PageCont,m_CurPageIndex,m_PageRowIndex;
	wstring m_Condition;
	int m_Table;



	DECLARE_MESSAGE_MAP()

};