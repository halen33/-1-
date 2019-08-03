#pragma once
#include<vector>
#include"Room.h"
#include"res.h"
#include<map>
#include "hiredis.h"
class CMyDB
{
public:
	CMyDB();
	virtual ~CMyDB();
private:
	int Connect(const char *ip, int port);
public:
	int AddGame(Game game);
	int DelGame(char* GameNum);
	
	int UpDataGame(char* GameNum,char* Sr,char*des);
	int GetGameList(vector<Game>&GameList);
	
	int GetScore(char* id,char*gameNum);
	int UpdateScore(char*id,char*gamenum,int score);
//int DelScore(char*id,char*gamenum);
public:
	redisContext *conn;
	char* m_GameStr;

};

