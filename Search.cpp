// Search.cpp : implementation file
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "Search.h"
#include "catalog.h"
#include "leftview.h"
#include <fstream.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define _MAX_LEVELS 63
/////////////////////////////////////////////////////////////////////////////
// CSearch dialog
CSearch::CSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearch)
	DDX_Control(pDX, IDC_LIST_SEARCH, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearch, CDialog)
	//{{AFX_MSG_MAP(CSearch)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_BN_CLICKED(IDC_LOCATE, OnLocate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearch message handlers
void CSearch::AddItem(Catalog fd, CString path, CString desc, int index)
{
	LV_ITEM	lvitem;
	char sNumBuff[_MAX_PATH];
	int iActualItem;
	int pos=path.ReverseFind(_T('\\'));
	CString sText;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem = 0;
	lvitem.iSubItem = 0;
	lvitem.iImage=(index==-1)?2:index;
	strcpy(sNumBuff,path.Right(path.GetLength()-pos-1));
	path.Delete(pos,path.GetLength()-pos);
	lvitem.pszText = sNumBuff;
	iActualItem = m_list.InsertItem(&lvitem);
	//Add File Path
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = 0;
	lvitem.iSubItem = 1;
	pos=path.Find("\\");
	if(pos!=-1)
		strcpy(sNumBuff,path.Right(path.GetLength()-pos-1));
	else
		strcpy(sNumBuff,"\\");
	lvitem.pszText = sNumBuff;
	m_list.SetItem(&lvitem);
	//Add Volume Name
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 2;
	if(pos!=-1)
		strcpy(sNumBuff,path.Left(pos));
	else
		strcpy(sNumBuff,path);
	lvitem.pszText = sNumBuff;
	m_list.SetItem(&lvitem);
	// Add Size column for files
	if(fd.nFileSizeLow != 0)
		itoa(fd.nFileSizeLow,sNumBuff,10);
	else
		strcpy(sNumBuff,"0");
	switch(fd.flag)
	{
	case 0:strcat(sNumBuff," Bytes");
		break;
	case 1:strcat(sNumBuff," KB");
		break;
	case 2:strcat(sNumBuff," MB");
		break;
	case 3:strcat(sNumBuff," GB");
		break;
	}
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 3;
	lvitem.pszText = sNumBuff;
	m_list.SetItem(&lvitem);
	// Add Time column
	SYSTEMTIME st;
	FileTimeToSystemTime(&fd.ftCreationTime,&st);
	sText.Format("%02d-%02d-%d %02d:%02d", st.wDay,st.wMonth,st.wYear,
		st.wHour,st.wMinute);
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 4;
	lvitem.pszText = sText.GetBuffer(sText.GetLength());
	m_list.SetItem(&lvitem);
	// Add Attribute column
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 5;
	GetAttribString(fd.dwFileAttributes,sNumBuff);
	lvitem.pszText = sNumBuff;//Insert Attributes
	m_list.SetItem(&lvitem);
	// Add Description column
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 6;
	strcpy(sNumBuff,desc);
	lvitem.pszText = sNumBuff;//Insert Attributes
	m_list.SetItem(&lvitem);
}

void CSearch::GetAttribString(DWORD attrib, char buff[])
{
	DWORD dwAttribs[]={ARCHIVE, COMPRESSED,	DIRECTORY, ENCRYPTED, HIDDEN, READONLY, SYSTEM};
	CString cAttribs="ACDEHRS";
	for(int i=0;i<7;i++)
		buff[i]=((attrib & dwAttribs[i])==dwAttribs[i])?buff[i]=cAttribs[i]:buff[i]='_';
	buff[i]=0;
}

