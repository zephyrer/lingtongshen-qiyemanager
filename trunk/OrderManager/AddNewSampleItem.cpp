#include "stdafx.h"
#include "AddNewSampleItem.h"

CAddNewSampleItem::CAddNewSampleItem(CWnd *pParent )
                  :CAddNewItem(CAddNewSampleItem::IDD,MAX_ROW,MAX_COLUMN,1,2,SAMPLE_ITEM,ADD_NEW_ITEM_SAMPLE_DLG_TITLE,pParent)
{

}

CAddNewSampleItem::~CAddNewSampleItem()
{



}


CString CAddNewSampleItem::GenerateOrderNo()
{
	return L"Y"+GetUniqueNo();

}