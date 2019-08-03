#include "Factory.h"
#include"res.h"


CFactory::CFactory()
{
	m_db=new CMyDB();
	game.clear();
	m_db->GetGameList(game);
	for(int i=0;i<game.size();i++){
		printf("game list [%d] = %s\n",i,game[i].GameNum);
		string s=game[i].GameNum;
		m_GameToRoom[s]=new CRoom();
	}
//	int n=sizeof(game[0].GameNum);
//	printf("balbbbbbbbbbbbbbbb%d\n",sizeof(game[0].GameNum));
//	for(int i=0;i<n;i++)
//	printf("%c\n",game[0].GameNum[i]);
//		printf("cunzzaideba$$$$$$$$$$$$$$$$$$$$\n");
/*
	char num[20]={0};
	strcpy(num,game[0].GameNum);
	
	int n1=strlen(num);
	int n=strlen(game[0].GameNum);
	printf("%d??????????????==%d\n",n,n1);

	int i=0;
	while(num[i]!='\0'||game[0].GameNum[i]!='\0'){
		printf("%c --%d-------%c\n",game[0].GameNum[i],i,num[i]);
		i++;
	}
	if(m_GameToRoom[num])
		printf("num-cunzzaideba$$$$$$$$$$$$$$$$$$$$\n");
	if(m_GameToRoom[game[0].GameNum])
		printf("gamenum--cunzzaideba$$$$$$$$$$$$$$$$$$$$\n");
*/
	pthread_t thr3;	
	if (pthread_create(&thr3,NULL,SendDeskProc,this) !=0) {
	//	printf("创建线程失败\n");
	//	return 0;
	}
	
}


CFactory::~CFactory()
{
	if(m_db){
		delete m_db;
		m_db=NULL;
	}
	map<string,CRoom*>::iterator ite=m_GameToRoom.begin();
	while(ite!=m_GameToRoom.end()){
		delete ite->second;
		ite->second=NULL;
	}

	map<int,UserGame*>::iterator ite1=m_SockToUser.begin();
	while(ite1!=m_SockToUser.end()){
	free(ite1->second);
	ite1->second=NULL;
	}
	
}
static const ProtocolMap m_ProtocolMapEntries[] =
{

	{ _DEF_PROTOCOL_LOGINHALL_RQ,&CFactory::DealLoginReq },
	{ _DEF_PROTOCOL_GAMECHOOSE_RQ,&CFactory::DealTheGame },
	{ _DEF_PROTOCOL_SITDOWN_RQ,&CFactory::DealSitDownReq},
	{ _DEF_PROTOCOL_GAMEREADY_RQ,&CFactory::DealReadyRq},
	{  _DEF_PROTOCOL_GAMEMSG_RQ,&CFactory::DealGameMsg},
	{ 0 }
};


