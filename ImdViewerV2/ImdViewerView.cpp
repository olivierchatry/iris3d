// ImdViewerView.cpp : implementation of the CImdViewerView class
//

#include "stdafx.h"
#include "ImdViewer.h"

#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImdViewerView

IMPLEMENT_DYNCREATE(CImdViewerView, COpenGLWnd)

BEGIN_MESSAGE_MAP(CImdViewerView, COpenGLWnd)
	// Standard printing commands
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CImdViewerView construction/destruction

CImdViewerView::CImdViewerView() :	_position(0.0f, 0.0f, -2.0f), 
									_at(0.0f, 0.0f, 0.0f), 
									_up(0.0f, 1.0f, 0.0f)
{
	// TODO: add construction code here
	_alpha = 0.0f;
	_beta = 0.0f;

}

CImdViewerView::~CImdViewerView()
{
}


void CImdViewerView::RotateAt(float angle, vect3d *axis)
{
	vect3d	tmp;
	vect3d	direction;

	direction = _at - _position;
	float c = cos(angle);
	float s = sin(angle);
	tmp.x =	 (c + ( 1 - c) * axis->x) * direction.x;
	tmp.x += ((1 - c) * axis->x * axis->y - axis->z * s) * direction.y;
	tmp.x += ((1 - c) * axis->x * axis->z + axis->y * s) * direction.z;
	tmp.y =  ((1 - c) * axis->x * axis->y + axis->z * s) * direction.x;
	tmp.y += (c + (1 - c) * axis->y) * direction.y;
	tmp.y += ((1 - c) * axis->y * axis->z - axis->x * s) * direction.z;
	tmp.z  = ((1 - c) * axis->x * axis->z - axis->y * s) * direction.x;
	tmp.z += ((1 - c) * axis->y * axis->z + axis->x * s) * direction.y;
	tmp.z += (c + (1 - c) * axis->z)		* direction.z;
	_at = _position + tmp;
}


void CImdViewerView::UpdateCamera()
{	
	_direction =  _at - _position;
	_direction.Normalize();
	_inv_strafe_left = _up.Cross(_direction);
	_inv_strafe_left.Normalize();
	RotateAt(_beta, &_inv_strafe_left);
	RotateAt(_alpha,  &_up);
	_inv_direction = -_direction;
	_strafe_left = -_inv_strafe_left;
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('w'))
	{
		_position += _direction;
		_at += _direction;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('s'))
	{
		_position -= _direction;
		_at -= _direction;
	}
	if (GetAsyncKeyState(VK_LEFT)  || GetAsyncKeyState('a'))
	{
		_position -= _strafe_left;
		_at -= _strafe_left;
	}
	if (GetAsyncKeyState(VK_RIGHT)  || GetAsyncKeyState('d'))
	{
		_position -= _inv_strafe_left;
		_at -= _inv_strafe_left;
	}
	_alpha = _beta = 0;
	gluLookAt(	_position._x, _position._y, _position._z,
		_at._x, _at._y, _at._z,
		_up._x, _up._y, _up._z);
}

BOOL CImdViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return COpenGLWnd::PreCreateWindow(cs);
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

void CImdViewerView::RenderMesh(imd2_mesh_t *mesh, size_t current_anim, imd2_vertex_t *buffer)
{
	size_t anim_offset = current_anim * mesh->imd2_mesh_header.num_vertex;
	if (buffer == 0)
		buffer = mesh->imd2_vertex;
	glBindTexture(GL_TEXTURE_2D, mesh->imd2_mesh_header.material_id);
	for (int i = 0; i < mesh->imd2_face.num_section; ++i)
	{
		imd2_face_section_t	*section = mesh->imd2_face.imd2_section + i;
		glBegin(GL_TRIANGLE_STRIP);
		for (int s = 0; s < section->num_indice; ++s)
		{
			size_t offset = *(section->indice + s) + anim_offset;
			imd2_vertex_t	*mapping = mesh->imd2_vertex + offset;
			imd2_vertex_t	*v = buffer + offset;
			glColor3f(	(mapping->color >> 16& 0xff) / 255.0f, 
						((mapping->color >> 8) & 0xff)  / 255.0f, 
						((mapping->color ) & 0xff)  / 255.0f) ;
			glNormal3fv(v->normal);
			glTexCoord2f(mapping->uv[0], 1.0f - mapping->uv[1]);
			glVertex3fv(v->pos);
		}
		glEnd();
	}

}

