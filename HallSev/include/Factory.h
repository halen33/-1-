#pragma once
#include"Room.h"
#include"MyDB.h"
#include<list>
class CFactory;
typedef void (CFactory::*PFUN)(int, char*);
//协议映射表
struct ProtocolMap
{
	PackdefType m_nType;
	PFUN     m_pfun;
};
struct UserGame{
	User*user;
	char GameNum[20];
	int DeskNum;

};
struct GameInfo{
	int m_socket;
	char id[16];
	int port;
};
class CFactory
{
public:
	CFactory();
	virtual ~CFactory();
public:
	CMyDB* m_db;
	vector<Game >game;
	list<int>m_Sock;//需要发送桌子情况的socket
	map<string,GameInfo*>m_GameToAddr;
	map<int,UserGame* >m_SockToUser;//在sitdown请求中获取人的信
	map<string, CRoom*> m_GameToRoom;
public:
	BOOL AddGame();
	//void MatchUser(User user);
	static void* SendDeskProc(void* lpParameter);
public:
	void DealTheGame(int sockfd, char * buf);
	void DealLoginReq(int sockfd, char * buf);
	void DealSitDownReq(int sockfd, char * buf);
	void DealGameList(int sockfd, char * buf);
	void DealAllGameList(int sockfd, char * buf);
	void DealGameRoom(int sockfd, char * buf);
	void DealReadyRq(int sockfd,char*buf);
	void DealGameMsg(int sockfd, char*buf);
	void DealData(char * szbuf,int sockfd);
};

