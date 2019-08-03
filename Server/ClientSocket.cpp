// ClientSocket.cpp: implementation of the CClientSocket class.
// Download by http://www.NewXing.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "ClientSocket.h"
#include "ServerDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
 * 
 */
CClientSocket::CClientSocket(CServerDlg *pServDlg, SOCKET s , WSAEVENT hEvent)
{
	m_pServDlg = pServDlg;
	m_s = s;
	m_hEvent = hEvent;
}

/*
 * 
 */
CClientSocket::~CClientSocket()
{
	closesocket(m_s);					//关闭套接字
	WSAEventSelect(m_s, m_hEvent, 0);	//取消网络事件
	WSACloseEvent(m_hEvent);			//释放事件对象
}

/*
 * 接收数据
 */
BOOL CClientSocket::Recv( void )
{
	int reVal;							//返回值
	WSAEventSelect(m_s, m_hEvent, 0);	//取消网络事件

	//获取数据包体的长度
	PACKETHDR packetHdr;			
	reVal = recv(m_s, (char*)&packetHdr, PACKETHDRLEN, 0);
	
	if ( 0 == reVal)
	{
		return FALSE;
	}else if (SOCKET_ERROR == reVal)//网络错误
	{
		int nErrCode = WSAGetLastError();
		if (WSAEWOULDBLOCK == nErrCode)
		{
			return TRUE;
		}
		AfxMessageBox("接收用户列表错误！");
		return FALSE;
	}else
	{
		CString	strUserInfo;			//用户信息
		int	nTotalLen = 0;				//已经读取字符数量
		int	nDataLen = packetHdr.len;	//数据长度
		int	nReadLen = 0;				//每次读取字符数量 
		while ( nTotalLen != nDataLen )
		{		
			char cRecv;								//接收字符
			nReadLen = recv(m_s, &cRecv, 1,0);		//每次接收一个字符
			if (SOCKET_ERROR == nReadLen)			//网络错误
			{
				if (WSAEWOULDBLOCK == WSAGetLastError())
				{
					continue;
				}
				AfxMessageBox(_T("读取客户端数据失败！"));
				reVal = FALSE;
			}else if (0 == nReadLen)
			{
				AfxMessageBox(_T("客户端关闭了连接！"));
				reVal = FALSE;
			}else if (nReadLen > 0)
			{
				if ('<' == cRecv)			//开始字符
				{				
					strUserInfo.Empty();
					
				}else if ('>' == cRecv)		//结束字符
				{					
					break;
				}else
				{
					strUserInfo += cRecv;	//添加字符
				}			
				nTotalLen += nReadLen;
			}		
		}

		//注册网络事件
		WSAEventSelect(m_s, m_hEvent, FD_READ|FD_CLOSE);
		//更新用户列表
		return (m_pServDlg->UpdateUserList(strUserInfo, this));
	}
	
	return TRUE;
}

/*
 * 发送数据
 */
BOOL CClientSocket::Send(const u_short type, const CString &strData)
{
	ASSERT(!strData.IsEmpty());
	
	int			nErrCode;				//返回值	
	PACKETHDR	packetHdr;				//定义包头

	packetHdr.type = type;				//类型
	packetHdr.len = strData.GetLength();//数据长度

	//发送包头
	nErrCode = send(m_s, (char*)&packetHdr, PACKETHDRLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("发送用户列表错误！");
		return FALSE;
	}
	//发送包体
	nErrCode = send(m_s, strData, packetHdr.len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("发送用户列表错误！");
		return FALSE;
	}
	return TRUE;	
}

/*
 * 保存用户信息
 */
void CClientSocket::SaveUserInfo( const CUserInfo &userInfo )
{
	m_UserInfo = userInfo;
}