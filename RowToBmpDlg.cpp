// RowToBmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessEx.h"
#include "RowToBmpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRowToBmpDlg dialog


CRowToBmpDlg::CRowToBmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRowToBmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRowToBmpDlg)
	m_sBmpOut = _T("");
	m_iHeight = 0;
	m_sRawIn = _T("");
	m_iWidth = 0;
	m_iBSQ = -1;
	m_iGray = -1;
	//}}AFX_DATA_INIT
}


void CRowToBmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRowToBmpDlg)
	DDX_Text(pDX, IDC_EDIT_BMPCIN, m_sBmpOut);
	DDX_Text(pDX, IDC_EDIT_HEIGHTCIN, m_iHeight);
	DDX_Text(pDX, IDC_EDIT_RAWCIN, m_sRawIn);
	DDX_Text(pDX, IDC_EDIT_WIDTHCIN, m_iWidth);
	DDX_Radio(pDX, IDC_RADIO_BSQ, m_iBSQ);
	DDX_Radio(pDX, IDC_RADIO_GRAY, m_iGray);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRowToBmpDlg, CDialog)
	//{{AFX_MSG_MAP(CRowToBmpDlg)
	ON_BN_CLICKED(IDC_BUTTON_RAWVIEW, OnButtonRawview)
	ON_BN_CLICKED(IDC_BUTTON_BMPVIEW, OnButtonBmpview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRowToBmpDlg message handlers

void CRowToBmpDlg::OnButtonRawview() 
{
	// TODO: Add your control notification handler code here
	//文件类型说明字符串
	static char BASED_CODE file[] = "RAW Files(*.RAW)|*.raw|所有文件(*.*)|*.*||";
	//文件对话框初始化，这里的第一个参数TRUE表示以打开方式显示文件对话框
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径（包括文件名）
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件名赋给格式转换对话框的成员变量m_sRawIn
	m_sRawIn = FileName;
	//数据刷新，即将m_sRawIn新得到的值显示在对话框中对应的控件上（注意参数false）
	UpdateData(FALSE);
}

void CRowToBmpDlg::OnButtonBmpview() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	CFileDialog SelectFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	SelectFile.DoModal();
	CString FileName;
	FileName = SelectFile.GetPathName();
	m_sBmpOut = FileName;
	UpdateData(FALSE);
}

