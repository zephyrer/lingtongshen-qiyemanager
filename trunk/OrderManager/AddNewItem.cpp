// AddNewOrderItem.cpp : implementation file
//

#include "stdafx.h"
#include "../Global/Global.h"
#include "OrderManager.h"
#include "AddNewItem.h"
#include "SelectView.h"
#include "MainFrm.h"
#include "OrderManagerView.h"


// CAddNewItem dialog

IMPLEMENT_DYNAMIC(CAddNewItem, CDialog)



CAddNewItem::CAddNewItem(
			UINT nIDTemplate,
			int     MaxRow,
			int		MaxCol,
			int		FixedRow,
			int		FixedCol,
			int     TableIndex,	
			CString Title,
			CWnd* pParent /*=NULL*/): CDialog(nIDTemplate,/*CAddNewItem::IDD,*/ pParent)
{
	m_bEditable=FALSE;
	m_OldSize = CSize(-1,-1);
	m_content_index=0;
	m_InsertTableIndex=TableIndex;
	m_MaxRow=MaxRow;
	m_MaxCol=MaxCol;
	m_FixedRow=FixedRow;
	m_FixedCol=FixedCol;
	m_Title=Title;
	pRecordThread=NULL;
	pProgCtrl=NULL;
	m_RecordEvent=NULL;
	m_pSQLDriver=NULL;

}

CAddNewItem::~CAddNewItem()
{
	if(pProgCtrl) delete pProgCtrl;
	if(pRecordThread) delete pRecordThread;
	if(m_pSQLDriver) delete m_pSQLDriver;

}

void CAddNewItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_ADD_ORDER, m_GridCtrl); 
}


BEGIN_MESSAGE_MAP(CAddNewItem, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ORDER_ADD_ROW,OnItemAddRow)
	ON_BN_CLICKED(IDC_BTN_ORDER_DEL_ROW,OnItemDelRow)
	ON_BN_CLICKED(IDP_ORDER_INPUT_CANCEL, OnItemAddCancel)
	ON_BN_CLICKED(IDC_DIAG_PRINT, OnDialogPrint)
	ON_BN_CLICKED(IDP_ORDER_INPUT_OK,OnItemAddOk)
	ON_MESSAGE(WM_USER_MSG,&CAddNewItem::OnProgMsg)	
END_MESSAGE_MAP()


BOOL CAddNewItem::CheckValidity(CString str)
{
	if(str.IsEmpty()) return FALSE;
	boost::wregex reg(L"\\s*");
	if(boost::regex_match(str.GetString(),reg)) return FALSE;
	return TRUE;
}


BOOL CAddNewItem::GetNewContent()
{
	map<wstring,wstring>tmpMap;
	vector<wstring>tempVec;
	tmpMap=CGlobal::GetDBTableMap(m_InsertTableIndex);
	int colnum=(int)tmpMap.size();
	int rownum=m_content_index;
	if(0==rownum) return FALSE;
	for(int row_index=1;row_index<=rownum;row_index++)
	{
		for(int col_index=0;col_index<colnum;col_index++)
		{
			CString str=m_GridCtrl.GetItemText(row_index,col_index);
			if(!CheckValidity(str))return FALSE;
			tempVec.push_back(str.GetString());			
		}
		m_vGridContent.push_back(tempVec);
		tempVec.clear();
	}
	return TRUE;
}

void CAddNewItem::OnDialogPrint()
{
	m_GridCtrl.Print();

}

BOOL CAddNewItem::WriteRecordToDatabase(int indx)
{
	return TRUE;
}
void CAddNewItem::GetOrderInfoFromOrderView(ORDER_INFO* pinfo)
{

}

void CAddNewItem::OnItemAddOk()
{
	pRecordThread=AfxBeginThread(WriteItemToDatabase,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	pRecordThread->m_bAutoDelete=FALSE;
	pRecordThread->ResumeThread();
}


LRESULT CAddNewItem::OnProgMsg( WPARAM wp,LPARAM lp)
{
	pProgCtrl->ShowWindow(SW_SHOW);
	pProgCtrl->SetPos((int)wp);
	return 1L;

}


void CAddNewItem::UpdateViewData()
{
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		 CDocTemplate *pDocTemp = theApp.GetNextDocTemplate(pos);
		 POSITION posdoc = pDocTemp->GetFirstDocPosition();
		 while (posdoc != NULL)
		 {
			  CDocument* pDoc = pDocTemp->GetNextDoc(posdoc);
			  if (pDoc != NULL)
			  {
				  POSITION posview = pDoc->GetFirstViewPosition();
				  if (posview != NULL)
				  {
					  COrderManagerView* pV = (COrderManagerView*)pDoc->GetNextView(posview);
					  pV->SendMessage(WM_USER_MSG_REFRESH);
				  }
			  }
		   }
	}	
}

void CAddNewItem::HideButtons()
{
	pAdd->EnableWindow(FALSE);
	pDel->EnableWindow(FALSE);
	pRec->EnableWindow(FALSE);
}

