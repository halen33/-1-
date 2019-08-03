// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0BEEB181_8066_45F5_AEE1_C60CC3DC99E6__INCLUDED_)
#define AFX_STDAFX_H__0BEEB181_8066_45F5_AEE1_C60CC3DC99E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls



//�ó�������
//header
#include <WINSOCK2.H>
#include <afxmt.h>
#pragma comment(lib, "ws2_32.lib")

//����
#define		SERVMESSAGE			200							//��������Ϣ
#define		USERINFOR			201							//�û���Ϣ

//�Զ�����Ϣ
#define WM_USER_ACCEPT		(WM_USER + 200)//FD_ACCEPT
#define WM_USER_READ		(WM_USER + 201)//FD_READ
#define WM_USER_CLOSE		(WM_USER + 202)//FD_CLOSE


//MACRO
#define		SERVERPORT			5559						//�������˿�
#define		MAX_NUM_EVENTS		WSA_MAXIMUM_WAIT_EVENTS		//����¼���������
#define		SERVER_WAIT_TIMEOUT	100							//˯��ʱ��

//�ṹ��С
#define		CLIENTPACKETLEN		(sizeof(CLINETPACKET))		//�ͻ������ݰ�����
#define		PACKETHDRLEN		(sizeof(PACKETHDR))			//��ͷ����

//��������ͻ��˷������ݰ�ͷ
typedef struct _packethdr
{
	u_short	type;	//����
	u_short	len;	//���ݰ�����(����)
}PACKETHDR, *PPACKETHDR;

//�̺߳�������
typedef struct _threadparam
{
	HWND	hServHwnd;		//�����ھ��
	BOOL	*pRunning;		//�߳�����״̬
	WORD	*pTotalEvent;	//�¼���������
	WSAEVENT*pArrEvent;		//�¼���������
	SOCKET	*pArrSocket;	//�׽��־������
	HANDLE	hEventExit;		//�߳��˳��¼����
}THREADPARAM, *PTHREADPARAM;



static long GetValidIp()
	{
		char szName[20] = {0};
		struct hostent *remoteHost = NULL;
		long ip;
		struct in_addr addr;

		//1.���������
		if(!gethostname(szName,sizeof(szName)))
		{
				//2.IPLIST		
			remoteHost = gethostbyname(szName);
			int i = 0;
			while (remoteHost->h_addr_list[i] != 0) {
                 addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
				 ip= addr.s_addr ;
            }
		}
		return ip;
	}





#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0BEEB181_8066_45F5_AEE1_C60CC3DC99E6__INCLUDED_)
