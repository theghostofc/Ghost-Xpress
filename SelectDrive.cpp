// SelectDrive.cpp : implementation file
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "SelectDrive.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDrive dialog


CSelectDrive::CSelectDrive(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDrive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDrive)
	m_cddrive = _T("");
	m_Volume = _T("");
	//}}AFX_DATA_INIT
}


void CSelectDrive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDrive)
	DDX_CBString(pDX, IDC_COMBO_CDDRIVE, m_cddrive);
	DDV_MaxChars(pDX, m_cddrive, 64);
	DDX_Text(pDX, IDC_EDIT_VOLUME, m_Volume);
	DDV_MaxChars(pDX, m_Volume, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDrive, CDialog)
	//{{AFX_MSG_MAP(CSelectDrive)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_CDDRIVE, OnSelchangeComboCddrive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDrive message handlers
void CSelectDrive::GetDrives()
{
	char drives[_MAX_PATH]={0};
	char temp[]="C:";
	char volName[20]={0}, fsName[10]={0};
	DWORD sno=0, maxFnameLen=0, fsFlags=0;
	GetLogicalDriveStrings(_MAX_PATH,drives);
	int i=0,j=0;
	while(!j)
	{
		if(drives[i]=='A'||drives[i]=='a')
		{
			i+=3;
			continue;
		}
		temp[0]=drives[i];
		if(!GetVolumeInformation(temp,volName,20,&sno,&maxFnameLen,
			&fsFlags, fsName,10))
			strcpy(volName,"No Media Present");
		char str[_MAX_PATH]={0};
		str[0]=0;
		if(GetDriveType(temp)==DRIVE_CDROM)
		{
			strcpy(str,strupr(temp));
			strcat(str," - ");
			strcat(str,volName);
			SendDlgItemMessage(IDC_COMBO_CDDRIVE,CB_ADDSTRING,0,
				(LPARAM)str);
			i+=3;
		}
		if(drives[i]==NULL && drives[i+1]==NULL)
			j=1;
		i++;
	}
	SendDlgItemMessage(IDC_COMBO_CDDRIVE,CB_SETCURSEL,0,0);
	OnSelchangeComboCddrive();
}

void CSelectDrive::OnSelchangeComboCddrive() 
{
	CString s;
	GetDlgItemText(IDC_COMBO_CDDRIVE,s);
	if(s.Find("No Media Present",0)!=-1)
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	else
		GetDlgItem(IDOK)->EnableWindow();
	s.Delete(0,5);
	GetDlgItem(IDC_EDIT_VOLUME)->SetWindowText(s);
}

/*void CSelectDrive::OnOK() 
{
	char buff[_MAX_PATH]={0};
	getcwd(buff,_MAX_PATH);
	strcat(buff,"\\Catalog");
	mkdir(buff);
	GetDlgItem(IDC_COMBO_CDDRIVE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CDDRIVE)->SetWindowText("Creating Catalog, Please Wait...");
	AfxBeginThread(SaveCatalog,(LPVOID)this,THREAD_PRIORITY_HIGHEST);
}

void CSelectDrive::OnCancel() 
{
	CDialog::OnCancel();
}*/

BOOL CSelectDrive::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SendDlgItemMessage(IDC_COMBO_CDDRIVE,CB_RESETCONTENT);
	CWaitCursor wait;
	GetDrives();
	SendDlgItemMessage(IDC_COMBO_CDDRIVE,CBN_SETFOCUS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectDrive::OnRefresh()
{
	OnInitDialog();
}
