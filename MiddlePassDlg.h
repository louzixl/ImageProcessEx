#if !defined(AFX_MIDDLEPASSDLG_H__222641A0_24DF_48BF_9E15_1CE4751A84D7__INCLUDED_)
#define AFX_MIDDLEPASSDLG_H__222641A0_24DF_48BF_9E15_1CE4751A84D7__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiddlePassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiddlePassDlg dialog

class CMiddlePassDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CMiddlePassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMiddlePassDlg)
	enum { IDD = IDD_DIALOG_MIDDLEPASS };
	CString	m_sMiddlePassIn;
	CString	m_sMiddlePassOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiddlePassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiddlePassDlg)
	afx_msg void OnButtonMiddlepassin();
	afx_msg void OnButtonMiddlepassout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDDLEPASSDLG_H__222641A0_24DF_48BF_9E15_1CE4751A84D7__INCLUDED_)
