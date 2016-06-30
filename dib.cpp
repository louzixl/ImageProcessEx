// dib.cpp

#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
#include <afxadv.h>
#include <io.h>
#include <errno.h>

IMPLEMENT_DYNAMIC(CDib, CObject)
//���캯������ʼ��
CDib::CDib()
{
	m_pBMI = NULL;
	m_pBits = NULL;
	m_pPalette = NULL;
}
//�����������ͷ��ڴ�
CDib::~CDib()
{
	Free();
}

void CDib::Free()
{
	//ȷ�����н��������ڴ�ĳ�Ա�������ͷ�
	if(m_pBits)
	{
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
	}
	if(m_pBMI)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
	}
	if(m_pPalette)
	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}
}
/**
 * [CDib::Paint description]��λͼ�Ӿ�������lpDIBRect�������豸����hDC�ϵľ�������lpDCRect
 * @param  hDC       [description]�豸�������
 * @param  lpDCRect  [description]�豸������������
 * @param  lpDIBRect [description]λͼ��������
 * @return           [description]���Ƴɹ�����TRUE�����򷵻�FALSE
 */
BOOL CDib::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect) const
{
	if(!m_pBMI)
		return FALSE;

	HPALETTE hPal = NULL; //�µ�ɫ����
	HPALETTE hOldPal = NULL; //��ʼ��ɫ��

	//��ȡλͼ�ĵ�ɫ��
	if(m_pPalette!=NULL)
	{
		hPal = (HPALETTE)m_pPalette->m_hObject;
		//��¼��ʼ��ɫ��
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	BOOL bSuccess;
	if((RECTWIDTH(lpDCRect)==RECTWIDTH(lpDIBRect)) && (RECTHEIGHT(lpDCRect)==RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC, //�豸�������
			lpDCRect->left, //Ŀ����������Ͻ�x����
			lpDCRect->top, //Ŀ���������Ͻ�y����
			RECTWIDTH(lpDCRect), //Ŀ������Ŀ��
			RECTHEIGHT(lpDCRect), //Ŀ������ĸ߶�
			lpDIBRect->left, //λͼ��������Ͻ�x����
			(int)Height()-lpDIBRect->top-RECTHEIGHT(lpDIBRect), //λͼ�������Ͻ�y����
			0, //��ʼɨ����
			(WORD)Height(), //ɨ��������
			m_pBits, //λͼ����ͷ
			m_pBMI, //λͼ��Ϣͷ
			DIB_RGB_COLORS); //��ɫ����
	else
		bSuccess = ::StretchDIBits(hDC, lpDCRect->left, lpDCRect->top, RECTWIDTH(lpDCRect), RECTHEIGHT(lpDCRect), \
			lpDIBRect->left, lpDIBRect->top, RECTWIDTH(lpDIBRect), RECTHEIGHT(lpDIBRect), m_pBits, m_pBMI, DIB_RGB_COLORS, SRCCOPY);
	//�ָ���ǰ�ĵ�ɫ��
	if(hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

	return bSuccess;
}

/**
 * [CDib::CreatePalette description]��������λͼ��ɫ��
 * @return [description]�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL CDib::CreatePalette()
{
	if(!m_pBMI)
		return FALSE;

	//�����ɫ��
	WORD wNumColors = NumColors();

	if(wNumColors!=0)
	{
		//�����߼���ɫ��ռ�
		HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*wNumColors);

		//�ڴ治�㣬����FALSE
		if(hLogPal==0)
			return FALSE;

		LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

		//���õ�ɫ��
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		for(int i=0; i<(int)wNumColors; ++i)
		{
			lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		//���ɵ�ɫ�壬�����ָ��
		if(m_pPalette)
		{
			m_pPalette->DeleteObject();
			delete m_pPalette;
		}
		m_pPalette = new CPalette;
		BOOL bResult = m_pPalette->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
		return bResult;
	}

	return TRUE;
}

/**
 * [CDib::Width description]λͼ�Ŀ��
 * @return [description]���ػ�õ�λͼ�Ŀ��
 */
DWORD CDib::Width() const
{
	if(!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biWidth;
}

/**
 * [CDib::Height description]λͼ�ĸ߶�
 * @return [description]����λͼ�ĸ߶�
 */
DWORD CDib::Height() const
{
	if(!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biHeight;
}

/**
 * [CDib::PaletteSize description]���λͼ��ɫ��ߴ�
 * @return [description]����λͼ��ɫ���С
 */
WORD CDib::PaletteSize() const
{
	if(!m_pBMI)
		return 0;
	return NumColors()*sizeof(RGBQUAD);
}

/**
 * [CDib::NumColors description]�����ɫ��
 * ���λ��Ϊ1��colors=2, ���4��colors=16, ���8��colors=256, ���24����ɫ����û����ɫ
 * @return [description]
 */
WORD CDib::NumColors() const
{
	if(!m_pBMI)
		return 0;
	WORD wBitCount; //DIB��bit��

	DWORD dwClrUsed;
	dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
	if(dwClrUsed!=0)
		return (WORD)dwClrUsed;

	//����λͼ��ɫ�������ɫ��
	wBitCount = m_pBMI->bmiHeader.biBitCount;

	//������ɫ��
	switch(wBitCount)
	{
		case 1: return 2;
		case 4: return 16;
		case 8: return 256;
		default: return 0;
	}
}

/**
 * [CDib::Save description]��ͼ�񱣴浽ָ���ļ�
 * @param  file [description]����λͼ���ļ�
 * @return      [description]�洢�ֽ���
 */
DWORD CDib::Save(CFile &file) const
{
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷָ��
	DWORD dwDIBSize;

	if(m_pBMI==NULL)
		return 0;

	//д�ļ�ͷ
	//�ļ��������2bitsҪ�ǡ�BM��
	bmfHdr.bfType = DIB_HEADER_MARKER; //"BM"
	//��Ϣͷ�͵�ɫ���С
	dwDIBSize = *(LPDWORD)&m_pBMI->bmiHeader+PaletteSize();
	//����ͼ���С
	if((m_pBMI->bmiHeader.biCompression==BI_RLE8)||(m_pBMI->bmiHeader.biCompression==BI_RLE4))
	{
		//�����RLEλͼ�����ܼ���ͼ��ߴ磬ֱ�Ӳ���biSizeImage
		dwDIBSize += m_pBMI->bmiHeader.biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize; //λͼ���ݿ��С

		//����RLEλͼ����СΪWIdth(DWORD aligned)*Height
		dwBmBitsSize = WIDTHBYTES((m_pBMI->bmiHeader.biWidth)*((DWORD)m_pBMI->bmiHeader.biBitCount))*m_pBMI->bmiHeader.biHeight;
		dwDIBSize += dwBmBitsSize;

		m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
	}

	//���������ļ���С������дͼ���ļ�ͷ
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	//����ͼ���ļ�ͷ��ͼ�����ֵ�ƫ��
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+m_pBMI->bmiHeader.biSize+PaletteSize();
	
	//�ļ�ͷ
	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	DWORD dwBytesSaved = sizeof(BITMAPFILEHEADER);

	//��Ϣͷ
	UINT nCount = sizeof(BITMAPINFO) + (NumColors()-1)*sizeof(RGBQUAD);
	dwBytesSaved += nCount;
	file.Write(m_pBMI, nCount);

	//���ݲ���
	DWORD dwBytes = m_pBMI->bmiHeader.biBitCount*Width();
	//����ÿ����������
	if(dwBytes%32==0)
		dwBytes /= 8;
	else
		dwBytes = dwBytes/8 + (32-dwBytes%32)/8 + (((32-dwBytes%32)%8>0)?1:0);
	nCount = dwBytes*Height();
	dwBytesSaved += nCount;
	file.WriteHuge(m_pBits, nCount);

	return dwBytesSaved;
}

/**
 * [CDib::Read description]��ָ����λͼ�ļ������ڴ�
 * @param  file [description]
 * @return      [description]�����bytes��
 */
DWORD CDib::Read(CFile &file)
{
	//�ͷ��ڴ�
	Free();

	BITMAPFILEHEADER bmfHeader;

	//�ж��ļ��Ƿ����
	if(file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
		return 0;
	if(bmfHeader.bfType!=DIB_HEADER_MARKER)
		return 0;
	DWORD dwReadBytes = sizeof(bmfHeader);

	//�����ڴ�
	m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)+256*sizeof(RGBQUAD));
	if(m_pBMI == 0)
		return 0;

	//���ļ�ͷ
	if(file.Read(m_pBMI, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER))!=(UINT)(bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}
	dwReadBytes += bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

	DWORD dwLength = file.GetLength();
	//�����ݿ�
	m_pBits = (LPBYTE)GlobalAllocPtr(GHND, dwLength-bmfHeader.bfOffBits);
	if(m_pBits==0)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}

	if(file.ReadHuge(m_pBits, dwLength-bmfHeader.bfOffBits)!=(dwLength-bmfHeader.bfOffBits))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
		return 0;
	}
	dwReadBytes += dwLength - bmfHeader.bfOffBits;

	CreatePalette();
	return dwReadBytes;
}

#ifdef _DEBUG
void CDib::Dump(CDumpContext &dc) const
{
	CObject::Dump(dc);
}
#endif

/**
 * [CDib::CopyToHandle description]����λͼ��һ��ȫ�ִ洢�ռ�
 * @return [description]ָ���µ�ȫ�ִ洢�ռ�
 */
HGLOBAL CDib::CopyToHandle() const
{
	CSharedFile file;
	try
	{
		if(Save(file)==0)
			return 0;
	}
	catch(CFileException * e)
	{
		e->Delete();
		return 0;
	}

	return file.Detach();
}

/**
 * [CDib::ReadFromHandle description]��ʼ���������ڴ�ռ�
 * @param  hGlobal [description]
 * @return         [description]�����byte��
 */
DWORD CDib::ReadFromHandle(HGLOBAL hGlobal)
{
	CSharedFile file;
	file.SetHandle(hGlobal, FALSE);
	DWORD dwResult = Read(file);
	file.Detach();
	return dwResult;
}

void CDib::Serialize(CArchive &ar)
{
	CFile *pFile = ar.GetFile();
	ASSERT(pFile!=NULL);
	if(ar.IsStoring())
	{
		Save(*pFile);
	}
	else
	{
		Read(*pFile); 
	}
}

/**
 * [LinearTransform description]�Ҷ����Ա任/����
 * @param  minout [description]���Ա任����
 * @param  maxout [description]���Ա任����
 * @return        [description]
 */
BOOL CDib::LinearTransform(int minout, int maxout)
{
	//�����ж�λͼָ���Ƿ�Ϊ�գ���Ϊ�գ�˵��û�ж������ݣ����򷵻�
	if(m_pBMI==NULL)
		return FALSE;
	//�õ�Ӱ���ÿ��������ռ������
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString i2s;
		i2s.Format("%d", wBitCount);
		CString strMsg = "����������ֻ����8λͼ�񣡸�ͼ��Ϊ" + i2s + "λͼ��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_OK);
		return FALSE;
	} 
	//��ȡӰ��Ŀ�Ⱥ͸߶�
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	//�����ַ�ָ���������������λͼ���Ĳ�����ʼ��ַ
	LPBYTE lpData = m_pBits;
	DWORD i, j;
	//�����8λλͼ
	if(wBitCount==8)
	{
		//ͳ��ԭͼ��������С�Ҷȼ���ԭ�Ҷȷ�Χ��
		float m_MinGray = 255, m_MaxGray = 0;
		for(i=0; i<nHeight; ++i)
		{
			for(j=0; j<nWidth; ++j)
			{
				if(*(lpData+lRowBytes*i+j)<m_MinGray)
					m_MinGray = *(lpData+lRowBytes*i+j);
				if(*(lpData+lRowBytes*i+j)>m_MaxGray)
					m_MaxGray = *(lpData+lRowBytes*i+j);
			}
		}
		//�������Ա任�����ֵ������ͼ��
		for(i=0; i<nHeight; ++i)
		{
			for(j=0; j<nWidth; ++j)
			{
				//���Ҷȱ任��ʽ����任���ÿ�����صĻҶ�ֵ
				*(lpData+lRowBytes*i+j) = (BYTE)(minout+(maxout-minout)/(m_MaxGray-m_MinGray)*(*(lpData+lRowBytes*i+j)-m_MinGray));
			}
		}
	}
	return TRUE;
}

BOOL CDib::HistoEquivalize()
{
	//�����ж�λͼָ���Ƿ�Ϊ�գ���Ϊ�գ�˵��û�ж������ݣ����򷵻�
	if(m_pBMI==NULL)
		return FALSE;
	//�õ�Ӱ���ÿ��������ռ������
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "��ֱ��ͼ����ֻ����8λͼ��" + bitsNum + "λͼ��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_OK);
		return FALSE;
	}
	//��ȡӰ��Ŀ�Ⱥ͸߶�
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	//�����ַ���ָ���������������λͼ���Ĳ�����ʼ��ַ
	LPBYTE lpData = m_pBits;

	DWORD i, j;
	//�����8λλͼ
	if(wBitCount==8)
	{
		//ͳ�Ƹ��Ҷȼ������ظ���
		double r[256];
		for(i=0; i<256; ++i)
			r[i] = 0.0;
		for(i=0; i<nHeight; ++i)
			for(j=0; j<nWidth; ++j)
				r[*(lpData+i*lRowBytes+j)] += 1.0;

		//������⻯������ȵ���ֵ
		double percentile[256];
		for(i=0; i<256; ++i)
		{
			percentile[i] = 0.0;
			for(j=0; j<=i; ++j)
				percentile[i] += r[j];
			percentile[i] *= (255.0/(double)(nWidth*nHeight));
		}

		//����ͼ��
		for(i=0; i<nHeight; ++i)
			for(j=0; j<nWidth; ++j)
				*(lpData+lRowBytes*i+j) = (BYTE)(percentile[*(lpData+lRowBytes*i+j)]);
	}
	return TRUE;
}

