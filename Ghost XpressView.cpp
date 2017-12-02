// Ghost XpressView.cpp : implementation of the CGhostXpressView class
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "leftview.h"
#include "Ghost XpressDoc.h"
#include "Ghost XpressView.h"
#include "descview.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressView

IMPLEMENT_DYNCREATE(CGhostXpressView, CListView)

BEGIN_MESSAGE_MAP(CGhostXpressView, CListView)
	//{{AFX_MSG_MAP(CGhostXpressView)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressView construction/destruction

CGhostXpressView::CGhostXpressView()
{
	// TODO: add construction code here

}

CGhostXpressView::~CGhostXpressView()
{
}

BOOL CGhostXpressView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style&=~LVS_TYPEMASK;
	cs.style|=LVS_REPORT;	
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressView drawing

void CGhostXpressView::OnDraw(CDC* pDC)
{
	CGhostXpressDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
}

void CGhostXpressView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CGhostXpressDoc* pDoc = GetDocument();
	pDoc->m_GxView = this;
	m_pApp=(CGhostXpressApp*)AfxGetApp();
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().SetImageList(&m_pApp->m_imageListL, LVSIL_NORMAL);
	GetListCtrl().SetImageList(&m_pApp->m_imageListS, LVSIL_SMALL);

	CRect rect;
	GetClientRect(&rect);
	int nColumnCount=GetListCtrl().GetHeaderCtrl()->GetItemCount();
	for (int i=0;i<nColumnCount;i++)
		GetListCtrl().DeleteColumn(0);
	GetListCtrl().InsertColumn(0, "Name", LVCFMT_LEFT , (rect.right / 4));
	GetListCtrl().InsertColumn(1, "Size", LVCFMT_RIGHT ,(rect.right / 8));
	GetListCtrl().InsertColumn(2, "Date / Time", LVCFMT_LEFT ,(rect.right / 4));
	GetListCtrl().InsertColumn(3, "Attributes", LVCFMT_LEFT , rect.right / 8);
	GetListCtrl().InsertColumn(4, "Description", LVCFMT_LEFT , rect.right);
}

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressView diagnostics

#ifdef _DEBUG
void CGhostXpressView::AssertValid() const
{
	CListView::AssertValid();
}

void CGhostXpressView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGhostXpressDoc* CGhostXpressView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGhostXpressDoc)));
	return (CGhostXpressDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressView message handlers
void CGhostXpressView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}
void CGhostXpressView::AddItem(FileData fd, int index)
{
	LV_ITEM	lvitem;
	char sNumBuff[_MAX_PATH];
	CLeftView *clv=GetDocument()->m_LeftView;
	int iActualItem;
	CString sText;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem = 0;
	lvitem.iSubItem = 0;
	lvitem.iImage=(index==-1)?2:index;
	lvitem.pszText = fd.cFileName;
	iActualItem = GetListCtrl().InsertItem(&lvitem);
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
	lvitem.iSubItem = 1;
	lvitem.pszText = sNumBuff;
	GetListCtrl().SetItem(&lvitem);
	// Add Time column
	SYSTEMTIME st;
	FileTimeToSystemTime(&fd.ftCreationTime,&st);
	sText.Format("%02d-%02d-%d %02d:%02d", st.wDay,st.wMonth,st.wYear,
		st.wHour,st.wMinute);
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 2;
	lvitem.pszText = sText.GetBuffer(sText.GetLength());
	GetListCtrl().SetItem(&lvitem);
	// Add Attribute column
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 3;
	GetAttribString(fd.dwFileAttributes,sNumBuff);
	lvitem.pszText = sNumBuff;//Insert Attributes
	GetListCtrl().SetItem(&lvitem);
	// Add Description column
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 4;
	strcpy(sNumBuff, fd.desc);
	lvitem.pszText = sNumBuff;//Insert Attributes
	GetListCtrl().SetItem(&lvitem);
}

void CGhostXpressView::GetAttribString(DWORD attrib, char buff[])
{
	DWORD dwAttribs[]={ARCHIVE, COMPRESSED,	DIRECTORY, ENCRYPTED, HIDDEN, READONLY, SYSTEM};
	CString cAttribs="ACDEHRS";
	for(int i=0;i<7;i++)
		buff[i]=((attrib & dwAttribs[i])==dwAttribs[i])?buff[i]=cAttribs[i]:buff[i]='_';
	buff[i]=0;
}

void CGhostXpressView::Refresh(CString strPathName)
{
	CArray<CFilesInfo, CFilesInfo> fileInfo;
	GetDocument()->GetFileFolderList(strPathName, fileInfo);
	GetListCtrl().SetHotItem(-1);
	GetListCtrl().DeleteAllItems();
	int size=fileInfo.GetSize();
	int index=0, found=0;
	for(int i=0;i<size;i++)
	{
		int j=1;
		found=0;
		if((fileInfo[i].fd.dwFileAttributes & DIRECTORY)==DIRECTORY)
			AddItem(fileInfo[i].fd, m_pApp->folIndex);
		else
		{
			CString extn=GetFileName(fileInfo[i].fd.cFileName, 1);
			for(j=1;j<m_pApp->dynIndex;j++)
			{
				if(!strcmpi(m_pApp->m_iconInfo[j].extn, extn))
				{
					index=j;
					found=1;
					break;
				}
			}
			AddItem(fileInfo[i].fd, (found)?index-1:0);
		}
	}
}

CString CGhostXpressView::GetFileName(CString pathName, int style)
{
	TCHAR ch=_T('\\');
	if(style)
		ch=_T('.');
	int pos=pathName.ReverseFind(ch);
	return pathName.Right(pathName.GetLength()-(pos+1));
}

void CGhostXpressView::OnViewRefresh() 
{
	GetDocument()->m_LeftView->SendMessage(WM_COMMAND,ID_VIEW_REFRESH);
}
void CGhostXpressView::OnEditFind() 
{
	GetDocument()->m_LeftView->SendMessage(WM_COMMAND,ID_EDIT_FIND);	
}

void CGhostXpressView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = GetListCtrl().GetNextItem(-1,LVNI_FOCUSED);
	CString s=GetListCtrl().GetItemText(nSel,0);
	GetDocument()->m_LeftView->SelectChild(s);
}

void CGhostXpressView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_BACK:GetDocument()->m_LeftView->SelectParent();
		break;
	case VK_RETURN:OnDblclk(NULL,NULL);
		break;
	default:CListView::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

void CGhostXpressView::Select(CString name)
{
	int nSel = -1;
	do
	{
		nSel=GetListCtrl().GetNextItem(nSel,LVNI_ALL);
		CString s=GetListCtrl().GetItemText(nSel,0);
		if(s==name)
			break;
	}while(nSel!=-1);
	GetListCtrl().SetHotItem(nSel);
}