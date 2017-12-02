// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "Ghost XpressDoc.h"
#include "Ghost XpressView.h"
#include "LeftView.h"
#include "Search.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_EDITLABELS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CGhostXpressDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	CGhostXpressDoc* pDoc = GetDocument();
	pDoc->m_LeftView = this;
	HICON hIcon;
	m_imageList.DeleteImageList();
	m_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	hIcon=ExtractIcon(0,"shell32.dll",3);//Closed Folder
	m_imageList.Add(hIcon);
	hIcon=ExtractIcon(0,"shell32.dll",4);//Open Folder
	m_imageList.Add(hIcon);
	hIcon=ExtractIcon(0,"shell32.dll",11);//CD Drive
	m_imageList.Add(hIcon);
	GetTreeCtrl().SetImageList(&m_imageList, TVSIL_NORMAL);
	count=0;
	lock=0;
	bEditing=false;
	OnViewRefresh();
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CGhostXpressDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGhostXpressDoc)));
	return (CGhostXpressDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers
CString CLeftView::GetPathFromItem(HTREEITEM hItem)
{
	CString strResult = GetTreeCtrl().GetItemText(hItem);
	HTREEITEM hParent;
	while((hParent=GetTreeCtrl().GetParentItem(hItem))!=NULL)
	{
		CString string=GetTreeCtrl().GetItemText(hParent);
		if(string.Right(1)!="\\")
			string+="\\";
		strResult=string+strResult;
		hItem=hParent;
	}
	return strResult;
}

void CLeftView::DeleteAllChildren(HTREEITEM hItem)
{
	HTREEITEM hChildItem;
	if((hChildItem=GetTreeCtrl().GetChildItem(hItem))==NULL)
		return;
	do
	{
		HTREEITEM hNextItem = GetTreeCtrl().GetNextSiblingItem(hChildItem);
		GetTreeCtrl().DeleteItem(hChildItem);
		hChildItem=hNextItem;
	}while(hChildItem != NULL);
}

void CLeftView::DeleteFirstChild(HTREEITEM hItem)
{
	HTREEITEM hChildItem;
	if((hChildItem=GetTreeCtrl().GetChildItem(hItem))!=NULL)
		GetTreeCtrl().DeleteItem(hChildItem);
}

void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CString string = GetPathFromItem(hItem);
	if(pNMTreeView->action == TVE_EXPAND)
	{
		DeleteFirstChild(hItem);
		AddDirectories(hItem);
		GetTreeCtrl().Select(hItem,TVGN_CARET);
		GetTreeCtrl().SortChildren(hItem);
	}
	else
	{
		DeleteAllChildren(hItem);
		GetTreeCtrl().InsertItem("", 1, 1, hItem);
	}
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(lock)
		return;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    CString strPathName = GetPathFromItem (hItem);
	GetDocument()->m_GxView->Refresh(strPathName);
	//MessageBox("OnSelCh->gxVRef");
	CGhostXpressApp *pApp=(CGhostXpressApp*)AfxGetApp();
	pApp->GetMainWnd()->GetMenu()->EnableMenuItem(ID_EDIT_CLEAR,MF_BYCOMMAND|((IsRoot())?MF_ENABLED:MF_GRAYED));
	CMainFrame *pMf=(CMainFrame*)pApp->GetMainWnd();
	pMf->m_wndStatusBar.SetWindowText(strPathName);
}

int CLeftView::AddDirectories(HTREEITEM hItem)
{
	HTREEITEM hnewitem;
	CString strPathName = GetPathFromItem(hItem);
	CArray<CFilesInfo, CFilesInfo> fileInfo;
	GetDocument()->GetFileFolderList(strPathName, fileInfo);
	//MessageBox("AddDir->LeftView");
	int size=fileInfo.GetSize();
	for(int i=0;i<size;i++)
	{
		if((fileInfo[i].fd.dwFileAttributes & DIRECTORY)==DIRECTORY)
		{
			hnewitem=GetTreeCtrl().InsertItem(fileInfo[i].fd.cFileName, 0, 1, hItem);
			GetTreeCtrl().InsertItem("", 1, 1, hnewitem);
		}
	}
	return count;
}

int CLeftView::IsRoot()
{
	CString s=GetPathFromItem(GetTreeCtrl().GetSelectedItem());
	if(s.Find('\\')==-1)
		return 1;
	return 0;
}

