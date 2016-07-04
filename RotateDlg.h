#if !defined(AFX_ROTATEDLG_H__EF997362_2810_4866_9388_862DAAF0E666__INCLUDED_)
#define AFX_ROTATEDLG_H__EF997362_2810_4866_9388_862DAAF0E666__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateDlg dialog

class CRotateDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CRotateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotateDlg)
	enum { IDD = IDD_DIALOG_ROTATE };
	int		m_nRotateAngel;
	CString	m_sRotateIn;
	CString	m_sRotateOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotateDlg)
	afx_msg void OnButtonRotatein();
	afx_msg void OnButtonRotateout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEDLG_H__EF997362_2810_4866_9388_862DAAF0E666__INCLUDED_)
