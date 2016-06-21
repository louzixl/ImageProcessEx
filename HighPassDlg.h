#if !defined(AFX_HIGHPASSDLG_H__E24CA665_C28B_491C_BD55_01425ED9B32C__INCLUDED_)
#define AFX_HIGHPASSDLG_H__E24CA665_C28B_491C_BD55_01425ED9B32C__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HighPassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHighPassDlg dialog

class CHighPassDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CHighPassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHighPassDlg)
	enum { IDD = IDD_DIALOG_HIGHPASS };
	CString	m_sHighPassIn;
	CString	m_sHighPassOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHighPassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHighPassDlg)
	afx_msg void OnButtonHighpassin();
	afx_msg void OnButtonHighpassout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIGHPASSDLG_H__E24CA665_C28B_491C_BD55_01425ED9B32C__INCLUDED_)