void CLeftView::Refresh()
{
	HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
	GetTreeCtrl().SendMessage(TVM_EXPAND, (WPARAM) (UINT)(TVE_EXPAND), (LPARAM)hItem);
	DeleteAllChildren(hItem);
	GetTreeCtrl().SendMessage(TVM_EXPAND, (WPARAM) (UINT)(TVE_COLLAPSE | TVE_COLLAPSERESET), (LPARAM)hItem);
	AddDirectories(hItem);
	GetTreeCtrl().SortChildren(hItem);
}

void CLeftView::OnViewRefresh()
{
	CWaitCursor wait;
	GetTreeCtrl().DeleteAllItems();
	WIN32_FIND_DATA fd;
	HANDLE hnd=FindFirstFile(".\\Catalog\\*.gxc",&fd);
	if(hnd == INVALID_HANDLE_VALUE)
	{
		GetDocument()->m_GxView->Refresh("");
		count=0;
		return;
	}
	CStringArray csa;
	do
	{
		if(fd.cFileName[0] != _T('.'))
		{
			CString s=fd.cFileName;
			int pos=s.Find(".");
			s.Delete(pos,4);
			csa.Add(s);
		}
	}while(FindNextFile(hnd,&fd));
	FindClose(hnd);
	int size=csa.GetSize();
	count=size;
	CString temp;
	for(int i=0;i<size-1;i++)
		for(int j=i+1;j<size;j++)
			if(csa[i]>csa[j])
			{
				temp=csa[i];
				csa[i]=csa[j];
				csa[j]=temp;
			}
	for(i=0;i<size;i++)
	{
		HTREEITEM hItem=GetTreeCtrl().InsertItem(csa[i],2,2);
		GetTreeCtrl().InsertItem("", 1, 1, hItem);
	}
	GetTreeCtrl().SortChildren(GetTreeCtrl().GetRootItem());
	GetTreeCtrl().Select(GetTreeCtrl().GetRootItem(),TVGN_CARET);
	AfxGetMainWnd()->SetWindowText("Ghost Xpress - Created by - Vivek Jain - ThE GhOsT Of C");
}

void CLeftView::OnEditFind() 
{
	if(!count)
		return;
	CSearch dlg;
	if(dlg.DoModal()==IDOK)
		SelectPath(dlg.strPathName);
}

void CLeftView::OnEditClear()//Delete
{
	if(IsRoot() && count)
		if(MessageBox("Do You Want to Delete this Catalog\nYou will not be able to Undo this Action",
			"Confirm",MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			CString s;
			s=GetTreeCtrl().GetItemText(GetTreeCtrl().GetSelectedItem());
			s.Insert(0,".\\Catalog\\");
			s+=".gxc";
			remove(s);
			GetTreeCtrl().DeleteItem(GetTreeCtrl().GetSelectedItem());
			OnViewRefresh();
		}
}

void CLeftView::SelectChild(CString name)
{
	HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
	Refresh();
	HTREEITEM hChildItem;
	if((hChildItem=GetTreeCtrl().GetChildItem(hItem))==NULL)
		return;
	HTREEITEM hNextItem=hChildItem;
	do
	{
		hChildItem=hNextItem;
		if(GetTreeCtrl().GetItemText(hChildItem)==name)
		{
			GetTreeCtrl().Select(hChildItem,TVGN_CARET);
			break;
		}
		hNextItem=GetTreeCtrl().GetNextSiblingItem(hChildItem);
	}while(hNextItem != NULL);	
}

void CLeftView::SelectParent()
{
	HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
	HTREEITEM hParentItem=GetTreeCtrl().GetParentItem(hItem);
	if(hParentItem!=NULL)
		GetTreeCtrl().Select(hParentItem,TVGN_CARET);
	GetTreeCtrl().SetFocus();
}

HTREEITEM CLeftView::GetItemRoot()
{
	HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
	if(hItem==NULL)
		return NULL;
	HTREEITEM hParent=hItem;
	do
	{
		hItem=hParent;
		HTREEITEM hParent=GetTreeCtrl().GetParentItem(hItem);		
	}while(hParent!=NULL);
	return hItem;
}

void CLeftView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CEdit* pEdit = GetTreeCtrl().GetEditControl();
	pEdit->SetLimitText(64);
	if(!IsRoot())
	{
		*pResult = 1;
		return;
	}
	bEditing=true;
	*pResult = 0;
}

