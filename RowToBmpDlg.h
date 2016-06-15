#if !defined(AFX_ROWTOBMPDLG_H__16CA687D_512B_49BD_8F97_4AFC35782AE7__INCLUDED_)
#define AFX_ROWTOBMPDLG_H__16CA687D_512B_49BD_8F97_4AFC35782AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RowToBmpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRowToBmpDlg dialog

class CRowToBmpDlg : public CDialog
{
// Construction
public:
	CRowToBmpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRowToBmpDlg)
	enum { IDD = IDD_DIALOG_RAWTOBMP };
	CString	m_sBmpOut;
	int		m_iHeight;
	CString	m_sRawIn;
	int		m_iWidth;
	int		m_iBSQ;
	int		m_iGray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRowToBmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRowToBmpDlg)
	afx_msg void OnButtonRawview();
	afx_msg void OnButtonBmpview();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROWTOBMPDLG_H__16CA687D_512B_49BD_8F97_4AFC35782AE7__INCLUDED_)
