// OrderManagerDoc.cpp : COrderManagerDoc ���ʵ��
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


// COrderManagerDoc ����/����

COrderManagerDoc::COrderManagerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

COrderManagerDoc::~COrderManagerDoc()
{
}

BOOL COrderManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
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




// COrderManagerDoc ���л�

void COrderManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// COrderManagerDoc ���

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


// COrderManagerDoc ����
