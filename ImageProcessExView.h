// ImageProcessExView.h : interface of the CImageProcessExView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSEXVIEW_H__2E99C75C_3738_44CE_8D49_29FA78BC39DA__INCLUDED_)
#define AFX_IMAGEPROCESSEXVIEW_H__2E99C75C_3738_44CE_8D49_29FA78BC39DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessExView : public CScrollView
{
protected: // create from serialization only
	CImageProcessExView();
	DECLARE_DYNCREATE(CImageProcessExView)

// Attributes
public:
	CImageProcessExDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessExView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessExView)
	afx_msg void OnRawtobmp();
	afx_msg void OnLinearenhence();
	afx_msg void OnHistoequivalize();
	afx_msg void OnLowpass();
	afx_msg void OnHighpass();
	afx_msg void OnMiddlepass();
	afx_msg void OnTransposition();
	afx_msg void OnTranslation();
	afx_msg void OnZoom();
	afx_msg void OnRotate();
	afx_msg void OnFourie();
	afx_msg void OnIfourie();
	afx_msg void OnBwhighpass();
	afx_msg void OnBwlowpass();
	afx_msg void OnRoberts();
	afx_msg void OnPrewitt();
	afx_msg void OnSobel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageProcessExView.cpp
inline CImageProcessExDoc* CImageProcessExView::GetDocument()
   { return (CImageProcessExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSEXVIEW_H__2E99C75C_3738_44CE_8D49_29FA78BC39DA__INCLUDED_)
