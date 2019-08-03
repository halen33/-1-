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
	BOOL 	Send(const u_short type, const CString &strData);	//��������
	BOOL	Recv(void);											//��������
	void	SaveUserInfo(const CUserInfo &userInfo);			//�����û���Ϣ

public:
	SOCKET		m_s;		//�׽���
	WSAEVENT	m_hEvent;	//�¼�������
	CUserInfo	m_UserInfo;	//�û���Ϣ

private:
	CClientSocket();
private:
	CServerDlg	*m_pServDlg;//CServerDlgָ��
};

#endif // !defined(AFX_CLIENTSOCKET_H__2E836D50_8979_42C9_9E9D_9F3DAE358C4F__INCLUDED_)