BOOL CSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_list.DeleteAllItems();
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText("");
	m_pApp=(CGhostXpressApp*)AfxGetApp();
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.SetImageList(&m_pApp->m_imageListL, LVSIL_NORMAL);
	m_list.SetImageList(&m_pApp->m_imageListS, LVSIL_SMALL);

	CRect rect;
	GetClientRect(&rect);
	m_list.InsertColumn(0, "Name", LVCFMT_LEFT , (rect.right / 4)+27);
	m_list.InsertColumn(1, "Path", LVCFMT_LEFT , (rect.right / 4));
	m_list.InsertColumn(2, "Volume Name", LVCFMT_LEFT ,(rect.right / 8)+10);
	m_list.InsertColumn(3, "Size", LVCFMT_RIGHT ,(rect.right / 16));
	m_list.InsertColumn(4, "Date / Time", LVCFMT_LEFT ,(rect.right / 8));
	m_list.InsertColumn(5, "Attributes", LVCFMT_LEFT , rect.right / 16);	
	m_list.InsertColumn(6, "Description", LVCFMT_LEFT , rect.right);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearch::OnOK() 
{
	m_pthread=AfxBeginThread(Search,(LPVOID)this,THREAD_PRIORITY_HIGHEST);
}

void CSearch::OnCancel() 
{
	if(!bClose)
	{
		SuspendThread(m_pthread->m_hThread);
		if(MessageBox("This will Stop the Current Search. Are you sure ?", "Are you sure ?", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES)
		{
			TerminateThread(m_pthread->m_hThread, 0);
			SetControls(TRUE);
			SetDlgItemText(IDC_EDIT_SEARCH,"");
		}
		else
			ResumeThread(m_pthread->m_hThread);
		return;
	}
	CDialog::OnCancel();
}

void CSearch::OnChangeEditSearch() 
{
	CString s;
	GetDlgItemText(IDC_EDIT_SEARCH,s);
	GetDlgItem(IDOK)->EnableWindow((s.GetLength())?TRUE:FALSE);
}

CString CSearch::GetFileName(CString pathName, int style)
{
	TCHAR ch=_T('\\');
	if(style)
		ch=_T('.');
	int pos=pathName.ReverseFind(ch);
	return pathName.Right(pathName.GetLength()-(pos+1));
}

UINT CSearch::Search(LPVOID pParam)
{
	CSearch *pSearch=(CSearch*)pParam;
	CString srchText;
	EncodedCatalog enc;
	Catalog cat;
	Files files;
	fstream file;
	CStringArray csa;
	DWORD parents[_MAX_LEVELS];
	BYTE level=0;
	pSearch->m_list.DeleteAllItems();
	pSearch->GetDlgItemText(IDC_EDIT_SEARCH, srchText);
	srchText.MakeUpper();
	WIN32_FIND_DATA fd;
	HANDLE hnd=FindFirstFile(".\\Catalog\\*.gxc",&fd);
	if(hnd == INVALID_HANDLE_VALUE)
	{
		pSearch->MessageBox("No Calalog Created","Error",MB_OK|MB_ICONSTOP);
		AfxEndThread(0);
		return 0;
	}
	pSearch->SetControls();
	CWaitCursor wait;
	DWORD parent=0;
	DWORD count=0;
	do
	{
		parent=0;
		level=0;
		csa.RemoveAll();
		memset((void*)&parents,0,sizeof(BYTE)*_MAX_LEVELS);
		if(fd.cFileName[0] != _T('.'))
		{
			CString sFile=".\\Catalog\\";
			sFile+=fd.cFileName;
			CString s, t, d;
			level++;
			file.open(sFile,ios::in|ios::binary|ios::nocreate);
			if(!file || !CCatalog::Verified(file))
			{
				pSearch->MessageBox("Error opening file","Error",MB_OK|MB_ICONSTOP);
				AfxEndThread(0);
			}
			cat=enc.Read(file);
			files.Read(file, cat.nFileName, cat.nDesc);
			csa.Add(files.cFileName);
			int index=-1, found=0;
			do
			{
				found=0;
				cat=enc.Read(file);
				files.Read(file, cat.nFileName, cat.nDesc);
				if(parent==cat.parent)
				{
					if(level<csa.GetSize())
						csa[level]=files.cFileName;
					else
						csa.Add(files.cFileName);
					if((cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
					{
						parents[level++]=parent;
						parent=cat.myIndex;
					}
				}
				else if(parent>cat.parent)
				{
					while(cat.parent!=parents[level-1])
						level--;
					parent=cat.myIndex;
					csa[level-1]=files.cFileName;
				}
				if(file.eof())
					break;
				t=files.cFileName;
				d=files.desc;
				d.MakeUpper();
				//if(t.Find(srchText)!=-1 || d.Find(srchText)!=-1)
				if(pSearch->wildcmp(srchText, t) || d.Find(srchText)!=-1)
				{
					count++;
					s.Empty();
					for(UCHAR i=0; i<level; i++)
					{
						s+=csa[i];
						s+="\\";
					}
					if((cat.dwFileAttributes & DIRECTORY)!=DIRECTORY)
						s+=t;//csa[i];
					else
					{
						int pos=s.ReverseFind(_T('\\'));
						s.Delete(pos,s.GetLength()-pos);
					}
					int j=1;
					if((cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
						pSearch->AddItem(cat, s, files.desc, pSearch->m_pApp->folIndex);
					else
					{
						CString extn=pSearch->GetFileName(files.cFileName, 1);
						for(j=1;j<pSearch->m_pApp->dynIndex;j++)
						{
							if(!strcmpi(pSearch->m_pApp->m_iconInfo[j].extn, extn))
							{
								index=j;
								found=1;
								break;
							}
						}
						pSearch->AddItem(cat, s, files.desc, (found)?index-1:0);
					}
				}
			}while(! file.eof());
			file.close();
		}
	}while(FindNextFile(hnd,&fd));
	FindClose(hnd);
	pSearch->SetControls(TRUE);
	CString s;
	s.Format("%u Item(s) Found", count);
	pSearch->GetDlgItem(IDC_COUNT)->SetWindowText(s);
	AfxEndThread(0);
	return 0;
}

void CSearch::OnLocate() 
{
	int nSel = m_list.GetNextItem(-1,LVNI_FOCUSED);
	int nCount=m_list.GetItemCount();
	if(!nCount)
		return;
	nSel=(nSel==-1 && nCount)?0:nSel;
	strPathName=m_list.GetItemText(nSel,2);
	CString path=m_list.GetItemText(nSel,1);
	CString file=m_list.GetItemText(nSel,0);
	strPathName+=(path=="\\")?(path+file):"\\"+path+"\\"+file;
	CDialog::OnOK();
}

void CSearch::SetControls(BOOL set)
{
	GetDlgItem(IDC_EDIT_SEARCH)->EnableWindow(set);
	GetDlgItem(IDOK)->EnableWindow(set);
	//GetDlgItem(IDC_LOCATE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_SETTINGS)->EnableWindow(set);
	bClose=(set)?true:false;
	if(set)
	{
		GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
		GetDlgItem(IDCANCEL)->SetWindowText("&Close");
	}
	else
		GetDlgItem(IDCANCEL)->SetWindowText("&Stop");
}

bool CSearch::wildcmp(const _TCHAR *wild, const _TCHAR *str)
{
	int slen = _tcslen(str);
	int wlen = _tcslen(wild);
	// if the compare string is too short, we're done
	int reqLen = 0;
	for (int i = 0; i < wlen; ++i)
		if (wild[i] != _T('*'))
			++reqLen;
	if (slen < reqLen)
		return false;
	// length is okay; now we do the comparing
	int w = 0, s = 0;
	for (; s < slen && w < wlen; ++s, ++w)
	{
		// chars match; keep going
		if (wild[w] == str[s])
			continue;
		// if we hit a '?' just go to the next char in both `str` and `wild`
		if (wild[w] == _T('?'))
			continue;
		// we hit an unlimited wildcard
		if (wild[w] == _T('*'))
		{
			// if it's the last char in the string, we're done
			if ((w + 1) == wlen)
				return true;
			bool ret = true;
			// for each remaining character in `wild`
			while (++w < wlen && ret)
			{
				ret=false;
				// for each remaining character in `str`
				for (int i = 0; i < (slen - s); ++i)
				{
					// if the char is the same as the current `wild` char
					if (str[s + i] == wild[w])
					{
						// compare from these points on
						ret = wildcmp(wild + w, str + s + i);
						// if successful, we're done
						if (ret)
							return true;
					}
				}
			}
			return ret;
		}
		// didn't hit a wildcard and chars don't match; failure
		else
			return false;
	}
	return /*(w >= wlen || ((w + 1) == wlen && wild[w] == _T('*'))) &&*/ s >= slen;
}