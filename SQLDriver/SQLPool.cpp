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
		S_LOG_FATAL(L"CSQLPool �������ӳ���!"<<e.what());  
    }  
    catch(std::runtime_error& e)  
    {  
		S_LOG_FATAL(L"CSQLPool ���г�����!"<<e.what());  
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
            S_LOG_FATAL(L"InitConnection ����CONNECTION����");  
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
		S_LOG_FATAL(L"CreateConnection �������ӳ���"<<e.what());  
        return NULL;  
    }  
    catch(std::runtime_error& e)  
    {  
		S_LOG_FATAL(L"CreateConnection ����ʱ����"<<e.what());  
        return NULL;  
    }  

}

//get connection from pool
Connection*  CSQLPool::GetConnection()
{
	S_TRACE_FUNCTION;
	CAutoLock locker(&g_spLock);
	Connection*  con;  
    if(connList.size()>0)//���ӳ������л�������  
    {  
        con=connList.front();//�õ���һ������  
        connList.pop_front();//�Ƴ���һ������  
        if(con->isClosed())//��������Ѿ����رգ�ɾ�������½���һ��  
        {  
            delete con;  
            con=this->CreateConnection();  
        }  
        //�������Ϊ�գ��򴴽����ӳ���  
        if(con==NULL)  
        {  
            --curSize;  
        }  
        return con;  
    } else{  
        if(curSize< maxSize)
		{
			//�����Դ����µ�����  
            con= this->CreateConnection();  
            if(con){  
                ++curSize;  
                return con;  
            }  
            else{  
                return NULL;  
            }  
        }else{//�������������Ѿ��ﵽmaxSize   
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
            S_LOG_FATAL(L"DestoryConnection ����connection����"<<e.what());  
        }  
        catch(std::exception&e)  
        {  
            S_LOG_FATAL(L"DestoryConnection ����connection����"<<e.what());  
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
        this->DestoryConnection(*icon);//�������ӳ��е�����  
    }  
    curSize=0;  
    connList.clear();//������ӳ��е�����   

}