// TranslationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "TranslationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg dialog


CTranslationDlg::CTranslationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTranslationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTranslationDlg)
	m_sTranslationIn = _T("");
	m_sTranslationOut = _T("");
	m_nDirectX = 0;
	m_nDirectY = 0;
	//}}AFX_DATA_INIT
}


void CTranslationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTranslationDlg)
	DDX_Text(pDX, IDC_EDIT_TRANSLATIONIN, m_sTranslationIn);
	DDX_Text(pDX, IDC_EDIT_TRANSLATIONOUT, m_sTranslationOut);
	DDX_Text(pDX, IDC_EDIT_DIRECTX, m_nDirectX);
	DDX_Text(pDX, IDC_EDIT_DIRECTY, m_nDirectY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTranslationDlg, CDialog)
	//{{AFX_MSG_MAP(CTranslationDlg)
	ON_BN_CLICKED(IDC_BUTTON_TRANSLATIONIN, OnButtonTranslationin)
	ON_BN_CLICKED(IDC_BUTTON_TRANSLATIONOUT, OnButtonTranslationout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg message handlers

void CTranslationDlg::OnButtonTranslationin() 
{
	// TODO: Add your control notification handler code here
	//文件类型说明字符串
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	//文件对话框初始化，这里的第一个参数TRUE表示以打开方式显示文件对话框
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径（包括文件名）
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件名赋给格式转换对话框的成员变量m_sLinearTransIn
	m_sTranslationIn = FileName;
	//数据刷新，即将m_sLinearTransIn新得到的值显示在对话框中对应的控件上（注意参数false）
	UpdateData(FALSE);
}

void CTranslationDlg::OnButtonTranslationout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	//第一个参数FALSE表示以保存方式显示文件对话框
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sTranslationOut = FileName;
	UpdateData(FALSE);
}

void CTranslationDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sTranslationIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);
//	DWORD x = m_nDirectX;
//	DWORD y = m_nDirectY;
	BOOL success = m_DIB.TranslationDIB(m_nDirectX, m_nDirectY);
	if(!success)
		CDialog::OnOK();
	else
		MessageBox("success~~~congratulations!", NULL, MB_ICONINFORMATION|MB_OK);
	EndWaitCursor();

	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CFile file2;
	file2.Open(m_sTranslationOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

	BOOL bSuccess = FALSE;
	BeginWaitCursor();
	bSuccess = m_DIB.Save(file2);
	file2.Close();
	EndWaitCursor();

	if(!bSuccess)
	{
		CString strMsg = "Wrong~~~";
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CDialog::OnOK();
}
