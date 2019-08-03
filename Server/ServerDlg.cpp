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

	if(FALSE == Init())//��ʼ��������
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
 * ��ʼ��
 */
BOOL CServerDlg::Init(void)
{
	InitListCtrlSetting();		//��ʼ���б���ͼ�ؼ�

	if(!InitSocket())			//��ʼ���׽���
		return FALSE;

	return TRUE;
}

/*
 * ��ʼ���б���ͼ�ؼ�
 */
void CServerDlg::InitListCtrlSetting( void )
{
	m_pImageList = new CImageList();							//����ͼ���б����	
	//32*32��8λ����ʼ��2��ͼ��
	m_pImageList->Create(32, 32, ILC_COLOR8|ILC_MASK, 2, 2);	
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_USER_PLAY_ICON));//��Ϸͼ��
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_USER_WAIT_ICON));//�ȴ�ͼ��	
	
	
	m_ctlUserList.SetImageList(m_pImageList, LVSIL_SMALL);		//ͼ���б�ֵ���б�ؼ�
	m_ctlUserList.SetExtendedStyle(m_ctlUserList.GetExtendedStyle()
										|LVS_EX_SUBITEMIMAGES);	//������չ���
	
	//�����б�ͷ
	m_ctlUserList.InsertColumn(0, _T("����"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(1, _T("״̬"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(2, _T("��ַ"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(3, _T("�˿�"), LVCFMT_CENTER);
	
	//���ø��еĿ��
	CRect rect;
	m_ctlUserList.GetClientRect(&rect);					//��ȡ�ͻ������
	int nWidth = rect.Width();
	m_ctlUserList.SetColumnWidth(0, nWidth * 25 / 100);	//����25%
	m_ctlUserList.SetColumnWidth(1, nWidth * 20 / 100);	//״̬20%
	m_ctlUserList.SetColumnWidth(2, nWidth * 35 / 100);	//IP35%
	m_ctlUserList.SetColumnWidth(3, nWidth * 20 / 100);	//��ַ20%


}

/*
 * ��ʼ���׽���
 */
BOOL CServerDlg::InitSocket( void )
{
	int		reVal;				//����ֵ
	WORD	wVersionRequested;	//�����Windows Sockets ʵ�ְ汾
	WSADATA	wsaData;			//����Э�̽��

	wVersionRequested = MAKEWORD(2, 2);	
	reVal = WSAStartup( wVersionRequested, &wsaData );//��ʼ���׽��ֶ�̬��
	if ( 0 != reVal ) 
	{
		return FALSE;
	}
	
	//�����׽���
	m_sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�����׽���
	if (INVALID_SOCKET == m_sListen)
	{
		WSACleanup();
		return FALSE;
	}	

	//��������ַ�Ի���
	/*if (IDOK != m_servAddrDlg.DoModal())
	{	
		closesocket(m_sListen);
		WSACleanup();
		return FALSE;
	}*/

	return TRUE;
}


/*
 *	��������
 */
void CServerDlg::OnStartService() 
{	
	if (FALSE == BeginListen())//�������׽��ֿ�ʼ����
	{
		AfxMessageBox("����������ʧ�ܣ�");
		closesocket(m_sListen);
		WSACleanup();
		EndDialog(IDCANCEL);
		return;
	}

	m_bRuning = TRUE;									//���÷���������״̬
	//���������߳��˳��¼�����
	m_hEventExit = CreateEvent(NULL, TRUE, FALSE, NULL);//�ֶ����ã���ʼ��δ���š�״̬
	
	//�̺߳�������
	PTHREADPARAM pParam = new THREADPARAM;
	pParam->hServHwnd = this->GetSafeHwnd();
	pParam->pRunning = &m_bRuning;
	pParam->pTotalEvent = &m_nTotalEvent;
	pParam->pArrEvent = m_arrEvent;
	pParam->pArrSocket = m_arrSocket;
	pParam->hEventExit = m_hEventExit;

	//���������߳�
	DWORD dwThread;	
	HANDLE hServiceThread = CreateThread(NULL, 0, ServiceThread, pParam, 0, &dwThread);
	CloseHandle(hServiceThread);

	//����������������ť��Ч
	(CButton*)GetDlgItem(IDOK)->EnableWindow(FALSE);	
}

/*
 * �������˳�
 */
void CServerDlg::OnStopService() 
{
	//���÷�����״̬
	m_bRuning = FALSE;

	WaitForSingleObject(m_hEventExit, INFINITE);//�ȴ��߳��˳�
	
	DeleteAllClientNodes();						//ɾ�����пͻ��˽ڵ�

	delete m_pImageList;
	m_pImageList = NULL;
	CloseHandle(m_hEventExit);		//�ر��߳��˳��¼����� 
	WSACloseEvent(m_arrEvent[0]);	//�رռ����׽��ֶ�Ӧ�¼�
	closesocket(m_sListen);			//�رռ����׽���
	WSACleanup();					//�ͷ��׽�����Դ
	CDialog::OnCancel();
}

/*
 *	��������ʼ����
 */
BOOL CServerDlg::BeginListen( void )
{
	int	reVal;										//����ֵ

	m_arrSocket[m_nTotalEvent]= m_sListen;//�������׽��ּ��뼯��
	m_arrEvent[m_nTotalEvent] = WSACreateEvent();	//�����׽����¼�����
	
	//Ϊ�����׽���ע��FD_ACCEPT�� FD_CLOSE�����¼�
	if(SOCKET_ERROR == WSAEventSelect(m_sListen, m_arrEvent[m_nTotalEvent], 
						FD_ACCEPT | FD_CLOSE))
		return FALSE;
	m_nTotalEvent++;//�׽����¼������1
	
	//�������׽��ֵ�ַ
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = /*htonl(m_servAddrDlg.m_dwServIP)*/INADDR_ANY;
	servAddr.sin_port = htons(/*m_servAddrDlg.m_shServPort*/SERVERPORT);
	
	//��
	reVal = bind(m_sListen,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == reVal)
		return FALSE;
	
	//����
	reVal = listen(m_sListen, SOMAXCONN);
	if (SOCKET_ERROR == reVal)
		return FALSE;
	
	return TRUE;
}

/*
 *	����׽��ֺ��¼�����
 */
void CServerDlg::ClearSocketAndEventFromArr( const int nIndex )
{	
	//�����׽��ֺ��¼���������������
	for (int i = nIndex - WSA_WAIT_EVENT_0; i < m_nTotalEvent; i++)
	{
		m_arrEvent[i] = m_arrEvent[i + 1];	
		m_arrSocket[i] = m_arrSocket[i + 1];
	}
	//������1		
	m_nTotalEvent--;
}

/*
 * �ӹ���ͻ��������в��Ҷ�Ӧ��CClientSocketָ��
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
 * �ӹ���ͻ�������ɾ���ڵ�
 */
void CServerDlg::DeleteClientNode( SOCKET s )
{
	POSITION		pos1 = NULL;			//��ʱ����
	POSITION		pos2 = NULL;			//��ʱ����
	CClientSocket	*pClientSocket = NULL;	//CClientSocketָ��
	BOOL			bFinder = FALSE;		//�Ƿ��ҵ����׽���

	m_csList.Lock();//�����ٽ���
	//�������������ҵ��׽��ֶ�Ӧ�Ľڵ�
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
	m_UserList.RemoveAt(pos2);	//��������ɾ���ýڵ�
	delete pClientSocket;		//�ͷ��ڴ�
	pClientSocket = NULL;
	m_csList.Unlock();			//�뿪�ٽ���
	
	UpdateListCtrl();			//���·������û��б�
	//SendUserList();				//�����û��б�	����������ˣ���Ҫ������һ�����˷��µ��б�������ʱ�Ȳ�д
}


/*
 * �����û��б� ����Ҫ�� ֻ������һ�����˹�ȥ
 */
void CServerDlg::SendUserList( CClientSocket* pClient)
{
	CString strUserInfoList;			//��ʱ����
	int DeskNum;

	in_addr userAddr;
	userAddr.S_un.S_addr = (long)pClient->m_UserInfo.m_lIP;
	char *c_ip = inet_ntoa(userAddr);
	int Port = ntohs(pClient->m_UserInfo.m_uPort);
	for (int i = 0; i < 100; i++)
	{
		//��������������� �ҵ��˵õ�����
		if( (DeskNum=pdesk->findUser(c_ip, Port) )!= -1)
		{
			break;
		}
	}
	MakeSendUserString(strUserInfoList, DeskNum);//׼���û��б�
	CClientSocket *D_Client = FindClientNode(pClient->m_s);
	//�õ�socket,Ȼ���ҵ�����һ���˵�ip port ����user list�ҵ����˷���ȥ
	m_csList.Lock();
	

	//�����û��б�
	POSITION pos = NULL;
	//������˷���ȥ׼���õ��û��б�
	D_Client->Send(USERINFOR, strUserInfoList);
	//for (pos = m_UserList.GetHeadPosition(); NULL != pos ;)
	//{
	//	//if(m_UserList.GetNext(pos)->)
	//	
	//	CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
	//	

	//	//pClientSocket->m_UserInfo.m_lIP//����ͻ��˵�ip �� port
	//	pClientSocket->Send(USERINFOR, strUserInfoList);	
	//}
	m_csList.Unlock();
}


/*
 * �����û��б� �յ�֮������û������ٷ���ȥ
 */
BOOL CServerDlg::UpdateUserList(const CString strUserInfo, CClientSocket* pClientSocket)
{
	//�����û���Ϣ
	//�����û���Ϣ��ע��˴�����Ҫ�������ϻ�á�
	int k;//����
	char *name=(char *)malloc(sizeof(50));
	//memset(name, 0, sizeof(name));
	U_Desk desk;
	CUserInfo userInfo = ParserUserInfo(strUserInfo,"0");//��Ҫ����
	pClientSocket->SaveUserInfo(userInfo);//�����û���Ϣ ������ ����ip��port �ȱ���ip port �ֿ��������֡�
	//ipҪ����һ����ת���ٱȽ�

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
	//�ж��Ƿ��ظ���¼
	//if (CUserInfo::LOGIN == userInfo.m_state)//��¼
	//{
	//	m_csList.Lock();
	//	//���������ظ�
	//	POSITION pos1 = NULL;
	//	POSITION pos2 = NULL;
	//	for (pos1 = m_UserList.GetHeadPosition(); (pos2 = pos1)!=NULL;)
	//	{
	//		CClientSocket *pClient = (CClientSocket*)m_UserList.GetNext(pos1);
	//		if (pClient->m_UserInfo.m_strName == userInfo.m_strName)//�����ظ�
	//		{
	//			pClientSocket->Send(SERVMESSAGE, "�û��ظ���¼��");	//������Ϣ
	//			m_csList.Unlock();
	//			return FALSE;			
	//		}			
	//	}
	//	m_csList.Unlock();

	//}else//�û�����״̬
	{
		m_csList.Lock();
		BOOL bFinder = FALSE;
		POSITION pos;
		for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
		{
			CClientSocket *pClient = (CClientSocket*)m_UserList.GetNext(pos);
			if (pClient->m_UserInfo.m_strName == userInfo.m_strName)//�޸�״̬
			{
				pClient->m_UserInfo.m_state = userInfo.m_state;
				bFinder = TRUE;
				break;
			}			
		}
		m_csList.Unlock();
		ASSERT(bFinder);	//�����ҵ����û�
	}

	UpdateListCtrl();		//���·������û��б�
	SendUserList(pClientSocket);			//�����û��б�	
	return TRUE;
}

/*
 * �����û���Ϣ
 */
CUserInfo CServerDlg::ParserUserInfo(const CString strUserInfo,char *name)
{
	CUserInfo userInfo;

	//string s(name);
	int nFind = strUserInfo.Find('|');
	ASSERT(-1 != nFind);
	strUserInfo.Left(nFind);		//����
	//memcpy((void *)userInfo.m_strName, name, sizeof(name));
	//userInfo.m_strName = _T(name);//������char*תstring
	if (strcmp(name, "0") == 0)
	{
		userInfo.m_strName = strUserInfo.Left(nFind);
	}
	else 
		userInfo.m_strName = _T(name);
	CString strRight = strUserInfo.Mid(nFind + 1);		//ʣ���ַ���
	
	nFind = strRight.Find('|');
	userInfo.m_lIP = atol(strRight.Left(nFind));		//IP
		
	strRight = strRight.Mid(nFind + 1);
	nFind = strRight.Find('|');
	userInfo.m_uPort = atoi(strRight.Left(nFind));		//�˿�

	userInfo.m_state = (CUserInfo::USERSTATE)(atoi(strRight.Right(1)));//״̬
	
	return userInfo;	
}
/*
 * ���û���Ϣת��ΪCString����<����|ip|port|state>
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
			if (strcmp(c_ip,desk.user[0].ip)==0&&Port==desk.user[0].port)//�����һ������ ����Ӧ���ŵ��˵�ip ��port�Ƚ�  ��ͬ�Ļ���ӵ��û����������ȥ
			{
			CUserInfo userInfo = pClientSocket->m_UserInfo;
			CString	strUserInfo;							//�û���Ϣ
			CString strUserName;							//����
			strUserName = userInfo.m_strName;
			CString strUserIP;								//IP
			strUserIP.Format("%ld", userInfo.m_lIP);
			CString strUserPort;							//Port
			strUserPort.Format("%d", userInfo.m_uPort);
			CString strUserState;							//״̬
			strUserState.Format("%d", (int)userInfo.m_state);
			strUserInfo = "<" + strUserName + "|"
				+ strUserIP + "|"
				+ strUserPort + "|"
				+ strUserState + ">";

			//�����б�
			strUserInfoList += strUserInfo;
		}
	if (strcmp(c_ip, desk.user[1].ip) == 0 && Port== desk.user[1].port)
	{ 
		CUserInfo userInfo = pClientSocket->m_UserInfo;
		CString	strUserInfo;							//�û���Ϣ
		CString strUserName;							//����
		strUserName = userInfo.m_strName;
		CString strUserIP;								//IP
		strUserIP.Format("%ld", userInfo.m_lIP);
		CString strUserPort;							//Port
		strUserPort.Format("%d", userInfo.m_uPort);
		CString strUserState;							//״̬
		strUserState.Format("%d", (int)userInfo.m_state);
		strUserInfo = "<" + strUserName + "|"
			+ strUserIP + "|"
			+ strUserPort + "|"
			+ strUserState + ">";

		//�����б�
		strUserInfoList += strUserInfo;
	}
	}

	m_csList.Unlock();
}
/*
 * �Գ���64���ͻ����׽��ֵĴ���
 */
void CServerDlg::HandleOverClientNum( SOCKET sClient )
{
	int			nErrCode;//����ֵ	
	PACKETHDR	packetLen;//�����ͷ
	
	CString strMessage(_T("���Ժ��ٵ�¼��������"));

	packetLen.type = SERVMESSAGE;			//����
	packetLen.len = strMessage.GetLength();	//���ݳ���

	//���Ͱ�ͷ
	nErrCode = send(sClient, (char*)&packetLen, PACKETHDRLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("������Ϣʧ�ܣ�");
		return ;
	}
	//���Ͱ���
	nErrCode = send(sClient, strMessage, packetLen.len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("������Ϣʧ�ܣ�");
		return ;
	}
	//�ر��׽���
	closesocket(sClient);
	sClient = INVALID_SOCKET;
}

/*
 * ���·���������
 */
void CServerDlg::UpdateListCtrl( void )
{
	//ɾ��ԭ��������
	m_csList.Lock();
	//m_ctlUserList.DeleteAllItems();
	
	POSITION	pos = NULL;		//λ�ñ���
	int			nIndex = 0;		//���
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		//ȡ���û���Ϣ
		CClientSocket *pClientSocket = (CClientSocket*)m_UserList.GetNext(pos);
		CUserInfo userInfo = pClientSocket->m_UserInfo;		
		
		//��һ��
		m_ctlUserList.InsertItem(nIndex, userInfo.m_strName, -1);//����		
		//�ڶ���
		LVITEM item;
		item.mask = LVIF_IMAGE;
		item.iItem = nIndex;
		item.iSubItem = 1;
		if (userInfo.m_state == CUserInfo::PLAY)				//��Ϸ״̬
		{
			item.iImage = 0;
		}else													//�ȴ���Ϸ״̬
		{
			item.iImage = 1;
		}
		m_ctlUserList.SetItem(&item);
		
		//������
		in_addr userAddr;
		userAddr.S_un.S_addr = (long)userInfo.m_lIP;
		CString strUserAddr(inet_ntoa(userAddr));				//IP
		m_ctlUserList.SetItemText(nIndex, 2, strUserAddr);
		
		//������
		CString strUserPort;									//�˿�
		strUserPort.Format("%d",ntohs(userInfo.m_uPort));	
		m_ctlUserList.SetItemText(nIndex,3,strUserPort);	
		
		nIndex++;		
	}
	m_csList.Unlock();	
}

/*
 *	�����̺߳���
 */
DWORD WINAPI CServerDlg::ServiceThread( void *pParam )
{
	PTHREADPARAM pThreadParam = (PTHREADPARAM)pParam;
	
	while(*pThreadParam->pRunning)//����������״̬
	{
		DWORD	dwIndex = 0;//����ֵ
		
		//�ȴ������¼�
		if ((dwIndex = WSAWaitForMultipleEvents(*pThreadParam->pTotalEvent, 
												pThreadParam->pArrEvent, 
												FALSE, SERVER_WAIT_TIMEOUT, FALSE))
												== WSA_WAIT_FAILED)
		{
			AfxMessageBox("�ȴ��¼�����");
			break;
		}		
	
		//��ʱ����
		if (WSA_WAIT_TIMEOUT == dwIndex)
		{
			Sleep(SERVER_WAIT_TIMEOUT);
			continue;
		}
		
		//���������¼�
		WSANETWORKEVENTS networkEvents;//�����¼��ṹ
		if (WSAEnumNetworkEvents(pThreadParam->pArrSocket[dwIndex - WSA_WAIT_EVENT_0], 
								pThreadParam->pArrEvent[dwIndex - WSA_WAIT_EVENT_0], 
								&networkEvents) == SOCKET_ERROR)
		{
			AfxMessageBox("�����¼��ṹ����");
			break;
		}
		
		//FD_ACCEPT�����¼�
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)//�����������
			{
				AfxMessageBox("FD_ACCEPT_BIT�����¼�����");
				break;
			}
			//֪ͨ���߳�
			::SendMessage(pThreadParam->hServHwnd, WM_USER_ACCEPT, dwIndex, NULL);
		}		
		
		//����FD_READ�����¼�	
		if (networkEvents.lNetworkEvents & FD_READ )
		{
			if (networkEvents.iErrorCode[FD_READ_BIT] != 0)//�����������
			{
				AfxMessageBox("FD_READ_BIT�����¼�����");
				break;
			}else
			{
				//֪ͨ���߳�
				::SendMessage(pThreadParam->hServHwnd, WM_USER_READ,dwIndex, NULL);
			}
		}		
		//����FD_CLOSE�����¼�	
		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{	
			if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)//�����������
			{
				AfxMessageBox("FD_CLOSE_BIT�����¼�����");
				break;
			}else
			{
				//֪ͨ���߳�
				::SendMessage(pThreadParam->hServHwnd, WM_USER_CLOSE,dwIndex,NULL);	
			}			
		}
	}	

	SetEvent(pThreadParam->hEventExit);	//֪ͨ���߳�
	delete pThreadParam;				//�ͷ���Դ
	pThreadParam = NULL;	
	return 0;	
}

void CServerDlg::DeleteAllClientNodes( void )
{
	POSITION		pos = NULL;
	CClientSocket	*pClientSocket = NULL;
	
	m_csList.Lock();
	//�������������ҵ��׽��ֶ�Ӧ�Ľڵ�
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
 * FD_CLOSE�¼���Ӧ����
 */
afx_msg LRESULT CServerDlg::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	int nIndex = wParam;
	//ɾ���ýڵ�
	DeleteClientNode(m_arrSocket[nIndex - WSA_WAIT_EVENT_0]);
	//�����׽��ֺ��¼���������������
	ClearSocketAndEventFromArr(nIndex - WSA_WAIT_EVENT_0);	
	return 0;
}

/*
 *	FD_READ�¼���Ӧ���� �ӿͻ�����������Ҷ�Ӧ��ָ�� Ȼ�������ͻ��˽��� ,����֮�󣬸����û�����
 */
afx_msg LRESULT CServerDlg::OnSocketRead( WPARAM wParam, LPARAM lParam )
{
	//��ȡ�ͻ���ָ��
	CClientSocket *pClientSocket = FindClientNode(
									m_arrSocket[wParam - WSA_WAIT_EVENT_0]);
	if (NULL != pClientSocket)
	{
		BOOL reVal;						//����ֵ
		reVal = pClientSocket->Recv();	//��������
		if (FALSE == reVal)				//����ʧ�ܻ����û��ظ���¼
		{
			DeleteClientNode(m_arrSocket[wParam - WSA_WAIT_EVENT_0]);//ɾ���ýڵ�	
			//�����׽��ֺ��¼���������������
			ClearSocketAndEventFromArr(wParam - WSA_WAIT_EVENT_0);	
			return -1;
		}
	}
	return 0;
}

/*
 * ���ܿͻ�����������
 */
afx_msg LRESULT CServerDlg::OnSocketAccept( WPARAM wParam, LPARAM lParam )
{
	SOCKET sAccept = INVALID_SOCKET;//�׽���
	ASSERT(m_sListen == m_arrSocket[wParam - WSA_WAIT_EVENT_0]);
	
	//������������
	if ((sAccept = accept(m_arrSocket[wParam - WSA_WAIT_EVENT_0],NULL, NULL)) == INVALID_SOCKET)
	{		
		AfxMessageBox("���������������");
		return -1;
	}
	
	//�������ֵ
	if (m_nTotalEvent > WSA_MAXIMUM_WAIT_EVENTS)
	{
		HandleOverClientNum(sAccept);//�Գ���64���׽��ֵĴ���
		return 0;
	}			
	//Ϊ�����׽��ִ����¼�����
	if ((m_arrEvent[m_nTotalEvent] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		AfxMessageBox("Ϊ�½��׽��ִ����׽����¼��������");
		return 0;
	}
	m_arrSocket[m_nTotalEvent] = sAccept;//�����׽�������
	
	//Ϊ�׽���ע��FD_READ FD_CLOSE�����¼�
	if (WSAEventSelect(sAccept, m_arrEvent[m_nTotalEvent], FD_READ|FD_CLOSE) == SOCKET_ERROR)
	{
		AfxMessageBox("Ϊ�½��׽���ע�������¼�����");
		return 0;
	}
	
	//����CClientSocket����
	CClientSocket *pClient = new CClientSocket(this, sAccept, m_arrEvent[m_nTotalEvent]);
	
	m_csList.Lock();
	m_UserList.AddTail(pClient);//��������
	m_csList.Unlock();
	m_nTotalEvent++;			//������1
	return 0;
}