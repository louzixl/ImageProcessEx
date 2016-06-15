// ImageProcessExDoc.h : interface of the CImageProcessExDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSEXDOC_H__B2516A2B_45F2_44F0_AA79_FE84E0E15F2A__INCLUDED_)
#define AFX_IMAGEPROCESSEXDOC_H__B2516A2B_45F2_44F0_AA79_FE84E0E15F2A__INCLUDED_

#include "dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessExDoc : public CDocument
{
protected: // create from serialization only
	CImageProcessExDoc();
	DECLARE_DYNCREATE(CImageProcessExDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessExDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDib m_DIB;
	virtual ~CImageProcessExDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessExDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSEXDOC_H__B2516A2B_45F2_44F0_AA79_FE84E0E15F2A__INCLUDED_)
