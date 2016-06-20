#if !defined(AFX_LOWPASSEDDLG_H__66EF90FD_2BE9_47C8_874C_F849BAB85E40__INCLUDED_)
#define AFX_LOWPASSEDDLG_H__66EF90FD_2BE9_47C8_874C_F849BAB85E40__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LowPassedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLowPassedDlg dialog

class CLowPassedDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CLowPassedDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLowPassedDlg)
	enum { IDD = IDD_DIALOG_LOWPASS };
	CString	m_sLowPassedIn;
	CString	m_sLowPassedOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLowPassedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLowPassedDlg)
	afx_msg void OnButtonLowpassin();
	afx_msg void OnButtonLowpassout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOWPASSEDDLG_H__66EF90FD_2BE9_47C8_874C_F849BAB85E40__INCLUDED_)
