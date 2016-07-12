#if !defined(AFX_SOBELDLG_H__55334012_911C_436A_B313_006B484A805F__INCLUDED_)
#define AFX_SOBELDLG_H__55334012_911C_436A_B313_006B484A805F__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SobelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSobelDlg dialog

class CSobelDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CSobelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSobelDlg)
	enum { IDD = IDD_DIALOG_SOBEL };
	CString	m_sSobelIn;
	CString	m_sSobelOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSobelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSobelDlg)
	afx_msg void OnButtonSobelin();
	afx_msg void OnButtonSobelout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOBELDLG_H__55334012_911C_436A_B313_006B484A805F__INCLUDED_)