void CFactory::DealGameMsg(int sockfd, char*buf) {
	//找到相关内容，查看是发给客户端还是服务器的
	
	STRU_GAMEMSG_RQ *rq=(STRU_GAMEMSG_RQ*)rq;
	
	string gamenum;
	int desknum=m_SockToUser[sockfd]->DeskNum;
	gamenum=m_SockToUser[sockfd]->GameNum;
	User *user=m_SockToUser[sockfd]->user;
	int id;
	id=user->m_userId;
	
	STRU_GAMEMSG_RS rs;
	rs.m_nType=_DEF_PROTOCOL_GAMEMSG_RS;
	rs.m_socket=sockfd;
	strcpy(rs.Msg,rq->Msg);
	rs.MsgLen=rq->MsgLen;
	
	if(rq->ToUser==1){
	//找到这个人对应的桌子
	int num=m_GameToRoom[gamenum]->m_Desk[desknum].Num;
	int sock;
	int tempid;
	//发送给所有人除了他自己
	for(int i=0;i<num;i++){
		tempid=m_GameToRoom[gamenum]->m_Desk[desknum].user[i].user.m_userId;
		if(tempid==id)
			continue;
		sock=m_GameToRoom[gamenum]->m_Desk[desknum].user[i].m_socket;
		if(-1==send(sock,(char*)&rs,sizeof(rs),0))
			printf("to cli %s msg fail\n",tempid);
	}
	
	}
	else{
		//发给服务器
		int  sock=m_GameToAddr[gamenum]->m_socket;
		if(-1==send(sock,(char*)&rs,sizeof(rs),0))
			printf("to server  msg fail\n");

	}

}
void CFactory::DealLoginReq(int sockfd, char*buf) {
	
	printf("jinlai\n");
	if(buf==NULL){
	printf("用户数据无效\n");
	return;
	}
	STRU_LOGINHALL_RQ *rq=(STRU_LOGINHALL_RQ*)buf;
	if(rq->user.m_userId==NULL){
	printf("用户id无效\n");
	return;
	}
	
	UserGame*u=(UserGame*)malloc(sizeof(UserGame));
	u->user=&(rq->user);
	strcpy(u->GameNum,"");
	u->DeskNum=0;
	
	printf("user??????\n");
	
	m_SockToUser[sockfd]=u;
	printf("user\n");
	
	//发送回复
	STRU_LOGINHALL_RS rs; 
	rs.m_nType=_DEF_PROTOCOL_LOGINHALL_RS;
	rs.IfSuccess=1;
	//发送游戏列表
	struct STRU_HALL_GAMELIST_RS  gamelist;
	gamelist.m_nType= _DEF_PROTOCOL_GAMELIST_RS;
	 //从数据库获取
	for(int i=0;i<game.size();i++){
		printf("game list [%d] = %s\n",i,game[i].GameName);
		strcpy(gamelist.m_szGameList[i].GameNum,game[i].GameNum);
		strcpy(gamelist.m_szGameList[i].GameName,game[i].GameName);
		strcpy(gamelist.m_szGameList[i].GameInfo,game[i].GameInfo);
		// 游戏列表
	//	m_GameToRoom[game[i].GameNum]=new CRoom();

	}
	gamelist.GameCount=game.size();
	//send
	printf("gamelist size`:%d\n",sizeof(gamelist));
	int s;
	if((s=send(sockfd,(char*)&gamelist,sizeof(gamelist),0))<0){
		printf("send game list fail\n");
	}
	printf("send gamelist  success\n");
}
	
void CFactory::DealTheGame(int sockfd, char*buf) {
	//找到此游戏的房间信息并发s
	STRU_GAMECHOOSE_RQ* cho=(STRU_GAMECHOOSE_RQ*)buf;
	char thegame[20];
	printf("????????\n");

	strcpy(thegame, cho->GameNum);
	//存起来
	strcpy(m_SockToUser[sockfd]->GameNum,thegame);
	printf("cli-%d,choose the %s game\n",sockfd,thegame);
	//struct 
	STRU_GAMECHOOSE_RS roominfo;
	roominfo.m_nType=_DEF_PROTOCOL_GAMECHOOSE_RS;
	printf("????????????????\n");
//	for(int j=0;j<m_GameToRoom[thegame]->m_Desk.size();j++){
	//	roominfo.desk[j].Num=m_GameToRoom[thegame]->m_Desk[j].Num;
	//	roominfo.desk[j].user=(m_GameToRoom[thegame]->m_Desk[j].user)->user;
//	}
//excemple
roominfo.desk[0].Num=1;
User user;
user.m_userId=807879;
strcpy(user.m_userName,"wxx");
user.score=0;
user.IsReady=0;
roominfo.desk[0].user[0]=user;
//m_GameToRoom[thegame]->m_Desk[0].user[0].user=user;

	//send
	if(send(sockfd,(char*)&roominfo,sizeof(roominfo),0)<0)
	{
		printf("send game roominfo fail :%d\n",errno);
	}



//将此sock放入list中，持续发送
m_Sock.push_back(sockfd);
}

