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
	CUserInfo(const CUserInfo &userInfo);//¿½±´¹¹Ôìº¯Êý
	virtual ~CUserInfo();
	enum	USERSTATE{
			PLAY,		//ÓÎÏ·×´Ì¬
			WAIT,		//µÈ´ý×´Ì¬
			LOGIN,		//µÇÂ¼×´Ì¬
			UNKNOWN		//Î´Öª×´Ì¬
	};

public:
	CUserInfo& operator = (const CUserInfo &userInfo);//¸³Öµº¯Êý
public:
	CString		m_strName;	//Ãû³Æ
	u_long		m_lIP;		//IP
	int		m_uPort;	//¶Ë¿Ú
	USERSTATE	m_state;	//×´Ì¬
};

#endif // !defined(AFX_USERINFO_H__7276A6E4_CD8A_42F9_8DCA_8920BE666F12__INCLUDED_)
