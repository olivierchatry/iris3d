// ImdViewerView.cpp : implementation of the CImdViewerView class
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#include "DialogBarBoneList.h"
#include "DialogBarMeshInfo.h"
#include "DialogBarBoneTree.h"
#include "DialogBarAnim.h"
#include "DialogBarConfig.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int	grid_size = 80;
const float grid_increment = 10.0f;
const int	grid_primitive_count = grid_size * grid_size * 4;
// CImdViewerView

IMPLEMENT_DYNCREATE(CImdViewerView, CView)

BEGIN_MESSAGE_MAP(CImdViewerView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CImdViewerView construction/destruction

CImdViewerView::CImdViewerView() : _capture_mouvement(false), _position(0.0f, -2.0f, 0.0f), 
_at(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _d3d_device(0),
_object(0), _d3d_backbuffer(0), _d3d_depthbuffer(0), _d3d_object(0), _current_anim(0)
,_bone(0), _use_bone_for_animation(false)
{
	// TODO: add construction code here

}

CImdViewerView::~CImdViewerView()
{
	DestroyD3D();
}

BOOL CImdViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CImdViewerView drawing

void CImdViewerView::OnDraw(CDC* /*pDC*/)
{
	CImdViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CImdViewerView diagnostics

#ifdef _DEBUG
void CImdViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CImdViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImdViewerDoc* CImdViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImdViewerDoc)));
	return (CImdViewerDoc*)m_pDocument;
}
#endif //_DEBUG


void CImdViewerView::CreateGrid(int size, float increment)
{
	float	middle = (float) size / 2.0f;
	float	inc_middle = increment / 2.0f;
	_vb_grid.Allocate(_d3d_device, size * size * 8, D3DPT_LINELIST);
	vertex3d_t	*v = _vb_grid.Lock();
	for (float x = -middle; x < middle; ++x)
	{
		for (float y = - middle; y < middle; ++y)
		{
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) - inc_middle, 0.0f);v++;
		}	
	}
	_vb_grid.Unlock();
}

// CImdViewerView message handlers
void CImdViewerView::CreateD3D()
{
	_d3d_object = Direct3DCreate8(D3D_SDK_VERSION);
	ASSERT_VIEWER(_d3d_object != 0, "!!! Cannot create d3d object");
	D3DCAPS8	dev_caps;
	HRESULT		hres;		
	// look for hardware card, if no exit.
	hres = _d3d_object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &dev_caps);
	ASSERT_VIEWER(SUCCEEDED(hres), "No 3d hardware");
	memset(&_d3d_pp, 0, sizeof(_d3d_pp));
	GetClientRect(_old_client_rect);
	_d3d_pp.BackBufferWidth = _old_client_rect.Width();
	_d3d_pp.BackBufferHeight = _old_client_rect.Height();
	_d3d_pp.BackBufferCount = 1;
	// no faa
	_d3d_pp.MultiSampleType = D3DMULTISAMPLE_NONE; // no aa
	HWND	hwnd = this->GetSafeHwnd();
	_d3d_pp.hDeviceWindow = hwnd;
	_d3d_pp.Windowed = true;
	// stencil for shadow
	_d3d_pp.EnableAutoDepthStencil = true;
	_d3d_pp.AutoDepthStencilFormat = D3DFMT_D16;
	_d3d_pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	_d3d_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3d_pp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	// FIX ME : don't like voodoo card so ... don't really need this
	ASSERT_VIEWER((dev_caps.Caps2 & D3DCAPS2_CANRENDERWINDOWED), "cannot render on windows");
	D3DDISPLAYMODE current_displaye_mode;
	hres = _d3d_object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &current_displaye_mode);
	ASSERT_VIEWER(SUCCEEDED(hres), "get display mode");
	_d3d_pp.BackBufferFormat = current_displaye_mode.Format;
	// look for hardware 	vertex proecesind
	int flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	if (dev_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		flags = (dev_caps.DevCaps & D3DDEVCAPS_PUREDEVICE) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING;
	hres = _d3d_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, flags | D3DCREATE_MULTITHREADED, &_d3d_pp, &_d3d_device);
	ASSERT_VIEWER(SUCCEEDED(hres), "device creation");
	hres = _d3d_device->GetRenderTarget(&_d3d_backbuffer);
	ASSERT_VIEWER(SUCCEEDED(hres), "set backbuffer");
	hres = _d3d_device->GetDepthStencilSurface(&_d3d_depthbuffer);
	ASSERT_VIEWER(SUCCEEDED(hres), "set stencil buffer");	
	D3DXMATRIX	projection_matrix;
	D3DXMatrixPerspectiveFovLH(&projection_matrix, D3DX_PI/3, 
		(float) _old_client_rect.Width() / (float) _old_client_rect.Height(), 1.0f, 1000.0f);
	_d3d_device->SetTransform(D3DTS_PROJECTION, &projection_matrix);
	D3DMATERIAL8	material;
	material.Ambient.a = 0;
	material.Ambient.r = material.Ambient.g = material.Ambient.b = 0.0f;
	material.Diffuse.r = material.Diffuse.g = material.Diffuse.b = 0.1f;
	material.Specular.r = material.Specular.g = material.Specular.b = 0.1f;
	material.Power = 0.0f;
	_d3d_device->SetMaterial(&material);
	_d3d_device->SetRenderState(D3DRS_LIGHTING, 0);
	_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CImdViewerView::OnInitialUpdate()
{
	ResetDevice();
	CView::OnInitialUpdate();
}

