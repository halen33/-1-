#ifndef _PACKDEF_H
#define _PACKDEF_H 
#include<list>
#include <string>
#include<iostream>
#include <vector>
#include <string.h>
using namespace std;
#define BUF_SIZE 4096
#define _DEF_NAME_SIZE 1024

#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 5000
#define _DEF_PROTOCOL_BASE  100

#define _DEF_PROTOCOL_DESK_RS     _DEF_PROTOCOL_BASE  +1
typedef char PackdefType ;

struct User  //用户结构体
{
	char ip[16];
	int port;
	char name[16];
};
enum USERSTATE {
	PLAY,//游戏状态 
	WAIT,//等待状态   即刚刚坐上桌子对面还没人
	READY,//准备状态  客户端准备
	UNKNOWN//未知状态
};
//坐在桌子上的人全部信息

struct U_Desk {
	int Num;//桌上的人
	User user[2];
};
typedef struct STRU_DESK_RS
{
	PackdefType m_nType;
	U_Desk desk[100];//-1

};
#endif