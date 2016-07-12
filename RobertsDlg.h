#if !defined(AFX_ROBERTSDLG_H__EF22468A_91DF_4ABD_89BD_EA7D5ECA58BE__INCLUDED_)
#define AFX_ROBERTSDLG_H__EF22468A_91DF_4ABD_89BD_EA7D5ECA58BE__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobertsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobertsDlg dialog

class CRobertsDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CRobertsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRobertsDlg)
	enum { IDD = IDD_DIALOG_ROBERTS };
	CString	m_sRobertsIn;
	CString	m_sRobertsOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobertsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRobertsDlg)
	afx_msg void OnButtonRobertsin();
	afx_msg void OnButtonRobertsout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBERTSDLG_H__EF22468A_91DF_4ABD_89BD_EA7D5ECA58BE__INCLUDED_)
