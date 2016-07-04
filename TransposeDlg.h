#if !defined(AFX_TRANSPOSEDLG_H__44FA0626_8C1F_46F6_A40F_38B9C59B4A1B__INCLUDED_)
#define AFX_TRANSPOSEDLG_H__44FA0626_8C1F_46F6_A40F_38B9C59B4A1B__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransposeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransposeDlg dialog

class CTransposeDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CTransposeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransposeDlg)
	enum { IDD = IDD_DIALOG_TRANSPOSE };
	CString	m_sTransposeIn;
	CString	m_sTransposeOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransposeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransposeDlg)
	afx_msg void OnButtonTransposein();
	afx_msg void OnButtonTransposeout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPOSEDLG_H__44FA0626_8C1F_46F6_A40F_38B9C59B4A1B__INCLUDED_)
