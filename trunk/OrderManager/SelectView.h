#pragma once


// CSelectView dialog



class CSelectView : public CDialog
{
	DECLARE_DYNAMIC(CSelectView)

public:
	CSelectView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectView();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//virtual void OnCancel();
	//virtual void OnClose();

private:
	CListBox* m_pViewSelect;
	CFont m_Font;

	DECLARE_MESSAGE_MAP()
};