void CFactory::DealSitDownReq(int sockfd, char * buf)
{
	//判断怎么坐 并返回位置
	//enum sit_method { match, chose, newdesk };
	STRU_SITDOWN_RQ *req=(STRU_SITDOWN_RQ*)buf;
	STRU_SIT_POST post;
    post.m_nType=_DEF_PROTOCOL_SITDOWN_RS;

	printf("sit deal===========\n");

	string num;
	num=m_SockToUser[sockfd]->GameNum;
	//num=game[0].GameNum;
	

	printf("sit deal\n");
	//给;游戏的赋值
	User *user=m_SockToUser[sockfd]->user;
	switch(req->m_method){
	case chose:
	{	
		printf("chose\n");
		if(req->DeskNum<0)
		{
			printf("收到的选座错误\n");
			return ;
		}
			

		//判断这个位置是否能坐
		bool man=m_GameToRoom[num]->IsCanSit(req->DeskNum);

		if(!man){
			post.m_Result=0;
			post.DeskNum=req->DeskNum;
		}
		else{
			post.m_Result=1;
			post.DeskNum=req->DeskNum;
			
			//桌子加上人
			int Num=m_GameToRoom[num]->m_Desk[req->DeskNum].Num;
			m_GameToRoom[num]->m_Desk[req->DeskNum].user[Num].user=*user;
			m_GameToRoom[num]->m_Desk[req->DeskNum].user[Num].m_socket=sockfd;
			m_GameToRoom[num]->m_Desk[req->DeskNum].user[Num].m_state=WAIT;
			m_GameToRoom[num]->m_Desk[req->DeskNum].Num++;
		
		}
		m_SockToUser[sockfd]->DeskNum=req->DeskNum;
		//send
		if((send(sockfd,(const char*)&post,sizeof(post),0))<0){
		printf("send post fail for choose desk\n");
		
		int n=m_GameToRoom[num]->m_Desk[req->DeskNum].Num;
		if(n>1){
			STRU_PARTNET_RS pe;
			pe.m_nType=_DEF_PROTOCOL_PARTNET_RS;
			pe.desk.Num=n;
			for(int j=0;j<n;j++){
				pe.desk.user[j]=m_GameToRoom[num]->m_Desk[req->DeskNum].user[j].user;
			}
			for(int i=0;i<n;i++){
			int sock=m_GameToRoom[num]->m_Desk[req->DeskNum].user[i].m_socket;
			if((send(sock,(const char*)&pe,sizeof(pe),0))<0)
				printf("send user of desk info fail for choose desk\n");
			}
		}
	break;
	}
	}
	case newdesk:
	{	
		printf("new desk\n");
		//send
		STRU_SIT_POST post;
		post.m_nType=_DEF_PROTOCOL_SITDOWN_RS;
		post.m_Result=1;	
		post.DeskNum=m_GameToRoom[num]->AddDesk();
		int n=post.DeskNum;	
		//桌子加上人
		int Num=m_GameToRoom[num]->m_Desk[n].Num;
		m_GameToRoom[num]->m_Desk[n].user[Num].user=*user;
		m_GameToRoom[num]->m_Desk[n].user[Num].m_socket=sockfd;
		m_GameToRoom[num]->m_Desk[n].user[Num].m_state=WAIT;
		m_GameToRoom[num]->m_Desk[n].Num++;
		
		
		m_SockToUser[sockfd]->DeskNum=n;
		if((send(sockfd,(const char*)&post,sizeof(post),0))<0)
		{
			printf("send post fail for new desk\n");
		}
		break;
	}
	case match:
	{
		printf("match desk\n");
//		//加入匹配池，有需要线程来发
//		m_GameToRoom[num]->MatchDesk(sockfd,user);
		STRU_SIT_POST post;
		post.m_nType=_DEF_PROTOCOL_SITDOWN_RS;
		post.m_Result=1;	

printf("%s\n",num);

printf("++++++++++\n");
		int q=m_GameToRoom[num]->m_Desk.size();
		printf("q=========%d\n",q);
		int i=0;
		for( i=0;i<q;i++){
			if(m_GameToRoom[num]->m_Desk[i].Num<3)
			break;   	  
	    }
		post.DeskNum=i;
	    printf("match desk = %d\n",i);
		printf("desknum:%d\n",post.DeskNum);
				   
		int Num=m_GameToRoom[num]->m_Desk[post.DeskNum].Num;
		m_GameToRoom[num]->m_Desk[post.DeskNum].user[Num].user=*user;
		m_GameToRoom[num]->m_Desk[post.DeskNum].user[Num].m_socket=sockfd;
		m_GameToRoom[num]->m_Desk[post.DeskNum].user[Num].m_state=WAIT;
		m_GameToRoom[num]->m_Desk[post.DeskNum].Num++;
		
		m_SockToUser[sockfd]->DeskNum=post.DeskNum;
		printf("result:%d\n",post.m_Result);
		printf("desknum:%d\n",post.DeskNum);
		if((send(sockfd,(const char*)&post,sizeof(post),0))<0)
		{
			printf("send post fail for match desk\n");
		}
		int n=m_GameToRoom[num]->m_Desk[req->DeskNum].Num;
		if(n>1){
			STRU_PARTNET_RS pe;
			pe.m_nType=_DEF_PROTOCOL_PARTNET_RS;
			pe.desk.Num=n;
			for(int j=0;j<n;j++){
			pe.desk.user[j]=m_GameToRoom[num]->m_Desk[req->DeskNum].user[j].user;
			}
			for(int i=0;i<n;i++){
			int sock=m_GameToRoom[num]->m_Desk[req->DeskNum].user[i].m_socket;
			if((send(sock,(const char*)&pe,sizeof(pe),0))<0)
				printf("send user of desk info fail for match\n");
			}
		}
		break;
	
	}
	default:
	printf("cant find sit method\n");
	break;
	
	
	}
	m_Sock.remove(sockfd);
		
}
void CFactory::DealReadyRq(int sockfd,char* szbuf){
	//获取信息
///	m_SockToUser[sockfd]->user->m_state=READY;
	char GameNum[20];
	strcpy(GameNum,m_SockToUser[sockfd]->GameNum);
	int DeskNum=m_SockToUser[sockfd]->DeskNum;
	//改 ready
	int p=m_GameToRoom[GameNum]->m_Desk[DeskNum].Num;
	for(int i=0;i<p;i++){
		if(m_GameToRoom[GameNum]->m_Desk[DeskNum].user[i].m_socket==sockfd){
			m_GameToRoom[GameNum]->m_Desk[DeskNum].user[i].user.IsReady=1;
			m_GameToRoom[GameNum]->m_Desk[DeskNum].user[i].m_state=READY;
			break;
		}
	}
	

	//向所有人发desk的情况
	int n=m_GameToRoom[GameNum]->m_Desk[DeskNum].Num;
	STRU_GAMEREADY_RS rs;
	rs.m_nType= _DEF_PROTOCOL_GAMEMSG_RS;
	rs.usernum=n;
	for(int i=0;i<n;i++){
		rs.user[i]=m_GameToRoom[GameNum]->m_Desk[DeskNum].user[i].user;
	
	}
	for(int i=0;i<n;i++){
		int sock=m_GameToRoom[GameNum]->m_Desk[DeskNum].user[i].m_socket;
		if(-1==send(sock,(const  char*)&rs,sizeof(rs),0)){
		printf("send ready rs to %d fail\n",sock);
		}
		
	}
	
}
void* CFactory::SendDeskProc(void* lpParameter){
/*	CFactory *pthis=(CFactory*)lpParameter;
	list<int>::iterator ite=pthis->m_Sock.begin();
	char gamenum[20];
	int sock;
	STRU_GAMECHOOSE_RS rs;
	rs.m_nType=_DEF_PROTOCOL_GAMECHOOSE_RS;
//这里总觉得不是很好????
	while(1){
		//if(ite==NULL)
		//	continue;
		sock=*ite;
		strcpy(gamenum,pthis->m_SockToUser[sock]->GameNum);
		for(int i=0;i<pthis->m_GameToRoom[gamenum]->m_Desk.size();i++){
			
			rs.desk[i].Num=pthis->m_GameToRoom[gamenum]->m_Desk[i].Num;
			for(int j=0;j<rs.desk[i].Num;j++)
			rs.desk[i].user[j]=pthis->m_GameToRoom[gamenum]->m_Desk[i].user[j].user;
		}
		if(-1==send(sock,(char*)&rs,sizeof(rs),0)){
			printf("send sock%d fail\n",sock);
		}
		
	
	if(ite==pthis->m_Sock.end())
		ite=pthis->m_Sock.begin();
	}
	
*/
}

//void CFactory::MatchUser(User user){

	//加入匹配池
	//暂时先用直接放的方法，匹配池先用不上
	
//}

void CFactory::DealData(char *szbuf,int sockfd)
{
	//处理数据
	PackdefType *pType = (PackdefType*)szbuf;
printf("*pType:%d\n",*pType);
	//查找协议映射表---遍历数组
	int i = 0;
	while (1)
	{
		if (m_ProtocolMapEntries[i].m_nType == *pType)
		{
			(this->*m_ProtocolMapEntries[i].m_pfun)( sockfd,szbuf);
			return;
		}
		else if (m_ProtocolMapEntries[i].m_nType == 0 && m_ProtocolMapEntries[i].m_pfun == 0)
			return;

		i++;
	}

}
