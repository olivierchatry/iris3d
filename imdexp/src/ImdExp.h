/********************************************************************
	created:	2003/02/03
	created:	3:2:2003   20:17
	filename: 	e:\_dev\tmp\imdexp\src\imdexp.h
	file path:	e:\_dev\tmp\imdexp\src
	file base:	imdexp
	file ext:	h
	author:		Chatry Olivier alias gruiiik
	
	purpose:	Main exporter function.
*********************************************************************/
#pragma once

extern ClassDesc* GetAsciiExpDesc();
extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;


#define IMDEXP_VERSION			100			// Version number * 100
//#define FLOAT_OUTPUT	_T("%4.4f")	// Float precision for output
#define CFGFILENAME		_T("IMDEXP.CFG")	// Configuration file

class ImdExp: public SceneExport 
{
public:
	ImdExp();
	~ImdExp();

	// SceneExport methods
	int    ExtCount();     // Number of extensions supported 
	const TCHAR * Ext(int n);     // Extension #n (i.e. "ASC")
	const TCHAR * LongDesc();     // Long ASCII description (i.e. "Ascii Export") 
	const TCHAR * ShortDesc();    // Short ASCII description (i.e. "Ascii")
	const TCHAR * AuthorName();    // ASCII Author name
	const TCHAR * CopyrightMessage();   // ASCII Copyright message 
	const TCHAR * OtherMessage1();   // Other message #1
	const TCHAR * OtherMessage2();   // Other message #2
	unsigned int Version();     // Version number * 100 (i.e. v3.01 = 301) 
	void		ShowAbout(HWND hWnd);  // Show DLL's "About..." box
	int			DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0); // Export	file
	BOOL		SupportsOptions(int ext, DWORD options);
	Interface*	GetInterface();
	void		PopulateTreeView(HWND hwnd, INode* node = 0, HTREEITEM parent = 0);
	void		SetBeginFrame(int frame) { _plugin_config._begin_frame = frame;}
	void		SetEndFrame(int frame) { _plugin_config._end_frame = frame;}
	//////////////////////////////////////////////////////////////////////////
	// config read / write.
	//////////////////////////////////////////////////////////////////////////
	void		LoadConfig()
	{
		FILE *file = fopen(CFGFILENAME, "rb");
		if (file == NULL)
			return;
		fread(&_plugin_config, sizeof(_plugin_config), 1, file);
		fclose(file);
	}
	void		SaveConfig()
	{
		FILE *file = fopen(CFGFILENAME, "wb");
		if (file == NULL)
			return;
		fwrite(&_plugin_config, sizeof(_plugin_config), 1, file);
		fclose(file);
	}

	typedef	std::vector<ImportedElements *>	element_list_t;
	typedef	element_list_t::iterator		element_list_it_t;
	// exporter options.
	struct plugin_config_t
	{
		bool			_export_skin;
		bool			_sample_animation;
		bool			_only_selected;
		bool			_export_bones;
		TimeValue		_begin_frame;
		TimeValue		_end_frame;
		bool			_3d_window;
		bool			_log_window;
		bool			_dos_log_window;
		bool			_sample_matrix;
		bool			_export_object;
		TimeValue		_begin_bone_frame;
		TimeValue		_end_bone_frame;
	};
	std::string		_bone_file_name;
	std::string		_file_name;
	plugin_config_t		_plugin_config;
protected:

	Interface*								ip;
	int										_node_count;
	element_list_t							_elements;
	ImportedMaterial						*_material_list;
	Loger									*_log;
	//////////////////////////////////////////////////////////////////////////
	// imd writer fct.
	//////////////////////////////////////////////////////////////////////////
	unsigned short	CountElementOf(element_type type);
	void	SaveObjectFile(const TCHAR *c_file_name);
	void SaveBoneFile(const TCHAR *c_file_name, ImportedBone *bones);
	void	ExportImd2Material(imd2_object_t &object, std::string &path);
	std::string	GetFilePath(std::string str);
	std::string	GetFileName(std::string str);
	std::string	GetFileNameWithoutExtension(std::string str);
	int			TruncateValueToPower2(int value);
	void		ExportImd2Mesh(imd2_object_t &object);
	void		ExportImd2Tag(imd2_object_t &object);
	void		RecursiveSaveBone(imd2_bone_file_t *imd2_bone, BoneData *data, int &index, int parent = -1);
	Point3		&FixCoordSys(Point3 &pnt);
	Matrix3		&FixCoordSys(Matrix3 &tm);

	//////////////////////////////////////////////////////////////////////////
	// importer fct
	//////////////////////////////////////////////////////////////////////////
	
	void			ImportDataFromMax(INode *node);
	// light
	void			ImportLightData(INode *node, ObjectState &os);
	void			ImportLightPos(INode *node, TimeValue t, LightData &la);
	// mesh
	Matrix3			GetNodeOffsetTM(INode* node);
	void			ImportMeshData(INode *node, ObjectState &os);
	void			ImportTriangularData(INode *node, ObjectState &os);
	ImportedMesh*	ImportTriangularObject(INode *node, ObjectState &os);
	bool			ImportTriangularFace(Mesh *mesh, ImportedMesh *imported_mesh);
	void			ImportTriangularMapping(Mesh *mesh, ImportedMesh *imesh, uword *face_data);
	// -- sub part : weight
	ImportedSkin*	ImportSkinData(INode *node, ImportedMesh *imesh);
	int				GetNumBones(INode *node);
	bool			IsNodeBone(INode *node);
	bool			HaveChildBone(INode *node);

	// material 
	MaterialData*	ImportMaterialData(Mtl *max_material, ObjectState &os);
	// strip method
	bool			StripTriangularZappy(uword *face, int count, ImportedMesh *imported_mesh);
	bool			StripTriangularNVidia(uword *face, int count, ImportedMesh *imported_mesh);
	// tag
	void			ImportTagData(INode *node, ObjectState &os);
	ImportedTag*	ImportTagObject(INode *node, ObjectState &os);	// bones
	// utils
	Point3			GetCenter(Mesh &mesh);
	void			FreeAll();
	TriObject*		GetTriObjectFromNode(INode *node, TimeValue t);

	// bone import
	ImportedBone	*ImportBoneData();
	void			ImportBoneAnimation(INode *node, BoneData *bone_data);
	void			RecursiveImportBoneData(INode *node, ImportedBone *bone, BoneData *bone_data = 0);
	int				RecursiveGetBoneIndex(INode *root, INode *node, int &index);
	int				GetBoneIndex(INode *node);
	// iterate over all object modifier and find skin modifier.
	Modifier*		FindModifier(INode *node, Class_ID &class_id);
};

