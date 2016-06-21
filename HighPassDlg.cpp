// HighPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "HighPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHighPassDlg dialog


CHighPassDlg::CHighPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHighPassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHighPassDlg)
	m_sHighPassIn = _T("");
	m_sHighPassOut = _T("");
	//}}AFX_DATA_INIT
}


void CHighPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHighPassDlg)
	DDX_Text(pDX, IDC_EDIT_HIGHPASSIN, m_sHighPassIn);
	DDX_Text(pDX, IDC_EDIT_HIGHPASSOUT, m_sHighPassOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHighPassDlg, CDialog)
	//{{AFX_MSG_MAP(CHighPassDlg)
	ON_BN_CLICKED(IDC_BUTTON_HIGHPASSIN, OnButtonHighpassin)
	ON_BN_CLICKED(IDC_BUTTON_HIGHPASSOUT, OnButtonHighpassout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHighPassDlg message handlers

void CHighPassDlg::OnButtonHighpassin() 
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
	m_sHighPassIn = FileName;
	//数据刷新，即将m_sLinearTransIn新得到的值显示在对话框中对应的控件上（注意参数false）
	UpdateData(FALSE);
}

void CHighPassDlg::OnButtonHighpassout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	//第一个参数FALSE表示以保存方式显示文件对话框
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sHighPassOut = FileName;
	UpdateData(FALSE);
}

void CHighPassDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sHighPassIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);
	BOOL success = m_DIB.HighPassDib();
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
	file2.Open(m_sHighPassOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
