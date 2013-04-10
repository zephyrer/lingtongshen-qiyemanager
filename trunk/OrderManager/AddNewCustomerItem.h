#include "AddNewItem.h"
#include "Resource.h"
#pragma once

class CAddNewCustomerItem:public CAddNewItem
{
public:
	CAddNewCustomerItem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewCustomerItem();

	enum { IDD = IDD_DIALOG_ADD_NEW_ORDER };

public:
	void OnItemSetNo();

};