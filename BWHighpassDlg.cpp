// BWHighpassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "BWHighpassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBWHighpassDlg dialog


CBWHighpassDlg::CBWHighpassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBWHighpassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBWHighpassDlg)
	m_sBWHighpassIn = _T("");
	m_sBWHighpassOut = _T("");
	//}}AFX_DATA_INIT
}


void CBWHighpassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBWHighpassDlg)
	DDX_Text(pDX, IDC_EDIT_BWHIGHPASSIN, m_sBWHighpassIn);
	DDX_Text(pDX, IDC_EDIT_BWHIGHPASSOUT, m_sBWHighpassOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBWHighpassDlg, CDialog)
	//{{AFX_MSG_MAP(CBWHighpassDlg)
	ON_BN_CLICKED(IDC_BUTTON_BWHIGHPASSIN, OnButtonBwhighpassin)
	ON_BN_CLICKED(IDC_BUTTON_BWHIGHPASSOUT, OnButtonBwhighpassout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBWHighpassDlg message handlers

void CBWHighpassDlg::OnButtonBwhighpassin() 
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
	m_sBWHighpassIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CBWHighpassDlg::OnButtonBwhighpassout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sBWHighpassOut = FileName;
	UpdateData(FALSE);
}

void CBWHighpassDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sBWHighpassIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);

	//׼������
	LPBYTE lpImage = m_DIB.RETURN();
	int nWidth = m_DIB.Width();
	int nHeight = m_DIB.Height();
	int nRadius = 5;
	//�����˲�
	m_DIB.ButterWorthHighPass(lpImage, nWidth, nHeight, nRadius);
	
	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CFile file2;
	file2.Open(m_sBWHighpassOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
