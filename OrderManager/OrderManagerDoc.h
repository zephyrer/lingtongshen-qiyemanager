// OrderManagerDoc.h : COrderManagerDoc ��Ľӿ�
//


#pragma once


class COrderManagerDoc : public CDocument
{
protected: // �������л�����
	COrderManagerDoc();
	DECLARE_DYNCREATE(COrderManagerDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~COrderManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


