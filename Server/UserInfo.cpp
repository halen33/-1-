// UserInfo.cpp: implementation of the CUserInfo class.
// Download by http://www.NewXing.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "UserInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CUserInfo::CUserInfo():m_strName(""),m_lIP(0),m_uPort(0),m_state(UNKNOWN)
{
	
}
CUserInfo::CUserInfo(const CUserInfo &userInfo)
{
	m_strName = userInfo.m_strName;
	m_lIP = userInfo.m_lIP;
	m_uPort = userInfo.m_uPort;
	m_state = userInfo.m_state;
}


CUserInfo::~CUserInfo()
{

}

CUserInfo& CUserInfo::operator = (const CUserInfo &userInfo)
{
	m_strName = userInfo.m_strName;
	m_lIP = userInfo.m_lIP;
	m_uPort = userInfo.m_uPort;
	m_state = userInfo.m_state;
	return *this;
	
}