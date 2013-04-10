// OrderManagerDoc.cpp : COrderManagerDoc 类的实现
//

#include "stdafx.h"
#include "OrderManager.h"
#include "OrderManagerDoc.h"
#include "SelectView.h"
#include "../Global/Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COrderManagerDoc

IMPLEMENT_DYNCREATE(COrderManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(COrderManagerDoc, CDocument)
END_MESSAGE_MAP()


// COrderManagerDoc 构造/析构

COrderManagerDoc::COrderManagerDoc()
{
	// TODO: 在此添加一次性构造代码

}

COrderManagerDoc::~COrderManagerDoc()
{
}

BOOL COrderManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
    wchar_t   message[250]; 
	switch(g_CurTableIndex)
	{
	case ORDER_ITEM:
		wsprintf(message,L"%s",SELECT_ORDER); 
		break;
	case SAMPLE_ITEM:
		wsprintf(message,L"%s",SELECT_SAMPLE);  
		break;
	case CUSTOMER_ITEM:
		wsprintf(message,L"%s",SELECT_CUSTOMER);  
		break;
	default:
		wsprintf(message,L"%s",L"None"); 
		break;		
	}
    SetTitle(message); 
	return TRUE;
}




// COrderManagerDoc 序列化

void COrderManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// COrderManagerDoc 诊断

#ifdef _DEBUG
void COrderManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COrderManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COrderManagerDoc 命令
