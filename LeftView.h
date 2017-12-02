// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__7CBB6010_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_LEFTVIEW_H__7CBB6010_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGhostXpressDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CGhostXpressDoc* GetDocument();
	void SelectChild(CString name);
	void SelectParent();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void SelectPath(CString path);
	void SelectSibling(CString name);
	void Refresh();
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int count;
	int lock;
	bool bEditing;
	CImageList m_imageList;
	CString GetPathFromItem(HTREEITEM hItem);
	int AddDirectories(HTREEITEM hItem);
	void DeleteFirstChild(HTREEITEM hItem);
	void DeleteAllChildren(HTREEITEM hItem);
	int IsRoot();

// Generated message map functions
protected:
	int Rename(char *oldPath, char *newPath, char *newVolume);
	HTREEITEM GetItemRoot();
	//{{AFX_MSG(CLeftView)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewRefresh();
	afx_msg void OnEditFind();
	afx_msg void OnEditClear();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CGhostXpressDoc* CLeftView::GetDocument()
   { return (CGhostXpressDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__7CBB6010_B7F7_11D9_B539_FF062122A253__INCLUDED_)
