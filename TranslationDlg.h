#if !defined(AFX_TRANSLATIONDLG_H__6BCC7032_966B_44A7_9B9E_34CCDF92073C__INCLUDED_)
#define AFX_TRANSLATIONDLG_H__6BCC7032_966B_44A7_9B9E_34CCDF92073C__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranslationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg dialog

class CTranslationDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CTranslationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTranslationDlg)
	enum { IDD = IDD_DIALOG_TRANSLATION };
	CString	m_sTranslationIn;
	CString	m_sTranslationOut;
	DWORD	m_nDirectX;
	DWORD	m_nDirectY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranslationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTranslationDlg)
	afx_msg void OnButtonTranslationin();
	afx_msg void OnButtonTranslationout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSLATIONDLG_H__6BCC7032_966B_44A7_9B9E_34CCDF92073C__INCLUDED_)