/**
 * [LowPassDib description]��ͨ�˲�
 * @return [description]
 */
BOOL CDib::LowPassDib()
{
	//�����ж�λͼָ���Ƿ�Ϊ�գ���Ϊ��˵��û�ж������ݣ����򷵻�
	if(m_pBMI==NULL)
		return FALSE;
	//�õ�Ӱ���ÿ��������ռ�ı�����
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "�õ�ͨ�˲�ֻ����8λͼ�񣡸�ͼ��Ϊ" + bitsNum + "λͼ��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_OK);
		return FALSE;
	}

	//���ʵ�ʵ�λͼ����ռ�õ��ֽ���
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//�����������λͼͬ����С���ڴ�ռ�
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//���ڴ�����ʧ�ܣ�������Ϣ�������˳�
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//���ڴ����ɹ�����λͼ���ݿ�������������ڴ���
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);
	//��ȡӰ��Ŀ�Ⱥ͸߶�
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	LPBYTE lpData = m_pBits; //ԭλͼָ��
	LPBYTE lpOldBits = m_lpImageCopy; //ԭλͼ�Ŀ���

	//ͼ��任��ʼ
	DWORD i, j;
	//����9���ַ�ָ���������Ӧ3*3ģ���9�����صĴ洢��ַ
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
	//��8����λͼ���д���
	if(wBitCount==8){
		for(i=1; i<nHeight-1; ++i){
			for(j=1; j<nWidth-1; ++j){
				//�õ�ģ��ÿ�����صĵ�ַ�����˲���������ԭλͼ�����Ͻ��еģ�
				//����ʹ�ñ任������ػҶ����˲����㣨Ҫ�ÿ�����ͼ��
				p1 = lpOldBits + lRowBytes*(i-1)+(j-1);
				p2 = lpOldBits + lRowBytes*(i)+(j-1);
				p3 = lpOldBits + lRowBytes*(i+1)+(j-1);
				p4 = lpOldBits + lRowBytes*(i-1)+(j);
				//ʹp5ָ��ָ��ԭʼӰ���ģ�����Ķ�Ӧ����
				p5 = lpData + lRowBytes*(i)+(j);
				p6 = lpOldBits + lRowBytes*(i+1)+(j);
				p7 = lpOldBits + lRowBytes*(i-1)+(j+1);
				p8 = lpOldBits + lRowBytes*(i)+(j+1);
				p9 = lpOldBits + lRowBytes*(i+1)+(j+1);
				//�õ�ģ�����Ķ�Ӧԭλͼ�ĻҶ�ֵ
				*p5 = *(lpOldBits+lRowBytes*(i)+(j));

				//����ģ���������صĻҶ�ֵ��ͬʱ�Դ���255��С��0�����ؽ��д���
				int t = (int)((*p1+*p2+*p3+*p4+*p5+*p6+*p7+*p8+*p9)/9);
				if(t>255)
					*p5 = 255;
				else if(t<0)
					*p5 = 0;
				else
					*p5 = (BYTE)t;				
			}
		}
	}
	return TRUE;
}