float	*MultiplyMatrixVector3D(float *pos, float *matrix, float *res)
{
	res[0] = pos[0] * matrix[0] + pos[1] * matrix[4] + pos[2] * matrix[8] + 1.0f * matrix[12];
	res[1] = pos[0] * matrix[1] + pos[1] * matrix[5] + pos[2] * matrix[9] + 1.0f * matrix[13];
	res[2] = pos[0] * matrix[2] + pos[1] * matrix[6] + pos[2] * matrix[10] +  + 1.0f * matrix[14];
	return res;
}


void CImdViewerView::RenderSkinnedMesh(imd2_mesh_t *mesh, size_t current_anim, imd2_bone_file_t *bones)
{
	CImdViewerDoc	*pDoc = (CImdViewerDoc*) GetDocument();
	ASSERT(pDoc != 0);
	imd2_vertex_t	*skin_vertex = pDoc->GetSkinBuffer();
	size_t	count = mesh->imd2_mesh_header.num_vertex;
	memset(skin_vertex, 0, sizeof(imd2_vertex_t) * count);
	while (count --)
	{
		imd2_skin_t	*skin = mesh->imd2_skin + count;
		int	assigned = skin->num_bones_assigned;
		while (assigned --)
		{
			imd2_weight_t	*weight = skin->weight + assigned;
			// !!! SUPRA CRADE !!!
			for (int b = 0; b < bones->imd2_bone_file_header.bone_count; ++b)
				if (bones->bones[b].imd2_bone_header.bone_index == weight->bone_index)
				{
					imd2_bone_anim_t	*bone = bones->bones[b].imd2_bone_anim + current_anim;
					float	trans[3];
					MultiplyMatrixVector3D(mesh->imd2_vertex[count].pos, bone->matrix, trans);
					skin_vertex[count].pos[0] += trans[0] * weight->weight;
					skin_vertex[count].pos[1] += trans[1] * weight->weight;
					skin_vertex[count].pos[2] += trans[2] * weight->weight;
					break;
				}
		}
	}
	RenderMesh(mesh, 0, skin_vertex);
}

void CImdViewerView::Render()
{
	CImdViewerDoc	*pDoc = (CImdViewerDoc*) GetDocument();
	if (!pDoc)
		return;
	imd2_object_t		*object = pDoc->GetImdObject();
	imd2_bone_file_t	*bone = pDoc->GetImdBone();
	GLuint			*material = pDoc->GetImdMaterial();
	size_t			current_anim = pDoc->GetCurrentAnim();
	size_t			current_anim_plugin = current_anim;
	glLoadIdentity();
	UpdateCamera();
	SetContext();
	if (object)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 1.0f);
		glCallList(pDoc->GetOGLGrid());
		glPointSize(100.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		if (bone)
			current_anim = 0;
		for (int i = 0; i < object->imd2_object_header.num_tag; ++i)
			glVertex3fv(object->imd2_tag[i].tag_data[current_anim].pos);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		// glEnable(GL_LIGHTING);
		// Render all mesh.	
		for (int i = 0; i < object->imd2_object_header.num_mesh; ++i)
		{
			if (bone)
				RenderSkinnedMesh(object->imd2_mesh + i, current_anim_plugin, bone);
			else
				RenderMesh(object->imd2_mesh + i, current_anim);
		}
		glDisable(GL_TEXTURE_2D);
		// render plugins.
		for (size_t i = 0; i < theApp._plugins.size(); ++i)
		{
			Plugins	*plugin = theApp._plugins[i];
			if (plugin->_info->display_data)
				plugin->_plugins->PluginRender((unsigned int) current_anim_plugin, pDoc->GetImdBone());
		}
	}
	SwapGLBuffers();
}


// CImdViewerView diagnostics

#ifdef _DEBUG
void CImdViewerView::AssertValid() const
{
	COpenGLWnd::AssertValid();
}

void CImdViewerView::Dump(CDumpContext& dc) const
{
	COpenGLWnd::Dump(dc);
}

CImdViewerDoc* CImdViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImdViewerDoc)));
	return (CImdViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CImdViewerView message handlers

void CImdViewerView::OnSize(UINT nType, int cx, int cy)
{
	COpenGLWnd::OnSize(nType, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport( 0, 0, cx, cy );
	gluPerspective(45.0f, (float)(cx)/(float)(cy), 1.0f, 1000.0f);	
	glMatrixMode(GL_MODELVIEW);

}

void CImdViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint	old_point;
	static bool	init = false;
	if (init && nFlags & MK_LBUTTON)
	{
		CPoint	diff = point - old_point;
		_alpha += diff.x / 200.0f;
		_beta  += diff.y / 200.0f;
	}
	init = true;
	old_point = point;
	COpenGLWnd::OnMouseMove(nFlags, point);
}

