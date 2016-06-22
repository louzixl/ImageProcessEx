// MiddlePassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "MiddlePassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiddlePassDlg dialog


CMiddlePassDlg::CMiddlePassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiddlePassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiddlePassDlg)
	m_sMiddlePassIn = _T("");
	m_sMiddlePassOut = _T("");
	//}}AFX_DATA_INIT
}


void CMiddlePassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiddlePassDlg)
	DDX_Text(pDX, IDC_EDIT_MIDDLEPASSIN, m_sMiddlePassIn);
	DDX_Text(pDX, IDC_EDIT_MIDDLEPASSOUT, m_sMiddlePassOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiddlePassDlg, CDialog)
	//{{AFX_MSG_MAP(CMiddlePassDlg)
	ON_BN_CLICKED(IDC_BUTTON_MIDDLEPASSIN, OnButtonMiddlepassin)
	ON_BN_CLICKED(IDC_BUTTON_MIDDLEPASSOUT, OnButtonMiddlepassout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiddlePassDlg message handlers

void CMiddlePassDlg::OnButtonMiddlepassin() 
{
	// TODO: Add your control notification handler code here
	//�ļ�����˵���ַ���
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//�ļ��Ի����ʼ��������ĵ�һ������TRUE��ʾ�Դ򿪷�ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	//�����ļ��򿪶Ի���
	SelectFile.DoModal();
	//�õ���ѡ�ļ�·���������ļ�����
	CString FileName;
	FileName = SelectFile.GetPathName();
	//���õ����ļ���������ʽת���Ի���ĳ�Ա����m_sLinearTransIn
	m_sMiddlePassIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CMiddlePassDlg::OnButtonMiddlepassout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sMiddlePassOut = FileName;
	UpdateData(FALSE);
}

void CMiddlePassDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sMiddlePassIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);
	BOOL success = m_DIB.MedianFilterDIB();
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
	file2.Open(m_sMiddlePassOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
