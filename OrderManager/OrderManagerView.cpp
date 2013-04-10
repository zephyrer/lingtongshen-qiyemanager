// OrderManagerView.cpp : COrderManagerView 类的实现
//

#include "stdafx.h"
#include "OrderManager.h"
#include "OrderManagerDoc.h"
#include "OrderManagerView.h"
#include "SelectView.h"
#include "../Global/Global.h"
#include "ShowOrderGridView.h"
#include "ShowSampleGridView.h"
#include "ShowCustomerGridView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COrderManagerView

IMPLEMENT_DYNCREATE(COrderManagerView, CView)

BEGIN_MESSAGE_MAP(COrderManagerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_MSG_REFRESH,&COrderManagerView::OnRefresh)
	ON_MESSAGE(WM_USER_MSG_LOADVIEW,&COrderManagerView::OnLoadMsg)

	//ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
END_MESSAGE_MAP()

// COrderManagerView 构造/析构

COrderManagerView::COrderManagerView()
{
	// TODO: 在此处添加构造代码
	m_pGridView=NULL;
}

COrderManagerView::~COrderManagerView()
{
	if(m_pGridView)
		delete m_pGridView;
	if(pProgBar)
		delete pProgBar;
}


LRESULT COrderManagerView::OnRefresh(WPARAM wp,LPARAM lp)
{
	m_pGridView->m_GridViewData.clear();
	m_pGridView->SetTitle();
	BOOL rVal=m_pGridView->SetContent();
	if(!rVal) 
		S_LOG_FATAL(L"refresh content fail!,table index:"<<m_pGridView->GetTableIndex());
	m_pGridView->UpdateGridData();	
	return 1L;
}

LRESULT COrderManagerView::OnLoadMsg(WPARAM wp,LPARAM lp)
{
	pProgBar->ShowWindow(SW_SHOW);
	pProgBar->SetPos((int)wp);
	return 1L;

}

BOOL COrderManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COrderManagerView 绘制

void COrderManagerView::OnDraw(CDC* /*pDC*/)
{
	COrderManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// COrderManagerView 打印

BOOL COrderManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COrderManagerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加额外的打印前进行的初始化过程
	S_TRACE_FUNCTION;
	if (m_pGridView)
		m_pGridView->OnBeginPrinting(pDC,pInfo);
}

void COrderManagerView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加打印后进行的清除过程
	S_TRACE_FUNCTION;
	if (m_pGridView)
		m_pGridView->OnEndPrinting(pDC,pInfo);
}

void COrderManagerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	S_TRACE_FUNCTION;
	if (m_pGridView)
		m_pGridView->OnPrint(pDC,pInfo);

}

void COrderManagerView::OnInitGrid()
{
	//init grid view
	S_TRACE_FUNCTION;
	OnInitProgressBar();
	CRect rect;
	GetClientRect(rect);
	if(NULL==m_pGridView)
	{
		switch(g_CurTableIndex)
		{
		case ORDER_ITEM:
			m_pGridView=new CShowOrderGridView(this,rect);
			break;
		case SAMPLE_ITEM:
			m_pGridView=new CShowSampleGridView(this,rect);
			break;
		case CUSTOMER_ITEM:
			m_pGridView=new CShowCustomerGridView(this,rect);
			break;
		default:
			return;		
		}

		if(!m_pGridView->InitGrid())
		{
			::AfxMessageBox(L"初始化失败，请重试！");
		}

	}
}

void COrderManagerView::OnInitProgressBar()
{
	//make it in centre
	pProgBar=(CProgressCtrl*) new CProgressCtrl;
	CRect wndRect, prgRect;
	GetClientRect(wndRect);
	prgRect.left=wndRect.left+wndRect.Width()/2-100;
	prgRect.right=wndRect.right-wndRect.Width()/2+100;
	prgRect.top=wndRect.top+wndRect.Height()/2-10;
	prgRect.bottom=wndRect.bottom-wndRect.Height()/2+10;

	//create progress bar, hide by default	
	pProgBar->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,prgRect, (CWnd*)this,IDC_LOADVIEW_PROG_BAR);
	pProgBar->ShowWindow(SW_HIDE);
	pProgBar->SetRange(0,MAX_RECORDS_NUM);
}

unsigned int COrderManagerView::LoadGridView(LPVOID lParam)
{
	COrderManagerView* pView=(COrderManagerView*)lParam;

	pView->OnInitGrid();
#if 0
	SET_PB_LOAD_POS(pView,10);

	if(!pView->m_pGridView->InitGrid())
	{
		::AfxMessageBox(L"初始化失败，请重试！");
	}
	SET_PB_LOAD_POS(pView,20);
	pView->pProgBar->ShowWindow(SW_HIDE);
	
#endif
	pView->RedrawWindow();
	return 0;

}


void COrderManagerView::OnInitialUpdate() 
{
	S_TRACE_FUNCTION;
	CView::OnInitialUpdate();
	//init grid
	OnInitGrid();
#if 0
	pLoadThread=AfxBeginThread(LoadGridView,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	pLoadThread->m_bAutoDelete=FALSE;
	pLoadThread->ResumeThread();
#endif

}

void COrderManagerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(rect);
	m_pGridView->OnGridViewSize(rect);	

}

BOOL COrderManagerView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_pGridView->OnGridCMDMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void COrderManagerView::OnToggleReadonly()
{
    m_pGridView->OnGridToggleReadonly();
}

BOOL COrderManagerView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}



// COrderManagerView 诊断

#ifdef _DEBUG
void COrderManagerView::AssertValid() const
{
	CView::AssertValid();
}

void COrderManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COrderManagerDoc* COrderManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COrderManagerDoc)));
	return (COrderManagerDoc*)m_pDocument;
}


#endif //_DEBUG


// COrderManagerView 消息处理程序
