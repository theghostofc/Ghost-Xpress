#if !defined(AFX_DESCVIEW_H__7CBB601A_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_DESCVIEW_H__7CBB601A_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DescView.h : header file
//
#include "Ghost XpressDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDescView view

class CDescView : public CEditView
{
protected:
	CDescView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDescView)

// Attributes
public:
	CGhostXpressDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDescView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDescView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDescView)
	afx_msg void OnViewRefresh();
	afx_msg void OnEditFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESCVIEW_H__7CBB601A_B7F7_11D9_B539_FF062122A253__INCLUDED_)
