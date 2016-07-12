#if !defined(AFX_BWHIGHPASSDLG_H__33E3A9C2_431B_48EB_A92B_A739E245C7F6__INCLUDED_)
#define AFX_BWHIGHPASSDLG_H__33E3A9C2_431B_48EB_A92B_A739E245C7F6__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BWHighpassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBWHighpassDlg dialog

class CBWHighpassDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CBWHighpassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBWHighpassDlg)
	enum { IDD = IDD_DIALOG_BWHIGHPASS };
	CString	m_sBWHighpassIn;
	CString	m_sBWHighpassOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBWHighpassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBWHighpassDlg)
	afx_msg void OnButtonBwhighpassin();
	afx_msg void OnButtonBwhighpassout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BWHIGHPASSDLG_H__33E3A9C2_431B_48EB_A92B_A739E245C7F6__INCLUDED_)
