// ImdViewerView.h : interface of the CImdViewerView class
//


#pragma once


class CImdViewerView : public COpenGLWnd
{
protected: // create from serialization only
	CImdViewerView();
	DECLARE_DYNCREATE(CImdViewerView)

// Attributes
public:
	CImdViewerDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void		Render();
	void		UpdateCamera();
protected:
	void		RenderMesh(imd2_mesh_t *mesh, size_t current_anim = 0, imd2_vertex_t *buffer = 0);
	void		RenderSkinnedMesh(imd2_mesh_t *mesh, size_t current_anim, imd2_bone_file_t *bones);
	// Implementation
public:
	virtual ~CImdViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	RotateAt(float angle, vect3d *axis);
protected:
	float	_alpha;
	float	_beta;
	vect3d	_position;
	vect3d	_up;
	vect3d	_at;
	// mouse camera
	vect3d	_direction;
	vect3d	_inv_direction;
	vect3d	_strafe_left;
	vect3d	_inv_strafe_left;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ImdViewerView.cpp
inline CImdViewerDoc* CImdViewerView::GetDocument() const
   { return reinterpret_cast<CImdViewerDoc*>(m_pDocument); }
#endif

