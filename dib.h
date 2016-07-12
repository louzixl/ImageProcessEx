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

#include <complex>
using std::complex;

//CDib�ඨ��
class CDib:public CObject
{
	DECLARE_DYNAMIC(CDib)

public:
	CDib();
protected:
	DWORD bfOffsetBits;
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

	BOOL LinearTransform(int minout, int maxout);
	BOOL HistoEquivalize();
	BOOL LowPassDib();
	BOOL HighPassDib();
	BOOL MedianFilterDIB();
	BOOL TranslationDIB(DWORD dXOffset, DWORD dYOffset);
	void ZoomDIB(float fXZoomRatio, float fYZoomRatio);
	void RotateDIB(int iRotateAngle);
	void TransposeDIB();
	void FastFourie(complex<double> *pCTData, complex<double> *pCFData, int nLevel);
	void Fourie(complex<double> *pCTData, int nWidth, int nHeight, complex<double> *pCFData);
	void IFourie(complex<double> *pCFData, complex<double> *pCTData, int nWidth, int nHeight);
	void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);
	void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);
	BOOL RobertsDIB();
	BOOL PrewittDIB(int tag);
	BOOL SobelDIB(int tag);

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