/**
 * [CDib::HighPassDib description]3x3��ͨ�˲�
 * @return [description]
 */
BOOL CDib::HighPassDib()
{
	if(m_pBMI==NULL)
		return FALSE;
	//�õ�Ӱ���ÿ��������ռ�ı�����
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "�õ�ͨ�˲�ֻ����8λͼ�񣡸�ͼ��Ϊ" + bitsNum + "λͼ��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_OK);
		return FALSE;
	}

	//���ʵ�ʵ�λͼ����ռ�õ��ֽ���
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//������������λͼͬ����С���ڴ�ռ�
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//���ڴ�����ʧ�ܣ�������Ϣ�������˳�
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//���ڴ����ɹ�����λͼ���ݿ�������������ڴ���
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);
	//��ȡӰ��Ŀ�Ⱥ͸߶�
	DWORD nWidth = Width();
	DWORD nHeight = Height();

	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD lRowBytes = WIDTHBYTES(((DWORD)wBitCount)*nWidth);
	LPBYTE lpData = m_pBits; //ԭλͼָ��
	LPBYTE lpOldBits = m_lpImageCopy; //ԭλͼ�Ŀ���

	//ͼ��任��ʼ
	DWORD i, j;
	//����9���ַ�ָ���������Ӧ3x3ģ���9�����صĴ洢��ַ
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
	//��8����λͼ���д���
	if(wBitCount==8){
		for(i=1; i<nHeight-1; ++i){
			for(j=1; j<nWidth-1; ++j){
				//�õ�ģ��ÿ�����صĵ�ַ
				p1 = lpOldBits+lRowBytes*(i-1)+(j-1);
				p2 = lpOldBits+lRowBytes*(i)+(j-1);
				p3 = lpOldBits+lRowBytes*(i+1)+(j-1);
				p4 = lpOldBits+lRowBytes*(i-1)+j;
				p5 = lpData+lRowBytes*i+j;
				p6 = lpOldBits+lRowBytes*(i+1)+j;
				p7 = lpOldBits+lRowBytes*(i-1)+(j+1);
				p8 = lpOldBits+lRowBytes*i+(j+1);
				p9 = lpOldBits+lRowBytes*(i+1)+(j+1);

				//�õ�ģ�����Ķ�Ӧԭλͼ�ĻҶ�ֵ
				*p5 = *(lpOldBits+lRowBytes*i+j);
				//����ģ���������صĻҶ�ֵ��ͬʱ�Դ���255��С��0�����ؽ��д���
				int t = (int)(*p5*9-*p1-*p2-*p3-*p4-*p6-*p7-*p8-*p9);
				if(t>255)
					*p5 = 255;
				else if(t<0)
					*p5 = 0;
				else
					*p5 = (BYTE)t;				
			}
		}
	}
	return TRUE;
}

