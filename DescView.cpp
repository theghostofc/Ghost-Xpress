// DescView.cpp : implementation file
//

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "DescView.h"
#include "Ghost XpressDoc.h"
#include "leftview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDescView

IMPLEMENT_DYNCREATE(CDescView, CEditView)

CDescView::CDescView()
{
}

CDescView::~CDescView()
{
}


BEGIN_MESSAGE_MAP(CDescView, CEditView)
	//{{AFX_MSG_MAP(CDescView)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDescView drawing

void CDescView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDescView diagnostics

#ifdef _DEBUG
void CDescView::AssertValid() const
{
	CEditView::AssertValid();
}

void CDescView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

CGhostXpressDoc* CDescView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGhostXpressDoc)));
	return(CGhostXpressDoc*)m_pDocument;
}
/////////////////////////////////////////////////////////////////////////////
// CDescView message handlers

void CDescView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	GetEditCtrl().SetReadOnly();
	CGhostXpressDoc* pDoc = GetDocument();
	pDoc->m_DescView = this;	
}

void CDescView::OnViewRefresh() 
{
	GetDocument()->m_LeftView->SendMessage(WM_COMMAND,ID_VIEW_REFRESH);	
}

void CDescView::OnEditFind() 
{
	GetDocument()->m_LeftView->SendMessage(WM_COMMAND,ID_EDIT_FIND);
}
