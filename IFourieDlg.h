#if !defined(AFX_IFOURIEDLG_H__A5C4CE39_6F99_40A0_8DC7_F2AF53D3BEFA__INCLUDED_)
#define AFX_IFOURIEDLG_H__A5C4CE39_6F99_40A0_8DC7_F2AF53D3BEFA__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IFourieDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIFourieDlg dialog

class CIFourieDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CIFourieDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIFourieDlg)
	enum { IDD = IDD_DIALOG_IFOURIE };
	CString	m_sIFourieIn;
	CString	m_sIFourieOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIFourieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIFourieDlg)
	afx_msg void OnButtonIfouriein();
	afx_msg void OnButtonIfourieout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IFOURIEDLG_H__A5C4CE39_6F99_40A0_8DC7_F2AF53D3BEFA__INCLUDED_)
