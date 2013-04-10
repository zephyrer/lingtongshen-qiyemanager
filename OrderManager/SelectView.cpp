// SelectView.cpp : implementation file
//

#include "stdafx.h"
#include "OrderManager.h"
#include "SelectView.h"
#include "../Global/Global.h"



// CSelectView dialog

IMPLEMENT_DYNAMIC(CSelectView, CDialog)

CSelectView::CSelectView(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectView::IDD, pParent)
{
      m_pViewSelect=NULL;
}

CSelectView::~CSelectView()
{
	if(m_pViewSelect) delete m_pViewSelect;
}

void CSelectView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CSelectView::OnInitDialog()
{
	if(NULL==m_pViewSelect)
	{
		m_pViewSelect=new CListBox;
		m_pViewSelect->Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL, CRect(10,10,210,100), (CWnd*)this, IDC_LISTBOX_VIEW_SELECT);		
		m_pViewSelect->AddString(SELECT_ORDER);
		m_pViewSelect->AddString(SELECT_SAMPLE);
		m_pViewSelect->AddString(SELECT_CUSTOMER);		
		m_pViewSelect->SetCurSel(0);
		CGlobal::SetControlTextFont((CWnd*)m_pViewSelect,&m_Font,13);
		return TRUE;		
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(CSelectView, CDialog)
END_MESSAGE_MAP()


// CSelectView message handlers
void CSelectView::OnOK()
{
	g_CurTableIndex=m_pViewSelect->GetCurSel();
	EndDialog(IDOK);

}


