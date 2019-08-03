#pragma once
#include"res.h"
#include"sys.h"
#include<vector>
#include<map>
#include<sys/time.h>
struct MyUser{
	 User*user;
	 struct timeval tv;
	 int Level;//分三级
};
class CRoom
{
public:
	CRoom();
	virtual ~CRoom();
public:
 	User uu;
	U_GameInfo u;
	U_Desk desk;
	int OneDeskUserNum;
	vector<U_Desk> m_Desk;
//	map<int  ,MyUser*>m_SockToMyUser;
//	map<int ,User*>m_SockToUserroom;
public:
	int  AddDesk();
	BOOL UpDataDesk(Desk desk);
	bool IsCanSit(int DeskNum);
	int MatchDesk(int s,User *u);
	static void *MatchThreadFun(void *arg);
	
};

