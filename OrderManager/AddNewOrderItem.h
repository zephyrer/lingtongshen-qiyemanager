#include "AddNewItem.h"
#include "Resource.h"
#pragma once

class CAddNewOrderItem:public CAddNewItem
{
public:
	CAddNewOrderItem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewOrderItem();


public:
	CString GenerateOrderNo();

	enum { IDD = IDD_DIALOG_ADD_NEW_ORDER };

};