/**
 * [CDib::MedianFilterDIB description]��ֵ�˲�
 * @return [description]
 */
BOOL CDib::MedianFilterDIB()
{
	if(m_pBMI==NULL)
		return FALSE;
	//�õ�Ӱ���ÿ��������ռ������
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString strMsg;
		strMsg.Format(wBitCount);
		CString Msg = "����ֵ�˲���ֻ����8λͼ�񣡸�ͼ��Ϊ" + strMsg + "λͼ��";
		MessageBox(NULL, Msg, "ϵͳ��ʾ", MB_OK);
		return FALSE;
	}

	//���ʵ�ʵ�λͼ����ռ�õ��ֽ���
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//�����������λͼ��ͬ��С���ڴ�
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//���ڴ�����ʧ�ܣ�������Ϣ�������˳�
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//���ڴ����ɹ�����λͼ���ݿ�������������ڴ���
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);

	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//��ȡӰ��ÿ�е�ʵ�ʴ洢���
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));

	LPBYTE lpData = m_pBits; //ԭλͼָ��
	LPBYTE lpOldBits = m_lpImageCopy; //ԭλͼ�Ŀ���
	//ͼ��任��ʼ
	DWORD i, j;
	//����15���ַ�ָ���������Ӧ5x5ģ���25�����صĴ洢��ַ
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, 
	*p11, *p12, *p13, *p14, *p15, *p16, *p17, *p18, *p19, *p20, 
	*p21, *p22, *p23, *p24, *p25;
	//��8����λͼ���д���
	if(wBitCount==8){
		for(i=2; i<nHeight-2; ++i){
			for(j=2; j<nWidth-2; ++j){
				//�õ�ģ��ÿ�����صĵ�ַ
				p1 = lpOldBits + lRowBytes*(i-2) + (j-2);
				p2 = lpOldBits + lRowBytes*(i-1) + (j-2);
				p3 = lpOldBits + lRowBytes*(i) + (j-2);
				p4 = lpOldBits + lRowBytes*(i+1) + (j-2);
				p5 = lpOldBits + lRowBytes*(i+2) + (j-2);
				p6 = lpOldBits + lRowBytes*(i-2) + (j-1);
				p7 = lpOldBits + lRowBytes*(i-1) + (j-1);
				p8 = lpOldBits + lRowBytes*(i) + (j-1);
				p9 = lpOldBits + lRowBytes*(i+1) + (j-1);
				p10 = lpOldBits + lRowBytes*(i+2) + (j-1);
				p11 = lpOldBits + lRowBytes*(i-2) + (j);
				p12 = lpOldBits + lRowBytes*(i-1) + (j);
				//ʹp13ָ��ָ��ԭʼӰ���ģ�����Ķ�Ӧ����
				p13 = lpData + lRowBytes*(i) + (j);
				p14 = lpOldBits + lRowBytes*(i+1) + (j);
				p15 = lpOldBits + lRowBytes*(i+2) + (j);
				p16 = lpOldBits + lRowBytes*(i-2) + (j+1);
				p17 = lpOldBits + lRowBytes*(i-1) + (j+1);
				p18 = lpOldBits + lRowBytes*(i) + (j+1);
				p19 = lpOldBits + lRowBytes*(i+1) + (j+1);
				p20 = lpOldBits + lRowBytes*(i+2) + (j+1);
				p21 = lpOldBits + lRowBytes*(i-2) + (j+2);
				p22 = lpOldBits + lRowBytes*(i-1) + (j+2);
				p23 = lpOldBits + lRowBytes*(i) + (j+2);
				p24 = lpOldBits + lRowBytes*(i+1) + (j+2);
				p25 = lpOldBits + lRowBytes*(i+2) + (j+2);

				//�õ�ģ�����Ķ�Ӧԭλͼ�ĻҶ�ֵ
				*p13 = *(lpOldBits+lRowBytes*i+j);

				int t, temp[25];
				
				temp[0] = (int)(*p1); temp[1] = (int)(*p2); temp[2] = (int)(*p3);
				temp[3] = (int)(*p4); temp[4] = (int)(*p5); temp[5] = (int)(*p6);
				temp[6] = (int)(*p7); temp[7] = (int)(*p8); temp[8] = (int)(*p9);
				temp[9] = (int)(*p10); temp[10] = (int)(*p11); temp[11] = (int)(*p12);
				temp[12] = (int)(*p13); temp[13] = (int)(*p14); temp[14] = (int)(*p15);
				temp[15] = (int)(*p16); temp[16] = (int)(*p17); temp[17] = (int)(*p18);
				temp[18] = (int)(*p19); temp[19] = (int)(*p20); temp[20] = (int)(*p21);
				temp[21] = (int)(*p22); temp[22] = (int)(*p23); temp[23] = (int)(*p24);
				temp[24] = (int)(*p25);

				//���ݷ�����
				for(int j=1; j<25; ++j){
					for(int i=1; i<=25-j; ++i){
						if(temp[i]>temp[i+1]){
							t = temp[i];
							temp[i] = temp[i+1];
							temp[i+1] = t;
						}
					}
				}
				//���м�Ҷ�ֵ����ģ�����Ķ�Ӧ������
				*p13 = (BYTE)temp[12];
			}
		}
	}
	return TRUE;
}

