#include "stdafx.h"
#include "SearchView.h"

CSearchView::CSearchView(CWnd *pParent )
                  :CAddNewItem(CSearchView::IDD,MAX_ROW,MAX_COLUMN,1,1,ORDER_ITEM,SEARCH_TITLE,pParent)
{
	m_Table=ORDER_ITEM;
	pSearch=NULL;
	pFirst=NULL;
	pPrev=NULL;
	pNext=NULL;
	pLast=NULL;
	pTableSelect=NULL;
	pMatch=NULL;
	pPageProg=NULL;
	m_PageCont=0;
	m_CurPageIndex=1;
	m_RecordsCount=0;
	m_PageRowIndex=1;
}


CSearchView::~CSearchView()
{
	if (pSearch) delete pSearch;
	if (pFirst) delete pFirst;
	if (pPrev) delete pPrev;
	if (pNext) delete pNext;
	if (pLast) delete pLast;
	if (pTableSelect) delete pTableSelect;
	if (pMatch) delete pMatch;
	if (pPageProg) delete pPageProg;
}


BEGIN_MESSAGE_MAP(CSearchView, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDP_ORDER_INPUT_CANCEL, &CAddNewItem::OnItemAddCancel)
	ON_BN_CLICKED(IDC_DIAG_PRINT, &CAddNewItem::OnDialogPrint)
	ON_MESSAGE(WM_USER_MSG,&CAddNewItem::OnProgMsg)
	ON_BN_CLICKED(IDC_GRID_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_BTN_ORDER_DEL_ROW,OnItemDelRow)
	ON_BN_CLICKED(IDP_ORDER_INPUT_OK,OnItemAddOk)
	ON_BN_CLICKED(IDC_BTN_FIRST, OnSelectFirstPage)
	ON_BN_CLICKED(IDC_BTN_PREV, OnSelectPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnSelectNextPage)
	ON_BN_CLICKED(IDC_BTN_LAST, OnSelectLastPage)
	ON_LBN_SELCHANGE(IDC_COMBO_SELECT_TABLE, OnTableSelChange) 
END_MESSAGE_MAP()

void CSearchView::ResetResultIndex()
{
	m_PageCont=0;
	m_CurPageIndex=0;
	m_RecordsCount=0;
	m_PageRowIndex=1;
	SetPageState();
	m_CurPageIndex=1;
}

void CSearchView::OnItemDelRow()
{
	if(m_PageRowIndex<=1)
	{
		::AfxMessageBox(L"无数据！");
		return;	
	}
	if(m_GridCtrl.IsValid(m_GridCtrl.GetFocusCell()))
	{
		int row=m_GridCtrl.GetFocusCell().row;
		if(row<=1)
		{
			::AfxMessageBox(L"无效行！");
			return;			
		}
		int col=0;
		wstring szCondition;
		CString strKey1,strKey2;
		if(!CGlobal::GetKeywordOffset(m_Table,g_dbTableKeyList[m_Table].c_str(),&col))
		{
			::AfxMessageBox(L"删除失败，无法获取主键！");
			S_LOG_FATAL(L"删除失败,无法获取主键:"<<col);
			return;		
		}
		if(CUSTOMER_ITEM==m_Table)
		{
			strKey1=m_GridCtrl.GetItemText(row,col);
			strKey2=m_GridCtrl.GetItemText(row,col-1);
			szCondition=DB_TABLE_CUSTOMER_KEY1;
			szCondition=szCondition+L"=\""+strKey2.GetString()+L"\""+L" and "+DB_TABLE_CUSTOMER_KEY2+L"=\""+strKey1.GetString()+L"\"";		
		}else{
			strKey1=m_GridCtrl.GetItemText(row,col);
			szCondition=g_dbTableKeyList[m_Table];
			szCondition=szCondition+L"=\""+strKey1.GetString()+L"\"";	
		}

		if(!m_pSQLDriver->DeleteOneRecordWithCondition(m_Table,szCondition.c_str()))
		{
			::AfxMessageBox(L"删除失败，无法更新数据库！");
			S_LOG_FATAL(L"删除失败，无法更新数据库:"<<col);
			return;		
		}
		UpdateViewData();
		::AfxMessageBox(L"删除成功！");
		OnSearch();

	}else{		
		::AfxMessageBox(L"请点击单元格选择需要删除的行!");
	}

}


