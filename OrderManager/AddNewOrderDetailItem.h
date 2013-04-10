#include "AddNewItem.h"
#include "Resource.h"
#pragma once
using namespace std;

class CAddNewOrderDetailItem:public CAddNewItem
{
public:
	CAddNewOrderDetailItem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewOrderDetailItem();

	enum { IDD = IDD_DIALOG_ADD_NEW_ORDER };


private:
	static  unsigned int  WriteOrderDetailToDatabase(LPVOID lParam);
	BOOL SetDlgGridContent();
	void GetOrderInfoFromOrderView(ORDER_INFO* pinfo);
	CString GenerateOrderNo();
	void OnItemSetNo();
	afx_msg void OnItemAddOk();
	void OnItemDelRow();
	BOOL GetSentGoodsAmount(int* pAmount);
	BOOL CheckSentGoodsAmount(int* pAmount);
	


private:
	ORDER_INFO m_OrderInfo; 
	int m_LoadItemCount;

	

};