/**
 * [TranslationDIB description]ͼ��ƽ��
 * @param  dXOffset [description]ˮƽλ��
 * @param  dYOffset [description]��ֱλ��
 * @return          [description]
 */
BOOL CDib::TranslationDIB(DWORD dXOffset, DWORD dYOffset)
{
	if(m_pBMI==NULL)
		return FALSE;

	//���ʵ�ʵ�λͼ����ռ�õ��ֽ���
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;

	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy == NULL){
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		AfxMessageBox("Memory Allocate Error");
		return FALSE;
	}

	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);

	DWORD nWidth = Width();
	DWORD nHeight = Height();
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	LPBYTE lpData = m_pBits;
	LPBYTE lpOldBits = m_lpImageCopy;

	//��������DIB�е�����
	DWORD i, j;

	//������ԭDIB�е�����
	long i_0, j_0;

	for(i=0; i<nHeight; ++i){
		for(j=0; j<nWidth; ++j){
			LPBYTE lpTemp = lpData + lRowBytes*i+j;

			//�����������ԭDIB�е�����
			i_0 = i - dXOffset;
			j_0 = j - dYOffset;

			//�ж��Ƿ���ԭͼ��Χ��
			if((j_0>=0)&&(j_0<(long)nWidth)&&(i_0>=0)&&(i_0<(long)nHeight)){
				*lpTemp = *(lpOldBits + lRowBytes*i_0 + j_0);
			}
			else{
				//����ԭͼ����û�е����أ�ֱ�Ӹ�ֵΪ255
				*lpTemp = (BYTE)255;
			}
		}
	}
	return TRUE;
}

