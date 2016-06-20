#if !defined(AFX_LOWPASSDLG_H__F044DD01_E445_4478_887D_BCC8FF11E6DD__INCLUDED_)
#define AFX_LOWPASSDLG_H__F044DD01_E445_4478_887D_BCC8FF11E6DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LowPassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLowPassDlg dialog

class CLowPassDlg : public CDialog
{
// Construction
public:
	CLowPassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLowPassDlg)
	enum { IDD = IDD_DIALOG_LOWPASS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLowPassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLowPassDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOWPASSDLG_H__F044DD01_E445_4478_887D_BCC8FF11E6DD__INCLUDED_)
