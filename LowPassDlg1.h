#if !defined(AFX_LOWPASSDLG1_H__E1079BF8_CE58_47F5_975E_BEF12DA15780__INCLUDED_)
#define AFX_LOWPASSDLG1_H__E1079BF8_CE58_47F5_975E_BEF12DA15780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LowPassDlg1.h : header file
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

#endif // !defined(AFX_LOWPASSDLG1_H__E1079BF8_CE58_47F5_975E_BEF12DA15780__INCLUDED_)