void CImdViewerView::UpdateScene()
{
	CMainFrame *pMainFrm = (CMainFrame *) AfxGetApp()->GetMainWnd();
	ASSERT(pMainFrm != 0);
	_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0xFF,0,0xFF), 1, 0);
	UpdateCamera();
	UpdateFrame();
	_d3d_device->SetTexture(0, 0);
	_d3d_device->BeginScene();
	_d3d_device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	_d3d_device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	D3DXMATRIX world_object;
	D3DXMatrixIdentity(&world_object);
	_d3d_device->SetTransform(D3DTS_WORLD, &world_object);
	_vb_grid.Draw(_d3d_device, grid_primitive_count);
	if (_bone)
		_vb_bone.Draw(_d3d_device, _bone->imd2_bone_file_header.bone_count);
	if (_object)
	{
		if (pMainFrm->GetConfigBar().DisplayTag() && _object->imd2_object_header.num_tag)
		{
			_d3d_device->SetTexture(0, 0);
			DWORD value = TRUE;
			_d3d_device->SetRenderState(D3DRS_POINTSCALEENABLE, value);
			float	size = 10.0f;
			_d3d_device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&size));
			_vb_tag.Draw(_d3d_device, _object->imd2_object_header.num_tag);
		}
		for (size_t i = 0; i < _vb.size(); ++i)
		{
			IDirect3DTexture8	*tex = _textures_map[_textures_list[i]];
			HRESULT hres =  _d3d_device->SetTexture(0, tex);
			if (_object->imd2_object_header.matrix_sampling)
				if (!_use_bone_for_animation)
					memcpy((float *) world_object, _object->imd2_mesh[i].imd2_matrix[_current_anim].m, sizeof(float) * 16);
			_d3d_device->SetTransform(D3DTS_WORLD, &world_object);
			_vb[i].DrawIndexed(_d3d_device, D3DPT_TRIANGLESTRIP);
		}
	}
	_d3d_device->EndScene();
	_d3d_device->Present(NULL, NULL, NULL, NULL);		
}

void CImdViewerView::SetCameraAngle(float offset_y, float offset_z)
{
	_angle_y = offset_y;
	_angle_z = offset_z;
}

void CImdViewerView::RotateAt(float angle, D3DXVECTOR3 *axis)
{
	D3DXVECTOR3	tmp;
	D3DXVECTOR3 direction;

	D3DXVec3Subtract(&direction, &_at, &_position);
	float c = cosf(angle);
	float s = sinf(angle);

	tmp.x =	 (c + ( 1 - c) * axis->x) * direction.x;
	tmp.x += ((1 - c) * axis->x * axis->y - axis->z * s) * direction.y;
	tmp.x += ((1 - c) * axis->x * axis->z + axis->y * s) * direction.z;

	tmp.y =  ((1 - c) * axis->x * axis->y + axis->z * s) * direction.x;
	tmp.y += (c + (1 - c) * axis->y) * direction.y;
	tmp.y += ((1 - c) * axis->y * axis->z - axis->x * s) * direction.z;

	tmp.z  = ((1 - c) * axis->x * axis->z - axis->y * s) * direction.x;
	tmp.z += ((1 - c) * axis->y * axis->z + axis->x * s) * direction.y;
	tmp.z += (c + (1 - c) * axis->z)		* direction.z;
	D3DXVec3Add(&_at, &_position, &tmp);
}

