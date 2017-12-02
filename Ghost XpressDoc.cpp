// Ghost XpressDoc.cpp : implementation of the CGhostXpressDoc class
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "descview.h"
#include "Ghost XpressDoc.h"
#include "SelectDrive.h"
#include "Progress.h"
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressDoc

IMPLEMENT_DYNCREATE(CGhostXpressDoc, CDocument)

BEGIN_MESSAGE_MAP(CGhostXpressDoc, CDocument)
	//{{AFX_MSG_MAP(CGhostXpressDoc)
	ON_COMMAND(ID_FILE_NEW_CAT, OnFileNewCat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressDoc construction/destruction

CGhostXpressDoc::CGhostXpressDoc()
{
	// TODO: add one-time construction code here

}

CGhostXpressDoc::~CGhostXpressDoc()
{
}

BOOL CGhostXpressDoc::OnNewDocument()
{
	//if (!CDocument::OnNewDocument())
	//	return FALSE;
	SetTitle("Created by - Vivek Jain - ThE GhOsT Of C");
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGhostXpressDoc serialization

void CGhostXpressDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressDoc diagnostics

#ifdef _DEBUG
void CGhostXpressDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGhostXpressDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressDoc commands

void CGhostXpressDoc::SetDesc(CString desc)
{
	m_DescView->GetEditCtrl().SetWindowText(desc);
}

BOOL CGhostXpressDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString s=lpszPathName;
	s.MakeUpper();
	int pos=s.ReverseFind(_T('.'));
	if(pos==-1)
		return FALSE;
	if(s.Right(s.GetLength()-pos).Find("GXC")==-1)
		return FALSE;
	pos=s.ReverseFind(_T('\\'));
	s=lpszPathName;
	char buff[_MAX_PATH]={0};
	getcwd(buff,_MAX_PATH);
	strcat(buff,"\\Catalog");
	mkdir(buff);
	strcat(buff,s.Right(s.GetLength()-pos));
	CopyFile(lpszPathName,buff,FALSE);
	SendMessage(NULL,WM_COMMAND,ID_VIEW_REFRESH,0);
	SetTitle("Created by - Vivek Jain - ThE GhOsT Of C");
	return TRUE;
}

void CGhostXpressDoc::OnFileNewCat() 
{
	CSelectDrive dlg;
	if(dlg.DoModal()==IDOK)
	{
		CProgress dlgProgress;
		dlgProgress.m_Volume=dlg.m_Volume;
		dlgProgress.m_Drive=dlg.m_cddrive.GetAt(0);
		dlgProgress.DoModal();
		SendMessage(NULL,WM_COMMAND,ID_VIEW_REFRESH,0);
	}	
}

void CGhostXpressDoc::GetFileFolderList(CString strPathName, CArray<CFilesInfo, CFilesInfo> &filesInfo)
{
	filesInfo.RemoveAll();
	if(!strPathName.GetLength())
		return;
	m_DescView->SetWindowText("");
	if(strPathName=="")
		return;
	int index=0, found=0;
	int pos=strPathName.Find("\\");
	CString strVolume=".\\Catalog\\";
	if(pos!=-1)
		strVolume+=strPathName.Left(pos);
	else
		strVolume+=strPathName;
	strVolume+=".gxc";
	fstream file;
	file.open(strVolume,ios::in|ios::binary|ios::nocreate);
	if(!file || !CCatalog::Verified(file))
	{
		MessageBox(NULL,"Cannot Open Catalog or Invalid Catalog","Error",MB_OK|MB_ICONSTOP);
		SendMessage(NULL,WM_COMMAND,ID_VIEW_REFRESH,0);
		return;
	}
	CWaitCursor wait;
	CStringArray sz_pathList;
	DWORD count=0;
	CString t;
	while(strPathName.Find("\\",0)!=-1)
	{
		t=strPathName.SpanExcluding("\\");
		sz_pathList.Add(t);
		count++;
		strPathName.Delete(0,strPathName.Find("\\",0)+1);
	}
	if(strPathName.GetLength()>0)
	{
		sz_pathList.Add(strPathName);
		count++;
	}
	DWORD myIndex=0, parent=0xFFFFF;//MAXDWORD;
	Catalog cat;
	EncodedCatalog enc;
	Files files;
	DWORD mainParent=parent;
	for(DWORD level=0;level<count;level++)
	{	
		do
		{			
			memset((void*)&files,0,sizeof(files));
			cat=enc.Read(file);
			if(cat.parent==parent)
			{
				files.Read(file,cat.nFileName,cat.nDesc);
				mainParent=parent;
				parent=(sz_pathList[level]==files.cFileName)?cat.myIndex:parent;
			}
			else
				file.seekg(cat.nFileName+cat.nDesc,ios::cur);
		}while(sz_pathList[level]!=files.cFileName && file.good() && !file.eof());
	}
	SetDesc(files.desc);
	//CWnd cwnd;
	//cwnd.MessageBox(files.cFileName, "Getting Folder List for");
	do
	{
		cat=enc.Read(file);
		if(file.eof())
			break;
		if(cat.parent==parent)
		{
			files.Read(file,cat.nFileName,cat.nDesc);
			filesInfo.Add(CFilesInfo(FileData(cat,files)));
		}
		else
			file.seekg(cat.nFileName+cat.nDesc,ios::cur);
	}while(mainParent!=cat.parent && !file.eof());
	//int size=filesInfo.GetSize();
	file.close();
}