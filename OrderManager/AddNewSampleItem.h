#include "AddNewItem.h"
#include "Resource.h"
#pragma once

class CAddNewSampleItem:public CAddNewItem
{
public:
	CAddNewSampleItem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewSampleItem();

	enum { IDD = IDD_DIALOG_ADD_NEW_ORDER };

public:
	CString GenerateOrderNo();

};