/**
 * [CDib::ZoomDIB description]ͼ������
 * @param fXZoomRatio [description]X�����ű���
 * @param fYZoomRatio [description]Y�����ű���
 */
/*void CDib::ZoomDIB(float fXZoomRatio, float fYZoomRatio)
{
	//ԭͼ��Ŀ�Ⱥ͸߶�
	LONG lWidth;
	LONG lHeight;

	//���ź�ͼ��Ŀ�Ⱥ͸߶�
	LONG lNewWidth;
	LONG lNewHeight;

	//���ź�ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG lNewLineBytes;

	//ָ��ԭͼ���ָ��
	LPBYTE lpDIBBits;

	//ָ��Դ���ص�ָ��
	LPBYTE lpSrc;

	//���ź���DIB���
	LPBYTE lpZoomBits;

	//ָ������ͼ���Ӧ���ص�ָ��
	LPBYTE lpDst;

	//ָ������ͼ���ָ��
	LPBYTE lpNewDIB;
	LPBYTE lpNewDIBBits;

	//ѭ����������������DIB�е����꣩
	LONG i, j;

	//������ԭDIB�е�����
	LONG i_0, j_0;

	//ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	//����ԭͼ��
	LPBYTE lpDIB = (LPBYTE)::GlobalLock((HGLOBAL) m_pBits);
	//�ҵ�ԭͼ��DIB������ʼλ��
	lpDIBBits = m_pBits;

	lWidth = Width();
	lLineBytes = WIDTHBYTES(lWidth*8);
	lHeight = Height();

	//�������ź�ͼ��ʵ�ʿ��
	//�˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lNewWidth = (LONG)(lWidth*fXZoomRatio+0.5);

	//������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth*8);

	//�������ź��ͼ��߶�
	lNewHeight = (LONG)(lHeight*fXZoomRatio+0.5);

	//�����ڴ棬�Ա�����DIB
	lpZoomBits = (BYTE *)malloc(lNewLineBytes*lNewHeight+m_bmfHeader.bfOffBits);

	//�ж��Ƿ��ڴ����ʧ��
	if(lpZoomBits==NULL)
		return;
	//�����ڴ�
	lpNewDIB = (unsigned char *)::GlobalLock((HGLOBAL)lpZoomBits);

	//����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, m_bmfHeader.bfOffBits);

	//�ҵ���DIB������ʼλ��
	lpNewDIBBits = lpNewDIB + m_bmfHeader.bfOffBits;

	//���ͼ��ÿ�н��в���
	for(i=0; i<lNewHeight; ++i){
		for(j=0; j<lNewWidth; ++j){
			//ָ����DIB��i�У���j�����ص�ָ��
			//ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (unsigned char *)lpNewDIBBits + lNewLineBytes*(lNewHeight-1-i)+j;
			//�����������ԭDIB�е�����
			i_0 = (LONG)(i/fYZoomRatio+0.5);
			j_0 = (LONG)(j/fXZoomRatio+0.5);
			//�ж��Ƿ���ԭͼ��Χ��
			if((j_0>=0)&&(j_0<lWidth)&&(i_0>=0)&&(i_0<lHeight)){
				//ָ��ԭDIB��i_0�У���j_0�����ص�ָ��
				lpSrc = (unsigned char *)lpDIBBits + lLineBytes*(lHeight-1-i_0)+j_0;
				//��������
				*lpDst = *lpSrc;
			}
			else{
				//����ԭͼ����û�е����أ�ֱ�Ӹ�ֵΪ255
				*((unsigned char *)lpDst) = 255;
			}
		}
	}

	//�滻ԭͼ����ʾ��ͼ��
	m_pBMI->bmiHeader.biHeight = lNewHeight;
	m_pBMI->bmiHeader.biWidth = lNewWidth;
	m_pBMI->bmiHeader.biSizeImage = lNewHeight * lNewWidth;
	m_pBits = lpNewDIBBits;

	return;
}*/

