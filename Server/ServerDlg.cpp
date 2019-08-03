// ServerDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "ServerAddrDlg.h"
#include "UserInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };	

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_sListen = INVALID_SOCKET;
	memset(m_arrEvent, 0, MAX_NUM_EVENTS);
	memset(m_arrSocket, 0, MAX_NUM_EVENTS);
	m_nTotalEvent = 0; 
	m_bRuning = FALSE;
	m_hEventExit = NULL;
	//pdesk = desk;
	pdesk = new Desk;
	pdesk->InitNetWork();

}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ctlUserList);

}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnStartService)
	ON_BN_CLICKED(IDCANCEL, OnStopService)
	ON_MESSAGE(WM_USER_ACCEPT, OnSocketAccept)
	ON_MESSAGE(WM_USER_READ, OnSocketRead)
	//ON_MESSAGE(WM_USER_FIND, OnSocketfind)
	ON_MESSAGE(WM_USER_CLOSE, OnSocketClose)
END_MESSAGE_MAP()


BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	int len = sizeof(byte);
	len = sizeof(u_short);
	len = sizeof(PACKETHDR);

	if(FALSE == Init())//初始化服务器
		EndDialog(IDCANCEL);

	
	return TRUE;  
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
 * 初始化
 */
BOOL CServerDlg::Init(void)
{
	InitListCtrlSetting();		//初始化列表视图控件

	if(!InitSocket())			//初始化套接字
		return FALSE;

	return TRUE;
}

/*
 * 初始化列表视图控件
 */
