// ImdViewerDoc.h : interface of the CImdViewerDoc class
//


#pragma once

class CImdViewerDoc : public CDocument
{
protected: // create from serialization only
	CImdViewerDoc();
	DECLARE_DYNCREATE(CImdViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CImdViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif	
	GLuint	*GetImdMaterial()
	{
		return _imd_material;
	}

	std::string &GetImdPath()
	{
		return _path;
	}
	imd2_object_t	*GetImdObject()
	{
		return _imd_object;
	}
	GLuint			GetOGLGrid()
	{
		return _grid_ogl_list;
	}
	size_t				GetCurrentAnim()
	{
		return _current_anim;
	}
	void				SetCurrentAnim(size_t current_anim)
	{
		_current_anim = current_anim;
	}
	size_t				Animate();
	
	imd2_vertex_t	*GetSkinBuffer()
	{
		return _imd_skin_vertex;
	}

	void	SetImdBone(imd2_bone_file_t *bone);
	
	imd2_bone_file_t *GetImdBone()
	{
		return _imd_bone;
	}

protected:
	void				Destroy();
	imd2_object_t		*_imd_object;
	imd2_bone_file_t	*_imd_bone;
	imd2_vertex_t		*_imd_skin_vertex;
	//
	GLuint				*_imd_material;
	GLuint				_grid_ogl_list;
	int					_imd_material_count;
	void				LoadMaterial(const std::string &path);
	void				GenerateGrid(float size, float inc);
	size_t				_current_anim;
	std::string			_path;
	std::string			_file_name;
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};


