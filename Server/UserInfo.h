// UserInfo.h: interface for the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFO_H__7276A6E4_CD8A_42F9_8DCA_8920BE666F12__INCLUDED_)
#define AFX_USERINFO_H__7276A6E4_CD8A_42F9_8DCA_8920BE666F12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserInfo : public CObject  
{
public:
	CUserInfo();
	CUserInfo(const CUserInfo &userInfo);//�������캯��
	virtual ~CUserInfo();
	enum	USERSTATE{
			PLAY,		//��Ϸ״̬
			WAIT,		//�ȴ�״̬
			LOGIN,		//��¼״̬
			UNKNOWN		//δ֪״̬
	};

public:
	CUserInfo& operator = (const CUserInfo &userInfo);//��ֵ����
public:
	CString		m_strName;	//����
	u_long		m_lIP;		//IP
	int		m_uPort;	//�˿�
	USERSTATE	m_state;	//״̬
};

#endif // !defined(AFX_USERINFO_H__7276A6E4_CD8A_42F9_8DCA_8920BE666F12__INCLUDED_)
