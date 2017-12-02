// Ghost XpressView.h : interface of the CGhostXpressView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GHOSTXPRESSVIEW_H__7CBB600E_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_GHOSTXPRESSVIEW_H__7CBB600E_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "catalog.h"

class CGhostXpressView : public CListView
{
protected: // create from serialization only
	CGhostXpressView();
	DECLARE_DYNCREATE(CGhostXpressView)

// Attributes
public:
	CGhostXpressDoc* GetDocument();
	CGhostXpressApp *m_pApp;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGhostXpressView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void Select(CString name);
	void Refresh(CString strPathName);
	virtual ~CGhostXpressView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void GetAttribString(DWORD attrib, char buff[]);
	//void AddItem(Catalog fd, int index=-1);
	void AddItem(FileData fd, int index=-1);
// Generated message map functions
protected:
	CString GetFileName(CString pathName, int style=0);
	//{{AFX_MSG(CGhostXpressView)
	afx_msg void OnViewRefresh();
	afx_msg void OnEditFind();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Ghost XpressView.cpp
inline CGhostXpressDoc* CGhostXpressView::GetDocument()
   { return (CGhostXpressDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GHOSTXPRESSVIEW_H__7CBB600E_B7F7_11D9_B539_FF062122A253__INCLUDED_)
