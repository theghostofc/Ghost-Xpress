; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLeftView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ghost xpress.h"
LastPage=0

ClassCount=11
Class1=CDescView
Class2=CGhostXpressApp
Class3=CAboutDlg
Class4=CGhostXpressDoc
Class5=CGhostXpressView
Class6=CLeftView
Class7=CMainFrame
Class8=CProgress
Class9=CSearch
Class10=CSelectDrive
Class11=CSettings

ResourceCount=6
Resource1=IDD_SEARCH
Resource2=IDD_DIALOG1
Resource3=IDD_SELECTDRIVE
Resource4=IDD_PROGRESS
Resource5=IDD_ABOUTBOX
Resource6=IDR_MAINFRAME

[CLS:CDescView]
Type=0
BaseClass=CEditView
HeaderFile=DescView.h
ImplementationFile=DescView.cpp
LastObject=CDescView

[CLS:CGhostXpressApp]
Type=0
BaseClass=CWinApp
HeaderFile=Ghost Xpress.h
ImplementationFile=Ghost Xpress.cpp
LastObject=CGhostXpressApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Ghost Xpress.cpp
ImplementationFile=Ghost Xpress.cpp
LastObject=CAboutDlg

[CLS:CGhostXpressDoc]
Type=0
BaseClass=CDocument
HeaderFile=Ghost XpressDoc.h
ImplementationFile=Ghost XpressDoc.cpp
LastObject=CGhostXpressDoc

[CLS:CGhostXpressView]
Type=0
BaseClass=CListView
HeaderFile=Ghost XpressView.h
ImplementationFile=Ghost XpressView.cpp
LastObject=ID_EDIT_FIND

[CLS:CLeftView]
Type=0
BaseClass=CTreeView
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
LastObject=CLeftView

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame

[CLS:CProgress]
Type=0
BaseClass=CDialog
HeaderFile=Progress.h
ImplementationFile=Progress.cpp
Filter=D
VirtualFilter=dWC
LastObject=CProgress

[CLS:CSearch]
Type=0
BaseClass=CDialog
HeaderFile=Search.h
ImplementationFile=Search.cpp
LastObject=IDOK
Filter=D
VirtualFilter=dWC

[CLS:CSelectDrive]
Type=0
BaseClass=CDialog
HeaderFile=SelectDrive.h
ImplementationFile=SelectDrive.cpp
LastObject=CSelectDrive

[CLS:CSettings]
Type=0
BaseClass=CDialog
HeaderFile=Settings.h
ImplementationFile=Settings.cpp
LastObject=CSettings

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROGRESS]
Type=1
Class=CProgress
ControlCount=3
Control1=IDC_PROGRESS_CAT,msctls_progress32,1350565888
Control2=IDC_STATIC_CDDRIVE,static,1342308352
Control3=IDCANCEL,button,1073807360

[DLG:IDD_SEARCH]
Type=1
Class=CSearch
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_SEARCH,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDC_LIST_SEARCH,SysListView32,1350631429
Control5=IDC_LOCATE,button,1342242816
Control6=IDCANCEL,button,1342242816
Control7=IDC_COUNT,static,1342308352

[DLG:IDD_SELECTDRIVE]
Type=1
Class=CSelectDrive
ControlCount=7
Control1=IDC_STATIC_CDDRIVE,static,1342308352
Control2=IDC_COMBO_CDDRIVE,combobox,1344340227
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_VOLUME,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_BUTTON_REFRESH,button,1342242816

[DLG:IDD_SETTINGS]
Type=1
Class=CSettings

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW_CAT
Command2=ID_FILE_OPEN
Command3=ID_APP_EXIT
Command4=ID_EDIT_FIND
Command5=ID_VIEW_REFRESH
Command6=ID_EDIT_CLEAR
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_WINDOW_SPLIT
Command10=ID_VIEW_LARGEICON
Command11=ID_VIEW_SMALLICON
Command12=ID_VIEW_LIST
Command13=ID_VIEW_DETAILS
Command14=ID_APP_ABOUT
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW_CAT
Command2=ID_FILE_OPEN
Command3=ID_VIEW_REFRESH
Command4=ID_EDIT_CLEAR
Command5=ID_EDIT_FIND
Command6=ID_VIEW_LARGEICON
Command7=ID_VIEW_SMALLICON
Command8=ID_VIEW_LIST
Command9=ID_VIEW_DETAILS
Command10=ID_APP_ABOUT
CommandCount=10

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_FIND
Command2=ID_FILE_OPEN
Command3=ID_FILE_NEW_CAT
Command4=ID_FILE_OPEN
Command5=ID_APP_ABOUT
Command6=ID_VIEW_REFRESH
Command7=ID_NEXT_PANE
Command8=ID_PREV_PANE
CommandCount=8

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

