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

	virtual BOOL OnInitDialog();							//初始化对话框
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartService();							//启动服务器按钮响应函数
	afx_msg void OnStopService();							//退出服务器按钮响应函数
	afx_msg LRESULT OnSocketAccept(WPARAM wParam, LPARAM lParam);	//FD_ACCEPT网络事件自定义消息
	afx_msg LRESULT OnSocketRead(WPARAM wParam, LPARAM lParam);		//FD_READ网络事件自定义消息
//	afx_msg LRESULT OnSocketRead(WPARAM wParam, LPARAM lParam);		//定义一个网络事件获取socket
	afx_msg LRESULT OnSocketClose(WPARAM wParam, LPARAM lParam);	//FD_CLOSE网络事件自定义消息
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL	Init(void);										//初始化
	BOOL	UpdateUserList(const CString strUserInfo,		//更新用户列表
						CClientSocket* pClientSocket);		
	void	UpdateListCtrl(void);							//更新服务器界面用户列表
	
private:
	BOOL	InitSocket(void);								//初始化套结字
	void	InitListCtrlSetting(void);						//初始化列表视图控件
	BOOL	BeginListen(void);								//服务器开始监听
	
	void	ClearSocketAndEventFromArr(const int nIndex);	//删除客户端套接字
	void	DeleteClientNode(SOCKET s);						//从管理客户端链表删除节点
	void	DeleteAllClientNodes(void);						//删除所有客户端节点
	CClientSocket* FindClientNode(const SOCKET s);			//从管理客户端链表中查找对应的CClientSocket指针
	
	void	SendUserList(CClientSocket* );								//发送用户列表
	CUserInfo	ParserUserInfo(const CString strUserInfo,char *name);	//解析用户信息
	void	MakeSendUserString(CString &strUserInfoList,int num);	//把用户信息转换为CString对象
	void	HandleOverClientNum(SOCKET sClient);			//对超过64个客户端套接字的处理
	
	static DWORD WINAPI ServiceThread(void *pParam);		//服务器线程函数
	
public:
	
	SOCKET			m_sListen;						//监听套接字
	WSAEVENT		m_arrEvent[MAX_NUM_EVENTS];		//套接字事件对象数组
	SOCKET			m_arrSocket[MAX_NUM_EVENTS];	//套接字数组
	WORD			m_nTotalEvent;					//网络事件数量
	CImageList		*m_pImageList;					//图像列表对象
	BOOL			m_bRuning;						//服务器运行状态
	HANDLE			m_hEventExit;					//服务器退出事件
	
private:
	CObList				m_UserList;					//用户链表
	CServerAddrDlg		m_servAddrDlg;				//服务器地址对话框
	CCriticalSection	m_csList;					//临界区对象
	
	Desk *pdesk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__CC9BB1E4_1779_4786_A820_CB54826385DE__INCLUDED_)
