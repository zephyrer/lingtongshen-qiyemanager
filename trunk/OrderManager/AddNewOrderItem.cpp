#include "stdafx.h"
#include "AddNewOrderItem.h"

CAddNewOrderItem::CAddNewOrderItem(CWnd *pParent )
                  :CAddNewItem(CAddNewOrderItem::IDD,MAX_ROW,MAX_COLUMN,1,2,ORDER_ITEM,ADD_NEW_ITEM_ORDER_DLG_TITLE,pParent)
{

}

CAddNewOrderItem::~CAddNewOrderItem()
{



}


CString CAddNewOrderItem::GenerateOrderNo()
{
	return L"D"+GetUniqueNo();

}