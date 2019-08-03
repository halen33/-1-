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
	closesocket(m_s);					//�ر��׽���
	WSAEventSelect(m_s, m_hEvent, 0);	//ȡ�������¼�
	WSACloseEvent(m_hEvent);			//�ͷ��¼�����
}

/*
 * ��������
 */
BOOL CClientSocket::Recv( void )
{
	int reVal;							//����ֵ
	WSAEventSelect(m_s, m_hEvent, 0);	//ȡ�������¼�

	//��ȡ���ݰ���ĳ���
	PACKETHDR packetHdr;			
	reVal = recv(m_s, (char*)&packetHdr, PACKETHDRLEN, 0);
	
	if ( 0 == reVal)
	{
		return FALSE;
	}else if (SOCKET_ERROR == reVal)//�������
	{
		int nErrCode = WSAGetLastError();
		if (WSAEWOULDBLOCK == nErrCode)
		{
			return TRUE;
		}
		AfxMessageBox("�����û��б����");
		return FALSE;
	}else
	{
		CString	strUserInfo;			//�û���Ϣ
		int	nTotalLen = 0;				//�Ѿ���ȡ�ַ�����
		int	nDataLen = packetHdr.len;	//���ݳ���
		int	nReadLen = 0;				//ÿ�ζ�ȡ�ַ����� 
		while ( nTotalLen != nDataLen )
		{		
			char cRecv;								//�����ַ�
			nReadLen = recv(m_s, &cRecv, 1,0);		//ÿ�ν���һ���ַ�
			if (SOCKET_ERROR == nReadLen)			//�������
			{
				if (WSAEWOULDBLOCK == WSAGetLastError())
				{
					continue;
				}
				AfxMessageBox(_T("��ȡ�ͻ�������ʧ�ܣ�"));
				reVal = FALSE;
			}else if (0 == nReadLen)
			{
				AfxMessageBox(_T("�ͻ��˹ر������ӣ�"));
				reVal = FALSE;
			}else if (nReadLen > 0)
			{
				if ('<' == cRecv)			//��ʼ�ַ�
				{				
					strUserInfo.Empty();
					
				}else if ('>' == cRecv)		//�����ַ�
				{					
					break;
				}else
				{
					strUserInfo += cRecv;	//����ַ�
				}			
				nTotalLen += nReadLen;
			}		
		}

		//ע�������¼�
		WSAEventSelect(m_s, m_hEvent, FD_READ|FD_CLOSE);
		//�����û��б�
		return (m_pServDlg->UpdateUserList(strUserInfo, this));
	}
	
	return TRUE;
}

/*
 * ��������
 */
BOOL CClientSocket::Send(const u_short type, const CString &strData)
{
	ASSERT(!strData.IsEmpty());
	
	int			nErrCode;				//����ֵ	
	PACKETHDR	packetHdr;				//�����ͷ

	packetHdr.type = type;				//����
	packetHdr.len = strData.GetLength();//���ݳ���

	//���Ͱ�ͷ
	nErrCode = send(m_s, (char*)&packetHdr, PACKETHDRLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("�����û��б����");
		return FALSE;
	}
	//���Ͱ���
	nErrCode = send(m_s, strData, packetHdr.len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox("�����û��б����");
		return FALSE;
	}
	return TRUE;	
}

/*
 * �����û���Ϣ
 */
void CClientSocket::SaveUserInfo( const CUserInfo &userInfo )
{
	m_UserInfo = userInfo;
}