// PrewittDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "PrewittDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrewittDlg dialog


CPrewittDlg::CPrewittDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrewittDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrewittDlg)
	m_sPrewittIn = _T("");
	m_sPrewittOut = _T("");
	//}}AFX_DATA_INIT
}


void CPrewittDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrewittDlg)
	DDX_Text(pDX, IDC_EDIT_PREWITTIN, m_sPrewittIn);
	DDX_Text(pDX, IDC_EDIT_PREWITTOUT, m_sPrewittOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrewittDlg, CDialog)
	//{{AFX_MSG_MAP(CPrewittDlg)
	ON_BN_CLICKED(IDC_BUTTON_PREWITTIN, OnButtonPrewittin)
	ON_BN_CLICKED(IDC_BUTTON_PREWITTOUT, OnButtonPrewittout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrewittDlg message handlers

void CPrewittDlg::OnButtonPrewittin() 
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
	m_sPrewittIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CPrewittDlg::OnButtonPrewittout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sPrewittOut = FileName;
	UpdateData(FALSE);
}

void CPrewittDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sPrewittIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);

	//���޸�
	int tag = 0;
	BOOL success = m_DIB.PrewittDIB(tag);
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
	file2.Open(m_sPrewittOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
