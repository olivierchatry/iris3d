/*********************************************************
**  File name : OpenGLWnd.h
**  Iris Exporter V0.5
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#if !defined(AFX_OPENGLWND_H__83310F64_5D53_44AE_9D08_E29D6A961D6F__INCLUDED_)
#define AFX_OPENGLWND_H__83310F64_5D53_44AE_9D08_E29D6A961D6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd view

// The base class for all our viewport views.
// Handles all the basic OpenGL init stuff for
// MFC. A similar method could be used for
// another API like Direct 3D.

class COpenGLWnd : public CView
{
protected:
	COpenGLWnd();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COpenGLWnd)

// Attributes
public:
												// function. Used to change to easily
												// change what a viewport displays.
protected:
	HGLRC m_hRC; //Rendering Context
	CDC* m_pDC;  //Device Context

// Operations
public:
	// OpenGL init stuff
	BOOL SetupPixelFormat();
	BOOL InitOpenGL();

	// Each viewport uses its own context
	// so we need to make sure the correct
	// context is set whenever we make an
	// OpenGL command.
	void SetContext() { wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ); }
	void SwapGLBuffers() { 	SwapBuffers( m_pDC->GetSafeHdc() ); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLWnd)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COpenGLWnd();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLWND_H__83310F64_5D53_44AE_9D08_E29D6A961D6F__INCLUDED_)
