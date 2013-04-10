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
	Connection* GetConnection();//������ݿ�����  
	void ReleaseConnection(Connection *conn);//�����ݿ����ӷŻص����ӳص�������  
	static CSQLPool* GetInstance();//��ȡ���ݿ����ӳض���  

private:
	Connection* CreateConnection();//����һ������  
	void InitConnection(int iInitialSize);//��ʼ�����ݿ����ӳ�  
	void DestoryConnection(Connection *conn);//�������ݿ����Ӷ���  
	void DestoryConnPool();//�������ݿ����ӳ�  
	CSQLPool(wstring url,wstring user,wstring password,int maxSize);//���췽��  


private:  
	int curSize;//��ǰ�ѽ��������ݿ���������  
	int maxSize;//���ӳ��ж����������ݿ�������  
	wstring username;  
	wstring password;  
	wstring url;  
	list<Connection*>connList;//���ӳص���������  
	static CSQLPool *sqlPool; 	 
	Driver* driver; 
#if 0
	class CGarbo // ɾ��CSQLPool��ʵ�� 
    {
		public:
			~CGarbo();


    };
    static CGarbo Garbo;
#endif

};  
#endif  /*_SQLPOOL_H */  