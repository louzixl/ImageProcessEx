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
	m_sIFourieIn = FileName;
	//数据刷新，即将m_sLinearTransIn新得到的值显示在对话框中对应的控件上（注意参数false）
	UpdateData(FALSE);
}

void CIFourieDlg::OnButtonIfourieout() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	//第一个参数FALSE表示以保存方式显示文件对话框
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

	//为函数准备参数
	int nWidth = m_DIB.Width();
	int nHeight = m_DIB.Height();
	int x, y;
	double dTmpOne, dTmpTwo;
	double dReal, dImag;
	LPBYTE lpImage = m_DIB.RETURN();

	//傅里叶变换的宽度和高度
	int nTransWidth;
	int nTransHeight;

	//图像像素值
	unsigned char unchValue;

	//指向时域数据的指针
	complex<double> *pCTData;
	//指向频域数据的指针
	complex<double> *pCFData;

	//计算进行傅里叶变换的点数
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	//计算进行傅里叶变换的点数
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	//分配内存
	pCTData = new complex<double>[nTransWidth*nTransHeight];
	pCFData = new complex<double>[nTransWidth*nTransHeight];

	//初始化
	//图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTData[y*nTransWidth+x] = complex<double>(0, 0);
		}
	}

	//把图像数据传给pCTData
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			unchValue = lpImage[y*nWidth+x];
			pCTData[y*nTransWidth+x] = complex<double>(unchValue, 0);
		}
	}
	//进行傅里叶变换
	m_DIB.Fourie(pCTData, nWidth, nHeight, pCFData);
	//进行傅里叶反变换
	m_DIB.IFourie(pCFData, pCTData, nWidth, nHeight);
	//把变换的数据传给lpImage
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			dReal = pCTData[y*nTransWidth+x].real();
			dImag = pCTData[y*nTransWidth+x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal+dImag*dImag)+100));
			lpImage[y*nWidth+x] = unchValue;
		}
	}
	//释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	//修改m_DIB的数据
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
