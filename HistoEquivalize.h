#if !defined(AFX_HISTOEQUIVALIZE_H__B5080186_8C65_4D38_BA69_FDA66FE40052__INCLUDED_)
#define AFX_HISTOEQUIVALIZE_H__B5080186_8C65_4D38_BA69_FDA66FE40052__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoEquivalize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoEquivalize dialog

class CHistoEquivalize : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CHistoEquivalize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistoEquivalize)
	enum { IDD = IDD_DIALOG_HISTOEQUIVALIZE };
	CString	m_sHistoInput;
	CString	m_sHistoOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoEquivalize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistoEquivalize)
	afx_msg void OnButtonHistoinput();
	afx_msg void OnButtonHistooutput();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOEQUIVALIZE_H__B5080186_8C65_4D38_BA69_FDA66FE40052__INCLUDED_)
