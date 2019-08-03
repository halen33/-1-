#include "Room.h"



CRoom::CRoom()
{
	m_Desk.resize(_DEF_DESK_NUM);
	

//	pthread_t thr2;	
//	if (pthread_create(&thr2,NULL,MatchThreadFun,this) !=0) {
//		printf("创建线程失败\n");
//	}
}


CRoom::~CRoom()
{
}

int  CRoom::AddDesk()
{
	return (m_Desk.size()+1);
}
bool CRoom::IsCanSit(int DeskNum){

	//判断这个位置是否能坐
	int man=m_Desk[DeskNum].Num;
	if(man==3){
		return false;
	}
	else{
		return true;
	}
}

BOOL CRoom::UpDataDesk(Desk desk)
{
	return 0;
}

int CRoom::MatchDesk(int sockfd,User *u){

/*	for(int i=0;i<m_Desk.size();i++){
		if(m_Desk[i].Num<3)
			return i;
		else
			continue;
	}
	return 0;
*/
/*	struct timeval tv;
	gettimeofday(&tv,NULL);
	MyUser *usr;
	usr->user=u;
	usr->tv=tv;
	int score=u->score;
	usr->Level=score/300+1;
	m_SockToMyUser[sockfd]=usr;
	printf("%s加入匹配池成功\n",usr->user->m_userId);
return 1;
*/
}
void *CRoom::MatchThreadFun(void *arg){
/*	
	CRoom*p=(CRoom*)arg;
	int lev;
	int usernum=p->OneDeskUserNum;
	int num=usernum;//0-usernum-1
	MyUser* all[num];
	bzero(all,num);
	int ttv[num];
	bzero(ttv,num);
	num--;

	struct timeval tv1;
	map<int,MyUser*>::iterator iter=p->m_SockToMyUser.begin();
	//找最长
	gettimeofday(&tv1,NULL);
	struct timeval min=tv1;
	int  idd=0;
	int flag=true;
	while(iter!=p->m_SockToMyUser.end()){
		if(min.tv_sec-iter->second->tv.tv_sec>0){
			min=iter->second->tv;
			idd=iter->first;
			if(min.tv_sec-tv1.tv_sec>10)
				{
					flag=false;
					break;
	
				}
		}
	}
	STRU_SIT_POST post;
	post.m_nType=_DEF_PROTOCOL_SITDOWN_RS;
	post.m_Result=1;	
if(flag==true){
	//存起来
	all[0]=p->m_SockToMyUser[idd];
	ttv[0]=idd;
	struct timeval tv0;
	tv0=p->m_SockToMyUser[idd]->tv;
	lev=p->m_SockToMyUser[idd]->Level;
	p->m_SockToMyUser.erase(iter++);
	//找
	while(1){
		gettimeofday(&tv1,NULL);
		for(;iter!=p->m_SockToMyUser.end();iter++){//并不一定匹配到谁需要根据id去返回桌子号
			if(iter->second->Level==lev){
			//存起来 删除
				ttv[usernum-num]=iter->first;
				all[usernum-num]=iter->second;
				p->m_SockToMyUser.erase(iter);
				num--;
			}
			if(num==0)
				break;
			}
		if(num==0)
			break;
		else{
			double d;
			time_t s;
			suseconds_t u;
			gettimeofday(&tv1,NULL);

			s = tv1.tv_sec - tv0.tv_sec;
			u = tv1.tv_usec - tv0.tv_usec;

			d = s;
			d *= 1000000.0;//1 秒 = 10^6 微秒
			d += u;

			
			if(d>=10000000.0)
			{flag=false;
				break;
			}
		}
		
	}
	
	if(num!=0&&(lev==1||lev==3)){//zhao 2
		lev=2;
		for(iter=p->m_SockToMyUser.begin();iter!=p->m_SockToMyUser.end();iter++){
			if(iter->second->Level==lev){
		//存起来 删除
			ttv[usernum-num]=iter->first;
			all[usernum-num]=iter->second;
			(p->m_SockToMyUser).erase(iter);
			num--;
			}
			if(num==0){
				break;
			}
		}
	}
	int r=0;
	if(num==0){
		//为这几个人分桌子
		r=p->m_Desk.size()+1;
		//将桌子更新
		for(int i=0;i<usernum;i++){
			p->m_Desk[r].user[i]=*(all[i]->user);
			p->m_Desk[r].Num++;	
		}
		
		int sockfd;
		//将保存的id找到sock，发送回复
		for(int i=0;i<(p->OneDeskUserNum);i++){
			post.DeskNum=r;
			map<int ,MyUser*>::iterator it=p->m_SockToMyUser.begin();
			while (it != p->m_SockToMyUser.end())
			{
				if(it->second==all[i])
					{
						sockfd = (it)->first;
		break;	}}
			if((send(sockfd,(const char*)&post,sizeof(post),0))<0)
			{
			printf("send post fail for match desk\n");
			}
			break;
		}
	}
	else
		flag=false;
}
if(flag==false){
		//先放回去
		int r=0;
	
		//把之前拿出来的放回去
		int temp=1;
		int size=sizeof(ttv)/sizeof(ttv[0]);
		while(1){
			if(temp<size){
				MyUser *us=all[temp];
				p->m_SockToMyUser[ttv[temp]]=us;
			}
			temp++;
		}
		//直接放桌子里
		int ds=p->m_Desk.size()+1;
		for(r=0;r<ds;r++){
			if(p->m_Desk[r].Num<3){
			int num1=p->m_Desk[r].Num-1;
			p->m_Desk[r].user[num1]=*((all[0])->user);
			p->m_Desk[r].Num++;
			}
		}
		//发送回复
		int sockfd;
		map<int ,MyUser*>::iterator it=p->m_SockToMyUser.begin();
		while(it!=p->m_SockToMyUser.end())
		if(it->second==all[0]){
			 sockfd=it->first;	
			break;
		}
		post.DeskNum=r;
		if((send(sockfd,(const char*)&post,sizeof(post),0))<0)
		{
			printf("send post fail for match desk\n");
		}
	
}
	
*/
}