void CServerDlg::InitListCtrlSetting( void )
{
	m_pImageList = new CImageList();							//创建图像列表对象	
	//32*32，8位，初始化2个图标
	m_pImageList->Create(32, 32, ILC_COLOR8|ILC_MASK, 2, 2);	
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_USER_PLAY_ICON));//游戏图标
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_USER_WAIT_ICON));//等待图标	
	
	
	m_ctlUserList.SetImageList(m_pImageList, LVSIL_SMALL);		//图像列表赋值给列表控件
	m_ctlUserList.SetExtendedStyle(m_ctlUserList.GetExtendedStyle()
										|LVS_EX_SUBITEMIMAGES);	//设置扩展风格
	
	//设置列表头
	m_ctlUserList.InsertColumn(0, _T("名称"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(1, _T("状态"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(2, _T("地址"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(3, _T("端口"), LVCFMT_CENTER);
	
	//设置各列的宽度
	CRect rect;
	m_ctlUserList.GetClientRect(&rect);					//获取客户区宽度
	int nWidth = rect.Width();
	m_ctlUserList.SetColumnWidth(0, nWidth * 25 / 100);	//名称25%
	m_ctlUserList.SetColumnWidth(1, nWidth * 20 / 100);	//状态20%
	m_ctlUserList.SetColumnWidth(2, nWidth * 35 / 100);	//IP35%
	m_ctlUserList.SetColumnWidth(3, nWidth * 20 / 100);	//地址20%


}

/*
 * 初始化套接字
 */
BOOL CServerDlg::InitSocket( void )
{
	int		reVal;				//返回值
	WORD	wVersionRequested;	//请求的Windows Sockets 实现版本
	WSADATA	wsaData;			//返回协商结果

	wVersionRequested = MAKEWORD(2, 2);	
	reVal = WSAStartup( wVersionRequested, &wsaData );//初始化套接字动态库
	if ( 0 != reVal ) 
	{
		return FALSE;
	}
	
	//创建套接字
	m_sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字
	if (INVALID_SOCKET == m_sListen)
	{
		WSACleanup();
		return FALSE;
	}	

	//服务器地址对话框
	/*if (IDOK != m_servAddrDlg.DoModal())
	{	
		closesocket(m_sListen);
		WSACleanup();
		return FALSE;
	}*/

	return TRUE;
}


/*
 *	启动服务
 */
void CServerDlg::OnStartService() 
{	
	if (FALSE == BeginListen())//服务器套接字开始监听
	{
		AfxMessageBox("启动服务器失败！");
		closesocket(m_sListen);
		WSACleanup();
		EndDialog(IDCANCEL);
		return;
	}

	m_bRuning = TRUE;									//设置服务器运行状态
	//创建服务线程退出事件对象
	m_hEventExit = CreateEvent(NULL, TRUE, FALSE, NULL);//手动设置，初始“未传信”状态
	
	//线程函数参数
	PTHREADPARAM pParam = new THREADPARAM;
	pParam->hServHwnd = this->GetSafeHwnd();
	pParam->pRunning = &m_bRuning;
	pParam->pTotalEvent = &m_nTotalEvent;
	pParam->pArrEvent = m_arrEvent;
	pParam->pArrSocket = m_arrSocket;
	pParam->hEventExit = m_hEventExit;

	//创建服务线程
	DWORD dwThread;	
	HANDLE hServiceThread = CreateThread(NULL, 0, ServiceThread, pParam, 0, &dwThread);
	CloseHandle(hServiceThread);

	//设置启动服务器按钮无效
	(CButton*)GetDlgItem(IDOK)->EnableWindow(FALSE);	
}

/*
 * 服务器退出
 */
void CServerDlg::OnStopService() 
{
	//设置服务器状态
	m_bRuning = FALSE;

	WaitForSingleObject(m_hEventExit, INFINITE);//等待线程退出
	
	DeleteAllClientNodes();						//删除所有客户端节点

	delete m_pImageList;
	m_pImageList = NULL;
	CloseHandle(m_hEventExit);		//关闭线程退出事件对象 
	WSACloseEvent(m_arrEvent[0]);	//关闭监听套接字对应事件
	closesocket(m_sListen);			//关闭监听套接字
	WSACleanup();					//释放套接字资源
	CDialog::OnCancel();
}

/*
 *	服务器开始监听
 */
BOOL CServerDlg::BeginListen( void )
{
	int	reVal;										//返回值

	m_arrSocket[m_nTotalEvent]= m_sListen;//将监听套接字加入集合
	m_arrEvent[m_nTotalEvent] = WSACreateEvent();	//创建套接字事件对象
	
	//为监听套接字注册FD_ACCEPT和 FD_CLOSE网络事件
	if(SOCKET_ERROR == WSAEventSelect(m_sListen, m_arrEvent[m_nTotalEvent], 
						FD_ACCEPT | FD_CLOSE))
		return FALSE;
	m_nTotalEvent++;//套接字事件对象加1
	
	//服务器套接字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = /*htonl(m_servAddrDlg.m_dwServIP)*/INADDR_ANY;
	servAddr.sin_port = htons(/*m_servAddrDlg.m_shServPort*/SERVERPORT);
	
	//绑定
	reVal = bind(m_sListen,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == reVal)
		return FALSE;
	
	//监听
	reVal = listen(m_sListen, SOMAXCONN);
	if (SOCKET_ERROR == reVal)
		return FALSE;
	
	return TRUE;
}

/*
 *	清除套接字和事件对象
 */
void CServerDlg::ClearSocketAndEventFromArr( const int nIndex )
{	
	//将该套接字和事件对象从数组中清除
	for (int i = nIndex - WSA_WAIT_EVENT_0; i < m_nTotalEvent; i++)
	{
		m_arrEvent[i] = m_arrEvent[i + 1];	
		m_arrSocket[i] = m_arrSocket[i + 1];
	}
	//总数减1		
	m_nTotalEvent--;
}

/*
 * 从管理客户端链表中查找对应的CClientSocket指针
 */

CClientSocket* CServerDlg::FindClientNode( const SOCKET s )
{
	m_csList.Lock();
	POSITION pos = NULL;
	for (pos = m_UserList.GetHeadPosition(); pos != NULL;)
	{
		CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
		if (pClientSocket->m_s == s)
		{
			m_csList.Unlock();
			return pClientSocket;
		}
	}
	m_csList.Unlock();
		return NULL;
}

/*
 * 从管理客户端链表删除节点
 */
void CServerDlg::DeleteClientNode( SOCKET s )
{
	POSITION		pos1 = NULL;			//临时变量
	POSITION		pos2 = NULL;			//临时变量
	CClientSocket	*pClientSocket = NULL;	//CClientSocket指针
	BOOL			bFinder = FALSE;		//是否找到该套接字

	m_csList.Lock();//进入临界区
	//遍历整个链表找到套接字对应的节点
	for (pos1 = m_UserList.GetHeadPosition(); (pos2=pos1) != NULL;)
	{
		pClientSocket = (CClientSocket*)m_UserList.GetNext(pos1);
		if (pClientSocket->m_s == s)
		{
			bFinder = TRUE;
			break;
		}
	}
	ASSERT(TRUE == bFinder);
	m_UserList.RemoveAt(pos2);	//从链表中删除该节点
	delete pClientSocket;		//释放内存
	pClientSocket = NULL;
	m_csList.Unlock();			//离开临界区
	
	UpdateListCtrl();			//更新服务器用户列表
	//SendUserList();				//发送用户列表	如果有人走了，我要给跟他一桌的人发新的列表，这里暂时先不写
}


/*
 * 发送用户列表 这里要改 只发送这一桌的人过去
 */
void CServerDlg::SendUserList( CClientSocket* pClient)
{
	CString strUserInfoList;			//临时变量
	int DeskNum;

	in_addr userAddr;
	userAddr.S_un.S_addr = (long)pClient->m_UserInfo.m_lIP;
	char *c_ip = inet_ntoa(userAddr);
	int Port = ntohs(pClient->m_UserInfo.m_uPort);
	for (int i = 0; i < 100; i++)
	{
		//从桌子里找这个人 找到了得到桌号
		if( (DeskNum=pdesk->findUser(c_ip, Port) )!= -1)
		{
			break;
		}
	}
	MakeSendUserString(strUserInfoList, DeskNum);//准备用户列表
	CClientSocket *D_Client = FindClientNode(pClient->m_s);
	//得到socket,然后找到跟他一桌人的ip port 遍历user list找到这人发过去
	m_csList.Lock();
	

	//发送用户列表
	POSITION pos = NULL;
	//给这个人发回去准备好的用户列表。
	D_Client->Send(USERINFOR, strUserInfoList);
	//for (pos = m_UserList.GetHeadPosition(); NULL != pos ;)
	//{
	//	//if(m_UserList.GetNext(pos)->)
	//	
	//	CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
	//	

	//	//pClientSocket->m_UserInfo.m_lIP//这个客户端的ip 和 port
	//	pClientSocket->Send(USERINFOR, strUserInfoList);	
	//}
	m_csList.Unlock();
}


/*
 * 更新用户列表 收到之后更新用户链表再发过去
 */
BOOL CServerDlg::UpdateUserList(const CString strUserInfo, CClientSocket* pClientSocket)
{
	//解析用户信息
	//更新用户信息，注意此处名字要从桌子上获得。
	int k;//桌号
	char *name=(char *)malloc(sizeof(50));
	//memset(name, 0, sizeof(name));
	U_Desk desk;
	CUserInfo userInfo = ParserUserInfo(strUserInfo,"0");//需要名字
	pClientSocket->SaveUserInfo(userInfo);//保存用户信息 无名字 但有ip和port 先保存ip port 分开保存名字。
	//ip要经过一定的转换再比较

	in_addr userAddr;
	userAddr.S_un.S_addr = (long)userInfo.m_lIP;
	//userAddr.S_un.
	char *c_ip=inet_ntoa(userAddr);
	int Port = ntohs(userInfo.m_uPort);
	if ((k=pdesk->findUser(c_ip, Port) )!= -1)
	{
		desk=pdesk->numtoip(k);
		if (strcmp(desk.user[0].ip, c_ip) == 0 && desk.user[0].port == Port)
			memcpy(name, &desk.user[0].name, sizeof(desk.user[0].name));
		else if (strcmp(desk.user[1].ip, (char *)pClientSocket->m_UserInfo.m_lIP) == 0 && desk.user[1].port ==(int) pClientSocket->m_UserInfo.m_uPort)
			memcpy(name, &desk.user[1].name, sizeof(desk.user[1].name));
	}
	userInfo = ParserUserInfo(strUserInfo, name);
	pClientSocket->SaveUserInfo(userInfo);
	//判断是否重复登录
	//if (CUserInfo::LOGIN == userInfo.m_state)//登录
	//{
	//	m_csList.Lock();
	//	//不能名字重复
	//	POSITION pos1 = NULL;
	//	POSITION pos2 = NULL;
	//	for (pos1 = m_UserList.GetHeadPosition(); (pos2 = pos1)!=NULL;)
	//	{
	//		CClientSocket *pClient = (CClientSocket*)m_UserList.GetNext(pos1);
	//		if (pClient->m_UserInfo.m_strName == userInfo.m_strName)//名字重复
	//		{
	//			pClientSocket->Send(SERVMESSAGE, "用户重复登录！");	//发送消息
	//			m_csList.Unlock();
	//			return FALSE;			
	//		}			
	//	}
	//	m_csList.Unlock();

	//}else//用户更改状态
	{
		m_csList.Lock();
		BOOL bFinder = FALSE;
		POSITION pos;
		for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
		{
			CClientSocket *pClient = (CClientSocket*)m_UserList.GetNext(pos);
			if (pClient->m_UserInfo.m_strName == userInfo.m_strName)//修改状态
			{
				pClient->m_UserInfo.m_state = userInfo.m_state;
				bFinder = TRUE;
				break;
			}			
		}
		m_csList.Unlock();
		ASSERT(bFinder);	//必须找到该用户
	}

	UpdateListCtrl();		//更新服务器用户列表
	SendUserList(pClientSocket);			//发送用户列表	
	return TRUE;
}

/*
 * 解析用户信息
 */
CUserInfo CServerDlg::ParserUserInfo(const CString strUserInfo,char *name)
{
	CUserInfo userInfo;

	//string s(name);
	int nFind = strUserInfo.Find('|');
	ASSERT(-1 != nFind);
	strUserInfo.Left(nFind);		//名字
	//memcpy((void *)userInfo.m_strName, name, sizeof(name));
	//userInfo.m_strName = _T(name);//这里是char*转string
	if (strcmp(name, "0") == 0)
	{
		userInfo.m_strName = strUserInfo.Left(nFind);
	}
	else 
		userInfo.m_strName = _T(name);
	CString strRight = strUserInfo.Mid(nFind + 1);		//剩余字符串
	
	nFind = strRight.Find('|');
	userInfo.m_lIP = atol(strRight.Left(nFind));		//IP
		
	strRight = strRight.Mid(nFind + 1);
	nFind = strRight.Find('|');
	userInfo.m_uPort = atoi(strRight.Left(nFind));		//端口

	userInfo.m_state = (CUserInfo::USERSTATE)(atoi(strRight.Right(1)));//状态
	
	return userInfo;	
}
/*
 * 把用户信息转换为CString对象<名称|ip|port|state>
 */
void CServerDlg::MakeSendUserString( CString &strUserInfoList,int num )
{
	m_csList.Lock();
	int deskNum = num;
	U_Desk desk=pdesk->numtoip(num);
	POSITION pos = NULL;
	for(pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		
			CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
			in_addr userAddr;
			userAddr.S_un.S_addr = (long)pClientSocket->m_UserInfo.m_lIP;
			char *c_ip = inet_ntoa(userAddr);
			int Port = ntohs(pClientSocket->m_UserInfo.m_uPort);
			if (strcmp(c_ip,desk.user[0].ip)==0&&Port==desk.user[0].port)//如果是一桌的人 跟对应桌号的人的ip 和port比较  相同的话添加到用户表给他发过去
			{
			CUserInfo userInfo = pClientSocket->m_UserInfo;
			CString	strUserInfo;							//用户信息
			CString strUserName;							//名称
			strUserName = userInfo.m_strName;
			CString strUserIP;								//IP
			strUserIP.Format("%ld", userInfo.m_lIP);
			CString strUserPort;							//Port
			strUserPort.Format("%d", userInfo.m_uPort);
			CString strUserState;							//状态
			strUserState.Format("%d", (int)userInfo.m_state);
			strUserInfo = "<" + strUserName + "|"
				+ strUserIP + "|"
				+ strUserPort + "|"
				+ strUserState + ">";

			//加入列表
			strUserInfoList += strUserInfo;
		}
	if (strcmp(c_ip, desk.user[1].ip) == 0 && Port== desk.user[1].port)
	{ 
		CUserInfo userInfo = pClientSocket->m_UserInfo;
		CString	strUserInfo;							//用户信息
		CString strUserName;							//名称
		strUserName = userInfo.m_strName;
		CString strUserIP;								//IP
		strUserIP.Format("%ld", userInfo.m_lIP);
		CString strUserPort;							//Port
		strUserPort.Format("%d", userInfo.m_uPort);
		CString strUserState;							//状态
		strUserState.Format("%d", (int)userInfo.m_state);
		strUserInfo = "<" + strUserName + "|"
			+ strUserIP + "|"
			+ strUserPort + "|"
			+ strUserState + ">";

		//加入列表
		strUserInfoList += strUserInfo;
	}
	}

	m_csList.Unlock();
}
/*
 * 对超过64个客户端套接字的处理
 */
void CServerDlg::HandleOverClientNum( SOCKET sClient )
{
	int			nErrCode;//返回值	
	PACKETHDR	packetLen;//定义包头
	
	CString strMessage(_T("请稍候再登录服务器。"));

	packetLen.type = SERVMESSAGE;			//类型
	packetLen.len = strMessage.GetLength();	//数据长度

	//发送包头
	nErrCode = send(sClient, (char*)&packetLen, PACKETHDRLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("发送消息失败！");
		return ;
	}
	//发送包体
	nErrCode = send(sClient, strMessage, packetLen.len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("发送消息失败！");
		return ;
	}
	//关闭套接字
	closesocket(sClient);
	sClient = INVALID_SOCKET;
}

/*
 * 更新服务器界面
 */
void CServerDlg::UpdateListCtrl( void )
{
	//删除原来数据项
	m_csList.Lock();
	//m_ctlUserList.DeleteAllItems();
	
	POSITION	pos = NULL;		//位置变量
	int			nIndex = 0;		//序号
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		//取出用户信息
		CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
		CUserInfo userInfo = pClientSocket->m_UserInfo;		
		
		//第一列
		m_ctlUserList.InsertItem(nIndex, userInfo.m_strName, -1);//名称		
		//第二列
		LVITEM item;
		item.mask = LVIF_IMAGE;
		item.iItem = nIndex;
		item.iSubItem = 1;
		if (userInfo.m_state == CUserInfo::PLAY)				//游戏状态
		{
			item.iImage = 0;
		}else													//等待游戏状态
		{
			item.iImage = 1;
		}
		m_ctlUserList.SetItem(&item);
		
		//第三列
		in_addr userAddr;
		userAddr.S_un.S_addr = (long)userInfo.m_lIP;
		CString strUserAddr(inet_ntoa(userAddr));				//IP
		m_ctlUserList.SetItemText(nIndex, 2, strUserAddr);
		
		//第四列
		CString strUserPort;									//端口
		strUserPort.Format("%d",ntohs(userInfo.m_uPort));	
		m_ctlUserList.SetItemText(nIndex,3,strUserPort);	
		
		nIndex++;		
	}
	m_csList.Unlock();	
}

/*
 *	服务线程函数
 */
DWORD WINAPI CServerDlg::ServiceThread( void *pParam )
{
	PTHREADPARAM pThreadParam = (PTHREADPARAM)pParam;
	
	while(*pThreadParam->pRunning)//服务器运行状态
	{
		DWORD	dwIndex = 0;//返回值
		
		//等待网络事件
		if ((dwIndex = WSAWaitForMultipleEvents(*pThreadParam->pTotalEvent, 
												pThreadParam->pArrEvent, 
												FALSE, SERVER_WAIT_TIMEOUT, FALSE))
												== WSA_WAIT_FAILED)
		{
			AfxMessageBox("等待事件错误！");
			break;
		}		
	
		//超时处理
		if (WSA_WAIT_TIMEOUT == dwIndex)
		{
			Sleep(SERVER_WAIT_TIMEOUT);
			continue;
		}
		
		//发生网络事件
		WSANETWORKEVENTS networkEvents;//网络事件结构
		if (WSAEnumNetworkEvents(pThreadParam->pArrSocket[dwIndex - WSA_WAIT_EVENT_0], 
								pThreadParam->pArrEvent[dwIndex - WSA_WAIT_EVENT_0], 
								&networkEvents) == SOCKET_ERROR)
		{
			AfxMessageBox("网络事件结构错误！");
			break;
		}
		
		//FD_ACCEPT网络事件
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)//发生网络错误
			{
				AfxMessageBox("FD_ACCEPT_BIT网络事件错误！");
				break;
			}
			//通知主线程
			::SendMessage(pThreadParam->hServHwnd, WM_USER_ACCEPT, dwIndex, NULL);
		}		
		
		//发生FD_READ网络事件	
		if (networkEvents.lNetworkEvents & FD_READ )
		{
			if (networkEvents.iErrorCode[FD_READ_BIT] != 0)//发生网络错误
			{
				AfxMessageBox("FD_READ_BIT网络事件错误！");
				break;
			}else
			{
				//通知主线程
				::SendMessage(pThreadParam->hServHwnd, WM_USER_READ,dwIndex, NULL);
			}
		}		
		//发生FD_CLOSE网络事件	
		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{	
			if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)//发生网络错误
			{
				AfxMessageBox("FD_CLOSE_BIT网络事件错误！");
				break;
			}else
			{
				//通知主线程
				::SendMessage(pThreadParam->hServHwnd, WM_USER_CLOSE,dwIndex,NULL);	
			}			
		}
	}	

	SetEvent(pThreadParam->hEventExit);	//通知主线程
	delete pThreadParam;				//释放资源
	pThreadParam = NULL;	
	return 0;	
}

void CServerDlg::DeleteAllClientNodes( void )
{
	POSITION		pos = NULL;
	CClientSocket	*pClientSocket = NULL;
	
	m_csList.Lock();
	//遍历整个链表找到套接字对应的节点
	for (pos = m_UserList.GetHeadPosition(); pos != NULL;)
	{
		pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
		delete pClientSocket;

	}
	m_UserList.RemoveAll();
	m_csList.Unlock();
	pClientSocket = NULL;	
}

/*
 * FD_CLOSE事件响应函数
 */
afx_msg LRESULT CServerDlg::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	int nIndex = wParam;
	//删除该节点
	DeleteClientNode(m_arrSocket[nIndex - WSA_WAIT_EVENT_0]);
	//将该套接字和事件对象从数组中清除
	ClearSocketAndEventFromArr(nIndex - WSA_WAIT_EVENT_0);	
	return 0;
}