void CAddNewItem::ExposeButtons()
{
	pAdd->EnableWindow(TRUE);
	pDel->EnableWindow(TRUE);
	pRec->EnableWindow(TRUE);
}

unsigned int  CAddNewItem::WriteItemToDatabase(LPVOID lParam)
{
	S_TRACE_FUNCTION;
	DWORD rValue=100;
	int counter=0;
	//Disable buttons
	CAddNewItem* pDiag=(CAddNewItem*)lParam;
	pDiag->HideButtons();

	HWND hWnd=pDiag->GetSafeHwnd();
	SET_PB_POS(hWnd,counter++);

	if(pDiag->GetNewContent())
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
				S_LOG_FATAL(L"WriteItemToDatabase 录入失败！"); 
				break;		
			}
			index++;
			
		}
		if(index==recordNum)
		{
			SET_PB_POS(hWnd,MAX_RECORDS_NUM);
			Sleep(500);
			::AfxMessageBox(L"成功录入！");
			pDiag->UpdateViewData();			
		}
		pDiag->m_vGridContent.clear();

	}else
		::AfxMessageBox(L"请填入数据！");

	pDiag->ExposeButtons();
	pDiag->pProgCtrl->ShowWindow(SW_HIDE);
	return 0;

}



void CAddNewItem::StopRecordThread()
{
	if(NULL==pRecordThread) return;
	::SetEvent(m_RecordEvent);
	int nTimes=100;
	while(TRUE)
	{
		DWORD dwExitCode=0;
		::GetExitCodeThread(pRecordThread->m_hThread,&dwExitCode);
		if(STILL_ACTIVE!=dwExitCode) break;
		if(0==nTimes--)
		{
			S_LOG_FATAL(L"Stop record thread fail!")
			::TerminateThread(pRecordThread,0);
			break;		
		}
		Sleep(50);	
	}
	::CloseHandle(m_RecordEvent);
	m_RecordEvent=NULL;
	delete pRecordThread;
	pRecordThread=NULL;
}

void CAddNewItem::OnItemAddCancel()
{
	int ret=::AfxMessageBox(L"确认退出？",MB_YESNO);
	if(IDYES==ret)
	{
		StopRecordThread();
		CAddNewItem::EndDialog(0);	
	}
}

