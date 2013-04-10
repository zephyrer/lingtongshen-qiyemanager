#include "stdafx.h"
#include "AddNewOrderDetailItem.h"
#include "../Global/Global.h"

CAddNewOrderDetailItem::CAddNewOrderDetailItem(CWnd *pParent )
                  :CAddNewItem(CAddNewOrderDetailItem::IDD,MAX_ROW,MAX_COLUMN,1,3,ORDER_DETAIL,ORDER_SEND_DETAIL_TITLE,pParent)
{
	m_LoadItemCount=0;

}

CAddNewOrderDetailItem::~CAddNewOrderDetailItem()
{


}

CString CAddNewOrderDetailItem::GenerateOrderNo()
{
	return L"M"+GetUniqueNo();
}

void CAddNewOrderDetailItem::OnItemSetNo()
{
	CString sOrderDetailNo;
	sOrderDetailNo=GenerateOrderNo();
	m_GridCtrl.SetItemText(m_content_index,1,m_OrderInfo.orderNo.c_str());
	m_GridCtrl.SetItemText(m_content_index,2,sOrderDetailNo);
	m_GridCtrl.Invalidate();

}

void CAddNewOrderDetailItem::OnItemDelRow()
{
	if(m_GridCtrl.IsValid(m_GridCtrl.GetFocusCell()))
	{
		int row=m_GridCtrl.GetFocusCell().row;
		if(row<=m_LoadItemCount)
		{
			::AfxMessageBox(L"该记录已存在，不能删除！");
			return;		
		}
		m_GridCtrl.DeleteRow(m_GridCtrl.GetFocusCell().row);
		m_content_index--;
		CString szIndex;
		for(DWORD index=1;index<=m_content_index;index++)
		{
			szIndex.Format(L"%d",index);
			m_GridCtrl.SetItemText(index,0,szIndex);
		}
		UpdateDlgGridData();
	}else{		
		::AfxMessageBox(L"请点击单元格选择需要删除的行");
	}

}

BOOL CAddNewOrderDetailItem::SetDlgGridContent()
{
	wstring szCondition=DB_TABLE_ORDER_KEY;
	szCondition=szCondition+L"="+L"\""+m_OrderInfo.orderNo+L"\"";
	m_pSQLDriver->ReadTableWithCondition(ORDER_DETAIL,szCondition.c_str(),(int)cfgFileItem.mapOrderDetail.size(),&m_vGridContent);
	m_LoadItemCount=(int)m_vGridContent.size();
	int colindex=1;
	int rowindex=0;
	while(rowindex<m_LoadItemCount)
	{
		OnItemAddBlankRow();
		for(colindex=1;colindex<(int)cfgFileItem.mapOrderDetail.size();colindex++)
		{
			LPCTSTR str=m_vGridContent[rowindex][colindex].c_str();
			m_GridCtrl.SetItemText(rowindex+1,colindex,str);
			m_GridCtrl.Invalidate();		
		}		
		rowindex++;	
	}
	m_vGridContent.clear();
	UpdateDlgGridData();
	
	return TRUE;
}


void CAddNewOrderDetailItem::GetOrderInfoFromOrderView(ORDER_INFO* pinfo)
{
	m_OrderInfo=*pinfo;
}

void CAddNewOrderDetailItem::OnItemAddOk()
{
	pRecordThread=AfxBeginThread(WriteOrderDetailToDatabase,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	pRecordThread->m_bAutoDelete=FALSE;
	pRecordThread->ResumeThread();
}


BOOL CAddNewOrderDetailItem::GetSentGoodsAmount(int* pAmount)
{
	int offset=0,amount=0,rowNum=(int)m_vGridContent.size();
	if(CGlobal::GetKeywordOffset(m_InsertTableIndex,DB_TABLE_ORDER_SENT_COUNT,&offset) && rowNum>0)
	{
		for(int index=0;index<rowNum;index++)
		{
			amount +=_wtoi(m_vGridContent[index][offset].c_str());		
		}
		*pAmount=amount;
		return TRUE;	
	}
	return FALSE;
}

BOOL CAddNewOrderDetailItem::CheckSentGoodsAmount(int* pAmount)
{
	int amount=0;
	int orderCount=_wtoi(m_OrderInfo.orderCount.c_str());
	if(GetNewContent())
	{
		if(GetSentGoodsAmount(&amount))
		{
			if(amount<=orderCount)
			{
				*pAmount=amount;
				return TRUE;		
			}else
				::AfxMessageBox(L"累计发货数量超出订货数量！");
		}else
			::AfxMessageBox(L"统计发货数量出错！");
	}else
		::AfxMessageBox(L"请填入数据！");

	return FALSE;
}


unsigned int  CAddNewOrderDetailItem::WriteOrderDetailToDatabase(LPVOID lParam)
{
	S_TRACE_FUNCTION;
	DWORD rValue=100;
	int counter=0,amount=0;
	//Disable buttons
	CAddNewOrderDetailItem* pDiag=(CAddNewOrderDetailItem*)lParam;
	pDiag->HideButtons();


	HWND hWnd=pDiag->GetSafeHwnd();
	SET_PB_POS(hWnd,counter++);

	if(pDiag->CheckSentGoodsAmount(&amount))
	{
		SET_PB_POS(hWnd,counter);
		int recordNum=(std::min)((int)pDiag->m_vGridContent.size(),MAX_RECORDS_NUM);
		int index=0;
		int step=MAX_RECORDS_NUM/recordNum;
		while(index<recordNum)
		{
			rValue=::WaitForSingleObject(pDiag->m_RecordEvent,0);
			if(rValue==WAIT_OBJECT_0 || rValue==WAIT_FAILED )
			{
				pDiag->m_vGridContent.clear();
				return 0;
			}

			if(pDiag->m_pSQLDriver->WriteOneRecordToDataBase(pDiag->m_InsertTableIndex,&pDiag->m_vGridContent.at(index)))
			{
				SET_PB_POS(hWnd,step*(index+1));				
			
			}else{
				::AfxMessageBox(L"录入失败！");
				S_LOG_FATAL(L"WriteOrderDetailToDatabase fail !"); 
				break;		
			}
			index++;
			
		}
		if(index==recordNum)
		{
			wstring szContent,szCondition;
			wchar_t szSentNum[255],szLeftNum[255];
			swprintf(szSentNum,255,L"%d",amount);
			swprintf(szLeftNum,255,L"%d",_wtoi(pDiag->m_OrderInfo.orderCount.c_str())-amount);
			szContent=DB_TABLE_ORDER_SENT_COUNT;
			szContent=szContent+L"="+szSentNum+L", "+DB_TABLE_ORDER_LEFT_COUNT+L"="+szLeftNum;
			szCondition=DB_TABLE_ORDER_KEY;
			szCondition=szCondition+L"=\""+pDiag->m_OrderInfo.orderNo+L"\"";
			if(pDiag->m_pSQLDriver->UpdateOneRecordInDataBase(ORDER_ITEM,szContent.c_str(),szCondition.c_str()))
			{
				SET_PB_POS(hWnd,MAX_RECORDS_NUM);
				//Sleep(500);
				::AfxMessageBox(L"成功录入！");
				pDiag->m_LoadItemCount=recordNum;
				pDiag->UpdateViewData();

			}else
				::AfxMessageBox(L"更新订单信息失败！");											
		}
			
	}
	pDiag->m_vGridContent.clear();
	pDiag->ExposeButtons();
	pDiag->pProgCtrl->ShowWindow(SW_HIDE);
	return 0;

}