/*
 *	FD_READ事件响应函数 从客户端链表里查找对应的指针 然后从这个客户端接收 ,接手之后，更新用户链表
 */
afx_msg LRESULT CServerDlg::OnSocketRead( WPARAM wParam, LPARAM lParam )
{
	//获取客户端指针
	CClientSocket *pClientSocket = FindClientNode(
									m_arrSocket[wParam - WSA_WAIT_EVENT_0]);
	if (NULL != pClientSocket)
	{
		BOOL reVal;						//返回值
		reVal = pClientSocket->Recv();	//接收数据
		if (FALSE == reVal)				//操作失败或者用户重复登录
		{
			DeleteClientNode(m_arrSocket[wParam - WSA_WAIT_EVENT_0]);//删除该节点	
			//将该套接字和事件对象从数组中清除
			ClearSocketAndEventFromArr(wParam - WSA_WAIT_EVENT_0);	
			return -1;
		}
	}
	return 0;
}

/*
 * 接受客户端连接请求
 */
afx_msg LRESULT CServerDlg::OnSocketAccept( WPARAM wParam, LPARAM lParam )
{
	SOCKET sAccept = INVALID_SOCKET;//套接字
	ASSERT(m_sListen == m_arrSocket[wParam - WSA_WAIT_EVENT_0]);
	
	//接受连接请求
	if ((sAccept = accept(m_arrSocket[wParam - WSA_WAIT_EVENT_0],NULL, NULL)) == INVALID_SOCKET)
	{		
		AfxMessageBox("接受连接请求错误！");
		return -1;
	}
	
	//超过最大值
	if (m_nTotalEvent > WSA_MAXIMUM_WAIT_EVENTS)
	{
		HandleOverClientNum(sAccept);//对超过64个套接字的处理
		return 0;
	}			
	//为接受套接字创建事件对象
	if ((m_arrEvent[m_nTotalEvent] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		AfxMessageBox("为新建套接字创建套接字事件对象错误！");
		return 0;
	}
	m_arrSocket[m_nTotalEvent] = sAccept;//加入套接字数组
	
	//为套接字注册FD_READ FD_CLOSE网络事件
	if (WSAEventSelect(sAccept, m_arrEvent[m_nTotalEvent], FD_READ|FD_CLOSE) == SOCKET_ERROR)
	{
		AfxMessageBox("为新建套接字注册网络事件错误！");
		return 0;
	}
	
	//创建CClientSocket对象
	CClientSocket *pClient = new CClientSocket(this, sAccept, m_arrEvent[m_nTotalEvent]);
	
	m_csList.Lock();
	m_UserList.AddTail(pClient);//加入链表
	m_csList.Unlock();
	m_nTotalEvent++;			//总数加1
	return 0;
}