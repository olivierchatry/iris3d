// ImdViewerDoc.cpp : implementation of the CImdViewerDoc class
//

#include "stdafx.h"
#include "ImdViewer.h"

#include "MainFrm.h"
#include "ImdViewerDoc.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImdViewerDoc

IMPLEMENT_DYNCREATE(CImdViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CImdViewerDoc, CDocument)
END_MESSAGE_MAP()


// CImdViewerDoc construction/destruction

CImdViewerDoc::CImdViewerDoc()
{
	_imd_object = 0;
	_imd_material_count = 0;
	_imd_material = 0;
	_grid_ogl_list = 0;
	_current_anim = 0;
	_imd_skin_vertex = 0;
}

CImdViewerDoc::~CImdViewerDoc()
{
}



// CImdViewerDoc serialization

void CImdViewerDoc::Serialize(CArchive& ar)
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


// CImdViewerDoc diagnostics

#ifdef _DEBUG
void CImdViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImdViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImdViewerDoc commands
void	CImdViewerDoc::LoadMaterial(const std::string &path)
{
	size_t count = _imd_object->imd2_object_header.num_material;
	_imd_material = new GLuint[count];
	for (size_t i = 0; i < count; ++i)
	{
		imd2_material_t *m = _imd_object->imd2_material + i;
		std::string		file_name(path);
		file_name += "\\";
		file_name += m->file_name;
		_imd_material[i] = ilutGLLoadImage((ILstring) file_name.c_str());
		glBindTexture(GL_TEXTURE_2D, _imd_material[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		// change all id in imd file.
		for (size_t sub = 0; sub < _imd_object->imd2_object_header.num_mesh; ++sub)
		{
			imd2_mesh_t	*mesh = _imd_object->imd2_mesh + sub;
			if (mesh->imd2_mesh_header.material_id == m->material_id)
				mesh->imd2_mesh_header.material_id = _imd_material[i];
		}
	}
}

void CImdViewerDoc::GenerateGrid(float size, float inc)
{
	float	middle = size / 2.0f;
	float	x;
	float	y;
	float	inc_middle;

	inc_middle = inc /2.0f;
	_grid_ogl_list = glGenLists(1);
	glNewList(_grid_ogl_list, GL_COMPILE_AND_EXECUTE);
	for (x = -middle;x < middle; x += 1.0f)
		for (y = -middle;y < middle; y += 1.0f)
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f((x * inc) - inc_middle, 0.0f, (y * inc) - inc_middle);
			glVertex3f((x * inc) - inc_middle, 0.0f, (y * inc) + inc_middle);
			glVertex3f((x * inc) + inc_middle, 0.0f, (y * inc) + inc_middle);
			glVertex3f((x * inc) + inc_middle, 0.0f, (y * inc) - inc_middle);
			glVertex3f((x * inc) - inc_middle, 0.0f, (y * inc) - inc_middle);
			glEnd();
		}
	glEndList();
}

size_t CImdViewerDoc::Animate()
{
	if (_imd_object == 0)
		return 0;
	size_t	max_anim = _imd_bone == 0 ? _imd_object->imd2_object_header.num_anim : _imd_bone->imd2_bone_file_header.anim_count;
	_current_anim ++;
	if (_current_anim >= max_anim)
		_current_anim = 0;
	return _current_anim;
}

BOOL CImdViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	Destroy();
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	GenerateGrid(40.0f, 10.0f);	
	std::string	file(lpszPathName);
	std::string	path(file.substr(0, file.find_last_of("\\/")));
	std::string bone_file_name(file.substr(0, file.find_last_of(".")));
	_path = path;
	_current_anim = 0;
	bone_file_name += ".imdbone";	
	_imd_object = load_imd2(lpszPathName);
	if (_imd_object == 0)
		return FALSE;
	// create skin buffer if needed.
	int	max_size = 0;
	if (_imd_object->imd2_object_header.have_skin)
	{
		for (int i = 0; i < _imd_object->imd2_object_header.num_mesh; ++i)
		{
			imd2_mesh_t	*m = _imd_object->imd2_mesh + i;
			if (m->imd2_mesh_header.have_skin)
				max_size = max(max_size, m->imd2_mesh_header.num_vertex);
		}
		_imd_skin_vertex = new imd2_vertex_t[max_size];
	 }
	LoadMaterial(path);
	CMainFrame *pMainFrame = (CMainFrame*) theApp.GetMainWnd();
	if (pMainFrame == 0)
		return TRUE;
	FILE *fd = fopen(bone_file_name.c_str(), "rb");
	if (fd)
	{
		fclose(fd);
		pMainFrame->GetBonesListBar().Insert(bone_file_name.c_str());		
	}
	pMainFrame->SetAnimation(0, _imd_object->imd2_object_header.num_anim - 1);
	return TRUE;
}


void CImdViewerDoc::SetImdBone(imd2_bone_file_t *bone)
{
	_imd_bone = bone;
	CMainFrame *pMainFrame = (CMainFrame*) theApp.GetMainWnd();
	ASSERT(pMainFrame!=0);
	if (bone)
		pMainFrame->SetAnimation(0, bone->imd2_bone_file_header.anim_count - 1);
	else
		if (_imd_object)
			pMainFrame->SetAnimation(0, _imd_object->imd2_object_header.num_anim - 1);
		else
			pMainFrame->SetAnimation(0, 0);
}

void CImdViewerDoc::Destroy()
{
	CMainFrame	*pMainFrame = (CMainFrame*) GetRoutingFrame();
	if (pMainFrame)
		pMainFrame->GetBonesListBar().Clear();
	if (_imd_object)
		free_imd2(_imd_object);
	if (_imd_skin_vertex)
		delete _imd_skin_vertex;
	_imd_skin_vertex = 0;
	_imd_object = 0;
	_imd_bone	= 0;
	if (_imd_material_count)
	{
		glDeleteTextures(_imd_material_count, _imd_material);
		delete _imd_material;
	}
	_imd_material = 0;
	_imd_material_count = 0;
	glDeleteLists(_grid_ogl_list, 1);
	_grid_ogl_list = 0;
	for (size_t i = 0; i < theApp._plugins.size(); ++i)
	{
		Plugins	*plugin = theApp._plugins[i];
		plugin->_plugins->PluginDestroy();
	}
}

void CImdViewerDoc::OnCloseDocument()
{
	Destroy();
	CDocument::OnCloseDocument();
}
