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
	m_sTranslationIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CTranslationDlg::OnButtonTranslationout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
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
