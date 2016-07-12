// BWLowpassDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "BWLowpassDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBWLowpassDlg dialog


CBWLowpassDlg::CBWLowpassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBWLowpassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBWLowpassDlg)
	m_sBWLowpassIn = _T("");
	m_sBWLowpassOut = _T("");
	//}}AFX_DATA_INIT
}


void CBWLowpassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBWLowpassDlg)
	DDX_Text(pDX, IDC_EDIT_BWLOWPASSIN, m_sBWLowpassIn);
	DDX_Text(pDX, IDC_EDIT_BWLOWPASSOUT, m_sBWLowpassOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBWLowpassDlg, CDialog)
	//{{AFX_MSG_MAP(CBWLowpassDlg)
	ON_BN_CLICKED(IDC_BUTTON_BWLOWPASSIN, OnButtonBwlowpassin)
	ON_BN_CLICKED(IDC_BUTTON_BWLOWPASSOUT, OnButtonBwlowpassout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBWLowpassDlg message handlers

void CBWLowpassDlg::OnButtonBwlowpassin() 
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
	m_sBWLowpassIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CBWLowpassDlg::OnButtonBwlowpassout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sBWLowpassOut = FileName;
	UpdateData(FALSE);
}

void CBWLowpassDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sBWLowpassIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);

	//׼������
	LPBYTE lpImage = m_DIB.RETURN();
	int nWidth = m_DIB.Width();
	int nHeight = m_DIB.Height();
	int nRadius = 5;
	//�����˲�
	m_DIB.ButterWorthLowPass(lpImage, nWidth, nHeight, nRadius);
	
	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CFile file2;
	file2.Open(m_sBWLowpassOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
