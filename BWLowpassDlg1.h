#if !defined(AFX_BWLOWPASSDLG1_H__2683AD38_B5A7_4E21_B1A6_8E3CF1A4B970__INCLUDED_)
#define AFX_BWLOWPASSDLG1_H__2683AD38_B5A7_4E21_B1A6_8E3CF1A4B970__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BWLowpassDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBWLowpassDlg dialog

class CBWLowpassDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CBWLowpassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBWLowpassDlg)
	enum { IDD = IDD_DIALOG_BWLOWPASS };
	CString	m_sBWLowpassIn;
	CString	m_sBWLowpassOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBWLowpassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBWLowpassDlg)
	afx_msg void OnButtonBwlowpassin();
	afx_msg void OnButtonBwlowpassout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BWLOWPASSDLG1_H__2683AD38_B5A7_4E21_B1A6_8E3CF1A4B970__INCLUDED_)