void CSearchView::OnItemAddOk()
{
	if(m_PageRowIndex<=1)
	{
		::AfxMessageBox(L"无数据！");
		return;	
	}
	if(m_GridCtrl.IsValid(m_GridCtrl.GetFocusCell()))
	{
		int row=m_GridCtrl.GetFocusCell().row;
		if(row<=1)
		{
			::AfxMessageBox(L"无效行！");
			return;			
		}
		map<wstring,wstring>tmpMap;
		vector<wstring>tempVec;
		tmpMap=CGlobal::GetDBTableMap(m_Table);
		int colnum=(int)tmpMap.size();
		for(int col_index=0;col_index<colnum;col_index++)
		{
			CString str=m_GridCtrl.GetItemText(row,col_index);
			if(!CheckValidity(str))
			{
				::AfxMessageBox(L"请输入有效数据!");			
			}
			tempVec.push_back(str.GetString());			
		}
		if(!m_pSQLDriver->WriteOneRecordToDataBase(m_Table,&tempVec))
		{
			::AfxMessageBox(L"更新数据库失败!");
			return;
		
		}
		UpdateViewData();
		::AfxMessageBox(L"更新成功！");
		OnSearch();

	}else{
		::AfxMessageBox(L"请点击单元格选择需要更新的行!");
	}

}

void CSearchView::OnSelectFirstPage()
{
	if(m_PageRowIndex<=1) return;
	if(m_CurPageIndex==1) return;
	m_CurPageIndex=1;
	OnPageClick();

}
void CSearchView::OnSelectPrevPage()
{
	if(m_PageRowIndex<=1) return;
	if(m_CurPageIndex<=1) return;
	m_CurPageIndex--;
	OnPageClick();

}
void CSearchView::OnSelectNextPage()
{
	if(m_PageRowIndex<=1) return;
	if(m_CurPageIndex>=m_PageCont) return;
	m_CurPageIndex++;
	OnPageClick();

}

void CSearchView::OnSelectLastPage()
{
	if(m_PageRowIndex<=1) return;
	if(m_CurPageIndex==m_PageCont)return;
	m_CurPageIndex=m_PageCont;
	OnPageClick();
}

unsigned int  CSearchView::ClickPage(LPVOID lParam)
{
	CSearchView* pDiag=(CSearchView*)lParam;
	wstring szMatch;
	HWND hWnd=pDiag->GetSafeHwnd();
	int counter=0;
	if(!pDiag->QueryShowRecords())::AfxMessageBox(L"查询失败！");
	Sleep(1000);
	pDiag->pProgCtrl->ShowWindow(SW_HIDE);
	return 0;
}

void CSearchView::GetSearchCondition(map<wstring,wstring>* pMapCondition)
{
	map<wstring,wstring>tmpMap;
	vector<wstring>titleVec;
	tmpMap=CGlobal::GetDBTableMap(m_Table);
	int colnum=(int)tmpMap.size();
	int row_index=1;
	//get title vector
	for(map<wstring,wstring>::iterator iter=tmpMap.begin();iter!=tmpMap.end();iter++)
	{
		titleVec.push_back(iter->second);
	}
	//set condition map
	for(int col_index=1;col_index<colnum;col_index++)
	{
		CString str=m_GridCtrl.GetItemText(row_index,col_index);
		if(CheckValidity(str))
			pMapCondition->insert(pair<wstring,wstring>(titleVec[col_index],str.GetString()));			
	}

}


