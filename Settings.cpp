// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "ghost xpress.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog


CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettings)
	//}}AFX_DATA_INIT
}


void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
	DDX_Control(pDX, IDC_CATALOGS, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettings, CDialog)
	//{{AFX_MSG_MAP(CSettings)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers

BOOL CSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT);
	m_list.DeleteAllItems();
	WIN32_FIND_DATA fd;
	HANDLE hnd=FindFirstFile(".\\Catalog\\*.gxc",&fd);
	if(hnd == INVALID_HANDLE_VALUE)
	{
		MessageBox("No Calalog Created","Error",MB_OK|MB_ICONSTOP);
	}
	int pos=0;
	do
	{
		if(fd.cFileName[0] != _T('.'))
		{
			CString s=fd.cFileName;
			int pos=s.ReverseFind(_T('.'));
			s.Delete(pos,s.GetLength()-pos);
			m_list.InsertItem(0, s);
		}
	}while(FindNextFile(hnd,&fd));
	FindClose(hnd);
	pos=m_list.GetNextItem(-1,LVNI_ALL);
	do
	{
		m_list.SetCheck(pos);
		pos=m_list.GetNextItem(pos,LVNI_ALL);
	}while(pos!=-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettings::OnRefresh() 
{
	OnInitDialog();
}

void CSettings::OnOK() 
{
	cats.Empty();
	int pos=m_list.GetNextItem(-1,LVNI_ALL);
	do
	{
		if(m_list.GetCheck(pos))
		{
			cats+=m_list.GetItemText(pos,0);
			cats+=";";
		}
		pos=m_list.GetNextItem(pos,LVNI_ALL);
	}while(pos!=-1);	
	CDialog::OnOK();
}

void CSettings::OnCancel() 
{
	cats.Empty();
	int pos=m_list.GetNextItem(-1,LVNI_ALL);		
	do
	{
		cats+=m_list.GetItemText(pos,0);
		cats+=";";
		pos=m_list.GetNextItem(pos,LVNI_ALL);
	}while(pos!=-1);	
	CDialog::OnCancel();
}
