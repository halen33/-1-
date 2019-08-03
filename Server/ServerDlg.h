// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__CC9BB1E4_1779_4786_A820_CB54826385DE__INCLUDED_)
#define AFX_SERVERDLG_H__CC9BB1E4_1779_4786_A820_CB54826385DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ServerAddrDlg.h"
#include "ClientSocket.h"
#include"Desk.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CUserInfo;

class CServerDlg : public CDialog
{
	// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListCtrl	m_ctlUserList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();							//��ʼ���Ի���
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartService();							//������������ť��Ӧ����
	afx_msg void OnStopService();							//�˳���������ť��Ӧ����
	afx_msg LRESULT OnSocketAccept(WPARAM wParam, LPARAM lParam);	//FD_ACCEPT�����¼��Զ�����Ϣ
	afx_msg LRESULT OnSocketRead(WPARAM wParam, LPARAM lParam);		//FD_READ�����¼��Զ�����Ϣ
//	afx_msg LRESULT OnSocketRead(WPARAM wParam, LPARAM lParam);		//����һ�������¼���ȡsocket
	afx_msg LRESULT OnSocketClose(WPARAM wParam, LPARAM lParam);	//FD_CLOSE�����¼��Զ�����Ϣ
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL	Init(void);										//��ʼ��
	BOOL	UpdateUserList(const CString strUserInfo,		//�����û��б�
						CClientSocket* pClientSocket);		
	void	UpdateListCtrl(void);							//���·����������û��б�
	
private:
	BOOL	InitSocket(void);								//��ʼ���׽���
	void	InitListCtrlSetting(void);						//��ʼ���б���ͼ�ؼ�
	BOOL	BeginListen(void);								//��������ʼ����
	
	void	ClearSocketAndEventFromArr(const int nIndex);	//ɾ���ͻ����׽���
	void	DeleteClientNode(SOCKET s);						//�ӹ���ͻ�������ɾ���ڵ�
	void	DeleteAllClientNodes(void);						//ɾ�����пͻ��˽ڵ�
	CClientSocket* FindClientNode(const SOCKET s);			//�ӹ���ͻ��������в��Ҷ�Ӧ��CClientSocketָ��
	
	void	SendUserList(CClientSocket* );								//�����û��б�
	CUserInfo	ParserUserInfo(const CString strUserInfo,char *name);	//�����û���Ϣ
	void	MakeSendUserString(CString &strUserInfoList,int num);	//���û���Ϣת��ΪCString����
	void	HandleOverClientNum(SOCKET sClient);			//�Գ���64���ͻ����׽��ֵĴ���
	
	static DWORD WINAPI ServiceThread(void *pParam);		//�������̺߳���
	
public:
	
	SOCKET			m_sListen;						//�����׽���
	WSAEVENT		m_arrEvent[MAX_NUM_EVENTS];		//�׽����¼���������
	SOCKET			m_arrSocket[MAX_NUM_EVENTS];	//�׽�������
	WORD			m_nTotalEvent;					//�����¼�����
	CImageList		*m_pImageList;					//ͼ���б����
	BOOL			m_bRuning;						//����������״̬
	HANDLE			m_hEventExit;					//�������˳��¼�
	
private:
	CObList				m_UserList;					//�û�����
	CServerAddrDlg		m_servAddrDlg;				//��������ַ�Ի���
	CCriticalSection	m_csList;					//�ٽ�������
	
	Desk *pdesk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__CC9BB1E4_1779_4786_A820_CB54826385DE__INCLUDED_)