void CImdViewerView::UpdateCamera()
{
	D3DXVECTOR3	axis, sub;
	D3DXVec3Subtract(&_direction, &_at, &_position);
	D3DXVec3Cross(&_inv_strafe_left, &_up, &_direction);
	D3DXVec3Normalize(&_inv_strafe_left, &_inv_strafe_left);		
	RotateAt(_angle_z, &_inv_strafe_left);
	RotateAt(_angle_y, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&_direction, &_direction);
	_inv_direction = -_direction;
	_strafe_left = -_inv_strafe_left;
	D3DXMATRIX	view_matrix;
	D3DXMatrixLookAtLH(&view_matrix,
		&_position,
		&_at, 
		&_up);
	_angle_z = _angle_y = 0;
	_d3d_device->SetTransform(D3DTS_VIEW, &view_matrix);
}

void	CImdViewerView::AddVectorToCamera(D3DXVECTOR3 *vec)
{
	D3DXVec3Add(&_position, &_position, vec);
	D3DXVec3Add(&_at, &_at, vec);
}

void CImdViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (_capture_mouvement)
	{
		POINT new_pos;
		CRect	rect, rect_wnd;
		this->GetWindowRect(rect_wnd);
		GetClientRect(rect);
		int	wnd_middle_x = rect_wnd.left + (rect.right - rect.left) / 2;
		int	wnd_middle_y = rect_wnd.top + (rect.bottom - rect.top) / 2;
		GetCursorPos(&new_pos);
		if( (new_pos.x == wnd_middle_x) && (new_pos.y == wnd_middle_y))
			return;
		int offset_y = wnd_middle_x - new_pos.x;
		int offset_z = wnd_middle_y - new_pos.y;
		SetCursorPos(wnd_middle_x, wnd_middle_y);
		if (offset_y != 0 || offset_z != 0)
			SetCameraAngle(offset_y / 1000.0f, offset_z / 1000.0f);
	}
	CView::OnMouseMove(nFlags, point);
}

void CImdViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_UP)
		GoUp();
	if(nChar == VK_DOWN)
		GoDown();
	if(nChar == VK_LEFT)
		StrafeLeft();
	if(nChar == VK_RIGHT)
		StrafeRight();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CImdViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	_capture_mouvement = !_capture_mouvement;
	if (_capture_mouvement)
		ShowCursor(FALSE);
	else
		ShowCursor(TRUE);
	CView::OnLButtonDown(nFlags, point);
}

void CImdViewerView::DestroyD3D()
{
	_vb_bone.Deallocate();
	_vb_grid.Deallocate();
	_vb_tag.Deallocate();
	for (size_t i = 0; i < _vb.size(); ++i)
		_vb[i].Deallocate();
	for (size_t i = 0; i < _skin_mesh.size(); ++i)
	{
		delete [] _skin_mesh[i].skin;
		delete [] _skin_mesh[i].v;
	}
	_skin_mesh.clear();

	_vb.clear();
	DestroyMaterial();
	if (_d3d_backbuffer) 
		_d3d_backbuffer->Release();
	if (_d3d_depthbuffer) 
		_d3d_depthbuffer->Release();
	if (_d3d_device) 
		_d3d_device->Release();
	if (_d3d_object) 
		_d3d_object->Release();
	_d3d_device = 0;
	_d3d_object = 0;
	_d3d_depthbuffer = 0;
	_d3d_backbuffer = 0;
}

void CImdViewerView::ResetDevice()
{
	DestroyD3D();
	CreateD3D();
	CreateGrid(grid_size, grid_increment);
	if (_object)
		SetImd2Object(_object, _path);
	if (_bone)
		SetBoneFile(_bone);
}

void CImdViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect	client_rect;
	
	//////////////////////////////////////////////////////////////////////////
	GetClientRect(client_rect);
	_d3d_pp.BackBufferWidth  = client_rect.right - client_rect.left;
	_d3d_pp.BackBufferHeight = client_rect.bottom - client_rect.top;
	if (_d3d_device && ( (client_rect.Width() != _old_client_rect.Width()) ||  (client_rect.Height() != _old_client_rect.Height()) ))
		ResetDevice();
}

void	CImdViewerView::FillVertexBuffer(VBILVect3d &vb, imd2_mesh_t *mesh, int anim)
{
	int			vertex_count = mesh->imd2_mesh_header.num_vertex;
	vertex3d_t	*vertices = vb.Lock();
	int			offset = anim * vertex_count;
	Mesh		*skin = 0;
	if (_object->imd2_object_header.have_skin && mesh->imd2_mesh_header.have_skin)
	{
		_skin_mesh.push_back(Mesh());
		skin = &(_skin_mesh.back());
		skin->num_vertex = vertex_count;
		skin->v = new vertex3d_t[vertex_count];
		skin->num_skin = 0;
		// count total skinned vertex.
		for (int i = 0; i < vertex_count; ++i)
			skin->num_skin += mesh->imd2_skin[i].num_bones_assigned;
		skin->skin = new Skin[skin->num_skin];
	}
	int skin_idx = 0;
	for (int v = 0; v < vertex_count; ++v)
	{
		imd2_vertex_t	*imd_v = &(mesh->imd2_vertex[v + offset]);
		vertices->_pos.x = imd_v->pos[0];
		vertices->_pos.y = imd_v->pos[1];
		vertices->_pos.z = imd_v->pos[2];
		vertices->_u = imd_v->uv[0];
		vertices->_v = imd_v->uv[1];
		vertices->_norm.x = imd_v->normal[0];
		vertices->_norm.y = imd_v->normal[1];
		vertices->_norm.z = imd_v->normal[2];
		vertices->_color = imd_v->color;
		if (skin)
		{
			skin->v[v] = *vertices;
			for (int i = 0; i < mesh->imd2_skin[v].num_bones_assigned; ++i)
			{
				skin->skin[skin_idx].bone_index = mesh->imd2_skin[v].weight[i].bone_index;
				skin->skin[skin_idx].vertex_index = v;
				skin->skin[skin_idx].weight = mesh->imd2_skin[v].weight[i].weight;
				skin_idx ++;
			}
		}
		vertices++;

	}
	vb.Unlock();
}

void	CImdViewerView::DestroyMaterial()
{
	for (size_t i = 0; i < _textures.size(); ++i)
		if (_textures[i])
			_textures[i]->Release();
	_textures.clear();
}

void	CImdViewerView::LoadMaterial(imd2_object_t *imd2_object, std::string &path)
{
	int	material_count = imd2_object->imd2_object_header.num_material;
	std::string	file_name;
	DestroyMaterial();
	_textures.resize(material_count);
	for (int material = 0; material < material_count; ++material)
	{
		imd2_material_t	*mat = &(imd2_object->imd2_material[material]);
		ILuint	il_id;
		if (strcmp(mat->file_name, "") == 0)
			continue;
		ilGenImages(1, &il_id);
		ilBindImage(il_id);
		_textures[material] = 0;
		file_name = path;
		file_name += "\\";
		file_name += mat->file_name;
		if (ilLoadImage((ILstring) file_name.c_str())  == IL_TRUE)
			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE)
			{
				ilutSetInteger(ILUT_D3D_POOL, D3DPOOL_MANAGED);
				_textures[material] = ilutD3D8Texture(_d3d_device);
			}
		ilDeleteImages(1, &il_id);
		_textures_map[mat->material_id] = _textures[material];
	}
}

void	CImdViewerView::FillTagBuffer(VBVect3d &vb, int anim)
{
	vertex3d_t	*v = _vb_tag.Lock();
	for (int i = 0; i < _object->imd2_object_header.num_tag; ++i)
	{
		imd2_tag_t *tag = &(_object->imd2_tag[i]);
		v->_color = 0xff00ff00;
		v->_pos.x = tag->tag_data[anim].pos[0];
		v->_pos.y = tag->tag_data[anim].pos[1];
		v->_pos.z = tag->tag_data[anim].pos[2];
		v++;
	}
	_vb_tag.Unlock();
}

