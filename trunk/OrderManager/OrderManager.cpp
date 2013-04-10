// OrderManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "OrderManager.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "OrderManagerDoc.h"
#include "OrderManagerView.h"
#include "../Global/Global.h"
#include "../SQLDriver/SQLDriver.h"
#include "../ConfigParser/ConfigParser.h"
#include "AddNewItem.h"
#include "AddNewOrderItem.h"
#include "AddNewSampleItem.h"
#include "AddNewCustomerItem.h"
#include "SelectView.h"
#include "SearchView.h"
#include "SplashWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COrderManagerApp

BEGIN_MESSAGE_MAP(COrderManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COrderManagerApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_NEW, &COrderManagerApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	//add new order item
	ON_COMMAND(ID_BTN_ADD_NEW_ORDER, &COrderManagerApp::OnNewItemInsert)
	ON_COMMAND(ID_TOOLBAR_SEARCH, &COrderManagerApp::OnInfoSearch)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COrderManagerApp ����

COrderManagerApp::COrderManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_ProgStarted=FALSE;
}


// Ψһ��һ�� COrderManagerApp ����

COrderManagerApp theApp;



// COrderManagerApp ��ʼ��

BOOL COrderManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_OrderManagerTYPE,
		RUNTIME_CLASS(COrderManagerDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(COrderManagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CSplashWnd::EnableSplashScreen(TRUE);

	//set default log level--tracelog
	TCHAR szLogPath[MAX_PATH];
	ZeroMemory(szLogPath, MAX_PATH);
	if(0<::GetModuleFileName(NULL,szLogPath, _countof(szLogPath)))
	{
		std::wstring wsFilePathName=szLogPath;
		size_t pos=wsFilePathName.rfind(L"\\");
		if(0<pos)
		{
			wsFilePathName=wsFilePathName.substr(0,pos+1);
			wsFilePathName=wsFilePathName+LogFileName;
			gLogger.setLogPath(wsFilePathName);
			gLogger.setLogLevel(CMyLogger::toLoggerLevel(OM_LOG_ALL));
			//parse ini file
			S_LOG_DEBUG(L"parse ini file...");
			CConfigParser cfg;
			if(!cfg.parseConfigFile()) return FALSE;

			//get parameters for mysql connection
			g_dbp.sqlDBName=cfg.getSQLDBName();
			g_dbp.sqlURL=cfg.getSQLUrl();
			g_dbp.sqlUserName=cfg.getSQLUserName();
			g_dbp.sqlPWD=cfg.getSQLPwd();
			S_LOG_DEBUG(L"sql url:"<<g_dbp.sqlURL<<L",username:"<<g_dbp.sqlUserName<<L",password:"<<g_dbp.sqlPWD<<L",DB name:"<<g_dbp.sqlDBName);
			
			//set log level
			gLogger.setLogLevel(CMyLogger::toLoggerLevel(cfg.getLogLevel()));
		}
	}

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;




	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// �������ѳ�ʼ���������ʾ����������и���
	//pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	m_ProgStarted=TRUE;


	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void COrderManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void COrderManagerApp::OnInfoSearch()
{
	CAddNewItem* pSearch=new CSearchView();
	pSearch->DoModal();
	if(pSearch) delete pSearch;
}

void COrderManagerApp::OnNewItemInsert()
{
	CSelectView svDlg;
	CAddNewItem* pAddNewItem=NULL;
	if(IDOK==svDlg.DoModal())
	{
		switch(g_CurTableIndex)
		{
		case ORDER_ITEM:
			pAddNewItem=new CAddNewOrderItem();
			break;
		case SAMPLE_ITEM:
			pAddNewItem=new CAddNewSampleItem();
			break;
		case CUSTOMER_ITEM:
			pAddNewItem=new CAddNewCustomerItem();
			break;
		default:
			return;		
		}		
		pAddNewItem->DoModal();
	}
	if(pAddNewItem) delete pAddNewItem;
}

// COrderManagerApp ��Ϣ�������

void COrderManagerApp::OnFileNew()
{
	if(m_ProgStarted) 
	{
		CSelectView svDlg;
		if(IDOK==svDlg.DoModal()) CWinApp::OnFileNew();		
	}else
		CWinApp::OnFileNew();
}



BOOL COrderManagerApp::GetProgState()
{
	return m_ProgStarted;

}

