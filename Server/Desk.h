#include <Winsock2.h>
#include<iostream>

#include <process.h> 
#include"packdef.h"
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
class Desk
{
public:
	Desk();
	~Desk();
	bool InitNetWork();
	void UnInitNetWork();
public:

	//bool SendData(long lSendIp, char *szbuf, int nlen);//发送数据(发给服务器)
public:
	static  unsigned  __stdcall ThreadProc(void *);//创建线程函数 用来收消息 更新桌子
	void UpdateDeskinfor(STRU_DESK_RS *rs);
	int findUser(char *ip, int port);
	U_Desk numtoip(int num);
private:
	SOCKET clientSock;
	bool isClientwork;
	HANDLE m_hThread;//收消息
	
	U_Desk mydesk[100];
	
};
