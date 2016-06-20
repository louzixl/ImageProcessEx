// dib.cpp

#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
#include <afxadv.h>
#include <io.h>
#include <errno.h>

IMPLEMENT_DYNAMIC(CDib, CObject)
//构造函数，初始化
CDib::CDib()
{
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