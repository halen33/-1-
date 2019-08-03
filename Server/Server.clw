; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUserInfo
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"

ClassCount=5
Class1=CServerApp
Class2=CServerDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_SERVER_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_SERVER_ADDRESS_DLG (English (U.S.))
Class4=CServerAddrDlg
Resource4=IDD_CLIENT_LOGIN_DIALOG
Class5=CUserInfo
Resource5=IDD_ABOUTBOX

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D
LastObject=IDOK
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVER_DIALOG]
Type=1
Class=CServerDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SERVER_LIST,SysListView32,1350631425

[DLG:IDD_SERVER_ADDRESS_DLG (English (U.S.))]
Type=1
Class=CServerAddrDlg
ControlCount=7
Control1=IDC_SERVER_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_SERVER_PORT_EDIT,edit,1350639744
Control3=IDOK,button,1342242816
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352

[CLS:CServerAddrDlg]
Type=0
HeaderFile=ServerAddrDlg.h
ImplementationFile=ServerAddrDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CServerAddrDlg

[CLS:CUserInfo]
Type=0
HeaderFile=UserInfo.h
ImplementationFile=UserInfo.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_CLIENT_LOGIN_DIALOG]
Type=1
Class=?
ControlCount=10
Control1=IDC_CLIENT_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_CLIENT_NAME_EDIT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_CLIENT_NAME_STATIC,static,1342308352
Control6=IDC_CLIENT_GROUPBOX_IP,button,1342177287
Control7=IDC_CLIENT_GROUPBOX_USER,button,1342177287
Control8=IDC_CLIENT_PORT_EDIT,edit,1350639616
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