void CAddNewItem::OnItemDelRow()
{
	if(m_GridCtrl.IsValid(m_GridCtrl.GetFocusCell()))
	{
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

void CAddNewItem::OnItemSetNo()
{
	CString sOrderNo;
	sOrderNo=GenerateOrderNo();
	m_GridCtrl.SetItemText(m_content_index,1,sOrderNo);
	UpdateDlgGridData();
}

void CAddNewItem::OnItemAddBlankRow()
{
	CString sOrderIndex;
	sOrderIndex.Format(L"%d",++m_content_index);	
	m_GridCtrl.InsertRow(sOrderIndex);
	UpdateDlgGridData();
}

void CAddNewItem::OnItemAddRow()
{
	if(m_content_index>=MAX_RECORDS_NUM)
	{
		::AfxMessageBox(L"超过最大记录数！");
		S_LOG_FATAL(L"超过最大记录数:"<<m_content_index); 
		return;	
	}
	OnItemAddBlankRow();
	OnItemSetNo();
}


CString CAddNewItem::GetUniqueNo()
{
	SYSTEMTIME sys; 
	CString szYear,szMonth,szDay,szHour,szMin,szSec,szMiliSec;
	GetLocalTime( &sys );
	szYear.Format(L"%4d-",sys.wYear);
	szMonth.Format(L"%02d-",sys.wMonth);
	szDay.Format(L"%02d:",sys.wDay);
	szHour.Format(L"%02d:",sys.wHour);
	szMin.Format(L"%02d:",sys.wMinute);
	szSec.Format(L"%02d.",sys.wSecond);
	szMiliSec.Format(L"%03d",sys.wMilliseconds);
	return szYear+szMonth+szDay+szHour+szMin+szSec+szMiliSec;
}

CString CAddNewItem::GenerateOrderNo()
{
	return GetUniqueNo();
}


void CAddNewItem::UpdateDlgGridData()
{
	m_GridCtrl.Invalidate();
	m_GridCtrl.AutoSize(GVS_BOTH);
}

BOOL CAddNewItem::SetDlgGridContent()
{
	//set order index and order no by default
	OnItemAddRow();
	return TRUE;
}

BOOL CAddNewItem::OnInitDlgGrid()
{
	//init grid view
	SetWindowText(m_Title);
	CRect rect;
	GetClientRect(rect);
	m_OldSize = CSize(rect.Width(), rect.Height());
	m_GridCtrl.SetEditable(TRUE);
	m_GridCtrl.EnableDragAndDrop(FALSE);
	//m_GridCtrl.SetHeaderSort(TRUE);
	//init title
	SetDlgGridTitle(m_InsertTableIndex);	
	return SetDlgGridContent();
}


BOOL CAddNewItem::SetDlgGridTitle(int table_index)
{
	m_InsertTableIndex=table_index;
	int col_max=(std::min)(m_MaxCol, (int)CGlobal::GetDBTableMap(m_InsertTableIndex).size());
	try {
		m_GridCtrl.SetRowCount(1);
		m_GridCtrl.SetColumnCount(col_max);
		m_GridCtrl.SetFixedRowCount(1);
		m_GridCtrl.SetFixedColumnCount(m_FixedCol);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		S_LOG_FATAL(L"Grid exception!"); 
		return FALSE;
	}

	map<wstring,wstring>tmpMapTitle;
	tmpMapTitle=CGlobal::GetDBTableMap(m_InsertTableIndex);
	map<wstring,wstring>::iterator iter;
	int index=0;
	for(iter=tmpMapTitle.begin();iter!=tmpMapTitle.end();iter++)
	{
		m_GridCtrl.SetItemText(0,index,iter->second.c_str());
		index++;
	}
	UpdateDlgGridData();
	return TRUE;
}


void CAddNewItem::OnInitProgressBar()
{
	//make it in centre
	pProgCtrl=(CProgressCtrl*) new CProgressCtrl;
	CRect wndRect, prgRect;
	GetClientRect(wndRect);
	prgRect.left=wndRect.left+wndRect.Width()/2-100;
	prgRect.right=wndRect.right-wndRect.Width()/2+100;
	prgRect.top=wndRect.top+wndRect.Height()/2-10;
	prgRect.bottom=wndRect.bottom-wndRect.Height()/2+10;

	//create progress bar, hide by default	
	pProgCtrl->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,prgRect, (CWnd*)this,IDC_ADDORDER_PROG_BAR);
	pProgCtrl->ShowWindow(SW_HIDE);
	pProgCtrl->SetRange(0,MAX_RECORDS_NUM);
}

void CAddNewItem::ReLayout()
{


}

BOOL CAddNewItem::OnInitDialog()
{
		CDialog::OnInitDialog();
		pAdd=(CButton*)GetDlgItem(IDC_BTN_ORDER_ADD_ROW);
	    pDel=(CButton*)GetDlgItem(IDC_BTN_ORDER_DEL_ROW);
		pRec=(CButton*)GetDlgItem(IDP_ORDER_INPUT_OK);
		//init SQL
		m_pSQLDriver=new CSQLDriver();
		if(!OnInitDlgGrid())
		{
			S_LOG_FATAL(L"Init dialog grid fail!");
			::AfxMessageBox(L"窗口初始化失败，请重试！");
			CAddNewItem::EndDialog(0);
			return FALSE;		
		}
		OnInitProgressBar();
		m_RecordEvent=::CreateEvent(NULL,FALSE,FALSE,NULL);
		if(NULL==m_RecordEvent)
		{
			S_LOG_FATAL(L"create record event fail!");
			::AfxMessageBox(L"窗口事件初始化失败，请重试！");
			CAddNewItem::EndDialog(0);
			return FALSE;		
		}

		ReLayout();

		return TRUE;
}


void CAddNewItem::UpdateControlPostion(CWnd* pWnd,CRect rect, LPARAM lParam)
{
	CSize* pTranslate = (CSize*) lParam;
	pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top, rect.Width(), rect.Height(), FALSE);


}

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
	CWnd* pWnd = CWnd::FromHandle(hwnd);
	CSize* pTranslate = (CSize*) lParam;
	CAddNewItem* pDlg = (CAddNewItem*) pWnd->GetParent();
	if (!pDlg) return FALSE;
	CRect rect;
	pWnd->GetWindowRect(rect);
	pDlg->ScreenToClient(rect);
	if (hwnd == pDlg->m_GridCtrl.GetSafeHwnd())
	{
		pWnd->MoveWindow(rect.left, rect.top, 
							 rect.Width()+pTranslate->cx, 
							 rect.Height()+pTranslate->cy, FALSE);
	}else if(pWnd->GetDlgCtrlID()==IDC_ADDORDER_PROG_BAR){
		pWnd->MoveWindow(rect.left+pTranslate->cx/2, rect.top+pTranslate->cy/2, 
							 rect.Width(), rect.Height(), FALSE);
	}else if(pWnd->GetDlgCtrlID()==IDC_DIAG_PRINT)
		pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, rect.Width(), rect.Height(), FALSE);
	else
		pDlg->UpdateControlPostion(pWnd,rect,lParam);
	pDlg->Invalidate();
	return TRUE;
}

void CAddNewItem::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	if (cx <= 1 || cy <= 1 ) 
        return;
	CSize Translate(cx - m_OldSize.cx, cy - m_OldSize.cy);
	::EnumChildWindows(GetSafeHwnd(), EnumProc, (LPARAM)&Translate);
	m_OldSize = CSize(cx,cy);

}

void CAddNewItem::OnCancel()
{
	OnItemAddCancel();
}


// CAddNewItem message handlers
