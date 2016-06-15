// ImageProcessEx.h : main header file for the IMAGEPROCESSEX application
//

#if !defined(AFX_IMAGEPROCESSEX_H__61D96AA5_A379_495B_8007_7D8D89F28635__INCLUDED_)
#define AFX_IMAGEPROCESSEX_H__61D96AA5_A379_495B_8007_7D8D89F28635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExApp:
// See ImageProcessEx.cpp for the implementation of this class
//

class CImageProcessExApp : public CWinApp
{
public:
	CImageProcessExApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessExApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageProcessExApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSEX_H__61D96AA5_A379_495B_8007_7D8D89F28635__INCLUDED_)
