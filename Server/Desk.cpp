#include "Desk.h"



Desk::Desk()
{
	for (int i = 0; i < 100; i++)
	{
		mydesk[i].Num=0;
	}
	isClientwork = 1;
}


Desk::~Desk()
{
}
bool Desk::InitNetWork()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);//大厅ip 和 port
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// 创建socket
	clientSock = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSock < 0) { perror("sock error"); exit(-1); }
	// 连接服务端
	if (connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("connect error");
		exit(-1);
	}
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadProc, this, 0, 0);
	
	return true;
}
void Desk::UnInitNetWork()
{
	isClientwork = false;
	if (clientSock)
	{
		closesocket(clientSock);
		clientSock = NULL;
	}
	if (m_hThread)//关闭线程
	{
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 100))
			TerminateThread(m_hThread, -1);

		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	WSACleanup();


}
unsigned  __stdcall Desk::ThreadProc(void * lpvoid)
{
	//线程里收消息
	Desk *pthis = (Desk*)lpvoid;//强转类型
	char szbuf[BUF_SIZE] = { 0 };//定义缓冲区
	sockaddr_in addrClient;//网络地址
	int nSize = sizeof(addrClient);
	//int nRelRecvNum;//接收数据
	while (pthis->isClientwork)//init后
	{
		int ret = recv(pthis->clientSock, szbuf, BUF_SIZE, 0);

		// ret= 0 服务端关闭
		if (ret == 0) {
			cout<<"Server closed connection: %d\n"<<pthis->clientSock;
			closesocket(pthis->clientSock);
			pthis->isClientwork = 0;
		}

		if (ret >0)
		{
			STRU_DESK_RS *rs = (STRU_DESK_RS *)szbuf;
			
			//更新桌子
			pthis->UpdateDeskinfor(rs);
		}
		memset(szbuf, 0, sizeof(szbuf));
		return 0;
	}
	return 0;
}
void Desk::UpdateDeskinfor(STRU_DESK_RS *rs)
{
	for (int i = 0; i < 100;i++)
	{
		if(rs->desk[i].Num==0)
		{
			break;
		}
		//	)
		mydesk[i].Num = rs->desk[i].Num;
		memcpy(&mydesk[i].user[0].ip, rs->desk[i].user[0].ip, sizeof(mydesk[i].user[0].ip));
		memcpy(mydesk[i].user[0].name, rs->desk[i].user[0].name, sizeof(rs->desk[i].user[0]));
		mydesk[i].user[0].port = rs->desk[i].user[0].port;

		memcpy(&mydesk[i].user[1].ip, &rs->desk[i].user[1].ip, sizeof(rs->desk[i].user[1]));
		memcpy(&mydesk[i].user[1].name, &rs->desk[i].user[1].name, sizeof(rs->desk[i].user[1]));
		mydesk[i].user[1].port = rs->desk[i].user[1].port;

	}
}
int Desk::findUser(char *ip, int port)
{
	for (int i = 0; i < 100; i++)
	{
		if ((strcmp(mydesk[i].user[0].ip, ip)) == 0 && mydesk[i].user[0].port == port)
		{
			return i;
		}
		else if ((strcmp(mydesk[i].user[1].ip, ip)) == 0 && mydesk[i].user[0].port == port)
		{
			return i;
		}
		//if(mydesk[i].user[0].ip)
	}
	return -1;
}
U_Desk Desk::numtoip(int num)
{
	U_Desk usr;
	usr.Num = mydesk[num].Num;
	memcpy(usr.user[0].ip, mydesk[num].user[0].ip, sizeof(mydesk[num].user[0].ip));
	memcpy(usr.user[0].name, mydesk[num].user[0].name, sizeof(mydesk[num].user[0].name));
	usr.user[0].port = mydesk[num].user[0].port;
	memcpy(usr.user[1].ip, mydesk[num].user[1].ip, sizeof(mydesk[num].user[1].ip));
	memcpy(usr.user[1].name, mydesk[num].user[1].name, sizeof(mydesk[num].user[1].name));
	usr.user[1].port = mydesk[num].user[1].port;
	return usr;
}