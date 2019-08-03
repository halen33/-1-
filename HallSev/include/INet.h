#pragma once
#include"sys.h"
#include"res.h"
#include"Factory.h"
class CINet
{

public:
	CINet();
	virtual ~CINet();
	BOOL InitSock();
	void GetIp();
	void addfd(int epfd, int fd);
	void  RecvProc();
	static void* SendNumProc(void* lpParameter);
public:
	char *m_ip;
	int OnlineNum;
	CFactory* m_fac;
private:
	int socklogin;
	int sockCli;
	struct sockaddr_in loginaddr;
};

