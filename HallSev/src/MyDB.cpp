#include "MyDB.h"
#include<cstring>
#include<string.h>
#include<stdlib.h>
using namespace std;
CMyDB::CMyDB()
{
	conn = NULL;
	Connect("127.0.0.1",6379);
}


CMyDB::~CMyDB()
{
	if(conn)
	redisFree(conn);
}

int CMyDB::Connect(const char * ip, int port)
{
	conn = redisConnect("127.0.0.1", 6379);
	if (conn != NULL && conn->err)
	{
		printf("redis connect fail Error: %s\n", conn->errstr);
		return 0;
		// handle error
	}
	return 1;

}

int CMyDB::AddGame(Game game)
{//wenti
		char GameNum[20];
		strcpy(GameNum,game.GameNum);
		char GameStr[64];
		strcpy(GameStr,game.GameName);
		char GameInfo[4096];
		strcpy(GameInfo,game.GameInfo);
	printf("addgame start\n");



	redisReply *reply;
	reply =(redisReply*) redisCommand(conn, "SADD GameList  %s", GameNum);//如果集合中有就不加，没有就加入
	if(reply==NULL)
		return 0;
	freeReplyObject(reply);
	printf("rpush end\n");
	//链接字符串 playnum@@gamestr@@gameinfo
	char s[1024];
	sprintf(s, "%d",game.playnum);
//	char str1[1024];
//	strcpy(str1,GameStr);
	char* str="@@";
	strcat(s,str);
	strcat(s,GameStr);
	strcat(s,str);
	strcat(s,GameInfo);
	printf("add str??? %s\n",s);
	redisReply *re;
	re =(redisReply*) redisCommand(conn, "SET %s %s",GameNum, s);//设置游戏信息string-string结构
	if(re==NULL)
		return 0;
	freeReplyObject(re);
	return 1;
}

int CMyDB::DelGame(char* GameNum)
{
	redisReply*reply = (redisReply*)redisCommand(conn, "SREM GameList %s", GameNum);//从集合中删除

	freeReplyObject(reply);
	redisReply*re =(redisReply*)redisCommand(conn,"DEL %s",GameNum);//string-string删除
	freeReplyObject(re);
	return 1;
}

int CMyDB::UpDataGame(char* gamenum,char * GameStr,char*gameinfo)
{
	Game game;
	strcpy(game.GameNum,gamenum);
	strcpy(game.GameName,GameStr);
	strcpy(game.GameInfo,gameinfo);
	//先删掉gamenum
	//再加新的
	if(DelGame(gamenum)&&AddGame(game)==0){
	printf("UpDataGame fail\n");
	return 0;
	}
	return 1;
}

int CMyDB::GetGameList(vector<Game>&GameList)
{
	printf("get game start\n");
	redisReply*	reply = (redisReply*)redisCommand(conn,"SMEMBERS GameList");//列出集合所有信息
	if (reply->type != REDIS_REPLY_ARRAY) {
		printf("Failed to execute command.\n");
		freeReplyObject(reply);
		redisFree(conn);
		return 0;
	}
	//放入参数
	int j;
	char str2[4096];
	Game game;
	for (j = 0; j < reply->elements; ++j) {
		printf("==========================================\n");
		printf("gamenum :%s\n",reply->element[j]->str);
		strncpy(game.GameNum,reply->element[j]->str,strlen(reply->element[j]->str));


		redisReply*re = (redisReply*)redisCommand(conn, "get %s", reply->element[j]->str);

		if(re->str==NULL){
		printf("get info fail\n");
		}
		strcpy(str2,re->str);
//分割字符串
		char list[3][4096]={""};

		char seg[]="@@";
		char* to=strtok(str2,seg);
		int i=0;
		while(to!=NULL){

			strcpy(list[i],to);
			i++;

		printf("%s\n",to);
		to=strtok(NULL,"@@");

		}
		int n=atoi(list[0]);
		game.playnum=n;
		strcpy(game.GameName,list[1]);
		strcpy(game.GameInfo,list[2]);

		printf("%d----------to\n",game.playnum);
		printf("%s----------to\n",game.GameName);
		printf("%s----------to\n",game.GameInfo);
		printf("%d\n",j);
		GameList.push_back(game);
		freeReplyObject(re);
	}
	freeReplyObject(reply);
}

int CMyDB::GetScore(char* id,char*GameNum){
	//hash

	redisReply*reply = (redisReply*)redisCommand(conn, "HGET %s %s",id, GameNum);//get key value
	int i;
	i=atoi(reply->str);
	freeReplyObject(reply);
	return i;

}
int CMyDB::UpdateScore(char*id,char*gamenum,int score){

	redisReply*reply = (redisReply*)redisCommand(conn, "HSET %s %s %d",id, gamenum,score);//设置信息
	freeReplyObject(reply);
return 1;
}

