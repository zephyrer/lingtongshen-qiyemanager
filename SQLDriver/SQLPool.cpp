#include "stdafx.h"
#include "SQLPool.h"
#include "../ConfigParser/IniReader.h"

CSQLPool* CSQLPool::sqlPool=NULL;

//constructor
CSQLPool::CSQLPool(wstring url, wstring userName, wstring password, int maxSize)
{
	S_TRACE_FUNCTION;
	this->maxSize=maxSize;  
    this->curSize=0;  
    this->username=userName;  
    this->password=password;  
    this->url=url;  
    try{  
        this->driver=sql::mysql::get_driver_instance();  
    }  
    catch(SQLException& e)  
    {  
		S_LOG_FATAL(L"CSQLPool 驱动连接出错!"<<e.what());  
    }  
    catch(std::runtime_error& e)  
    {  
		S_LOG_FATAL(L"CSQLPool 运行出错了!"<<e.what());  
    }  
    this->InitConnection(maxSize/2); 

}

//get sql instance
CSQLPool* CSQLPool::GetInstance()
{
	if(NULL==sqlPool)
	{
		sqlPool=new CSQLPool(g_dbp.sqlURL,g_dbp.sqlUserName,g_dbp.sqlPWD,50);	
	}		
	return sqlPool;
}

//init pool connection
void CSQLPool::InitConnection(int iInitialSize)
{
	S_TRACE_FUNCTION;
	CAutoLock locker(&g_spLock);
	Connection* conn;
	for(int index=0;index<iInitialSize;index++)  
    {  
        conn=this->CreateConnection();  
        if(conn){  
            connList.push_back(conn);  
            ++(this->curSize);  
        }  
        else  
        {  
            S_LOG_FATAL(L"InitConnection 创建CONNECTION出错");  
        }  
    }  

}

//create connection
Connection* CSQLPool::CreateConnection()
{
	Connection* conn;
	std::string sURL=CUtil::ws2s(url);
	std::string sName=CUtil::ws2s(username);
	std::string sPWD=CUtil::ws2s(password);
    try{  
		conn=driver->connect(sURL,sName,sPWD);
        return conn;  
    }  
    catch(sql::SQLException& e)  
    {  
		S_LOG_FATAL(L"CreateConnection 创建连接出错"<<e.what());  
        return NULL;  
    }  
    catch(std::runtime_error& e)  
    {  
		S_LOG_FATAL(L"CreateConnection 运行时出错"<<e.what());  
        return NULL;  
    }  

}

//get connection from pool
Connection*  CSQLPool::GetConnection()
{
	S_TRACE_FUNCTION;
	CAutoLock locker(&g_spLock);
	Connection*  con;  
    if(connList.size()>0)//连接池容器中还有连接  
    {  
        con=connList.front();//得到第一个连接  
        connList.pop_front();//移除第一个连接  
        if(con->isClosed())//如果连接已经被关闭，删除后重新建立一个  
        {  
            delete con;  
            con=this->CreateConnection();  
        }  
        //如果连接为空，则创建连接出错  
        if(con==NULL)  
        {  
            --curSize;  
        }  
        return con;  
    } else{  
        if(curSize< maxSize)
		{
			//还可以创建新的连接  
            con= this->CreateConnection();  
            if(con){  
                ++curSize;  
                return con;  
            }  
            else{  
                return NULL;  
            }  
        }else{//建立的连接数已经达到maxSize   
            return NULL;  
        }  
    }  
}

//recycle conection
void CSQLPool::ReleaseConnection(Connection *conn)
{
	if(conn){  
        CAutoLock locker(&g_spLock); 
        connList.push_back(conn);  
    }  

}

//
CSQLPool::~CSQLPool()
{
	this->DestoryConnPool(); 

}


//destroy one connection
void CSQLPool::DestoryConnection(Connection *conn)
{
	if(conn)  
    {  
        try{  
            conn->close();  
        }  
        catch(sql::SQLException&e)  
        {  
            S_LOG_FATAL(L"DestoryConnection 销毁connection出错！"<<e.what());  
        }  
        catch(std::exception&e)  
        {  
            S_LOG_FATAL(L"DestoryConnection 销毁connection出错！"<<e.what());  
        }  
        delete conn;  
    }  
}

//destroy all connections

void CSQLPool::DestoryConnPool()
{
	CAutoLock locker(&g_spLock);
	list<Connection*>::iterator icon;  
    for(icon=connList.begin();icon!=connList.end();++icon)  
    {  
        this->DestoryConnection(*icon);//销毁连接池中的连接  
    }  
    curSize=0;  
    connList.clear();//清空连接池中的连接   

}