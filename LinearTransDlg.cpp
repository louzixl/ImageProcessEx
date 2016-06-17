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
	ON_BN_CLICKED(IDC_BUTTON_LINEARINPUT, OnButtonLinearinput)
	ON_BN_CLICKED(IDC_BUTTON_LINEAROUTPUT, OnButtonLinearoutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinearTransDlg message handlers

void CLinearTransDlg::OnButtonLinearinput() 
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
	m_sLinearTransIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CLinearTransDlg::OnButtonLinearoutput() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sLinearTransOut = FileName;
	UpdateData(FALSE);
}

void CLinearTransDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sLinearTransIn, CFile::modeRead|CFile::shareDenyWrite);
	/*CFileException fe1;
	if(!file1.Open(m_sLinearTransIn, CFile::modeRead|CFile::shareDenyWrite, &fe1))
	{
		ReportSaveLoadException(m_sLinearTransIn, &fe1, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}*/

	//DeleteContents();  //��֪��ʲô��˼
	BeginWaitCursor(); //��ʾɳ©��꣬�����û�ϵͳæ

	//����Read DIBFile������ȡλͼ������LinearTransform����������������
//	TRY
//	{
		m_DIB.Read(file1);
		BOOL success = m_DIB.LinearTransform(50, 180);
		if(!success)
			CDialog::OnOK();
		else
			MessageBox("success~~~congratulations!", NULL, MB_ICONINFORMATION|MB_OK);
//	}
/*	CATCH(CFileException, eLoad)
	{
		file1.Abort();
		EndWaitCursor();
		ReportSaveLoadException(m_sLinearTransIn, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH
*/		
	EndWaitCursor(); //�ָ���ǰ�Ĺ��

	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";

		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
		//return FALSE;
	}
	//SetPathName(m_sLinearTransIn); //����
	//SetModifiedFlag(FALSE);

	CFile file2;
	file2.Open(m_sLinearTransOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);
/*	CFileException fe2;

	if(!file2.Open(m_sLinearTransOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive, &fe2))
	{
		ReportSaveLoadException(m_sLinearTransOut, &fe2, TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
*/
	//����SaveDIB�����洢λͼ
	BOOL bSuccess = FALSE;
//	TRY
//	{
		BeginWaitCursor();
		bSuccess = m_DIB.Save(file2);
		file2.Close();
//	}
/*	CATCH(CException, eSave)
	{
		file2.Abort();
		EndWaitCursor();
		ReportSaveLoadException(m_sLinearTransOut, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH;
*/
	EndWaitCursor();
	//SetModifiedFlag(FALSE);

	if(!bSuccess)
	{
		CString strMsg = "Wrong~~~";
		//strMsg.LoadString(IDS_CANNOT_SAVE_DIB);
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CDialog::OnOK();
}
