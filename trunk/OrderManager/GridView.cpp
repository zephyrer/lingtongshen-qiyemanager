#include "stdafx.h"
#include "GridView.h"
#include "../Global/Global.h"




CGridView::CGridView(	
		CWnd*   pWnd,
		CRect   Rect,
		int     MaxRow,
		int		MaxCol,
		int		FixedRow,
		int		FixedCol,
		int     TableIndex)
{
	m_pWnd=pWnd;
	m_Rect=Rect;
	m_MaxRow=MaxRow;
	m_MaxCol=MaxCol;
	m_FixedRow=FixedRow;
	m_FixedCol=FixedCol;
	m_TableIndex=TableIndex;
	m_pGridCtrl=NULL;
	pfnGridViewCallback=NULL;
	m_pSQLDriver=NULL;
	m_ListNo=1;

}

CGridView::~CGridView()
{
	if(m_pGridCtrl) delete m_pGridCtrl;
	if(m_pSQLDriver) delete m_pSQLDriver;
}

BOOL CGridView::SetContent()
{
	if(NULL==m_pSQLDriver)
	{
		m_pSQLDriver=new CSQLDriver;		
	}
	return m_pSQLDriver->OpenSQL(m_TableIndex,&m_GridViewData);	
}

BOOL CGridView::SetTitle()
{
	map<wstring,wstring>tmpMapTitle;
	vector<wstring> tempV;
	tmpMapTitle=CGlobal::GetDBTableMap(m_TableIndex);
	map<wstring,wstring>::iterator iter;
	for(iter=tmpMapTitle.begin();iter!=tmpMapTitle.end();iter++)
	{
		tempV.push_back(iter->second.c_str());	
	}
	SetGridViewData(tempV);
	UpdateGridData();
	return TRUE;
}


void CGridView::SetGVClickCallbackFunc()
{
	

}

BOOL CGridView::GetRowOrderInfo(int row,ORDER_INFO* pInfo)
{
	int in[4];
	if( CGlobal::GetKeywordOffset(m_TableIndex,DB_TABLE_ORDER_KEY,&in[0])
		&&CGlobal::GetKeywordOffset(m_TableIndex,DB_TABLE_ORDER_COUNT,&in[1])
		&&CGlobal::GetKeywordOffset(m_TableIndex,DB_TABLE_ORDER_SENT_COUNT,&in[2])
		&&CGlobal::GetKeywordOffset(m_TableIndex,DB_TABLE_ORDER_LEFT_COUNT,&in[3]))
	{
		pInfo->orderNo=m_GridViewData[row][in[0]];
		pInfo->orderCount=m_GridViewData[row][in[1]];
		pInfo->orderSendCount=m_GridViewData[row][in[2]];
		pInfo->orderLeftCount=m_GridViewData[row][in[3]];
		return TRUE;
	}
	return FALSE;
}


int CGridView::GetTableIndex()
{
	return m_TableIndex;
}

BOOL CGridView::InitGrid()
{
	S_TRACE_FUNCTION;
	if(m_pGridCtrl)
		return FALSE;			
	m_pGridCtrl=new CGridCtrl;
	if(!m_pGridCtrl) return FALSE;		
	// Create the Gridctrl window
	m_pGridCtrl->Create(m_Rect, m_pWnd, 1230);
	// fill it up with stuff
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE);
	
	//virtual mode
	m_pGridCtrl->SetVirtualMode(TRUE);
    m_pGridCtrl->SetCallbackFunc(GridCallback,(LPARAM)this);

	SetGVClickCallbackFunc();
	//remove unsed colms
	int col_max=(std::min)(m_MaxCol, (int)CGlobal::GetDBTableMap(m_TableIndex).size());
	try {
		m_pGridCtrl->SetRowCount(m_MaxRow);
		m_pGridCtrl->SetColumnCount(col_max);
		m_pGridCtrl->SetFixedRowCount(m_FixedRow);
		m_pGridCtrl->SetFixedColumnCount(m_FixedCol);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		S_LOG_FATAL(L"Grid init exception!!");
		return FALSE;
	}
	m_pGridCtrl->AutoSize(GVS_BOTH);
	SetTitle();
	BOOL rVal=SetContent();
	if(!rVal) S_LOG_FATAL(L"数据库初始化失败!");
	UpdateGridData();
	return rVal;
}


void CGridView::SetGridViewData(vector<wstring> vstr)
{
	m_GridViewData.push_back(vstr);
}

void CGridView::OnDraw(CDC* pDC)
{


}

BOOL CALLBACK CGridView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
 {
	 CGridView* pVal=(CGridView*)lParam;
	 if(!pVal->m_GridViewData.empty())
	 {
		 if(pDispInfo->item.row < (int)pVal->m_GridViewData.size() && pDispInfo->item.col <(int)pVal->m_GridViewData[0].size() )
		 {
				pDispInfo->item.strText=pVal->m_GridViewData[pDispInfo->item.row][pDispInfo->item.col].c_str();	 
		 }		 	  
	 }	 
    return TRUE;
 
 }

void CGridView::SetCellText(int row, int col, LPCTSTR str)
{
	m_pGridCtrl->SetItemText(row,col,str);

}

BOOL CGridView::OnGridLoadComplete()
{
	return m_pGridCtrl->IsGridLoadComplete();

}

BOOL CGridView::InsertGridWithOneRow(LPCTSTR str)
{
	m_pGridCtrl->InsertRow(str);
	return FALSE;

}

BOOL CGridView::DeleteGridWithOneRow()
{
	if(m_pGridCtrl->IsValid(m_pGridCtrl->GetFocusCell()))
	{
		m_pGridCtrl->DeleteRow(m_pGridCtrl->GetFocusCell().row);
		return TRUE;

	}else{		
		return FALSE;
	}

}

BOOL CGridView::InsertGridWithOneColumn(LPCTSTR str)
{
	m_pGridCtrl->InsertColumn(str);
	return FALSE;
}


void CGridView::UpdateGridData()
{
	m_pGridCtrl->Invalidate();
	m_pGridCtrl->AutoSize(GVS_BOTH);
}



void CGridView::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	if(m_pGridCtrl) m_pGridCtrl->OnBeginPrinting(pDC,pInfo);

}
void CGridView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pGridCtrl) m_pGridCtrl->OnPrint(pDC,pInfo);
}
void CGridView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pGridCtrl) m_pGridCtrl->OnEndPrinting(pDC,pInfo);
}


void CGridView::OnGridViewSize(CRect rect)
{
	if (m_pGridCtrl->GetSafeHwnd())
	{
		m_pGridCtrl->MoveWindow(rect);
	}

}
BOOL CGridView::OnGridCMDMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
        if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;

	return FALSE;
	
}
void CGridView::OnGridToggleReadonly()
{
	if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
    {
        CCellID cell = m_pGridCtrl->GetFocusCell();
        if (m_pGridCtrl->IsValid(cell))
        {
            int nState = m_pGridCtrl->GetItemState(cell.row, cell.col);
            if (m_pGridCtrl->IsCellEditable(cell))
                m_pGridCtrl->SetItemState(cell.row, cell.col, nState | GVIS_READONLY);
            else
                m_pGridCtrl->SetItemState(cell.row, cell.col, nState & ~GVIS_READONLY);
        }
    }
}