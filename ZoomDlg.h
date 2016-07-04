#if !defined(AFX_ZOOMDLG_H__1DB67150_F5F1_4A54_BBA3_A140758FFD9B__INCLUDED_)
#define AFX_ZOOMDLG_H__1DB67150_F5F1_4A54_BBA3_A140758FFD9B__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog

class CZoomDlg : public CDialog
{
// Construction
public:
	CDib m_DIB;
	CZoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZoomDlg)
	enum { IDD = IDD_DIALOG_ZOOM };
	CString	m_sZoomIn;
	CString	m_sZoomOut;
	float	m_fZoomX;
	float	m_fZoomY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomDlg)
	afx_msg void OnButtonZoomin();
	afx_msg void OnButtonZoomout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDLG_H__1DB67150_F5F1_4A54_BBA3_A140758FFD9B__INCLUDED_)
