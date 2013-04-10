#include "stdafx.h"
#include "AddNewCustomerItem.h"

CAddNewCustomerItem::CAddNewCustomerItem(CWnd *pParent )
                  :CAddNewItem(CAddNewCustomerItem::IDD,MAX_ROW,MAX_COLUMN,1,1,CUSTOMER_ITEM,ADD_NEW_ITEM_CUSTOMER_DLG_TITLE,pParent)
{

}

CAddNewCustomerItem::~CAddNewCustomerItem()
{



}

void CAddNewCustomerItem::OnItemSetNo()
{

}