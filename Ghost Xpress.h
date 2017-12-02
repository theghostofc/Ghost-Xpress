// Ghost Xpress.h : main header file for the GHOST XPRESS application
//

#if !defined(AFX_GHOSTXPRESS_H__7CBB6006_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_GHOSTXPRESS_H__7CBB6006_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGhostXpressApp:
// See Ghost Xpress.cpp for the implementation of this class
//
struct CIconInfo
{
	int index;
	CString extn;
};
class CGhostXpressApp : public CWinApp
{
public:
	CGhostXpressApp();
	CImageList m_imageListL;
	CImageList m_imageListS;
	CArray<CIconInfo, CIconInfo> m_iconInfo;
	int dynIndex;
	int folIndex;
	int exeIndex;
	void Init();
	void GetIcons();
	CSingleDocTemplate* pDocTemplate;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGhostXpressApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGhostXpressApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int IsExe(CString extn);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GHOSTXPRESS_H__7CBB6006_B7F7_11D9_B539_FF062122A253__INCLUDED_)
