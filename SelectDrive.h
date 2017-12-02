#if !defined(AFX_SELECTDRIVE_H__AAD47E62_B7FA_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_SELECTDRIVE_H__AAD47E62_B7FA_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDrive.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectDrive dialog

class CSelectDrive : public CDialog
{
// Construction
public:
	CSelectDrive(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CSelectDrive)
	enum { IDD = IDD_SELECTDRIVE };
	CString m_cddrive;
	CString	m_Volume;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectDrive)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GetDrives();
	// Generated message map functions
	//{{AFX_MSG(CSelectDrive)
	afx_msg void OnRefresh();
	afx_msg void OnSelchangeComboCddrive();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDRIVE_H__AAD47E62_B7FA_11D9_B539_FF062122A253__INCLUDED_)
