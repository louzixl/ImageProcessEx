// LinearTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "LinearTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinearTransDlg dialog


CLinearTransDlg::CLinearTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinearTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinearTransDlg)
	m_sLinearTransIn = _T("");
	m_sLinearTransOut = _T("");
	//}}AFX_DATA_INIT
}


void CLinearTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinearTransDlg)
	DDX_Text(pDX, IDC_EDIT_LINEARINPUT, m_sLinearTransIn);
	DDX_Text(pDX, IDC_EDIT_LINEAROUTPUT, m_sLinearTransOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinearTransDlg, CDialog)
	//{{AFX_MSG_MAP(CLinearTransDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinearTransDlg message handlers
