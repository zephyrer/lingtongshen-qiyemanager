#include "stdafx.h"
#include "AddNewItem.h"
#include "AddNewOrderDetailItem.h"
#include "ShowOrderGridView.h"


CShowOrderGridView::CShowOrderGridView(CWnd*   pWnd,CRect   Rect)
					:CGridView(pWnd,Rect,MAX_ROW,MAX_COLUMN,1,1,ORDER_ITEM)
{
	m_OrderNoOff=0;
	m_OrderCountOff=0;
	m_OrderSentCountOff=0;
	m_OrderLeftCountOff=0;

}

CShowOrderGridView::~CShowOrderGridView()
{

}



BOOL CALLBACK CShowOrderGridView::GridClickCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	CGridView* pVal=(CGridView*)lParam;
	int clickOff=0;
	if(!CGlobal::GetKeywordOffset(pVal->GetTableIndex(),DB_TABLE_ORDER_SENT_COUNT,&clickOff)) return FALSE;
	if(clickOff==pDispInfo->item.col 
		&& 0<pDispInfo->item.row 
		&& pDispInfo->item.row< (int)pVal->m_GridViewData.size() )
	{
		if(pVal->pfnGridViewCallback)pVal->pfnGridViewCallback(pDispInfo,lParam);
	}
	return TRUE;
}

BOOL CALLBACK CShowOrderGridView::OrderGridViewCallbackFun(GV_DISPINFO *pDispInfo, LPARAM lParam)
{	
	CGridView* pVal=(CGridView*)lParam;
	ORDER_INFO info;
	if(pVal->GetRowOrderInfo(pDispInfo->item.row,&info))
	{
		CAddNewItem* pShowDetail=new CAddNewOrderDetailItem();
		pShowDetail->GetOrderInfoFromOrderView(&info);
		pShowDetail->DoModal();
		delete pShowDetail;
		return TRUE;
	}
	return FALSE;
}


void CShowOrderGridView::SetGVClickCallbackFunc()
{
	m_pGridCtrl->SetClickCallbackFunc(GridClickCallback,(LPARAM)this);	
	this->SetGridViewCallback(OrderGridViewCallbackFun);
}