void	CImdViewerView::SetImd2Object(imd2_object_t *imd2_object, std::string &path)
{
	_path = path;
	_object = imd2_object;
	int	mesh_count = _object->imd2_object_header.num_mesh;
	_current_anim = 0;
	_vb.resize(mesh_count);
	_textures_list.resize(mesh_count);
	for (int i = 0; i < mesh_count; ++i)
	{
		imd2_mesh_t	*mesh = &(_object->imd2_mesh[i]);
		
		int	vertex_count = mesh->imd2_mesh_header.num_vertex;
		_vb[i].Allocate(_d3d_device, vertex_count, D3DPT_TRIANGLESTRIP);
		FillVertexBuffer(_vb[i], mesh, _current_anim);
		int	strip_count = mesh->imd2_face.num_section;	
		_textures_list[i] = mesh->imd2_mesh_header.material_id;
		_vb[i].PrealocateIBList(strip_count);
		for (int s = 0; s < strip_count; ++s)
		{
			imd2_face_section_t *strip = &(mesh->imd2_face.imd2_section[s]);
			int	num_indices = strip->num_indice;
			_vb[i].CreateIB(_d3d_device, s, num_indices, num_indices - 2);
			WORD *ib = _vb[i]._ib[s].Lock();
			for (int f = 0; f < num_indices; ++f)
			{
				*ib = strip->indice[f];
				ib++;
			}
			_vb[i]._ib[s].Unlock();
		}
	}
	LoadMaterial(imd2_object, path);
	SetBoneAnimation(_use_bone_for_animation);
	// tag
	int	num_tag = _object->imd2_object_header.num_tag;
	if (num_tag > 0)
	{
		_vb_tag.Allocate(_d3d_device, num_tag, D3DPT_POINTLIST);
		FillTagBuffer(_vb_tag, _current_anim);
	}
}

void	CImdViewerView::RecursiveSetBone(Bone *bone, int parent_index)
{
	for (int i = 0; i < _bone->imd2_bone_file_header.bone_count; ++i)
	{
		if (_bone->bones[i].imd2_bone_header.bone_parent == parent_index)
		{
			Bone	bone_add;
			bone_add.bone = &(_bone->bones[i]);
			bone_add.bone_index = i;
			RecursiveSetBone(&bone_add, _bone->bones[i].imd2_bone_header.bone_index);
			if (parent_index != -1)
				bone->child.push_back(bone_add);
			else
				_bone_hier.push_back(bone_add);
		}
	}
}

void	CImdViewerView::SetBoneFile(imd2_bone_file_t *bone)
{
	_bone = bone;
	_bone_hier.clear();
	int	num_bone = bone->imd2_bone_file_header.bone_count;
	_matrix.resize(num_bone);
	RecursiveSetBone(0, -1);
	_vb_bone.Allocate(_d3d_device, num_bone * 2, D3DPT_LINELIST);
	FillBoneVertexBuffer();
	// Animation bar ...
	SetBoneAnimation(_use_bone_for_animation);
}


vertex3d_t *CImdViewerView::RecursiveFillVertex(std::vector<Bone> &bones, vertex3d_t *v, Bone *parent_bone)
{
	for (size_t i = 0; i < bones.size(); ++i)
	{		
		imd2_bone_anim_t	*bone_anim = &(bones[i].bone->imd2_bone_anim[_current_anim]);
		if (parent_bone)
		{
			float				*pos = bone_anim->pos;
			D3DXMATRIX			matrix_anim(bone_anim->matrix);
			float				*parent_pos = parent_bone->bone->imd2_bone_anim[_current_anim].pos;
			
			// D3DXMatrixMultiply(&(_matrix[bones[i].bone_index]), &(_matrix[parent_bone->bone_index]), &matrix_anim);
			_matrix[bones[i].bone_index] = bone_anim->matrix;
			v->_pos.x =  parent_pos[0];
			v->_pos.y =  parent_pos[1];
			v->_pos.z =  parent_pos[2];
			v->_color = 0xff0000ff;
			v ++;
			v->_pos.x = pos[0];
			v->_pos.y = pos[1];
			v->_pos.z = pos[2];
			v->_color = 0xff0000ff;
			v ++;
		}
		else
			_matrix[bones[i].bone_index] = bone_anim->matrix;
		v = RecursiveFillVertex(bones[i].child, v, &(bones[i]));
		if (_object)
		{
			D3DXMATRIX	object_matrix(_object->imd2_mesh[0].imd2_matrix[0].m);
			D3DXMatrixMultiply(&(_matrix[bones[i].bone_index]), 
								&(_matrix[bones[i].bone_index]), 
								&object_matrix);
		}
	}
	return v;
}
								