void CSearchView::OnPageClick()
{
	DeleteContentRow();
	pRecordThread=AfxBeginThread(ClickPage,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	pRecordThread->m_bAutoDelete=FALSE;
	pRecordThread->ResumeThread();
}

void CSearchView::OnSearch()
{
	DeleteContentRow();
	pRecordThread=AfxBeginThread(SearchDataBase,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	pRecordThread->m_bAutoDelete=FALSE;
	pRecordThread->ResumeThread();

}

void CSearchView::GetMeetRecordsCount(int* pCount,map<wstring,wstring>* pMapCondition)
{


}

void CSearchView::DeleteContentRow()
{
	//delete original result rows
	while(m_PageRowIndex>1)
	{
		m_GridCtrl.DeleteRow(m_PageRowIndex--);
		UpdateDlgGridData();	
	}
}

void CSearchView::InsertBlanRow()
{
	m_GridCtrl.InsertRow(L"");
	m_PageRowIndex++;
}

void CSearchView::SetPageState()
{
	TCHAR szPageIndex[255];
	wstring szPageState;
	_itow_s(m_CurPageIndex,szPageIndex,10);
	szPageState=szPageIndex;
	szPageState +=L"/";
	_itow_s(m_PageCont,szPageIndex,10);
	szPageState =szPageState+szPageIndex;
	pPageProg->SetWindowText(szPageState.c_str());

}

BOOL CSearchView::QueryShowRecords()
{
	HWND hWnd=this->GetSafeHwnd();
	if(m_CurPageIndex>m_PageCont)m_CurPageIndex=1;
	int start_index=(m_CurPageIndex-1)*MAX_RECORDS_NUM;

	wstring szLimit;
	wchar_t szPos[255];
	swprintf(szPos,255,L"%d",start_index);
	szLimit=szPos;
	swprintf(szPos,255,L"%d",MAX_RECORDS_NUM);
	szLimit =szLimit+L","+szPos;

	DeleteContentRow();

	int colNum=(int)CGlobal::GetDBTableMap(m_Table).size();
	if(m_pSQLDriver->ReadTableSegmentWithCondition(m_Table,
		m_Condition.c_str(),szLimit.c_str(),colNum,&m_vGridContent))
	{
		int colindex=0;
		int rowindex=0;
		while(rowindex<(int)m_vGridContent.size())
		{
			SET_PB_POS(hWnd,rowindex);
			InsertBlanRow();
			for(colindex=0;colindex<colNum;colindex++)
			{
				if(colindex==0)
				{
					int rownum=m_RecordsCount-(m_CurPageIndex-1)*MAX_RECORDS_NUM-rowindex;
					TCHAR szIndex[255];
					_itow_s(rownum,szIndex,10);
					wstring szNum=szIndex;
					m_GridCtrl.SetItemText(m_PageRowIndex,0,szNum.c_str());			
				}else{
					LPCTSTR str=m_vGridContent[rowindex][colindex].c_str();
					m_GridCtrl.SetItemText(m_PageRowIndex,colindex,str);					
				}
			}		
			rowindex++;	
		}
		SET_PB_POS(hWnd,MAX_RECORDS_NUM);
		m_vGridContent.clear();
		Sleep(1000);
		UpdateDlgGridData();
		SetPageState();

		return TRUE;
	}

	return FALSE;

}

unsigned int  CSearchView::SearchDataBase(LPVOID lParam)
{
	CSearchView* pDiag=(CSearchView*)lParam;
	wstring szMatch;
	HWND hWnd=pDiag->GetSafeHwnd();
	int counter=0,start=0,step=0;
	if(WORD_MATCH==pDiag->pMatch->GetCheck())
		szMatch=L"like";
	else
		szMatch=L"regexp";

	map<wstring,wstring>mapCondition;
	map<wstring,wstring>::iterator iter;
	pDiag->GetSearchCondition(&mapCondition);
	if(mapCondition.size()>0)
	{
		//set condition
		iter=mapCondition.begin();
		pDiag->m_Condition=iter->first;
		pDiag->m_Condition=pDiag->m_Condition+L" "+szMatch+L"\""+iter->second+L"\"";
		iter++;
		while(iter!=mapCondition.end())
		{
			pDiag->m_Condition=pDiag->m_Condition+L" AND "+iter->first+L" "+szMatch+L"\""+iter->second+L"\"";
			iter++;
		}

		if(pDiag->m_pSQLDriver->ReadRecordsCountWithCondition(pDiag->m_Table,pDiag->m_Condition.c_str(),&pDiag->m_RecordsCount))
		{
			if(pDiag->m_RecordsCount>0)
			{
				(pDiag->m_RecordsCount%MAX_RECORDS_NUM)>0 ?pDiag->m_PageCont=1+pDiag->m_RecordsCount/MAX_RECORDS_NUM:pDiag->m_PageCont=pDiag->m_RecordsCount/MAX_RECORDS_NUM;
				pDiag->m_CurPageIndex=1;
				if(!pDiag->QueryShowRecords())::AfxMessageBox(L"查询失败！");
			
			}else
				::AfxMessageBox(L"无匹配记录！");
		}else
			::AfxMessageBox(L"查询记录数失败！");
	
	}else
		::AfxMessageBox(L"输入搜索条件！");
	Sleep(1000);

	pDiag->pProgCtrl->ShowWindow(SW_HIDE);

	return 0;


}


void CSearchView::OnTableSelChange()
{
	if(pTableSelect)
	{
		int index=pTableSelect->GetCurSel();
		if(m_Table!=index)
		{
			m_Table=index;
			SetDlgGridTitle(m_Table);
			SetSearchInput();
			ResetResultIndex();
		}
			
	}

}

void CSearchView::SetSearchInput()
{
	m_GridCtrl.InsertRow(L"输入搜索条件：");
	UpdateDlgGridData();

}

void CSearchView::AddResultGrid()
{
	int index=0;
	while(index<MAX_RECORDS_NUM)
	{
		m_GridCtrl.InsertRow(L"");
		index++;	
	}

}

BOOL CSearchView::SetDlgGridContent()
{
	SetDlgGridTitle(m_Table);
	SetSearchInput();
	return TRUE;


}

void CSearchView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_ADD_ORDER, m_GridCtrl); 
}

BOOL CSearchView::GetControlInfo(CWnd* pWnd, CWnd** pSafeWnd, CRect* pRect)
{
	HWND hwd=pWnd->GetSafeHwnd();
	*pSafeWnd = CWnd::FromHandle(hwd);
	CAddNewItem* pDlg = (CAddNewItem*) (*pSafeWnd)->GetParent();
	if (pDlg)
	{
		(*pSafeWnd)->GetWindowRect(*pRect);
		pDlg->ScreenToClient(*pRect);
		return TRUE;
	}
	return FALSE;
}

