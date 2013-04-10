#include "stdafx.h"
#include "ShowCustomerGridView.h"


CShowCustomerGridView::CShowCustomerGridView(CWnd*   pWnd,CRect   Rect)
					:CGridView(pWnd,Rect,MAX_ROW,MAX_COLUMN,1,1,CUSTOMER_ITEM)
{

}

CShowCustomerGridView::~CShowCustomerGridView()
{


}