void CLeftView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	if(pTVDispInfo->item.pszText!=NULL)
	{
		CString s=pTVDispInfo->item.pszText;
		s.TrimLeft();
		s.TrimRight();
		if(s.GetLength())
		{
			char oldName[_MAX_PATH]={".\\Catalog\\"};
			char newName[_MAX_PATH]={".\\Catalog\\"};
			strcat(oldName,GetTreeCtrl().GetItemText(GetTreeCtrl().GetSelectedItem()));
			strcat(newName,s);
			strcat(oldName,".gxc");
			strcat(newName,".gxc");
			*pResult=(Rename(oldName, newName, s.GetBuffer(s.GetLength())))?0:1;
			bEditing=false;
			return;
		}
	}
	bEditing=false;
	*pResult = 0;
}

void CLeftView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(bEditing)
		return;
	switch(nChar)
	{
	case VK_F2:GetTreeCtrl().SendMessage(TVM_EDITLABEL, 0, (LPARAM)GetTreeCtrl().GetSelectedItem());
		break;
	case VK_DELETE:OnEditClear();
		break;
	default:CTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

int CLeftView::Rename(char *oldPath, char *newPath, char *newVolume)
{
	fstream oldFile, newFile;
	newFile.open(newPath, ios::out|ios::binary|ios::noreplace);
	if(!newFile)
	{
		MessageBox("File with this name already exists", "Error", MB_OK|MB_ICONSTOP);
		return 1;
	}
	oldFile.open(oldPath, ios::in|ios::binary|ios::nocreate);
	if(!oldFile)
	{
		MessageBox("Error Renaming File", "Error", MB_OK|MB_ICONSTOP);
		newFile.close();
		remove(newPath);
		return 1;
	}
	EncodedCatalog enc;
	Files files;
	char ch[3]={0};
	oldFile.read(ch,sizeof(char)*3);
	if(ch[0]!='G' || ch[1]!='x')
	{
		if(ch[2]!='2')
		{
			MessageBox("Invalid Catalog", "Error", MB_OK|MB_ICONSTOP);
			newFile.close();
			return 1;
		}
	}
	newFile.write(ch,sizeof(char)*3);
	enc.Read(oldFile);
	files.Read(oldFile, enc.nFileName, enc.nDesc);
	strcpy(files.cFileName, newVolume);
	enc.nFileName=strlen(files.cFileName);
	enc.Write(newFile);
	files.Write(newFile, enc.nFileName, enc.nDesc);
	do
	{
		enc.Read(oldFile);
		files.Read(oldFile, enc.nFileName, enc.nDesc);
		enc.Write(newFile);
		files.Write(newFile, enc.nFileName, enc.nDesc);
	}while(!oldFile.eof() && newFile.good() && oldFile.good());
	oldFile.close();
	newFile.close();
	DWORD lAttrib = GetFileAttributes(oldPath);
	if(!SetFileAttributes(oldPath, lAttrib & (~FILE_ATTRIBUTE_READONLY)) || remove(oldPath))
	{
		MessageBox("Error Renaming File", "Error", MB_OK|MB_ICONSTOP);
		remove(newPath);
		return 1;
	}
	return 0;
}

void CLeftView::SelectSibling(CString name)
{
	HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
	HTREEITEM hNextItem=hItem;
	do
	{
		hItem=hNextItem;
		if(GetTreeCtrl().GetItemText(hItem)==name)
		{
			GetTreeCtrl().Select(hItem,TVGN_CARET);
			break;
		}
		hNextItem=GetTreeCtrl().GetNextSiblingItem(hItem);
	}while(hNextItem != NULL);	
}

void CLeftView::SelectPath(CString path)
{
	CString t;
	lock=1;
	GetTreeCtrl().Select(GetTreeCtrl().GetRootItem(),TVGN_CARET);
	if(path.Find("\\",0)!=-1)
	{
		t=path.SpanExcluding("\\");
		SelectSibling(t);
		GetTreeCtrl().SendMessage(TVM_EXPAND, (WPARAM) (UINT)(TVE_EXPAND),
			(LPARAM)GetTreeCtrl().GetSelectedItem());
		path.Delete(0,path.Find("\\",0)+1);
	}
	while(path.Find("\\",0)!=-1)
	{
		t=path.SpanExcluding("\\");
		path.Delete(0,path.Find("\\",0)+1);
		if(path.Find("\\",0)==-1)
			lock=0;
		SelectChild(t);
		GetTreeCtrl().SendMessage(TVM_EXPAND, (WPARAM) (UINT)(TVE_EXPAND),
			(LPARAM)GetTreeCtrl().GetSelectedItem());
	}
	GetDocument()->m_GxView->Select(path);
}
