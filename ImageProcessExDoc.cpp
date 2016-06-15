// ImageProcessExDoc.cpp : implementation of the CImageProcessExDoc class
//

#include "stdafx.h"
#include "ImageProcessEx.h"

#include "ImageProcessExDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExDoc

IMPLEMENT_DYNCREATE(CImageProcessExDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessExDoc, CDocument)
	//{{AFX_MSG_MAP(CImageProcessExDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExDoc construction/destruction

CImageProcessExDoc::CImageProcessExDoc()
{
	// TODO: add one-time construction code here

}

CImageProcessExDoc::~CImageProcessExDoc()
{
}

BOOL CImageProcessExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageProcessExDoc serialization

void CImageProcessExDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExDoc diagnostics

#ifdef _DEBUG
void CImageProcessExDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessExDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessExDoc commands

BOOL CImageProcessExDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	CFile file;
	CFileException fe;
	if(!file.Open(lpszPathName, CFile::modeRead|CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	BeginWaitCursor();

	//调用Read DIBFile函数读取位图
	TRY
	{
		m_DIB.Read(file);
	}
	CATCH(CFileException, eLoad)
	{
		file.Abort();
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH
		
	EndWaitCursor();

	if(!m_DIB.IsValid())
	{
		CString strMsg = "File can't open!";

		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);
	return TRUE;
}

BOOL CImageProcessExDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFile file;
	CFileException fe;

	if(!file.Open(lpszPathName, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	//调用SaveDIB函数存储位图
	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
		bSuccess = m_DIB.Save(file);
		file.Close();
	}
	CATCH(CException, eSave)
	{
		file.Abort();
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH;

	EndWaitCursor();
	SetModifiedFlag(FALSE);

	if(!bSuccess)
	{
		CString strMsg = "Wrong~~~";
		//strMsg.LoadString(IDS_CANNOT_SAVE_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION|MB_OK);
	}
	return TRUE;
}
