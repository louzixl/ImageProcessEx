#if !defined(AFX_FOURIEDLG_H__DD2EEEFF_96FB_47A4_BCE6_A17561669223__INCLUDED_)
#define AFX_FOURIEDLG_H__DD2EEEFF_96FB_47A4_BCE6_A17561669223__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FourieDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFourieDlg dialog

class CFourieDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CFourieDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFourieDlg)
	enum { IDD = IDD_DIALOG_FOURIE };
	CString	m_sFourieIn;
	CString	m_sFourieOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFourieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFourieDlg)
	afx_msg void OnButtonFouriein();
	afx_msg void OnButtonFourieout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOURIEDLG_H__DD2EEEFF_96FB_47A4_BCE6_A17561669223__INCLUDED_)
