// OrderManagerView.h : COrderManagerView ��Ľӿ�
//
#pragma once
#include "OrderManagerDoc.h"
#include "GridView.h"

class COrderManagerView : public CView
{
protected: // �������л�����
	COrderManagerView();
	DECLARE_DYNCREATE(COrderManagerView)

// ����
public:
	COrderManagerDoc* GetDocument() const;
	CGridView* m_pGridView;
	CWinThread *pLoadThread;
	CProgressCtrl *pProgBar;
	
// ����
public:
	void OnInitGrid();


// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	
// ʵ��
public:
	virtual ~COrderManagerView();
	static unsigned int LoadGridView(LPVOID lParam);
	void OnInitProgressBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnToggleReadonly();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnRefresh(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnLoadMsg(WPARAM wp,LPARAM lp);
	

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

private:

	
};

#ifndef _DEBUG  // OrderManagerView.cpp �еĵ��԰汾
inline COrderManagerDoc* COrderManagerView::GetDocument() const
   { return reinterpret_cast<COrderManagerDoc*>(m_pDocument); }
#endif

