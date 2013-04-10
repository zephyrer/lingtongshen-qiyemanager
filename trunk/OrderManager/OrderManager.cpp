// OrderManager.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_NEW, &COrderManagerApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	//add new order item
	ON_COMMAND(ID_BTN_ADD_NEW_ORDER, &COrderManagerApp::OnNewItemInsert)
	ON_COMMAND(ID_TOOLBAR_SEARCH, &COrderManagerApp::OnInfoSearch)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COrderManagerApp 构造

COrderManagerApp::COrderManagerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_ProgStarted=FALSE;
}


// 唯一的一个 COrderManagerApp 对象

COrderManagerApp theApp;



// COrderManagerApp 初始化

BOOL COrderManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_OrderManagerTYPE,
		RUNTIME_CLASS(COrderManagerDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
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

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;




	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 主窗口已初始化，因此显示它并对其进行更新
	//pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	m_ProgStarted=TRUE;


	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
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

// COrderManagerApp 消息处理程序

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