/**
 * [CDib::RotateDIB description]ͼ����ת
 * @param iRotateAngle [description]��ת�ĽǶȣ�0~360�㣩
 */
/*void CDib::RotateDIB(int iRotateAngle)
{
	//ԭͼ��Ŀ�Ⱥ͸߶�
	LONG lWidth;
	LONG lHeight;

	//��ת��ͼ��Ŀ�Ⱥ͸߶�
	LONG lNewWidth;
	LONG lNewHeight;

	//ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	//��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG lNewLineBytes;

	//ָ��ԭͼ���ָ��
	LPBYTE lpDIBBits;

	//ָ��ԭ���ص�ָ��
	LPBYTE lpSrc;

	//��ת����DIB���
	//HDIB hDIB;
	
	//ָ����תͼ��ͼ���Ӧ���ص�ָ��
	LPBYTE lpDst;

	//ָ����תͼ���ָ��
	LPBYTE lpNewDIB;
	LPBYTE lpNewDIBBits;

	//ѭ����������������DIB�е����꣩
	LONG i, j;

	//������ԭDIB�е�����
	LONG i_0, j_0;

	//��ת�Ƕȣ����ȣ�
	float fRotateAngle;

	//��ת�Ƕȵ����Һ�����
	float fSina, fCosa;

	//ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;

	//��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	//�����м䳣��
	float f1, f2;

	//����ԭͼ��
	LPBYTE lpDIB = (LPBYTE)::GlobalLock((HGLOBAL) m_pBits);

	//�ҵ���DIBͼ��������ʼλ��
	lpDIBBits = m_pBits;

	//��ȡͼ��ġ���ȡ���4�ı�����
	lWidth = Width();

	//����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth*8);

	//��ȡͼ��ĸ߶�
	lHeight = Height();

	//����ת�ǶȴӶ�ת��Ϊ����
	fRotateAngle = (float)RADIAN(iRotateAngle);

	//������ת�Ƕȵ����Һ�����
	fSina = (float)sin((double)fRotateAngle);
	fCosa = (float)cos((double)fRotateAngle);

	//����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float)(-(lWidth-1)/2);
	fSrcY1 = (float)((lHeight-1)/2);
	fSrcX2 = (float)((lWidth-1)/2);
	fSrcY2 = (float)((lHeight-1)/2);
	fSrcX3 = (float)(-(lWidth-1)/2);
	fSrcY3 = (float)(-(lHeight-1)/2);
	fSrcX4 = (float)((lWidth-1)/2);
	fSrcY4 = (float)(-(lHeight-1)/2);

	//������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 = fCosa*fSrcX1 + fSina*fSrcY1;
	fDstY1 = -fSina*fSrcX1 + fCosa*fSrcY1;
	fDstX2 = fCosa*fSrcX2 + fSina*fSrcY2;
	fDstY2 = -fSina*fSrcX2 + fCosa*fSrcY2;
	fDstX3 = fCosa*fSrcX3 + fSina*fSrcY3;
	fDstY3 = -fSina*fSrcX3 + fCosa*fSrcY3;
	fDstX4 = fCosa*fSrcX4 + fSina*fSrcY4;
	fDstY4 = -fSina*fSrcX4 + fCosa*fSrcY4;

	//������ת���ͼ��ʵ�ʿ��
	lNewWidth = (LONGXmax(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
	//������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth*8);

	//������ת��ͼ��ĸ߶�
	lNewHeight = (LONG)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

	//�������������������Ժ�ÿ�ζ�������
	f1 = (float)(-0.5*(lNewWidth-1)*fCosa-0.5*(lNewHeight-1)*fSina+0.5*(lWidth-1));
	f2 = (float)(0.5*(lNewWidth-1)*fSina-0.5*(lNewHeight-1)*fCosa+0.5*(lHeight-1));

	//�����ڴ棬�Ա�����DIB
	LPBYTE lpImageRotate = (BYTE *)malloc(lNewLineBytes*lNewHeight+m_bmfHeader.bfOffBits);

	//�ж��Ƿ��ڴ����ʧ��
	if(lpImageRotate==NULL)
		return; //�����ڴ�ʧ��

	//�����ڴ�
	lpNewDIB = (LPBYTE)::GlobalLock((HGLOBAL)lpImageRotate);

	//����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, m_bmfHeader.bfOffBits);

	//�ҵ���DIB������ʼλ��
	lpNewDIBBits = lpNewDIB + m_bmfHeader.bfOffBits;

	//���ͼ��ÿ�н��в���
	for(i=0; i<lNewHeight; ++i){
		for(j=0; j<lNewWidth; ++j){
			//ָ����DIB��i�У���j�����ص�ָ��
			//ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (LPBYTE)lpNewDIBBits + lNewLineBytes*(lNewHeight-1-i) + j;

			//�����������ԭDIB�е�����
			i_0 = (LONG)(-((float)j)*fSina+((float)i)*fCosa+f2+0.5);
			j_0 = (LONG)(((float)j)*fCosa+((float)i)*fSina+f1+0.5);

			//�ж��Ƿ���ԭͼ��Χ��
			if((j_0>=0)&&(j_0<lWidth)&&(i_0>=0)&&(i_0<lHeight)){
				//ָ��ԭDIB��i_0�У���j_0�����ص�ָ��
				lpSrc = (LPBYTE)lpDIBBits + lLineBytes * (lHeight-1-i_0) + j_0;

				//��������
				*lpDst = *lpSrc;
			}
			else{
				//����ԭͼ����û�е����أ�ֱ�Ӹ�ֵΪ255
				*((unsigned char *)lpDst) = 255;
			}
		}
	}

	//�滻ԭͼ����ʾ��ͼ��
	m_pBMI->bmiHeader.biHeight = lNewHeight;
	m_pBMI->bmiHeader.biWidth = lNewWidth;
	m_pBMI->bmiHeader.biSizeImage = lNewHeight * lNewWidth;
	m_pBits = lpNewDIBBits;

	//����
	return;
}*/

