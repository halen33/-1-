#include "INet.h"
#include"res.h"


CINet::CINet()
{
	m_fac=new CFactory();
	OnlineNum=8;
	sockCli=0;
	socklogin=0;

}


CINet::~CINet()
{
delete m_fac;
m_fac=NULL;
}
void CINet::GetIp(){
	int sockfd;
	struct ifconf ifconf;
	struct ifreq *ifreq;
	char buf[512];//缓冲区
	//初始化ifconf
	ifconf.ifc_len =512;
	ifconf.ifc_buf = buf;
	if ((sockfd =socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("socket" );
		exit(1);
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf); //获取所有接口信息

	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)ifconf.ifc_buf;
	//printf("ifconf.ifc_len:%d\n",ifconf.ifc_len);
	//printf("sizeof (struct ifreq):%d\n",sizeof (struct ifreq));

	for (int i=(ifconf.ifc_len/sizeof (struct ifreq)); i>0; i--)
	{
		if(ifreq->ifr_flags == AF_INET){ //for ipv4
			if(0==strcmp(ifreq->ifr_name,"ens33")){
				printf("name =[%s]\n" , ifreq->ifr_name);
			//	printf("local addr = [%s]\n" ,inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
				m_ip=inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
			printf("%s\n",m_ip);
			break;
			}
			ifreq++;
		}
	}
	
}
BOOL CINet::InitSock()
{
	//loginSev的链接
	socklogin=socket(AF_INET,SOCK_STREAM,0);
	bzero(&loginaddr, sizeof(loginaddr));
	loginaddr.sin_family = AF_INET;
	loginaddr.sin_addr.s_addr = inet_addr(LOGIN_SEV_IP);
	loginaddr.sin_port = htons(LOGIN_SEV_PORT);

//	if(connect(socklogin, (struct sockaddr*)&loginaddr, sizeof(struct sockaddr))<0)
//	{
//		printf("连接loginsev失败\n");
//		return 0;

//	}
//	pthread_t thr2;	
//	if (pthread_create(&thr2,NULL,SendNumProc,this) !=0) {
//		printf("创建线程失败\n");
//		return 0;
//	}
	
	printf("开始接受cli连接\n");
	//cli的链接
	GetIp();
	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(m_ip);
	server_address.sin_port = htons(8888);

	sockCli= socket(AF_INET, SOCK_STREAM, 0);
	if (sockCli < 0)
	{
		printf("socketCli error!\n");
		return 0;
	}
	int ret = bind(sockCli, (struct sockaddr*)&server_address, sizeof(server_address));
	if (ret == -1)
	{
		printf("bind error!\n");
		return 0;
	}
	ret = listen(sockCli, 8);
	if (ret == -1)
	{
		printf("listen error!\n");
		return 0;
	}
	
	printf("开始接受cli连接\n");

	RecvProc();
	return 1;
}
void  CINet::addfd(int epfd, int fd)
{
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	//setnonblocking(fd);
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}
	
void* CINet::SendNumProc(void* lpParameter){

	CINet *pthis=(CINet*)lpParameter;
	
int i=1;
	STRU_HALL_INFO hall;
	hall.m_nType=_DEF_PROTOCOL_HALLINFO;
	hall.port=8888;
	while(1){
printf("srart to send num to loginsev\n");
	hall.m_ManNum=pthis->OnlineNum;
	printf("%d\n",pthis->OnlineNum);
	//发送
	if(send(pthis->socklogin,&hall,sizeof(hall),0)<=0){
	printf("发送在线人数失败\n");
	return 0;
	}
	

	//wait
	sleep(SEND_TIMES);
printf(" send num to loginsev %d\n",i);
	i++;
	}
}
void CINet::RecvProc(){
	 CINet *p=this;
	//用epoll
	int epfd;
	char buf[4096];
	memset(buf,0,4096);
	epoll_event events[1000];
	epfd = epoll_create(8);
	assert(epfd != -1);
	addfd(epfd, sockCli);
	printf("进入proc\n");
	while (1)
	{
		//返回有事件的数量
		int number = epoll_wait(epfd, events, 1000, -1);
		if ((number < 0) && (errno != EINTR))
		{
			printf("my epoll is failure!\n");
		}
		printf("有事件的socket数量%d\n", number);
		for (int i = 0; i<number; i++)
		{
			int sockfd1 = events[i].data.fd;
			if (sockfd1 == sockCli)//有新用户连接
			{
				printf("have a user\n");
				struct sockaddr_in client_address;
				socklen_t client_addresslength = sizeof(client_address);
				int client_fd = accept(sockCli, (struct sockaddr*)&client_address, &client_addresslength);
				if (client_fd < 0)
				{
					printf("errno is %d\n", errno);
					continue;
				}
				cout << epfd << " " << client_fd << endl;
				//再用epoll 注册接收事件
				addfd(epfd, client_fd);
				cout << "client_fd:" << client_fd << "****\n";
			
			}

			else if (events[i].events & EPOLLRDHUP){
			printf("client:%d 异常关闭\n",sockfd1);
			continue;
			}
			else if (events[i].events & EPOLLIN)//可以读取
			{
				
				printf("can read\n");
				int len=read(sockfd1, buf, BUF_SIZE);//读
				if(len< 0)
				{
					printf("error :%d\n",errno);
					printf("read null or fail\n");
					continue;

				}
				else{
					printf("read success\n");
					p->m_fac->DealData(buf,sockfd1);
					printf("deal success\n");
				}
				bzero(buf,sizeof(buf));
			}
			else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				printf(" epoll wait wrong\n");
				/*出现异常则关闭客户端连接*/
			
			}
			else{
				printf("xieru??\n");

				}
		}
	}
	printf("FAIL!\n");
	close(epfd);
	close(sockCli);
}



	
