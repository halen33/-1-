// ClientSocket.h: interface for the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__2E836D50_8979_42C9_9E9D_9F3DAE358C4F__INCLUDED_)
#define AFX_CLIENTSOCKET_H__2E836D50_8979_42C9_9E9D_9F3DAE358C4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "UserInfo.h"

class CServerDlg;


class CClientSocket : public CObject  
{
public:
	CClientSocket(CServerDlg *pServDlg, SOCKET s, WSAEVENT hEvent);
	virtual ~CClientSocket();

public:
	BOOL 	Send(const u_short type, const CString &strData);	//发送数据
	BOOL	Recv(void);											//接收数据
	void	SaveUserInfo(const CUserInfo &userInfo);			//保存用户信息

public:
	SOCKET		m_s;		//套接字
	WSAEVENT	m_hEvent;	//事件对象句柄
	CUserInfo	m_UserInfo;	//用户信息

private:
	CClientSocket();
private:
	CServerDlg	*m_pServDlg;//CServerDlg指针
};

#endif // !defined(AFX_CLIENTSOCKET_H__2E836D50_8979_42C9_9E9D_9F3DAE358C4F__INCLUDED_)
