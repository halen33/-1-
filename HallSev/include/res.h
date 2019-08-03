#ifndef _PACKDEF_H
#define _PACKDEF_H 

#include<list>
#include <string>
#include<iostream>
using namespace std;


typedef  int BOOL;
#define  DWORD int
//边界值
#define _DEF_PORT 1234    
#define _DEF_NAME_SIZE  64       //用户名长度
#define _DEF_PASSWORD_SIZE  64
#define _DEF_ID_SIZE   64             //用户id
#define _DEF_GAME_NUM  100    //游戏数
#define _DEF_DESK_NUM  100     //桌子数
#define _DEF_SIZE 4096       
#define _DEF_BUFFERSIZE  2000*1024   
#define _MAX_PROTOCOL_LEN 100    //协议长度

#define SEND_TIMES 3//每1秒发一次人数消息

#define LOGIN_SEV_IP "192.168.43.9"
#define LOGIN_SEV_PORT 4000
#define BUF_SIZE 4096


//#define _MAX_PATH_LEN  100
//#define _MAX_NAME_LEN  100
//#define _DEF_STREAMSIZE  1024/**1024*/


//协议包类型

#define _DEF_PROTOCOL_BASE  100
#define _DEF_PROTOCOL_ONLINE_RQ                 _DEF_PROTOCOL_BASE  +0    //登录
#define _DEF_PROTOCOL_ONLINE_RS                  _DEF_PROTOCOL_BASE  +1
#define _DEF_PROTOCOL_OFFLINE_RQ                _DEF_PROTOCOL_BASE  +2    //下线
#define  _DEF_PROTOCOL_REGISTER_RQ              _DEF_PROTOCOL_BASE  +3     //注册
#define  _DEF_PROTOCOL_REGISTER_RS               _DEF_PROTOCOL_BASE  +4

#define _DEF_PROTOCOL_LOGINHALL_RQ  _DEF_PROTOCOL_BASE+8
#define _DEF_PROTOCOL_LOGINHALL_RS  _DEF_PROTOCOL_BASE+9
#define _DEF_PROTOCOL_CONNECTHALL_RQ      _DEF_PROTOCOL_BASE+8   //连接大厅请求
#define _DEF_PROTOCOL_CONNECTHALL_RS       _DEF_PROTOCOL_BASE+9    //连接大厅回应

#define  _DEF_PROTOCOL_GAMELIST_RS	             _DEF_PROTOCOL_BASE +5    //游戏列表回复
#define _DEF_PROTOCOL_GAMECHOOSE_RQ	     _DEF_PROTOCOL_BASE +6    //游戏选择请求
#define _DEF_PROTOCOL_GAMECHOOSE_RS	     _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_HALLINFO _DEF_PROTOCOL_BASE+100    //登录
#define _DEF_PROTOCOL_SITDOWN_RQ           _DEF_PROTOCOL_BASE +20
#define _DEF_PROTOCOL_SITDOWN_RS   _DEF_PROTOCOL_BASE +21//对用户选择桌子的回复  满了就回复不能坐，能做就把位置发给用户,并对本桌其他人发送本桌信息
#define _DEF_PROTOCOL_PARTNET_RS   _DEF_PROTOCOL_BASE +22//回复桌子上人的信息,若此桌子上加入了人我就重新把本桌信息发过去
#define _DEF_PROTOCOL_GAMEREADY_RQ   _DEF_PROTOCOL_BASE +23//请求准备
#define _DEF_PROTOCOL_GAMEREADY_RS  _DEF_PROTOCOL_BASE +24//继续等待或者开始
#define _DEF_PROTOCOL_GAMEOVER_RQ  _DEF_PROTOCOL_BASE +25//
#define _DEF_PROTOCOL_GAMEOVER_RS  _DEF_PROTOCOL_BASE +26

#define _DEF_PROTOCOL_GAMEMSG_RQ  _DEF_PROTOCOL_BASE +27//要转发的消息
#define _DEF_PROTOCOL_GAMEMSG_RS  _DEF_PROTOCOL_BASE +28//转发给你的消息





//协议包

#define PackdefType int
//上线请求 上线回复 下线请求

struct User  //用户结构体
{
	int m_userId;
	char m_userName[_DEF_NAME_SIZE];
	int score;
	int IsReady;
};

struct Desk  //桌子结构体
{
	int Num;//桌子上现在坐得人
	User user[20];
};



enum USERSTATE {
	PLAY,//游戏状态 
	WAIT,//等待状态   即刚刚坐上桌子对面还没人
	READY,//准备状态  客户端准备
	UNKNOWN//未知状态
};
//坐在桌子上的人全部信息
 struct U_GameInfo
 {
//	char m_IP[16]//IP
//	int m_uPort;//游戏端口
	int  m_socket;
	USERSTATE m_state;//状态
	User user;
 };
struct U_Desk{
	int Num;
	U_GameInfo user[10];
};



typedef struct STRU_LOGINHALL_RQ
{
	PackdefType m_nType;
	User user;
} STRU_CONNECT_HALL;
typedef struct STRU_LOGINHALL_RS
{
	PackdefType m_nType;
	int IfSuccess;
}STRU_CONNECT_HALL_RS;

struct STRU_HALL_INFO
{
	PackdefType m_nType;
	int m_ManNum;
	int port;
};

enum sit_method { match, chose, newdesk };
struct STRU_SITDOWN_RQ
{
	PackdefType m_nType;
	sit_method m_method;
	int DeskNum;//-1

};

struct STRU_SIT_POST
{
		PackdefType m_nType;
		int m_Result;//  0/1
		int DeskNum;
};
struct Game{
	int playnum;
	char GameNum[20];
	char GameName[64];
	char GameInfo[4096];

};
struct STRU_HALL_GAMELIST_RS  //游戏列表回复包
{
	PackdefType m_nType;
	Game	m_szGameList[5];
	int GameCount;
};

struct STRU_GAMECHOOSE_RQ
{
	PackdefType m_nType;
	char GameNum[20];
}; 

struct STRU_GAMECHOOSE_RS
{
	PackdefType m_nType;
	Desk desk[_DEF_DESK_NUM];
};
struct STRU_PARTNET_RS//回复桌子上人的信息,若此桌子上加入了人我就重新把本桌信息发过去
{
	PackdefType m_nType;
	Desk desk;
};
struct STRU_GAMEREADY_RQ  //请求准备
{
	PackdefType m_nType;

};
struct STRU_GAMEREADY_RS
{
	PackdefType m_nType;
	User *user;
	int usernum;
	
};
//转发消息 客户端发来的
struct STRU_GAMEMSG_RQ{
	PackdefType m_nType;
	int ToUser;//0-server  1-cli
	char Msg[4096];
	int MsgLen;
};
//我发给服务器，服务器回复，我发给客户端
typedef struct STRU_GAMEMSG_RS{
	PackdefType m_nType;
	int m_socket;
	char Msg[4096];
	int MsgLen;
}STRU_GAMEMSG_TOSEV_RQ,STRU_GAMEMSG_TOSEV_RS;

//STRU_GAMEOVER_RQ
//STRU_GAMEOVER_RS

#endif
