// dib.h

#ifndef _INC_DIB
#define _INC_DIB

//位图常数
#define PALVERSION 0x300
//位图头文件标记
#define DIB_HEADER_MARKER ((WORD)('M'<<8)|'B')
//常用宏
#define RECTWIDTH(lpRect) ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)
//WIDTHBYTES表示图像扫描行宽
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

//CDib类定义
class CDib:public CObject
{
	DECLARE_DYNAMIC(CDib)

public:
	CDib();
protected:
	LPBYTE m_pBits;
	LPBITMAPINFO m_pBMI;
public:
	CPalette *m_pPalette;
public:
	DWORD Width() const; //获取位图宽度
	DWORD Height() const; //获取位图高度
	WORD NumColors() const; //返回位图的调色板索引位数
	BOOL IsValid() const {return (m_pBMI != NULL); }
public:
	BOOL Paint(HDC, LPRECT, LPRECT) const; //显示位图
	HGLOBAL CopyToHandle() const; //拷贝到句柄
	DWORD Save(CFile &file) const; //保存位图
	DWORD Read(CFile &file); //读取位图文件
	DWORD ReadFromHandle(HGLOBAL hGlobal);
	LPBYTE RETURN()
	{
		return m_pBits;
	}
	LPBYTE DD(LPBYTE m)
	{
		m_pBits = m;
		return m_pBits;
	}
	void Invalidate() { Free(); }
	virtual void Serialize(CArchive &ar);

	BOOL LinearTransform(int minout, int maxout);

//执行部分
public:
	virtual~ CDib();
protected:
	BOOL CreatePalette();
	WORD PaletteSize() const;
	void Free();
public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext &dc) const;
	#endif
protected:
	CDib &operator=(CDib &dib);
};

#endif //! _INC_DIB