/**
 * [TransposeDIB description]ͼ��ת��
 */
 /*void CDib::TransposeDIB()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	LONG lWidth;
	LONG lHeight;

	//ָ��ԭͼ���ָ��
	LPBYTE lpDIBBits;

	//ָ��ԭ���ص�ָ��
	LPBYTE lpSrc;

	//ָ��ת��ͼ���Ӧ���ص�ָ��
	LPBYTE lpDst;

	//ָ��ת��ͼ���ָ��
	LPBYTE lpNewDIBBits;

	//ѭ������
	LONG i, j;

	//ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	//��ͼ��ÿ�е��ֽ���
	LONG lNewLineBytes;

	//�ҵ�ԭDIBͼ��������ʼλ��
	lpDIBBits = m_pBits;

	//��ȡͼ��ĸ߶ȡ���ȣ�4�ı�����
	lWidth = Width();
	lHeight = Height();

	//����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lHeight * 8);

	//��ʱ�����ڴ棬�Ա�����ͼ��
	LPBYTE hNewDIBBits = (BYTE *)malloc(lWidth * lNewLineBytes);

	//�ж��Ƿ��ڴ����ʧ��
	if(hNewDIBBits==NULL)
		return;

	//�����ڴ�
	lpNewDIBBits = (unsigned char *)LocalLock(hNewDIBBits);

	//���ͼ��ÿ�н��в���
	for(i=0; i<lHeight; ++i){
		for(j=0; j<lWidth; ++j){
			//ָ��ԭDIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//ָ��ת��DIB��j�У���i�����ص�ָ��
			//ע��˴�lWidth��lHeight��ԭDIB�Ŀ�Ⱥ͸߶ȣ�Ӧ�û���
			lpDst = (unsigned char *)lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;
			//��������
			*lpDst = *lpSrc;
		}
	}

	//����ת�ú��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth*lNewLineBytes);
	//�滻ԭͼ����ʾ��ͼ��
	m_pBMI->bmiHeader.biHeight = lWidth;
	m_pBMI->bmiHeader.biWidth = lHeight;
	m_pBits = lpNewDIBBits;

	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);

	return;
}*/