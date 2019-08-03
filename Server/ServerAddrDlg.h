#if !defined(AFX_SERVERADDRDLG_H__DB11BAE2_440A_449E_81BD_CFB2440F375C__INCLUDED_)
#define AFX_SERVERADDRDLG_H__DB11BAE2_440A_449E_81BD_CFB2440F375C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerAddrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerAddrDlg dialog

class CServerAddrDlg : public CDialog
{
// Construction
public:
	CServerAddrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerAddrDlg)
	enum { IDD = IDD_SERVER_ADDRESS_DLG };
	CIPAddressCtrl	m_ctlServIP;
	CEdit	m_ctlServPort;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerAddrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerAddrDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwServIP;	//服务器IP地址
	SHORT m_shServPort;	//服务器端口
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERADDRDLG_H__DB11BAE2_440A_449E_81BD_CFB2440F375C__INCLUDED_)
