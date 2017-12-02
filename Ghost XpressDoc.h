// Ghost XpressDoc.h : interface of the CGhostXpressDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GHOSTXPRESSDOC_H__7CBB600C_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_GHOSTXPRESSDOC_H__7CBB600C_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "catalog.h"

class CGhostXpressView;
class CLeftView;
class CDescView;

class CGhostXpressDoc : public CDocument
{
protected: // create from serialization only
	CGhostXpressDoc();
	DECLARE_DYNCREATE(CGhostXpressDoc)

// Attributes
public:
	CGhostXpressView* m_GxView;
	CLeftView* m_LeftView;
	CDescView* m_DescView;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGhostXpressDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetFileFolderList(CString strPathName, CArray<CFilesInfo, CFilesInfo> &filesInfo);
	void SetDesc(CString desc);
	virtual ~CGhostXpressDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGhostXpressDoc)
	afx_msg void OnFileNewCat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GHOSTXPRESSDOC_H__7CBB600C_B7F7_11D9_B539_FF062122A253__INCLUDED_)
