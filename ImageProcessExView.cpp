// ImageProcessExView.cpp : implementation of the CImageProcessExView class
//

#include "stdafx.h"
#include "ImageProcessEx.h"

#include "ImageProcessExDoc.h"
#include "ImageProcessExView.h"
#include "RowToBmpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView

IMPLEMENT_DYNCREATE(CImageProcessExView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessExView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessExView)
	ON_COMMAND(ID_RAWTOBMP, OnRawtobmp)
	ON_COMMAND(ID_LINEARENHENCE, OnLinearenhence)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView construction/destruction

CImageProcessExView::CImageProcessExView()
{
	// TODO: add construction code here

}

CImageProcessExView::~CImageProcessExView()
{
}

BOOL CImageProcessExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView drawing

void CImageProcessExView::OnDraw(CDC* pDC)
{
	CImageProcessExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(pDoc->m_DIB.IsValid())
	{
		int cxDIB = (int)pDoc->m_DIB.Width(); //位图x方向大小
		int cyDIB = (int)pDoc->m_DIB.Height(); //位图y方向大小
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if(pDC->IsPrinting())
		{
			//获取打印页面大小
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			//获取打印机每英寸像素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)/((double)cxDIB * cxInch)); //标记，看此行是否有错误
			rcDest.right = cxPage;
		}
		else
		{
			rcDest = rcDIB;
		}
		pDoc->m_DIB.Paint(pDC->m_hDC, &rcDest, &rcDIB);
	}
}

void CImageProcessExView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView printing

BOOL CImageProcessExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView diagnostics

#ifdef _DEBUG
void CImageProcessExView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessExView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessExDoc* CImageProcessExView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessExDoc)));
	return (CImageProcessExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExView message handlers

void CImageProcessExView::OnRawtobmp() 
{
	// TODO: Add your command handler code here
	CRowToBmpDlg dlg; //定义格式转换对话框类对象
	dlg.DoModal(); //用来弹出格式转换对话框
}

void CImageProcessExView::OnLinearenhence() 
{
	// TODO: Add your command handler code here
	//文件类型说明字符串
	static char BASED_CODE filedlg1[] = "BMP Files(*.BMP)|*.bmp|所有文件(*.*)|*.*||";
	//文件对话框初始化，这里的第一个参数TRUE表示以打开方式显示文件对话框
	CFileDialog SelectFile1(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filedlg1, NULL);
	//弹出文件打开对话框
	SelectFile1.DoModal();
	//得到所选文件路径（包括文件名）
	CString FileName1;
	FileName1 = SelectFile1.GetPathName();

	CFile file1, file2;
	file1.Open(FileName1, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);
	CDib linearTrans;
	linearTrans.Read(file1);
	linearTrans.LinearTransform(0, 255);

	static char BASED_CODE filedlg2[] = "BMP Files(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	CFileDialog SelectFile2(FALSE, "BMP", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filedlg2, NULL);
	SelectFile2.DoModal();
	CString FileName2;
	FileName2 = SelectFile2.GetPathName();
	file2.Open(FileName2, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive, NULL);
	linearTrans.Save(file2);
	file1.Close();
	file2.Close();
}
