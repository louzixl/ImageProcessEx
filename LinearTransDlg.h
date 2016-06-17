#if !defined(AFX_LINEARTRANSDLG_H__05619C92_BA3C_416D_B27C_AA68761DBC46__INCLUDED_)
#define AFX_LINEARTRANSDLG_H__05619C92_BA3C_416D_B27C_AA68761DBC46__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinearTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinearTransDlg dialog

class CLinearTransDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CLinearTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinearTransDlg)
	enum { IDD = IDD_DIALOG_LINEARTRANS };
	CString	m_sLinearTransIn;
	CString	m_sLinearTransOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinearTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinearTransDlg)
	afx_msg void OnButtonLinearinput();
	afx_msg void OnButtonLinearoutput();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEARTRANSDLG_H__05619C92_BA3C_416D_B27C_AA68761DBC46__INCLUDED_)
