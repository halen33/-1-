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

	//bool SendData(long lSendIp, char *szbuf, int nlen);//��������(����������)
public:
	static  unsigned  __stdcall ThreadProc(void *);//�����̺߳��� ��������Ϣ ��������
	void UpdateDeskinfor(STRU_DESK_RS *rs);
	int findUser(char *ip, int port);
	U_Desk numtoip(int num);
private:
	SOCKET clientSock;
	bool isClientwork;
	HANDLE m_hThread;//����Ϣ
	
	U_Desk mydesk[100];
	
};