void CRowToBmpDlg::OnOK() 
{
	// TODO: Add extra validation here
	//数据刷新
	UpdateData(TRUE);
	//声明DIB句柄HDIB
	DECLARE_HANDLE(HDIB);
	HDIB hDIB; //定义HDIB句柄变量，用来存放bmp位图
	LPSTR pDIB; //定义字符指针变量，用来存放位图数据
	//打开文件
	CFile fileraw, filebmp; //CFile读写文件
	//以读和拒绝写的方式打开所选文件名为m_sRawIn的Raw格式文件
	fileraw.Open(m_sRawIn, CFile::modeRead|CFile::shareDenyWrite, NULL);
	//得到文件的大小
	DWORD filesize = fileraw.GetLength();
	//根据Raw格式文件的大小来申请分配内存空间
	hDIB = (HDIB)::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, filesize);
	//若分配不成功，即hDIB==0，则程序返回
	if(hDIB==0)
	{
		return;
	}
	//若分配成功，得到句柄内存的其实地址
	pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);
	//将Raw文件内容读到pDIB所指向的内存块
	if(m_iGray==0) //如果Raw文件是灰度，则不需要考虑m_iBSQ
		fileraw.ReadHuge(pDIB, filesize);
	else //如果Raw文件是真彩色，需要考虑m_iBSQ
	{
		if(m_iBSQ==0)
		{
			//这里由学生添加
			AfxMessageBox("真彩色BSQ方式没有实现，由学生添加", MB_OK, 0);
			return;
		}
		if(m_iBSQ==1)
		{
			//这里由学生添加
			AfxMessageBox("真彩色BIL方式没有实现，由学生添加", MB_OK, 0);
			return;
		}
		if(m_iBSQ==2)
		{
			//这里由学生添加
			AfxMessageBox("真彩色BIP方式没有实现，由学生添加", MB_OK, 0);
			return;
		}
	}
	//不允许其他程序同时打开所选BMP文件
	filebmp.Open(m_sBmpOut, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);
	//写文件：文件头、位图信息图、调色板、位图数据
	//根据BMP文件由四部分组成，下面分别给各部分赋值并写入BMP文件
	BITMAPFILEHEADER bmfHdr; //位图文件头
	BITMAPINFOHEADER bmpHdr; //位图信息头
	RGBQUAD rgb[256];        //调色板

	//位图文件头部分信息初始化
	bmfHdr.bfType = 'MB';
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	//位图信息头部分信息初始化
	bmpHdr.biSize = 40;
	bmpHdr.biWidth = m_iWidth;
	bmpHdr.biHeight = m_iHeight;
	bmpHdr.biPlanes = 1;
	bmpHdr.biCompression = BI_RGB;
	bmpHdr.biXPelsPerMeter = 0;
	bmpHdr.biYPelsPerMeter = 0;
	bmpHdr.biClrUsed = 0;
	bmpHdr.biClrImportant = 0;

	if(m_iGray==0)
	{
		//若Raw文件为灰度图像，给相应位图文件头和位图信息头部分内容初始化
		bmpHdr.biBitCount = 8;
		bmfHdr.bfOffBits = 1078; //bmfHdr.bfOffBits=14+40+256*4;
		bmpHdr.biSizeImage = (((m_iWidth*8)+31)/32*4)*m_iHeight;
		bmfHdr.bfSize = bmfHdr.bfOffBits+bmpHdr.biSizeImage;
		//调色板初始化
		for(int i=0; i<256; ++i)
		{
			rgb[i].rgbBlue = i;
			rgb[i].rgbGreen = i;
			rgb[i].rgbRed = i;
			rgb[i].rgbReserved = 0;
		}
	}
	else
	{
		//若Raw文件为真彩色图像，给相应的位图文件头和位图信息头部分内容初始化
		bmpHdr.biBitCount = 24;
		bmfHdr.bfOffBits = 54; //bmfHdr.bfOffBits=14+40;
		bmpHdr.biSizeImage = (((m_iWidth*24)+31)/32*4)*m_iHeight;
		bmfHdr.bfSize = bmfHdr.bfOffBits+bmpHdr.biSizeImage;
	}

	//将初始化好的位图文件头和位图信息头写入文件（注意是按位图文件结构顺序来写的）
	filebmp.Write(&bmfHdr, sizeof(bmfHdr)); //写位图文件头
	filebmp.Write(&bmpHdr, sizeof(bmpHdr)); //写位图信息头
	if(m_iGray==0)
		filebmp.Write(rgb, sizeof(RGBQUAD)*256); //如果是灰度图像，将调色板写入文件
	int h = m_iHeight;
	int w = m_iWidth;
	int iWidthBytes = bmpHdr.biSizeImage/m_iHeight; //位图文件的实际每行的存储宽度
	//开始写第四部分（图像数据）
	//注意每一行的字节数必须是四的整数倍，不足的补零
	//像素按照从下到上、从左到右的顺序排列
	int zero = 0;
	for(int i=0; i<h; ++i)
	{
		filebmp.Write(pDIB+w*(h-i-1), w);
		if(w!=iWidthBytes)
			for(int j=0; j<iWidthBytes-w; ++j)
				filebmp.Write(&zero, 1);
	}

	//释放句柄
	::GlobalUnlock((HGLOBAL) hDIB);
	::GlobalFree((HGLOBAL) hDIB);
	//关闭文件
	fileraw.Close();
	filebmp.Close();

	CDialog::OnOK();
}
