// dib.h

#ifndef _INC_DIB
#define _INC_DIB

//λͼ����
#define PALVERSION 0x300
//λͼͷ�ļ����
#define DIB_HEADER_MARKER ((WORD)('M'<<8)|'B')
//���ú�
#define RECTWIDTH(lpRect) ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)
//WIDTHBYTES��ʾͼ��ɨ���п�
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

//CDib�ඨ��
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
	DWORD Width() const; //��ȡλͼ���
	DWORD Height() const; //��ȡλͼ�߶�
	WORD NumColors() const; //����λͼ�ĵ�ɫ������λ��
	BOOL IsValid() const {return (m_pBMI != NULL); }
public:
	BOOL Paint(HDC, LPRECT, LPRECT) const; //��ʾλͼ
	HGLOBAL CopyToHandle() const; //���������
	DWORD Save(CFile &file) const; //����λͼ
	DWORD Read(CFile &file); //��ȡλͼ�ļ�
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

//ִ�в���
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