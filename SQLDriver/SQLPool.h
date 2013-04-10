 /*  
         *File: SQLPool.h  

*/  
#ifndef _SQLPOOL_H  
#define _SQLPOOL_H  

#include "../Global/Global.h"
using namespace std;  
using namespace sql;  


  
class CSQLPool{  
public:  
	~CSQLPool();  
	Connection* GetConnection();//获得数据库连接  
	void ReleaseConnection(Connection *conn);//将数据库连接放回到连接池的容器中  
	static CSQLPool* GetInstance();//获取数据库连接池对象  

private:
	Connection* CreateConnection();//创建一个连接  
	void InitConnection(int iInitialSize);//初始化数据库连接池  
	void DestoryConnection(Connection *conn);//销毁数据库连接对象  
	void DestoryConnPool();//销毁数据库连接池  
	CSQLPool(wstring url,wstring user,wstring password,int maxSize);//构造方法  


private:  
	int curSize;//当前已建立的数据库连接数量  
	int maxSize;//连接池中定义的最大数据库连接数  
	wstring username;  
	wstring password;  
	wstring url;  
	list<Connection*>connList;//连接池的容器队列  
	static CSQLPool *sqlPool; 	 
	Driver* driver; 
#if 0
	class CGarbo // 删除CSQLPool的实例 
    {
		public:
			~CGarbo();


    };
    static CGarbo Garbo;
#endif

};  
#endif  /*_SQLPOOL_H */  