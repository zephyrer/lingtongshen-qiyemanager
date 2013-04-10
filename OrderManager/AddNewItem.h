#pragma once
#include "../SQLDriver/SQLDriver.h"

// CAddNewItem dialog

class CAddNewItem : public CDialog
{
	DECLARE_DYNAMIC(CAddNewItem)

public:
	CAddNewItem(
		UINT nIDTemplate,
		int     MaxRow,
		int		MaxCol,
		int		FixedRow,
		int		FixedCol,		
		int     TableIndex,	
		CString Title,
		CWnd* pParent = NULL); 

	virtual ~CAddNewItem();

//new thread
public:
	static  unsigned int  WriteItemToDatabase(LPVOID lParam);
	virtual void GetOrderInfoFromOrderView(ORDER_INFO* pinfo);
	virtual void UpdateControlPostion(CWnd* pWnd,CRect rect, LPARAM lParam);

//methods for overwrite/load
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void ReLayout();
	virtual void OnCancel();
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnItemSetNo();
	virtual void OnItemAddBlankRow();
	virtual afx_msg void OnItemAddRow();
	virtual afx_msg void OnItemDelRow();
	virtual afx_msg void OnItemAddCancel();
	virtual afx_msg void OnItemAddOk();
	virtual afx_msg void OnDialogPrint();
	virtual afx_msg LRESULT OnProgMsg(WPARAM wp,LPARAM lp);
	virtual BOOL OnInitDlgGrid();
	virtual void UpdateDlgGridData();
	virtual BOOL SetDlgGridTitle(int indx);
	virtual BOOL SetDlgGridContent();
	virtual void OnInitProgressBar();
	virtual void StopRecordThread();
	virtual BOOL GetNewContent();
	virtual BOOL WriteRecordToDatabase(int indx);
	virtual CString GenerateOrderNo();
	virtual CString GetUniqueNo();
	virtual void UpdateViewData();
	virtual void HideButtons();
	virtual void ExposeButtons();
	virtual BOOL CheckValidity(CString str);
	

public:
	CGridCtrl m_GridCtrl;

protected:
	DWORD m_content_index;
	int m_InsertTableIndex;
	int m_MaxRow;
	int	m_MaxCol;
	int	m_FixedRow;
	int	m_FixedCol;
	CProgressCtrl *pProgCtrl;
	CWinThread *pRecordThread;
	HANDLE m_RecordEvent;
	vector<vector<wstring> >m_vGridContent;
	CSize m_OldSize;
	BOOL	m_bEditable;
	CString m_Title;
	CSQLDriver* m_pSQLDriver;
	CButton *pAdd;
	CButton *pDel;
	CButton *pRec;
	//CFont m_Font;


	DECLARE_MESSAGE_MAP()
};
