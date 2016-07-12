#if !defined(AFX_PREWITTDLG_H__092FC732_E676_4F85_9A1E_921D2CA4F87F__INCLUDED_)
#define AFX_PREWITTDLG_H__092FC732_E676_4F85_9A1E_921D2CA4F87F__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrewittDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrewittDlg dialog

class CPrewittDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CPrewittDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrewittDlg)
	enum { IDD = IDD_DIALOG_PREWITT };
	CString	m_sPrewittIn;
	CString	m_sPrewittOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrewittDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrewittDlg)
	afx_msg void OnButtonPrewittin();
	afx_msg void OnButtonPrewittout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREWITTDLG_H__092FC732_E676_4F85_9A1E_921D2CA4F87F__INCLUDED_)
