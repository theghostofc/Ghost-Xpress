#if !defined(AFX_PROGRESS_H__354116EB_A90C_4E51_BC90_CEDA40FAFADF__INCLUDED_)
#define AFX_PROGRESS_H__354116EB_A90C_4E51_BC90_CEDA40FAFADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Progress.h : header file
//
#include "catalog.h"
//#include "FilesDataSet.h"
/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class CProgress : public CDialog
{
// Construction
public:
	CProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA
	CString	m_Volume;
	DWORD m_Index;
	char m_Drive;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetMP3Data(CString strPathName);
	CCatalog *cat;
	CWinThread* m_pthread;
	fstream file;
	volatile bool abort;
	volatile bool bClose;
	void Sort(CCatalog *cat);
	static UINT SaveCatalog(LPVOID pParam);
	DWORD Create(CCatalog *cat);
	//void Convert(CCatalog *f, const WIN32_FIND_DATA fd);
	void Convert(CCatalog *f, const WIN32_FIND_DATA fd, const CSizeDesc sizeDesc);
	CString GetFileName(CString pathName, int style=0);
	CString GetDesc(CString filePath);
	BYTE GetAttribs(DWORD attribs);
	void Finalize(CCatalog *cat, DWORD parent, BYTE init=0);
	CSizeDesc CreateCatalog(CCatalog *cat, char path[_MAX_PATH]);//CString path);
	CString ShowTree(CCatalog *cat);
	// Generated message map functions
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESS_H__354116EB_A90C_4E51_BC90_CEDA40FAFADF__INCLUDED_)
