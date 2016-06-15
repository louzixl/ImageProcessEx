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
	//�ļ�����˵���ַ���
	static char BASED_CODE file[] = "RAW Files(*.RAW)|*.raw|�����ļ�(*.*)|*.*||";
	//�ļ��Ի����ʼ��������ĵ�һ������TRUE��ʾ�Դ򿪷�ʽ��ʾ�ļ��Ի���
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file, NULL);
	//�����ļ��򿪶Ի���
	SelectFile.DoModal();
	//�õ���ѡ�ļ�·���������ļ�����
	CString FileName;
	FileName = SelectFile.GetPathName();
	//���õ����ļ���������ʽת���Ի���ĳ�Ա����m_sRawIn
	m_sRawIn = FileName;
	//����ˢ�£�����m_sRawIn�µõ���ֵ��ʾ�ڶԻ����ж�Ӧ�Ŀؼ��ϣ�ע�����false��
	UpdateData(FALSE);
}

void CRowToBmpDlg::OnButtonBmpview() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE file[] = "BMP Files(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
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
	//����ˢ��
	UpdateData(TRUE);
	//����DIB���HDIB
	DECLARE_HANDLE(HDIB);
	HDIB hDIB; //����HDIB����������������bmpλͼ
	LPSTR pDIB; //�����ַ�ָ��������������λͼ����
	//���ļ�
	CFile fileraw, filebmp; //CFile��д�ļ�
	//�Զ��;ܾ�д�ķ�ʽ����ѡ�ļ���Ϊm_sRawIn��Raw��ʽ�ļ�
	fileraw.Open(m_sRawIn, CFile::modeRead|CFile::shareDenyWrite, NULL);
	//�õ��ļ��Ĵ�С
	DWORD filesize = fileraw.GetLength();
	//����Raw��ʽ�ļ��Ĵ�С����������ڴ�ռ�
	hDIB = (HDIB)::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, filesize);
	//�����䲻�ɹ�����hDIB==0������򷵻�
	if(hDIB==0)
	{
		return;
	}
	//������ɹ����õ�����ڴ����ʵ��ַ
	pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);
	//��Raw�ļ����ݶ���pDIB��ָ����ڴ��
	if(m_iGray==0) //���Raw�ļ��ǻҶȣ�����Ҫ����m_iBSQ
		fileraw.ReadHuge(pDIB, filesize);
	else //���Raw�ļ������ɫ����Ҫ����m_iBSQ
	{
		if(m_iBSQ==0)
		{
			//������ѧ�����
			AfxMessageBox("���ɫBSQ��ʽû��ʵ�֣���ѧ�����", MB_OK, 0);
			return;
		}
		if(m_iBSQ==1)
		{
			//������ѧ�����
			AfxMessageBox("���ɫBIL��ʽû��ʵ�֣���ѧ�����", MB_OK, 0);
			return;
		}
		if(m_iBSQ==2)
		{
			//������ѧ�����
			AfxMessageBox("���ɫBIP��ʽû��ʵ�֣���ѧ�����", MB_OK, 0);
			return;
		}
	}
	//��������������ͬʱ����ѡBMP�ļ�
	filebmp.Open(m_sBmpOut, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);
	//д�ļ����ļ�ͷ��λͼ��Ϣͼ����ɫ�塢λͼ����
	//����BMP�ļ����Ĳ�����ɣ�����ֱ�������ָ�ֵ��д��BMP�ļ�
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ
	BITMAPINFOHEADER bmpHdr; //λͼ��Ϣͷ
	RGBQUAD rgb[256];        //��ɫ��

	//λͼ�ļ�ͷ������Ϣ��ʼ��
	bmfHdr.bfType = 'MB';
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	//λͼ��Ϣͷ������Ϣ��ʼ��
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
		//��Raw�ļ�Ϊ�Ҷ�ͼ�񣬸���Ӧλͼ�ļ�ͷ��λͼ��Ϣͷ�������ݳ�ʼ��
		bmpHdr.biBitCount = 8;
		bmfHdr.bfOffBits = 1078; //bmfHdr.bfOffBits=14+40+256*4;
		bmpHdr.biSizeImage = (((m_iWidth*8)+31)/32*4)*m_iHeight;
		bmfHdr.bfSize = bmfHdr.bfOffBits+bmpHdr.biSizeImage;
		//��ɫ���ʼ��
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
		//��Raw�ļ�Ϊ���ɫͼ�񣬸���Ӧ��λͼ�ļ�ͷ��λͼ��Ϣͷ�������ݳ�ʼ��
		bmpHdr.biBitCount = 24;
		bmfHdr.bfOffBits = 54; //bmfHdr.bfOffBits=14+40;
		bmpHdr.biSizeImage = (((m_iWidth*24)+31)/32*4)*m_iHeight;
		bmfHdr.bfSize = bmfHdr.bfOffBits+bmpHdr.biSizeImage;
	}

	//����ʼ���õ�λͼ�ļ�ͷ��λͼ��Ϣͷд���ļ���ע���ǰ�λͼ�ļ��ṹ˳����д�ģ�
	filebmp.Write(&bmfHdr, sizeof(bmfHdr)); //дλͼ�ļ�ͷ
	filebmp.Write(&bmpHdr, sizeof(bmpHdr)); //дλͼ��Ϣͷ
	if(m_iGray==0)
		filebmp.Write(rgb, sizeof(RGBQUAD)*256); //����ǻҶ�ͼ�񣬽���ɫ��д���ļ�
	int h = m_iHeight;
	int w = m_iWidth;
	int iWidthBytes = bmpHdr.biSizeImage/m_iHeight; //λͼ�ļ���ʵ��ÿ�еĴ洢���
	//��ʼд���Ĳ��֣�ͼ�����ݣ�
	//ע��ÿһ�е��ֽ����������ĵ�������������Ĳ���
	//���ذ��մ��µ��ϡ������ҵ�˳������
	int zero = 0;
	for(int i=0; i<h; ++i)
	{
		filebmp.Write(pDIB+w*(h-i-1), w);
		if(w!=iWidthBytes)
			for(int j=0; j<iWidthBytes-w; ++j)
				filebmp.Write(&zero, 1);
	}

	//�ͷž��
	::GlobalUnlock((HGLOBAL) hDIB);
	::GlobalFree((HGLOBAL) hDIB);
	//�ر��ļ�
	fileraw.Close();
	filebmp.Close();

	CDialog::OnOK();
}
