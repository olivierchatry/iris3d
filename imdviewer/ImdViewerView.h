// ImdViewerView.h : interface of the CImdViewerView class
//


#pragma once

struct Bone
{
	int			bone_index;
	imd2_bone_t *bone;
	std::vector<Bone> child;
};

struct Skin
{
	float	weight;
	int		bone_index;
	int		vertex_index;
};

struct Mesh
{
	int			num_skin;
	int			num_vertex;
	Skin		*skin;
	vertex3d_t	*v;
};

class CImdViewerView : public CView
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
protected:

// Implementation
public:
	virtual ~CImdViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	UpdateScene();
protected:
	IDirect3D8			*_d3d_object;
	IDirect3DDevice8	*_d3d_device;
	IDirect3DSurface8	*_d3d_backbuffer;
	IDirect3DSurface8	*_d3d_depthbuffer;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void		CreateD3D();
	void		DestroyD3D();
	void		FillVertexBuffer(VBILVect3d &vb, imd2_mesh_t *mesh, int anim);
	VBVect3d	_vb_grid;
	void		CreateGrid(int size, float increment);
	void		SetCameraAngle(float offset_x, float offset_y);	
	void		RotateAt(float angle, D3DXVECTOR3 *axis);
	void		UpdateCamera();
	void		AddVectorToCamera(D3DXVECTOR3 *vec);
	bool		_capture_mouvement;
	float		_angle_x;
	float		_angle_y;
	float		_angle_z;
	bool		_capture_movement;
	float		_zoom;
	D3DXVECTOR3	_position;
	D3DXVECTOR3	_up;
	D3DXVECTOR3	_at;
	// mouse camera
	D3DXVECTOR3				_direction;
	D3DXVECTOR3				_inv_direction;
	D3DXVECTOR3				_strafe_left;
	D3DXVECTOR3				_inv_strafe_left;
	D3DPRESENT_PARAMETERS	_d3d_pp;
	CRect					_old_client_rect;
	void	GoUp()
	{
		AddVectorToCamera(&_direction);
	}

	void	GoDown()
	{
		AddVectorToCamera(&_inv_direction);
	}

	void	StrafeLeft()
	{
		AddVectorToCamera(&_strafe_left);
	}

	void	StrafeRight()
	{
		AddVectorToCamera(&_inv_strafe_left);
	}
	imd2_object_t			*_object;
	std::vector<VBILVect3d>	_vb;
	std::vector<Mesh>	_skin_mesh;
	std::vector<D3DXMATRIX>	_matrix;

	std::vector<int>		_textures_list;
	std::vector<IDirect3DTexture8*>		_textures;
	std::string							_path;
	std::map<int, IDirect3DTexture8* >	_textures_map;
	//////////////////////////////////////////////////////////////////////////
	// animation)
	void	FillTagBuffer(VBVect3d &vb, int anim);
	int	_current_anim;
	void	UpdateFrame();
	// Bones.
	imd2_bone_file_t	*_bone;
	std::vector<Bone>	_bone_hier;
	VBVect3d			_vb_bone;
	VBVect3d			_vb_tag;
	void	RecursiveSetBone(Bone *bone, int parent_index);
    vertex3d_t *RecursiveFillVertex(std::vector<Bone> &bones, vertex3d_t *v, Bone *parent_bone);
	void	FillBoneVertexBuffer();
	bool	_use_bone_for_animation;
	void	SkinnedAnimation();

public:
	void	SetBoneAnimation(bool bone_animation);
	void	SetBoneFile(imd2_bone_file_t *bone);
	virtual void OnInitialUpdate();
	void	DestroyMaterial();
	void	LoadMaterial(imd2_object_t *imd2_object, std::string &path);
	void	SetImd2Object(imd2_object_t *imd2_object, std::string &path);
	void	ResetDevice();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in ImdViewerView.cpp
inline CImdViewerDoc* CImdViewerView::GetDocument() const
   { return reinterpret_cast<CImdViewerDoc*>(m_pDocument); }
#endif


