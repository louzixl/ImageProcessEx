// dib.cpp

#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
#include <afxadv.h>
#include <io.h>
#include <errno.h>
#include <math.h>

IMPLEMENT_DYNAMIC(CDib, CObject)
//构造函数，初始化
CDib::CDib()
{
	bfOffsetBits = 0;
	m_pBMI = NULL;
	m_pBits = NULL;
	m_pPalette = NULL;
}
//析构函数，释放内存
CDib::~CDib()
{
	Free();
}

void CDib::Free()
{
	//确保所有将被分配内存的成员变量被释放
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
 * [CDib::Paint description]将位图从矩形区域lpDIBRect拷贝到设备环境hDC上的矩形区域lpDCRect
 * @param  hDC       [description]设备环境句柄
 * @param  lpDCRect  [description]设备环境矩形区域
 * @param  lpDIBRect [description]位图矩形区域
 * @return           [description]绘制成功返回TRUE，否则返回FALSE
 */
BOOL CDib::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect) const
{
	if(!m_pBMI)
		return FALSE;

	HPALETTE hPal = NULL; //新调色板句柄
	HPALETTE hOldPal = NULL; //初始调色板

	//获取位图的调色板
	if(m_pPalette!=NULL)
	{
		hPal = (HPALETTE)m_pPalette->m_hObject;
		//记录初始调色板
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	BOOL bSuccess;
	if((RECTWIDTH(lpDCRect)==RECTWIDTH(lpDIBRect)) && (RECTHEIGHT(lpDCRect)==RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC, //设备环境句柄
			lpDCRect->left, //目标区域的左上角x坐标
			lpDCRect->top, //目标区域左上角y坐标
			RECTWIDTH(lpDCRect), //目标区域的宽度
			RECTHEIGHT(lpDCRect), //目标区域的高度
			lpDIBRect->left, //位图区域的左上角x坐标
			(int)Height()-lpDIBRect->top-RECTHEIGHT(lpDIBRect), //位图区域左上角y坐标
			0, //开始扫描行
			(WORD)Height(), //扫描总行数
			m_pBits, //位图数据头
			m_pBMI, //位图信息头
			DIB_RGB_COLORS); //颜色参数
	else
		bSuccess = ::StretchDIBits(hDC, lpDCRect->left, lpDCRect->top, RECTWIDTH(lpDCRect), RECTHEIGHT(lpDCRect), \
			lpDIBRect->left, lpDIBRect->top, RECTWIDTH(lpDIBRect), RECTHEIGHT(lpDIBRect), m_pBits, m_pBMI, DIB_RGB_COLORS, SRCCOPY);
	//恢复以前的调色板
	if(hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

	return bSuccess;
}

/**
 * [CDib::CreatePalette description]用来生成位图调色板
 * @return [description]成功返回TRUE，否则返回FALSE
 */
BOOL CDib::CreatePalette()
{
	if(!m_pBMI)
		return FALSE;

	//获得颜色数
	WORD wNumColors = NumColors();

	if(wNumColors!=0)
	{
		//分配逻辑调色板空间
		HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*wNumColors);

		//内存不足，返回FALSE
		if(hLogPal==0)
			return FALSE;

		LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

		//设置调色板
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		for(int i=0; i<(int)wNumColors; ++i)
		{
			lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		//生成调色板，并获得指针
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
 * [CDib::Width description]位图的宽度
 * @return [description]返回获得的位图的宽度
 */
DWORD CDib::Width() const
{
	if(!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biWidth;
}

/**
 * [CDib::Height description]位图的高度
 * @return [description]返回位图的高度
 */
DWORD CDib::Height() const
{
	if(!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biHeight;
}

/**
 * [CDib::PaletteSize description]获得位图调色板尺寸
 * @return [description]返回位图调色板大小
 */
WORD CDib::PaletteSize() const
{
	if(!m_pBMI)
		return 0;
	return NumColors()*sizeof(RGBQUAD);
}

/**
 * [CDib::NumColors description]获得颜色数
 * 如果位数为1：colors=2, 如果4：colors=16, 如果8：colors=256, 如果24：颜色表中没有颜色
 * @return [description]
 */
WORD CDib::NumColors() const
{
	if(!m_pBMI)
		return 0;
	WORD wBitCount; //DIB的bit数

	DWORD dwClrUsed;
	dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
	if(dwClrUsed!=0)
		return (WORD)dwClrUsed;

	//根据位图调色板计算颜色数
	wBitCount = m_pBMI->bmiHeader.biBitCount;

	//返回颜色数
	switch(wBitCount)
	{
		case 1: return 2;
		case 4: return 16;
		case 8: return 256;
		default: return 0;
	}
}

/**
 * [CDib::Save description]将图像保存到指定文件
 * @param  file [description]保存位图的文件
 * @return      [description]存储字节数
 */
DWORD CDib::Save(CFile &file) const
{
	BITMAPFILEHEADER bmfHdr; //位图文件头指针
	DWORD dwDIBSize;

	if(m_pBMI==NULL)
		return 0;

	//写文件头
	//文件类型最初2bits要是“BM”
	bmfHdr.bfType = DIB_HEADER_MARKER; //"BM"
	//信息头和调色板大小
	dwDIBSize = *(LPDWORD)&m_pBMI->bmiHeader+PaletteSize();
	//计算图像大小
	if((m_pBMI->bmiHeader.biCompression==BI_RLE8)||(m_pBMI->bmiHeader.biCompression==BI_RLE4))
	{
		//如果是RLE位图，不能计算图像尺寸，直接采用biSizeImage
		dwDIBSize += m_pBMI->bmiHeader.biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize; //位图数据块大小

		//不是RLE位图，大小为WIdth(DWORD aligned)*Height
		dwBmBitsSize = WIDTHBYTES((m_pBMI->bmiHeader.biWidth)*((DWORD)m_pBMI->bmiHeader.biBitCount))*m_pBMI->bmiHeader.biHeight;
		dwDIBSize += dwBmBitsSize;

		m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
	}

	//计算整个文件大小，并书写图像文件头
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	//计算图像文件头到图像数字的偏移
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+m_pBMI->bmiHeader.biSize+PaletteSize();
	
	//文件头
	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	DWORD dwBytesSaved = sizeof(BITMAPFILEHEADER);

	//信息头
	UINT nCount = sizeof(BITMAPINFO) + (NumColors()-1)*sizeof(RGBQUAD);
	dwBytesSaved += nCount;
	file.Write(m_pBMI, nCount);

	//数据部分
	DWORD dwBytes = m_pBMI->bmiHeader.biBitCount*Width();
	//计算每行像素数据
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
 * [CDib::Read description]将指定的位图文件读入内存
 * @param  file [description]
 * @return      [description]读入的bytes数
 */
DWORD CDib::Read(CFile &file)
{
	//释放内存
	Free();

	BITMAPFILEHEADER bmfHeader;

	//判断文件是否存在
	if(file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
		return 0;
	if(bmfHeader.bfType!=DIB_HEADER_MARKER)
		return 0;
	DWORD dwReadBytes = sizeof(bmfHeader);

	//分配内存
	m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)+256*sizeof(RGBQUAD));
	if(m_pBMI == 0)
		return 0;

	//读文件头
	if(file.Read(m_pBMI, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER))!=(UINT)(bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}
	bfOffsetBits = bmfHeader.bfOffBits;
	dwReadBytes += bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

	DWORD dwLength = file.GetLength();
	//读数据块
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
 * [CDib::CopyToHandle description]拷贝位图到一个全局存储空间
 * @return [description]指向新的全局存储空间
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
 * [CDib::ReadFromHandle description]初始化给定的内存空间
 * @param  hGlobal [description]
 * @return         [description]读入的byte数
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
 * [LinearTransform description]灰度线性变换/拉伸
 * @param  minout [description]线性变换下限
 * @param  maxout [description]线性变换上限
 * @return        [description]
 */
BOOL CDib::LinearTransform(int minout, int maxout)
{
	//首先判断位图指针是否为空，若为空，说明没有读到数据，程序返回
	if(m_pBMI==NULL)
		return FALSE;
	//得到影像的每个像素所占比特数
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString i2s;
		i2s.Format("%d", wBitCount);
		CString strMsg = "该线性拉伸只处理8位图像！该图像为" + i2s + "位图像！";
		MessageBox(NULL, strMsg, "系统提示", MB_OK);
		return FALSE;
	} 
	//获取影像的宽度和高度
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//获取影像每行的实际存储宽度
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	//定义字符指针变量，用来保存位图第四部分起始地址
	LPBYTE lpData = m_pBits;
	DWORD i, j;
	//如果是8位位图
	if(wBitCount==8)
	{
		//统计原图像最大和最小灰度级（原灰度范围）
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
		//计算线性变换后的新值并更新图像
		for(i=0; i<nHeight; ++i)
		{
			for(j=0; j<nWidth; ++j)
			{
				//按灰度变换公式计算变换后的每个像素的灰度值
				*(lpData+lRowBytes*i+j) = (BYTE)(minout+(maxout-minout)/(m_MaxGray-m_MinGray)*(*(lpData+lRowBytes*i+j)-m_MinGray));
			}
		}
	}
	return TRUE;
}

BOOL CDib::HistoEquivalize()
{
	//首先判断位图指针是否为空，若为空，说明没有读到数据，程序返回
	if(m_pBMI==NULL)
		return FALSE;
	//得到影像的每个像素所占比特数
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "该直方图均衡只处理8位图像！" + bitsNum + "位图像！";
		MessageBox(NULL, strMsg, "系统提示", MB_OK);
		return FALSE;
	}
	//获取影像的宽度和高度
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//获取影像每行的实际存储宽度
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	//定义字符串指针变量，用来保存位图第四部分起始地址
	LPBYTE lpData = m_pBits;

	DWORD i, j;
	//如果是8位位图
	if(wBitCount==8)
	{
		//统计各灰度级的像素个数
		double r[256];
		for(i=0; i<256; ++i)
			r[i] = 0.0;
		for(i=0; i<nHeight; ++i)
			for(j=0; j<nWidth; ++j)
				r[*(lpData+i*lRowBytes+j)] += 1.0;

		//计算均衡化后各亮度的新值
		double percentile[256];
		for(i=0; i<256; ++i)
		{
			percentile[i] = 0.0;
			for(j=0; j<=i; ++j)
				percentile[i] += r[j];
			percentile[i] *= (255.0/(double)(nWidth*nHeight));
		}

		//更新图像
		for(i=0; i<nHeight; ++i)
			for(j=0; j<nWidth; ++j)
				*(lpData+lRowBytes*i+j) = (BYTE)(percentile[*(lpData+lRowBytes*i+j)]);
	}
	return TRUE;
}

/**
 * [LowPassDib description]低通滤波
 * @return [description]
 */
BOOL CDib::LowPassDib()
{
	//首先判断位图指针是否为空，若为空说明没有读到数据，程序返回
	if(m_pBMI==NULL)
		return FALSE;
	//得到影像的每个像素所占的比特数
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "该低通滤波只处理8位图像！该图像为" + bitsNum + "位图像！";
		MessageBox(NULL, strMsg, "系统提示", MB_OK);
		return FALSE;
	}

	//获得实际的位图数据占用的字节数
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//分配与待处理位图同样大小的内存空间
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//若内存申请失败，弹出消息并返回退出
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//若内存分配成功，将位图数据拷贝到新申请的内存中
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);
	//获取影像的宽度和高度
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//获取影像每行的实际存储宽度
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));
	LPBYTE lpData = m_pBits; //原位图指针
	LPBYTE lpOldBits = m_lpImageCopy; //原位图的拷贝

	//图像变换开始
	DWORD i, j;
	//定义9个字符指针变量，对应3*3模板的9个像素的存储地址
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
	//对8比特位图进行处理
	if(wBitCount==8){
		for(i=1; i<nHeight-1; ++i){
			for(j=1; j<nWidth-1; ++j){
				//得到模板每个像素的地址，因滤波运算是在原位图基础上进行的，
				//不能使用变换后的像素灰度作滤波运算（要用拷贝的图像）
				p1 = lpOldBits + lRowBytes*(i-1)+(j-1);
				p2 = lpOldBits + lRowBytes*(i)+(j-1);
				p3 = lpOldBits + lRowBytes*(i+1)+(j-1);
				p4 = lpOldBits + lRowBytes*(i-1)+(j);
				//使p5指针指向原始影像的模板中心对应像素
				p5 = lpData + lRowBytes*(i)+(j);
				p6 = lpOldBits + lRowBytes*(i+1)+(j);
				p7 = lpOldBits + lRowBytes*(i-1)+(j+1);
				p8 = lpOldBits + lRowBytes*(i)+(j+1);
				p9 = lpOldBits + lRowBytes*(i+1)+(j+1);
				//得到模板中心对应原位图的灰度值
				*p5 = *(lpOldBits+lRowBytes*(i)+(j));

				//计算模板中心像素的灰度值，同时对大于255或小于0的像素进行处理
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
 * [CDib::HighPassDib description]3x3高通滤波
 * @return [description]
 */
BOOL CDib::HighPassDib()
{
	if(m_pBMI==NULL)
		return FALSE;
	//得到影像的每个像素所占的比特数
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString bitsNum;
		bitsNum.Format(wBitCount);
		CString strMsg = "该低通滤波只处理8位图像！该图像为" + bitsNum + "位图像！";
		MessageBox(NULL, strMsg, "系统提示", MB_OK);
		return FALSE;
	}

	//获得实际的位图数据占用的字节数
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//分配与待处理的位图同样大小的内存空间
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//若内存申请失败，弹出消息并返回退出
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//若内存分配成功，将位图数据拷贝到新申请的内存中
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);
	//获取影像的宽度和高度
	DWORD nWidth = Width();
	DWORD nHeight = Height();

	//获取影像每行的实际存储宽度
	DWORD lRowBytes = WIDTHBYTES(((DWORD)wBitCount)*nWidth);
	LPBYTE lpData = m_pBits; //原位图指针
	LPBYTE lpOldBits = m_lpImageCopy; //原位图的拷贝

	//图像变换开始
	DWORD i, j;
	//定义9个字符指针变量，对应3x3模板的9个像素的存储地址
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
	//对8比特位图进行处理
	if(wBitCount==8){
		for(i=1; i<nHeight-1; ++i){
			for(j=1; j<nWidth-1; ++j){
				//得到模板每个像素的地址
				p1 = lpOldBits+lRowBytes*(i-1)+(j-1);
				p2 = lpOldBits+lRowBytes*(i)+(j-1);
				p3 = lpOldBits+lRowBytes*(i+1)+(j-1);
				p4 = lpOldBits+lRowBytes*(i-1)+j;
				p5 = lpData+lRowBytes*i+j;
				p6 = lpOldBits+lRowBytes*(i+1)+j;
				p7 = lpOldBits+lRowBytes*(i-1)+(j+1);
				p8 = lpOldBits+lRowBytes*i+(j+1);
				p9 = lpOldBits+lRowBytes*(i+1)+(j+1);

				//得到模板中心对应原位图的灰度值
				*p5 = *(lpOldBits+lRowBytes*i+j);
				//计算模板中心像素的灰度值，同时对大于255和小于0的像素进行处理
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
 * [CDib::MedianFilterDIB description]中值滤波
 * @return [description]
 */
BOOL CDib::MedianFilterDIB()
{
	if(m_pBMI==NULL)
		return FALSE;
	//得到影像的每个像素所占比特数
	WORD wBitCount = m_pBMI->bmiHeader.biBitCount;
	if(wBitCount!=8)
	{
		CString strMsg;
		strMsg.Format(wBitCount);
		CString Msg = "该中值滤波器只处理8位图像！该图像为" + strMsg + "位图像！";
		MessageBox(NULL, Msg, "系统提示", MB_OK);
		return FALSE;
	}

	//获得实际的位图数据占用的字节数
	DWORD m_dwSizeImage = m_pBMI->bmiHeader.biSizeImage;
	//分配与待处理位图相同大小的内存
	LPBYTE m_lpImageCopy = (BYTE *)malloc(m_dwSizeImage);
	if(m_lpImageCopy==NULL)
	{
		//若内存申请失败，弹出消息并返回退出
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		AfxMessageBox("Memory Allocate error");
		return FALSE;
	}
	//若内存分配成功，将位图数据拷贝到新申请的内存中
	memcpy(m_lpImageCopy, m_pBits, m_dwSizeImage);

	//获取影像每行的实际存储宽度
	DWORD nWidth = Width();
	DWORD nHeight = Height();
	//获取影像每行的实际存储宽度
	DWORD lRowBytes = WIDTHBYTES(nWidth*((DWORD)wBitCount));

	LPBYTE lpData = m_pBits; //原位图指针
	LPBYTE lpOldBits = m_lpImageCopy; //原位图的拷贝
	//图像变换开始
	DWORD i, j;
	//定义15个字符指针变量，对应5x5模板的25个像素的存储地址
	BYTE *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, 
	*p11, *p12, *p13, *p14, *p15, *p16, *p17, *p18, *p19, *p20, 
	*p21, *p22, *p23, *p24, *p25;
	//对8比特位图进行处理
	if(wBitCount==8){
		for(i=2; i<nHeight-2; ++i){
			for(j=2; j<nWidth-2; ++j){
				//得到模板每个像素的地址
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
				//使p13指针指向原始影像的模板中心对应像素
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

				//得到模板中心对应原位图的灰度值
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

				//起泡法排序
				for(int j=1; j<25; ++j){
					for(int i=1; i<=25-j; ++i){
						if(temp[i]>temp[i+1]){
							t = temp[i];
							temp[i] = temp[i+1];
							temp[i+1] = t;
						}
					}
				}
				//将中间灰度值赋给模板中心对应的像素
				*p13 = (BYTE)temp[12];
			}
		}
	}
	return TRUE;
}

/**
 * [TranslationDIB description]图像平移
 * @param  dXOffset [description]水平位移
 * @param  dYOffset [description]垂直位移
 * @return          [description]
 */
BOOL CDib::TranslationDIB(DWORD dXOffset, DWORD dYOffset)
{
	if(m_pBMI==NULL)
		return FALSE;

	//获得实际的位图数据占用的字节数
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

	//像素在新DIB中的坐标
	DWORD i, j;

	//像素在原DIB中的坐标
	long i_0, j_0;

	for(i=0; i<nHeight; ++i){
		for(j=0; j<nWidth; ++j){
			LPBYTE lpTemp = lpData + lRowBytes*i+j;

			//计算该像素在原DIB中的坐标
			i_0 = i - dXOffset;
			j_0 = j - dYOffset;

			//判断是否在原图像范围内
			if((j_0>=0)&&(j_0<(long)nWidth)&&(i_0>=0)&&(i_0<(long)nHeight)){
				*lpTemp = *(lpOldBits + lRowBytes*i_0 + j_0);
			}
			else{
				//对于原图像中没有的像素，直接赋值为255
				*lpTemp = (BYTE)255;
			}
		}
	}
	return TRUE;
}

/**
 * [CDib::ZoomDIB description]图像缩放
 * @param fXZoomRatio [description]X轴缩放比例
 * @param fYZoomRatio [description]Y轴缩放比例
 */
void CDib::ZoomDIB(float fXZoomRatio, float fYZoomRatio)
{
	//原图像的宽度和高度
	LONG lWidth;
	LONG lHeight;

	//缩放后图像的宽度和高度
	LONG lNewWidth;
	LONG lNewHeight;

	//缩放后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG lNewLineBytes;

	//指向原图像的指针
	LPBYTE lpDIBBits;

	//指向源像素的指针
	LPBYTE lpSrc;

	//缩放后新DIB句柄
	LPBYTE lpZoomBits;

	//指向缩放图像对应像素的指针
	LPBYTE lpDst;

	//指向缩放图像的指针
	LPBYTE lpNewDIB;
	LPBYTE lpNewDIBBits;

	//循环变量（像素在新DIB中的坐标）
	LONG i, j;

	//像素在原DIB中的坐标
	LONG i_0, j_0;

	//图像每行的字节数
	LONG lLineBytes;

	//锁定原图像
	LPBYTE lpDIB = (LPBYTE)::GlobalLock((HGLOBAL) m_pBits);
	//找到原图像DIB像素起始位置
	lpDIBBits = m_pBits;

	lWidth = Width();
	lLineBytes = WIDTHBYTES(lWidth*8);
	lHeight = Height();

	//计算缩放后图像实际宽度
	//此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lNewWidth = (LONG)(lWidth*fXZoomRatio+0.5);

	//计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth*8);

	//计算缩放后的图像高度
	lNewHeight = (LONG)(lHeight*fXZoomRatio+0.5);

	//分配内存，以保存新DIB
	lpZoomBits = (BYTE *)malloc(lNewLineBytes*lNewHeight+bfOffsetBits);

	//判断是否内存分配失败
	if(lpZoomBits==NULL)
		return;
	//锁定内存
	lpNewDIB = (unsigned char *)::GlobalLock((HGLOBAL)lpZoomBits);

	//复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, bfOffsetBits);

	//找到新DIB像素起始位置
	lpNewDIBBits = lpNewDIB + bfOffsetBits;

	//针对图像每行进行操作
	for(i=0; i<lNewHeight; ++i){
		for(j=0; j<lNewWidth; ++j){
			//指向新DIB第i行，第j个像素的指针
			//注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (unsigned char *)lpNewDIBBits + lNewLineBytes*(lNewHeight-1-i)+j;
			//计算该像素在原DIB中的坐标
			i_0 = (LONG)(i/fYZoomRatio+0.5);
			j_0 = (LONG)(j/fXZoomRatio+0.5);
			//判断是否在原图像范围内
			if((j_0>=0)&&(j_0<lWidth)&&(i_0>=0)&&(i_0<lHeight)){
				//指向原DIB第i_0行，第j_0个像素的指针
				lpSrc = (unsigned char *)lpDIBBits + lLineBytes*(lHeight-1-i_0)+j_0;
				//复制像素
				*lpDst = *lpSrc;
			}
			else{
				//对于原图像中没有的像素，直接赋值为255
				*((unsigned char *)lpDst) = 255;
			}
		}
	}

	//替换原图像，显示新图像
	m_pBMI->bmiHeader.biHeight = lNewHeight;
	m_pBMI->bmiHeader.biWidth = lNewWidth;
	m_pBMI->bmiHeader.biSizeImage = lNewHeight * lNewWidth;
	m_pBits = lpNewDIBBits;

	return;
}

/**
 * [CDib::RotateDIB description]图像旋转
 * @param iRotateAngle [description]旋转的角度（0~360°）
 */
void CDib::RotateDIB(int iRotateAngle)
{
	//原图像的宽度和高度
	LONG lWidth;
	LONG lHeight;

	//旋转后图像的宽度和高度
	LONG lNewWidth;
	LONG lNewHeight;

	//图像每行的字节数
	LONG lLineBytes;

	//旋转后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG lNewLineBytes;

	//指向原图像的指针
	LPBYTE lpDIBBits;

	//指向原像素的指针
	LPBYTE lpSrc;

	//旋转后新DIB句柄
	//HDIB hDIB;
	
	//指向旋转图像图像对应像素的指针
	LPBYTE lpDst;

	//指向旋转图像的指针
	LPBYTE lpNewDIB;
	LPBYTE lpNewDIBBits;

	//循环变量（像素在新DIB中的坐标）
	LONG i, j;

	//像素在原DIB中的坐标
	LONG i_0, j_0;

	//旋转角度（弧度）
	float fRotateAngle;

	//旋转角度的正弦和余弦
	float fSina, fCosa;

	//原图像四个角的坐标（以图像中心为坐标系原点）
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;

	//旋转后四个角的坐标（以图像中心为坐标系原点）
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	//两个中间常量
	float f1, f2;

	//锁定原图像
	LPBYTE lpDIB = (LPBYTE)::GlobalLock((HGLOBAL) m_pBits);

	//找到的DIB图像像素起始位置
	lpDIBBits = m_pBits;

	//获取图像的“宽度”（4的倍数）
	lWidth = Width();

	//计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth*8);

	//获取图像的高度
	lHeight = Height();

	//将旋转角度从度转换为弧度
	fRotateAngle = (float)(iRotateAngle*3.141592653/180.0);

	//计算旋转角度的正弦和余弦
	fSina = (float)sin((double)fRotateAngle);
	fCosa = (float)cos((double)fRotateAngle);

	//计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float)(-(lWidth-1)/2);
	fSrcY1 = (float)((lHeight-1)/2);
	fSrcX2 = (float)((lWidth-1)/2);
	fSrcY2 = (float)((lHeight-1)/2);
	fSrcX3 = (float)(-(lWidth-1)/2);
	fSrcY3 = (float)(-(lHeight-1)/2);
	fSrcX4 = (float)((lWidth-1)/2);
	fSrcY4 = (float)(-(lHeight-1)/2);

	//计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 = fCosa*fSrcX1 + fSina*fSrcY1;
	fDstY1 = -fSina*fSrcX1 + fCosa*fSrcY1;
	fDstX2 = fCosa*fSrcX2 + fSina*fSrcY2;
	fDstY2 = -fSina*fSrcX2 + fCosa*fSrcY2;
	fDstX3 = fCosa*fSrcX3 + fSina*fSrcY3;
	fDstY3 = -fSina*fSrcX3 + fCosa*fSrcY3;
	fDstX4 = fCosa*fSrcX4 + fSina*fSrcY4;
	fDstY4 = -fSina*fSrcX4 + fCosa*fSrcY4;

	//计算旋转后的图像实际宽度
	lNewWidth = (LONG)(max(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
	//计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth*8);

	//计算旋转后图像的高度
	lNewHeight = (LONG)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

	//两个常数，这样不用以后每次都计算了
	f1 = (float)(-0.5*(lNewWidth-1)*fCosa-0.5*(lNewHeight-1)*fSina+0.5*(lWidth-1));
	f2 = (float)(0.5*(lNewWidth-1)*fSina-0.5*(lNewHeight-1)*fCosa+0.5*(lHeight-1));

	//分配内存，以保存新DIB
	LPBYTE lpImageRotate = (BYTE *)malloc(lNewLineBytes*lNewHeight+bfOffsetBits);

	//判断是否内存分配失败
	if(lpImageRotate==NULL)
		return; //分配内存失败

	//锁定内存
	lpNewDIB = (LPBYTE)::GlobalLock((HGLOBAL)lpImageRotate);

	//复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, bfOffsetBits);

	//找到新DIB像素起始位置
	lpNewDIBBits = lpNewDIB + bfOffsetBits;

	//针对图像每行进行操作
	for(i=0; i<lNewHeight; ++i){
		for(j=0; j<lNewWidth; ++j){
			//指向新DIB第i行，第j个像素的指针
			//注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (LPBYTE)lpNewDIBBits + lNewLineBytes*(lNewHeight-1-i) + j;

			//计算该像素在原DIB中的坐标
			i_0 = (LONG)(-((float)j)*fSina+((float)i)*fCosa+f2+0.5);
			j_0 = (LONG)(((float)j)*fCosa+((float)i)*fSina+f1+0.5);

			//判断是否在原图像范围内
			if((j_0>=0)&&(j_0<lWidth)&&(i_0>=0)&&(i_0<lHeight)){
				//指向原DIB第i_0行，第j_0个像素的指针
				lpSrc = (LPBYTE)lpDIBBits + lLineBytes * (lHeight-1-i_0) + j_0;

				//复制像素
				*lpDst = *lpSrc;
			}
			else{
				//对于原图像中没有的像素，直接赋值为255
				*((unsigned char *)lpDst) = 255;
			}
		}
	}

	//替换原图像，显示新图像
	m_pBMI->bmiHeader.biHeight = lNewHeight;
	m_pBMI->bmiHeader.biWidth = lNewWidth;
	m_pBMI->bmiHeader.biSizeImage = lNewHeight * lNewWidth;
	m_pBits = lpNewDIBBits;

	//返回
	return;
}

/**
 * [TransposeDIB description]图像转置
 */
void CDib::TransposeDIB()
{
	//图像的宽度和高度
	LONG lWidth;
	LONG lHeight;

	//指向原图像的指针
	LPBYTE lpDIBBits;

	//指向原像素的指针
	LPBYTE lpSrc;

	//指向转置图像对应像素的指针
	LPBYTE lpDst;

	//指向转置图像的指针
	LPBYTE lpNewDIBBits;

	//循环变量
	LONG i, j;

	//图像每行的字节数
	LONG lLineBytes;

	//新图像每行的字节数
	LONG lNewLineBytes;

	//找到原DIB图像像素起始位置
	lpDIBBits = m_pBits;

	//获取图像的高度、宽度（4的倍数）
	lWidth = Width();
	lHeight = Height();

	//计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lHeight * 8);

	//暂时分配内存，以保存新图像
	LPBYTE hNewDIBBits = (BYTE *)malloc(lWidth * lNewLineBytes);

	//判断是否内存分配失败
	if(hNewDIBBits==NULL)
		return;

	//锁定内存
	lpNewDIBBits = (unsigned char *)LocalLock(hNewDIBBits);

	//针对图像每行进行操作
	for(i=0; i<lHeight; ++i){
		for(j=0; j<lWidth; ++j){
			//指向原DIB第i行，第j个像素的指针
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//指向转置DIB第j行，第i个像素的指针
			//注意此处lWidth和lHeight是原DIB的宽度和高度，应该互换
			lpDst = (unsigned char *)lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;
			//复制像素
			*lpDst = *lpSrc;
		}
	}

	//复制转置后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth*lNewLineBytes);
	//替换原图像，显示新图像
	m_pBMI->bmiHeader.biHeight = lWidth;
	m_pBMI->bmiHeader.biWidth = lHeight;
	m_pBits = lpNewDIBBits;

	//释放内存
	LocalUnlock(hNewDIBBits);

	return;
}

/**
 * [CDib::Fourie description]二维傅里叶变换
 * @param pCTData [description]图像数据
 * @param nWidth  [description]数据宽度
 * @param nHeight [description]数据高度
 * @param pCFData [description]变换结果
 */
void CDib::Fourie(complex<double> *pCTData, int nWidth, int nHeight, complex<double> *pCFData)
{
	//循环控制变量
	int x, y;

	//临时变量
	double dTmpOne;
	double dTmpTwo;

	//进行傅里叶变换的宽度和高度（2的整数次幂）
	//图像的宽度和高度不一定为2的整数次幂
	int nTransWidth;
	int nTransHeight;

	//计算进行傅里叶变换的宽度（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	//计算进行傅里叶变换的高度（2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	//x, y（行列）方向上的迭代次数
	int nXLev;
	int nYLev;

	//计算x, y（行列）方向上的迭代次数
	nXLev = (int)(log(nTransWidth)/log(2)+0.5);
	nYLev = (int)(log(nTransHeight)/log(2)+0.5);

	for(y=0; y<nTransHeight; ++y){
		//x方向进行快速傅里叶变换
		FastFourie(&pCTData[nTransWidth*y], &pCFData[nTransWidth*y], nXLev);
	}

	//pCFData中目前存储了pCTData经过行变换的结果
	//为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
	//傅里叶行变换（实际上相当于对列进行傅里叶变换）
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTData[nTransHeight*x+y] = pCFData[nTransWidth*y+x];
		}
	}

	for(x=0; x<nTransWidth; ++x){
		//对x方向进行快速傅里叶变换，实际上相当于对原来的图像数据进行列方向的傅里叶变换
		FastFourie(&pCTData[x*nTransHeight], &pCFData[x*nTransHeight], nYLev);
	}

	//pCFData中目前存储了pCTData经过二维傅里叶变换的结果，但是为了方便列方向
	//的傅里叶变换，对其进行了转置，现在转回来
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTData[nTransWidth*y+x] = pCFData[nTransHeight*x+y];
		}
	}

	memcpy(pCTData, pCFData, sizeof(complex<double>)*nTransHeight*nTransWidth);
}

/**
 * [CDib::FastFourie description]快速傅里叶变换
 * @param pCTData [description]指向时域数据的指针，输入的需要变换的数据
 * @param pCFData [description]指向频域数据的指针，输出的经过变换的数据
 * @param nLevel  [description]傅里叶变换蝶形算法的级数，2的幂数
 */
void CDib::FastFourie(complex<double> *pCTData, complex<double> *pCFData, int nLevel)
{
	//循环控制变量
	int i, j, k;

	double PI = 3.1415926;

	//傅里叶变换点数
	int nCount = 0;

	//计算傅里叶变换点数
	nCount = (int)pow(2, nLevel);

	//某一级的长度
	int nBtFlyLen;
	nBtFlyLen = 0;

	//变换系数的角度=2*PI*i/nCount
	double dAngle;

	complex<double> *pCW;

	//分配内存，存储傅里叶变化需要的系数表
	pCW = new complex<double>[nCount/2];

	//计算傅里叶变换的系数
	for(i=0; i<nCount/2; ++i){
		dAngle = -2*PI*i/nCount;
		pCW[i] = complex<double> (cos(dAngle), sin(dAngle));
	}

	//变换需要的工作空间
	complex<double> *pCWork1, *pCWork2;

	//分配工作空间
	pCWork1 = new complex<double>[nCount];
	pCWork2 = new complex<double>[nCount];

	//临时变量
	complex<double> *pCTmp;

	//初始化，写入数据
	memcpy(pCWork1, pCTData, sizeof(complex<double>)*nCount);

	//临时变量
	int nInter;
	nInter = 0;

	//蝶形算法进行快速傅里叶变换
	for(k=0; k<nLevel; ++k){
		for(j=0; j<(int)pow(2,k); ++j){
			//计算长度
			nBtFlyLen = (int)pow(2, (nLevel-k));
			//倒序重排，加权计算
			for(i=0; i<nBtFlyLen/2; ++i){
				nInter = j*nBtFlyLen;
				pCWork2[i+nInter] = pCWork1[i+nInter] + pCWork1[i+nInter+nBtFlyLen/2];
				pCWork2[i+nInter+nBtFlyLen/2] = (pCWork2[i+nInter]-pCWork1[i+nInter+nBtFlyLen/2])*pCW[(int)(i*pow(2, k))];
			}
		}

		//交换pcWork1和pCWork2的数据
		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	//重新排序
	for(j=0; j<nCount; ++j){
		nInter = 0;
		for(i=0; i<nLevel; ++i){
			if(j&(1<<i))
				nInter += 1<<(nLevel-i-1);
		}
		pCFData[j] = pCWork1[nInter];
	}

	//释放内存空间
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = NULL;
	pCWork1 = NULL;
	pCWork2 = NULL;
}

/**
 * [IFourie description]二维傅里叶反变换
 * @param pCFData [description]频域数据
 * @param pCTData [description]时域数据
 * @param nWidth  [description]图像数据宽度
 * @param nHeight [description]图像数据高度
 */
void CDib::IFourie(complex<double> *pCFData, complex<double> *pCTData, int nWidth, int nHeight)
{
	int x, y;

	double dTmpOne;
	double dTmpTwo;

	//进行傅里叶变换的宽度和高度（2的整数次幂）
	//图像的宽度和高度不一定是2的整数次幂
	int nTransWidth;
	int nTransHeight;

	//计算傅里叶变换的宽度（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	//计算傅里叶变换的高度（2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;	

	//分配工作需要的内存空间
	complex<double> *pCWork = new complex<double>[nTransWidth*nTransHeight];

	//临时变量
	complex<double> *pCTmp;

	//为了利用傅里叶正变换，可以把傅里叶频域的数据取共轭
	//然后直接利用正变换，输出结果就是傅里叶反变换结果的共轭
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTmp = &pCFData[nTransWidth*y+x];
			pCWork[nTransWidth*y+x] = complex<double>(pCTmp->real(), -pCTmp->imag());
		}
	}

	//调用傅里叶正变换
	Fourie(pCWork, nWidth, nHeight, pCTData);
	//求时域点的共轭，求得最终结果
	//根据傅里叶变换原理，利用这样的方法求得的结果和实际的时域数据相差一个系数
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTmp = &pCTData[nTransWidth*y+x];
			pCTData[nTransWidth*y+x] = complex<double>(pCTmp->real()/(nTransWidth*nTransHeight), -pCTmp->imag()/(nTransWidth*nTransHeight));
		}
	}

	delete pCWork;
	pCWork = NULL;
}

/**
 * [ButterWorthLowPass description]ButterWorth低通滤波
 * @param lpImage [description]指向需要增强的图像数据
 * @param nWidth  [description]数据宽度
 * @param nHeight [description]数据高度
 * @param nRadius [description]ButterWorth低通滤波的“半功率”点
 */
/*void CDib::ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	int x, y;

	double dTmpOne, dTmpTwo;

	//ButterWorth滤波系数
	double H;

	//傅里叶变换的宽度和高度
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
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

	//傅里叶正变换
	Fourie(pCTData, nWidth, nHeight, pCFData);

	//开始实施ButterWorth低通滤波
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			H = (double)(y*y+x*x);
			H = H/(nRadius*nRadius);
			H = 1/(1+H);
			pCFData[y*nTransWidth+x] = complex<double>(pCFData[y*nTransWidth+x].real()*H, pCFData[y*nTransWidth+x].imag()*H);
		}
	}

	//对经过ButterWorth低通滤波的图像进行反变换
	IFourie(pCFData, pCTData, nWidth, nHeight);

	//把反变换的数据传给lpImage
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			dReal = pCTData[y*nTransWidth+x].real();
			dImag = pCTData[y*nTransWidth+x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal+dImag*dImag)));
			lpImage[y*nWidth+x] = unchValue;
		}
	}

	//释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
}
*/
/**
 * [ButterWorthHighPass description]ButterWorth高通滤波
 * @param lpImage [description]指向需要增强的图像数据
 * @param nWidth  [description]数据宽度
 * @param nHeight [description]数据高度
 * @param nRadius [description]ButterWorth高通滤波的“半功率”点
 */
/*void CDib::ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	int x, y;

	double dTmpOne;
	double dTmpTwo;

	//ButterWorth滤波系数
	double  H;

	//傅里叶变换的宽度和高度（2的整数次幂）
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
	//图像像素值
	unsigned char unchValue;

	//指向时域数据的指针
	complex<double> *pCTData;
	//指向频域数据的指针
	complex<double> *pCFData;

	//计算进行傅里叶变换的点数（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	//分配内存
	pCTData = new complex<double>[nTransWidth*nTransHeight];
	pCFData = new complex<double>[nTransWidth*nTransHeight];

	//初始化
	//图像数据的宽度和高度不一定是2的整数次幂，所以pCTData有一部分数据需要补0
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransWidth; ++x){
			pCTData[y*nTransWidth+x] = complex<double>(0,0);
		}
	}

	//把图像数据传给pCTData
	for(y=0; y<nHeight; ++y){
		for(x=0; x<nWidth; ++x){
			unchValue = lpImage[y*nWidth+x];
			pCTData[y*nTransWidth+x] = complex<double>(unchValue, 0);
		}
	}

	//傅里叶正变换
	Fourie(pCTData, nWidth, nHeight, pCFData);

	//下面开始实施ButterWorth高通滤波
	for(y=0; y<nTransHeight; ++y){
		for(x=0; x<nTransHeight; ++x){
			H = (double)(y*y+x*x);
			H = (nRadius*nRadius)/H;
			H = 1/(1+H);
			pCFData[y*nTransWidth+x] = complex<double>(H*(pCFData[y*nTransWidth+x].real()), H*(pCFData[y*nTransWidth+x].imag()));
		}
	}

	//对经过ButterWorth高通滤波的图像进行反变换
	IFourie(pCFData, pCTData, nWidth, nHeight);

	//把反变换的数据传给lpImage
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
}
*/