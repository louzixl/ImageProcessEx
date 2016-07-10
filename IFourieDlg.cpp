// IFourieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "IFourieDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIFourieDlg dialog


CIFourieDlg::CIFourieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIFourieDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIFourieDlg)
	m_sIFourieIn = _T("");
	m_sIFourieOut = _T("");
	//}}AFX_DATA_INIT
}


void CIFourieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIFourieDlg)
	DDX_Text(pDX, IDC_EDIT_IFOURIEIN, m_sIFourieIn);
	DDX_Text(pDX, IDC_EDIT_IFOURIEOUT, m_sIFourieOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIFourieDlg, CDialog)
	//{{AFX_MSG_MAP(CIFourieDlg)
	ON_BN_CLICKED(IDC_BUTTON_IFOURIEIN, OnButtonIfouriein)
	ON_BN_CLICKED(IDC_BUTTON_IFOURIEOUT, OnButtonIfourieout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIFourieDlg message handlers

void CIFourieDlg::OnButtonIfouriein() 
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
	m_sIFourieIn = FileName;
	//����ˢ�£�����m_sLinearTransIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CIFourieDlg::OnButtonIfourieout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
	//��һ������FALSE��ʾ�Ա��淽ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sIFourieOut = FileName;
	UpdateData(FALSE);
}

void CIFourieDlg::OnOK() 
{
	// TODO: Add extra validation here
	CFile file1;
	file1.Open(m_sIFourieIn, CFile::modeRead|CFile::shareDenyWrite);

	BeginWaitCursor();
	m_DIB.Read(file1);

	//Ϊ����׼������
	int nWidth = m_DIB.Width();
	int nHeight = m_DIB.Height();
	int x, y;
	double dTmpOne, dTmpTwo;
	double dReal, dImag;
	LPBYTE lpImage = m_DIB.RETURN();

	//����Ҷ�任�Ŀ�Ⱥ͸߶�
	int nTransWidth;
	int nTransHeight;

	//ͼ������ֵ
	unsigned char unchValue;

	//ָ��ʱ�����ݵ�ָ��
	complex<double> *pCTData;
	//ָ��Ƶ�����ݵ�ָ��
	complex<double> *pCFData;

	//������и���Ҷ�任�ĵ���
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	//������и���Ҷ�任�ĵ���
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	//�����ڴ�
	pCTData = new complex<double>[nTransWidth*nTransHeight];
	pCFData = new complex<double>[nTransWidth*nTransHeight];

	//��ʼ��
	//ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTData[y*nTransWidth+x] = complex<double>(0, 0);
		}
	}

	//��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			unchValue = lpImage[y*nWidth+x];
			pCTData[y*nTransWidth+x] = complex<double>(unchValue, 0);
		}
	}
	//���и���Ҷ�任
	m_DIB.Fourie(pCTData, nWidth, nHeight, pCFData);
	//���и���Ҷ���任
	m_DIB.IFourie(pCFData, pCTData, nWidth, nHeight);
	//�ѱ任�����ݴ���lpImage
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			dReal = pCTData[y*nTransWidth+x].real();
			dImag = pCTData[y*nTransWidth+x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal+dImag*dImag)+100));
			lpImage[y*nWidth+x] = unchValue;
		}
	}
	//�ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	//�޸�m_DIB������
	m_DIB.DD(lpImage);

	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";
		MessageBox(strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}

	CFile file2;
	file2.Open(m_sIFourieOut, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive);

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
