// Ghost Xpress.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ghost Xpress.h"

#include "MainFrm.h"
#include "Ghost XpressDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressApp

BEGIN_MESSAGE_MAP(CGhostXpressApp, CWinApp)
	//{{AFX_MSG_MAP(CGhostXpressApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressApp construction

CGhostXpressApp::CGhostXpressApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGhostXpressApp object

CGhostXpressApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressApp initialization

BOOL CGhostXpressApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGhostXpressDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	
	Init();
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CGhostXpressApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressApp message handlers

int CGhostXpressApp::ExitInstance() 
{
	m_iconInfo.RemoveAll();
	m_imageListL.DeleteImageList();
	m_imageListS.DeleteImageList();
	return CWinApp::ExitInstance();
}

void CGhostXpressApp::GetIcons()
{
	CIconInfo cii;
	HKEY key;
	char name[50];
	int count=0, index=1;
	m_imageListS.Add(ExtractIcon(0, "shell32.dll", 0));//Default Icon
	m_imageListL.Add(ExtractIcon(0, "shell32.dll", 0));
	cii.index=0;
	cii.extn="Unknown";
	m_iconInfo.Add(cii);
	RegOpenKeyEx(HKEY_CLASSES_ROOT, "", 0, KEY_ALL_ACCESS, &key);
	while(RegEnumKey(key, count, name, sizeof(name)) != ERROR_NO_MORE_ITEMS)
	{
		CString temp = name;
		if(temp.Left(1) == ".")
		{
			HKEY keyimagefile;
			BYTE *imagefile;
			imagefile = new BYTE[100];
			DWORD size = sizeof(BYTE[100]);
			RegOpenKey(HKEY_CLASSES_ROOT, name, &keyimagefile);
			RegQueryValueEx(keyimagefile, "", 0, 0, imagefile, &size);
			HKEY iconkey;
			CString deficondata;
			deficondata = imagefile;
			
			deficondata += "\\DefaultIcon";

			if(RegOpenKey(HKEY_CLASSES_ROOT, deficondata, &iconkey) == ERROR_SUCCESS)
			{
				BYTE *path;
				path = new BYTE[MAX_PATH];
				size = sizeof(BYTE[MAX_PATH]);

				RegQueryValueEx(iconkey, "", 0, 0, path, &size);

				if(strcmp((LPCTSTR) path, ""))
				{
					CString value, valuestr;
					int valint;

					valuestr = path;

					int charindex = valuestr.ReverseFind(',');
					valuestr = valuestr.Right(valuestr.GetLength() - charindex - 1);
					valint = atoi(valuestr);
					
					CString fullpath;

					fullpath = path;
					fullpath = fullpath.SpanExcluding(",");

					HICON hIcon;
					cii.index=index;
					cii.extn=name;
					cii.extn.Delete(0);
					hIcon = ExtractIcon(0, fullpath, valint);
					if(hIcon == NULL)
						hIcon = ExtractIcon(0, "shell32.dll", IsExe(name)?2:0);
					m_imageListS.Add(hIcon);
					m_imageListL.Add(hIcon);
					m_iconInfo.Add(cii);
					index++;
				}
			}
		}
		count++;
	}
}

void CGhostXpressApp::Init()
{
	m_imageListL.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 580); 
	m_imageListS.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 580); 
	CWaitCursor wait;
	GetIcons();
	dynIndex=m_iconInfo.GetSize()-1;
	int found=0;
	for(int i=1;i<dynIndex;i++)
	{
		if(!strcmpi(m_iconInfo[i].extn, "Folder"))
		{
			folIndex=i;
			found|=1;
		}
		if(!strcmpi(m_iconInfo[i].extn, "exe"))
		{
			exeIndex=i;
			found|=2;
		}
		if(found==3)
			break;
	}	
	if((found & 1)!=1)
	{
		CIconInfo cii;
		HICON hIcon=ExtractIcon(0, "Shell32.dll", 3);
		folIndex=m_imageListL.Add(hIcon);
		m_imageListS.Add(hIcon);
		cii.extn="Folder";
		cii.index=folIndex;
		m_iconInfo.Add(cii);
		dynIndex++;
	}
	if((found & 2)!=2)
	{
		CIconInfo cii;
		HICON hIcon=ExtractIcon(0, "Shell32.dll", 2);
		exeIndex=m_imageListL.Add(hIcon);
		m_imageListS.Add(hIcon);
		cii.extn="exe";
		cii.index=exeIndex;
		m_iconInfo.Add(cii);
		dynIndex++;
	}
}

int CGhostXpressApp::IsExe(CString extn)
{
	extn.Delete(0);
	if(extn.CompareNoCase("EXE")==0 ||
		extn.CompareNoCase("ICO")==0 ||
		extn.CompareNoCase("CUR")==0 ||
		extn.CompareNoCase("ANI")==0)
		return 1;
	return 0;
}