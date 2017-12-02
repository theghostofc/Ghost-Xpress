#if !defined(AFX_SEARCH_H__AAD47E61_B7FA_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_SEARCH_H__AAD47E61_B7FA_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Search.h : header file
//
#include "catalog.h"
/////////////////////////////////////////////////////////////////////////////
// CSearch dialog
class CGhostXpressDoc;
class CSearch : public CDialog
{
// Construction
public:
	CSearch(CWnd* pParent = NULL);   // standard constructor
	CGhostXpressApp* m_pApp;
// Dialog Data
	//{{AFX_DATA(CSearch)
	enum { IDD = IDD_SEARCH };
	CListCtrl	m_list;
	//}}AFX_DATA
	CGhostXpressDoc* GetDocument();
	CString strPathName;
	volatile bool bClose;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool wildcmp(const _TCHAR *wild, const _TCHAR *str);
	void SetControls(BOOL set=FALSE);
	CWinThread* m_pthread;
	static UINT Search(LPVOID pParam);
	CString GetFileName(CString pathName, int style=0);
	void GetAttribString(DWORD attrib, char buff[]);
	void AddItem(Catalog fd, CString path, CString desc, int index=-1);
	// Generated message map functions
	//{{AFX_MSG(CSearch)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnLocate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCH_H__AAD47E61_B7FA_11D9_B539_FF062122A253__INCLUDED_)