void CSearchView::UpdateControlPostion(CWnd* pWnd,CRect rect, LPARAM lParam)
{
	CSize* pTranslate = (CSize*) lParam;
	if((pWnd->GetDlgCtrlID()==IDC_BTN_FIRST) ||(pWnd->GetDlgCtrlID()==IDC_BTN_PREV) ||
		(pWnd->GetDlgCtrlID()==IDC_BTN_NEXT) ||(pWnd->GetDlgCtrlID()==IDC_BTN_LAST)||(pWnd->GetDlgCtrlID()==IDC_PAGE_PROG))
	{
		pWnd->MoveWindow(rect.left+pTranslate->cx/2, rect.top+pTranslate->cy, rect.Width(), rect.Height(), FALSE);
	
	}

	else
		pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top, rect.Width(), rect.Height(), FALSE);

}

void CSearchView::ReLayout()
{
	
	CWnd* pWnd;	
	CRect rect;
	
	//Relayout grid related
	if(GetControlInfo((CWnd*)&m_GridCtrl,&pWnd,&rect))
	{
		rect.bottom -=40;
		pWnd->MoveWindow(rect.left, rect.top,rect.Width(),rect.Height(), FALSE);

		rect.left =rect.left+rect.Width()/2-120;
		rect.top =rect.top+rect.Height()+10;
		rect.right =rect.left+40;
		rect.bottom=rect.top+20;
		pFirst=(CButton*)new CButton;
		pFirst->Create(L"首页",WS_CHILD|WS_VISIBLE,rect, (CWnd*)this,IDC_BTN_FIRST);
		CGlobal::SetControlTextFont((CWnd*)pFirst,&m_FontSmall,10);

		pPrev =(CButton*)new CButton;
		rect.left +=50;
		rect.right=rect.left+40;
		pPrev->Create(L"向前",WS_CHILD|WS_VISIBLE,rect, (CWnd*)this,IDC_BTN_PREV);
		pPrev->SetFont(&m_FontSmall);


		pPageProg=(CEdit*)new CEdit;
		rect.left +=50;
		rect.right=rect.left+40;
		pPageProg->Create(ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_READONLY | ES_CENTER,rect, (CWnd*)this, IDC_PAGE_PROG);
		pPageProg->SetFont(&m_FontSmall);
		pPageProg->SetWindowText(L"0/0");


		pNext =(CButton*)new CButton;
		rect.left +=50;
		rect.right=rect.left+40;
		pNext->Create(L"向后",WS_CHILD|WS_VISIBLE,rect, (CWnd*)this,IDC_BTN_NEXT);
		pNext->SetFont(&m_FontSmall);

		pLast =(CButton*)new CButton;
		rect.left +=50;
		rect.right=rect.left+40;
		pLast->Create(L"尾页",WS_CHILD|WS_VISIBLE,rect, (CWnd*)this,IDC_BTN_LAST);
		pLast->SetFont(&m_FontSmall);
	
	}
		
	//hide add button
	GetDlgItem(IDC_BTN_ORDER_ADD_ROW)->ShowWindow(SW_HIDE);
	//add search button
	pSearch=(CButton*)new CButton;
	if(GetControlInfo((CWnd*)pDel,&pWnd,&rect))
	{
		rect.top -=50;
		rect.bottom -=50;
		pSearch->Create(L"搜索",WS_CHILD|WS_VISIBLE,rect, (CWnd*)this,IDC_GRID_SEARCH);	
		CGlobal::SetControlTextFont((CWnd*)pSearch,&m_FontNormal,12);

		//add check box
		pMatch=(CButton*)new CButton;
		rect.top +=200;
		rect.bottom +=250;

		pMatch->Create(L"精确匹配",WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,rect,(CWnd*)this,IDC_CHECKBOX_MATCH);
		pMatch->SetFont(&m_FontSmall);

		//add combobox
		pTableSelect=(CComboBox*)new CComboBox;
		rect.left -=10;
		rect.right +=10;
		rect.top +=100;
		rect.bottom +=30;
		pTableSelect->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect, (CWnd*)this,IDC_COMBO_SELECT_TABLE);
		
		pTableSelect->InsertString(ORDER_ITEM,SELECT_ORDER);
		pTableSelect->InsertString(SAMPLE_ITEM,SELECT_SAMPLE);
		pTableSelect->InsertString(CUSTOMER_ITEM,SELECT_CUSTOMER);
		pTableSelect->InsertString(ORDER_DETAIL,SELECT_ORDER_DETAIL);
		pTableSelect->SetCurSel(ORDER_ITEM);
		pTableSelect->SetFont(&m_FontNormal);
	}


}