void	CImdViewerView::FillBoneVertexBuffer()
{
	vertex3d_t	*v = _vb_bone.Lock();
	RecursiveFillVertex(_bone_hier, v, 0);
	_vb_bone.Unlock();
}

void	CImdViewerView::SetBoneAnimation(bool bone_animation)
{
	_use_bone_for_animation = bone_animation;
	CMainFrame *pMainFrm = (CMainFrame *) theApp.GetMainWnd();
	ASSERT(pMainFrm != 0);
	pMainFrm->GetAnimBar().SetNumAnim(0);
	if (_use_bone_for_animation)
	{
		if (_bone)
			pMainFrm->GetAnimBar().SetNumAnim(_bone->imd2_bone_file_header.anim_count);
	}
	else
		if (_object)
			pMainFrm->GetAnimBar().SetNumAnim(_object->imd2_object_header.num_anim);
}

void	CImdViewerView::SkinnedAnimation()
{
	for (size_t i = 0; i < _skin_mesh.size(); ++i)
	{
		Mesh &mesh = _skin_mesh[i];
		vertex3d_t	*dest = _vb[i].Lock();

		// reset all vertices
		vertex3d_t	*tmp = dest;
		int	n = mesh.num_vertex;
		while (n--)
		{
			tmp->_pos = D3DXVECTOR3(0, 0, 0);
			tmp++;
		}
		D3DXMATRIX	mat;
		// for each vertex
		for (int s = 0; s < mesh.num_skin; ++s)
		{
			Skin &skin = mesh.skin[s];
			imd2_bone_anim_t	*bone = &(_bone->bones[skin.bone_index].imd2_bone_anim[_current_anim]);
			int	vi = skin.vertex_index;
			D3DXVECTOR3	bone_pos(bone->pos);
			D3DXVECTOR3	pos(mesh.v[vi]._pos);
			D3DXVECTOR3 pre;
			D3DXVECTOR3 trans;
			pre.x = pos.x - bone_pos.x;
			pre.y = pos.y - bone_pos.y;
			pre.z = pos.z - bone_pos.z;
			D3DXVec3TransformCoord(&trans, &pre, &_matrix[skin.bone_index]);
			dest[vi]._pos.x += (trans.x + bone_pos.x) * skin.weight;
			dest[vi]._pos.y += (trans.y + bone_pos.y) * skin.weight;
			dest[vi]._pos.z += (trans.z + bone_pos.z) * skin.weight;
/*			dest[vi]._pos.x += (trans.x) * skin.weight;
			dest[vi]._pos.y += (trans.y) * skin.weight;
			dest[vi]._pos.z += (trans.z) * skin.weight;*/
			continue;
		}
		_vb[i].Unlock();
	}
}


void	CImdViewerView::UpdateFrame()
{
	CMainFrame *pMainFrm = (CMainFrame *) theApp.GetMainWnd();
	ASSERT(pMainFrm != 0);
	int anim = pMainFrm->GetAnimBar().GetCurrentAnim();
	if (_current_anim != anim)
	{
		_current_anim = anim;
		if (_use_bone_for_animation == false)
		{
			if (_object && !(_object->imd2_object_header.matrix_sampling))
			{
				int	mesh_count = _object->imd2_object_header.num_mesh;
				for (int i = 0; i < mesh_count; ++i)
				{
					imd2_mesh_t	*mesh = &(_object->imd2_mesh[i]);
					FillVertexBuffer(_vb[i], mesh, _current_anim);
				}
				if (_object->imd2_object_header.num_tag)
					FillTagBuffer(_vb_tag, _current_anim);
			}
		}
		else
			if (_bone)
			{
				FillBoneVertexBuffer();
				SkinnedAnimation();
			}
	}
}

	