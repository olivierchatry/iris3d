/*********************************************************
**  File name : IrisImportObject.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __IRIS_IMPORT_OBJECT_H__
#define __IRIS_IMPORT_OBJECT_H__

//! IrisSection class.
/*!
Class that define a strip in a mesh. Because
the dreamcast doesn't support degenerated strip
(aka strip stiched together in a single big list)
we have to separate each strip in a different buffer.
Basically a strip is just a buffer that contain vertice
indices for each face.
*/
class	IrisSection
{
public:
	//!	IrisSection
	/*!
	Default constructor. Initialise variable (pointer set to NULL etc.).
	\return Nothing.
	*/
    IrisSection(){_indice = NULL;};
	//!	~IrisSection
	/*!
	Default destructor. Call Destroy method. 
	Basically delete allocated data.
	\return Nothing.
	*/
    ~IrisSection(){};
	//! GetNumIndices
	/*!
		This function return number of indices in the strip.
		\return Number of indices in the strip.
	*/
	int				GetNumIndice() { return _num_indice;}
	//! SetNumIndices
	/*!
		Allocate the number of strip.
		\param Number of strip.
	*/
    void    		SetNumIndice(int i)
    {
        _num_indice = i;
        if (_indice != NULL) delete [] _indice;
        _indice = new unsigned short[i];
    }
	//! GetIndice
	/*!
		Return a vertices indice from the indices list
		\param Indice in the buffer of the vertice indices you wan't.
		\return The vertice indice.
	*/
	unsigned short		GetIndice(int i){return (_indice[i]);}
	//! GetIndicePtr
	/*!
	Return the pointer of the vertice indices buffer.
	\return Pointer of the vertice indices buffer.
	*/
    unsigned short		*GetIndicePtr(void){return (_indice);}
    // protected:
    unsigned short	*_indice;
    int				_num_indice;
};

//! IrisImportMesh class.
/*!
	Define a mesh. A mesh is in fact a subobject. 
	A subobject can only used one texture.
*/
class	IrisImportMesh
{
public:
    IrisImportMesh();
    ~IrisImportMesh();
    //! Allocate
	/*!
	Allocate all buffer used by this mesh.
	\param num_vertex Number of vertex to allocate.
	\param num_anim Number of animation frame.
	\return Nothing.
	*/
	void	Allocate(int num_vertex, int num_anim, bool matrix_sampling = false, bool have_skin = false);
	//! Destroy
	/*!
	Deallocate all the data. 
	\return Nothing.
	*/
	void						Destroy(void);
	//! GetSection
	/*!
	Return a section of the mesh. A sesction is just a strip.
	\param i Index of the section in the mesh.
	\return A pointer to the section.
	*/
    IrisSection					&GetSection(int i){return _section[i];}
	//! Define number of bones used by our mesh.
	void	SetNumBones(int count);
	//! Return number of bones.
	int		GetNumBones() {return _num_bones;}
	//! Define total number of skinned vertices. Note that a vertices can have two or more weight.
	void	SetNumSkinnedVertices(int count);
	//! Return index on the skinned table of a certain bone.
	int		*GetSkinIndex() {return _skin_index;}
	//! Return vertices mapped by skin index.
	int		*GetSkinIndices() { return _skin_indices;}
	//! Return weight value;
	float	*GetSkinWeight() { return _skin_weight;}
	//! Allocate data for skinned mesh.
	void	AllocateSkin(int skinned_count, int bone_count)
	{
		if (_skin_index)
			delete _skin_index;
		if (_skin_weight)
			delete _skin_weight;
		if (_skin_indices)
			delete _skin_indices;
		_num_skinned = skinned_count;
		_skin_index = new int[skinned_count];
		_skin_indices = new int[bone_count];
		_skin_weight = new float[skinned_count];
	}
	//! GetNumSkinned
	/*!
		\return Number of skinned vertices.
	*/
	int	GetNumSkinned()
	{
		return _num_skinned;
	}
	
	//! GetNumSection
	/*!
	\return Number of section ine the mesh.
	*/
    int							GetNumSection(){return _num_section;}
	//! GetSize
	/*!
		\return Size of the mesh in memory.
	*/
    int							GetSize();
	//! GetNumVertex
	/*!
		\return Number of vertices in the mesh.
	*/
    int							GetNumVertex(){return _num_vertex;}
    void						SetNumSection(int i)
    {
        if (_section != NULL)
        {
            free(_section[0]._indice);
            delete [] _section;
        }
        _section = new IrisSection[i];
        _num_section = i;

    }
	//! Return whatever or not the object is skinned.
	bool	HaveSkin() { return _skin_weight != 0;	}
	//! Tell the user if the object was animated using matrix or sampled vertices.
	bool	SampledMatrix() {return _matrix != 0;}
	//! GetVertex
	/*!
		Return a pointer to the original vertices data.
		\return Pointer to original vertices data.
	*/
    vect3d		    		*GetVertex(){return _vertex;}
	//! GetTransformedVertex
	/*!
		Return a pointer to 3d transformed vertices. The transformation
		use IrisCamera, World and Object matrix.
		\return Pointer to 3d transformated vertices.
	*/
    vect3d	    			*GetTransformedVertex(){return _trans_vertex;}
	//! Get2DVertex
	/*!
		Return a pointer to 2d projected vertices.
		\return Pointer to 2d projected vertices.
	*/
    vertex2dtl				*Get2DVertex(){return _2d_vertex;}
	//! GetNormal
	/*!
		Return a pointer to orginal normal of the mesh.
		\return Pointer to original normal.
	*/
    vect3d					*GetNormal(){return _normal;}
	//! GetTransformedNormal
	/*!
		Return a pointer to 3d transformed normal. We just use
		Rotation component of the concatenation matrix between
		World and Object matrix.
		\return Pointer to 3d transformed normal.
	*/
    vect3d					*GetTransformedNormal(){return _trans_normal;}
	//! GetVertex
	/*!
	Return a vertice from the vertices buffer.
	\param i Index of the vertice in the buffer.
	\return The vertice.
	*/
    vect3d		    		*GetVertex(int i){return &_vertex[i];}
	//! GetTransformedVertex
	/*!
	Return a 3d transformed vertice from the vertices buffer.
	\param i Index of the vertice in the buffer. The transformation
	use IrisCamera, World and Object matrix.
	\return The 3d transformated vertice.
	*/
    vect3d		    		*GetTransformedVertex(int i){return &_trans_vertex[i];}
	//! Get2DVertex
	/*!
	Return a 2d projected vertices from the vertices buffer.
	\param i Index of the vertice in the buffer.
	\return The 2d projected vertices.
	*/
    vertex2dtl				*Get2DVertex(int i){return &_2d_vertex[i];}
	//! GetNormal
	/*!
	Return a normal from the normal buffer.
	\param i Index of the normal in the buffer.
	\return The normal.
	*/
    vect3d					*GetNormal(int i){return &_normal[i];}
	//! GetTransformedNormal
	/*!
	\param i Index of the normal in the buffer.
	\return The transformed normal using rotation component of 3d transformation matrix (view + world).
	*/
    vect3d					*GetTransformedNormal(int i){return &_trans_normal[i];}
	//! GetName
	/*!
		Return the name of the mesh.
		\return Name of the mesh.
	*/
	char				*GetName(){return _name;}
	//! GetMaterial
	/*!
		Return the material used by the mesh.
		\return Pointer to a material.
	*/
    IrisMaterial			*GetMaterial() {return _material;}
	//! SetMaterial
	/*! 
		Define the material of the mesh used for rendering.
		Can be NULL.
		\param material Pointer to a material.
		\return Nothing.
	*/
    void					SetMaterial(IrisMaterial	*material) {_material = material;}
	//! ComputeCenter
	/*!
	Compute barycenter of the mesh. Used for bounding sphere.
	/note This function is automatically called when tou load an 
	object from a file.
	\return Nothing.
	*/
    void					ComputeCenter();
	//! ComputeRadius
	/*!
	Compute radius of a bounding sphere around the mesh.
	/note This function is automatically called when tou load an 
	object from a file.
	\return Nothing.
	*/
    void					ComputeRadius();
	//! SetRadius
	/*!
	Define the bounding sphere radius.
	\param value Radius of the bounding sphere.
	\return nothing.
	*/
    void					SetRadius(float value) {_radius = value;}
	//! GetRadius
	/*!
	Return the bounding sphere radius linked to the mesh.
	\return Bounding sphere radius.
	*/
    float					GetRadius() const		{return _radius;}
	//! GetCenter
	/*!
	Return barycenter of the mesh computed by ComputeCenter.
	\return Barycenter of the mesh.
	*/
    vect3d	    			&GetCenter() {return _center;}
	//! SetClipped
	/*!
		Define if the mesh need to be clipped, displayed or occluded
		\param clipped How the mesh need to be draw.
	*/
    void					SetClipped(IrisFrustrum::e_res clipped) {_clipped = clipped;}
	//! GetClipped
	/*!
		Return how the mesh need to be clipped.
		\return Clipping mode of the mesh.
	*/
    IrisFrustrum::e_res			GetClipped(void) {return _clipped;}
	//! GetLighted
	/*!
		Indicate if the lighting computation need to be done.
		\todo If there is more than one light in the scene and the mesh
		is, at least in one light, we compute lighting for all the light !!!
		must be fixed.
	*/
    bool					GetLighted(void) {return _lighted;}
	//! SetLighted
	/*!
		Define is the mesh need to be lighted.
		\param lighted Lighted or not.
	*/
    void					SetLighted(bool lighted) {_lighted = lighted;}
	//! Getcolor
	/*!
		Return a color value from the color buffer.
		\param index Indice in the color buffer.
		\return IrisColor value.
	*/
    uint32					GetColor(unsigned int index) {return _color[index];}
	//! SetColor
	/*!
	Define a color in the color buffer
	\param index Indice in the color buffer
	\param color uint32 (ARGB 8x8x8x8) value to be placed in the color buffer.
	\return Nothing.
	*/
    void					SetColor(unsigned int index, uint32 color) {_color[index] = color;}
	//! GetColor
	/*!
	Return a pointer to the color buffer.
	\return Pointer to the color buffer.
	*/
    uint32					*GetColor() {return _color;}
	//! GetColorPtr
	/*!
	Return a pointer to the color buffer with offset.
	\param index Offset in the color buffer.
	\return Pointer to the color buffer with the offset added.
	*/
    uint32					*GetColorPtr(unsigned int index) {return (_color + index);}
	//! UpdateColorBuffer
	/*!
	Update all vertex color with color buffer.
	\return Nothing.
	*/
	void					UpdateColorBuffer();
	//! Return pointer to matrix animation.
	matrix					*GetMatrix() {return _matrix;}
	vect3d					*GetSkinnedVertex() {return _skinned_vertex;}
	vect3d					*GetSkinnedNormal() { return _skinned_normal;}
	//! GetUserData
	/*!
	Return user data defined in max object properties.
	Can be very usefull if you wan't to script some
	action ...
	\return User data of the tag.
	*/
	char		*GetUserData()
	{
		return _user_data;
	}
	void	SetUserDataSize(int size);

protected:
	matrix					*_matrix __attribute__((aligned(32)));
    float					_radius;
    vect3d					_center;
    IrisMaterial			*_material;
    vect3d					*_vertex;
    int						_num_skinned;
	vect3d					*_normal;
	//	Skined data.
	vect3d					*_skinned_normal;
	vect3d					*_skinned_vertex;
	
	vect3d					*_trans_normal;
    vect3d					*_trans_vertex;
    vertex2dtl				*_2d_vertex;
    uint32					*_color;
    int						_num_vertex;
    IrisFrustrum::e_res		_clipped;
    bool					_lighted;
    IrisSection				*_section;
	// bones
	float					*_skin_weight;
	int						*_skin_index;
	int						*_skin_indices;
	int						_num_bones;
	int						_num_section;
	char					*_user_data;
    char					_name[IMD_MAX_NAME];
};

//////////////////////////////////////////////////////////////////////////

//! IrisImportTag class.
/*!
Class that define a tag. A tag is a point linked to a mesh.
This tag can be animated. Note that there is no 
vertex interpolation beetween animation currently.
\todo Vertex interpolation between animation.
*/
class IrisImportTag
{
public:
	//!	IrisImportTag 
	/*!
		Default constructor. Initialise variable (pointer set to NULL etc.).
		\return Nothing.
	*/
	IrisImportTag();
	//!	~IrisImportTag
	/*!
		Default destructor. Call Destroy method. 
		Basically delete allocated data.
		\return Nothing.
	*/
	~IrisImportTag();				
	//! GetName
	/*!
		Return name of the tag. The name is defined in max,
		it's simply the name of the object.
		\return Name of the tag.
	*/
	char *GetName(){return _name;}
	//! GetUserData
	/*!
		Return user data defined in max object properties.
		Can be very usefull if you wan't to script some
		action ...
		\return User data of the tag.
	*/
	void	SetUserDataSize(int size);
	char *GetUserData(){return _user_data;}
	//! GetPos
	/*!
		Return a constant vect3d that define position of the 
		tag in object space (World transformation are not applyed).
		\todo Apply world trasformation from parent object.
		\param current_anim Define wich position on the animation you want. 
		\return The position of the tag at current_animation. Note
		that this value is constant and cannot be modified.
	*/
	const vect3d	&GetPos(int current_anim = 0) const;
	//! GetPos
	/*!
	Return a vect3d that define position of the 
	tag in object space (World transformation are not applyed).
	\todo Apply world trasformation from parent object.
	\param current_anim Define wich position on the animation you want. 
	\return The position of the tag at current_animation. 
	*/
	vect3d			&GetPos(int current_anim = 0);
	//!SetNumAnim
	/*!
	Called at file loading, you do not have to call this function.
	It simply allocate memory for the tag data.
	\param num_anim Define the number of animation that the tag will
	contain.
	\return Nothing.
	*/
	void	SetNumAnim(int num_anim);
protected:
	//!Destroy
	/*!
	Called by the destructor. Just delete internal tag data.
	*/
	void		Destroy();
	char		_name[IMD_MAX_NAME];
	char		*_user_data;
	vect3d		*_pos;
};


//! IrisImportObject class.
/*!
Class that definine an imported object from 
an imd or imd2 file. 
\todo Currently, each vertex define for each animation a texture coordinate and color
value. In reality texture coordinate and color value are just defined once per object.
*/
class	IrisImportObject
{

public:
	//! IrisImportObject
	/*!
	Default constructor. Initialise variable (pointer set to NULL etc.).
	\return nothing
	*/
	IrisImportObject();
	//! ~IrisImportObject
	/*!
	Default destructor. Call Destroy method. 
	Basically delete allocated data.
	\return Nothing.
	*/
	~IrisImportObject();
	//! Destroy
	/*!
	Delete internal object data. After a call to this method, you cannot
	use the object anymore. 
	\note Normally you do not call directly this method and let the class IrisObject do it for you.
	\return Nothing.
	*/
	void			Destroy();
	//! LoadFromFile
	/*!
	Load an imd or imd2 file from specified path and file_name. Optionally apply
	a scale to the 3d data.
	\param file_path Define path where it can find the file and material data (aka texture)
	\param file_name Define file name of the imd file.
	\param scale Define scale value to be applyed when loading the object.
	\note Normally you do not call directly this method and let the class IrisObject do it for you.
	\return False if it cannot load file or true if file was loaded.
	*/
    bool			LoadFromFile(char *file_path, char *file_name, float scale = 1.0f);
    //	get
	//! GetNumMaterial
	/*!
	Return number of material used by the object.
	\return Number of material used.
	*/
    int				GetNumMaterial(){return _num_material;}
	//! GetNumMesh
	/*!
	Return number of mesh in the object. A mesh is a subobject.
	\note A mesh can just have one material.
	\return Number of mesh.
	*/
    int				GetNumMesh(){return _num_mesh;}
    
	//! GetNumTag
	/*!
	Return number of tag linked to the object.
	\return Number of tag
	*/
	int					GetNumTag() {return _num_tag;}
	//! GetNumAnim
	/*!
	Return number of animation frame in object
	\return number of animation frame.
	*/
    int					GetNumAnim(){return _num_anim;};
    // ---
	//! GetMesh
	/*!
	Return a mesh in the object. A mesh is, in fact, a subobject.
	\param i Index of the mesh in the object.
	\return A reference to an IrisImportMesh.
	*/
    IrisImportMesh 		&GetMesh(int i){return	_mesh[i];}
	//! GetMaterial
	/*!
	Return a material used by the object. A material is currently
	just a texture.
	\param i Index of the material in the object.
	\return A pointer to the material
	*/
    IrisMaterial		*GetMaterial(int i){return	_material[i];}
	//! GetTag
	/*!
	Return a tag linked to the object. 
	\param i Index of the tag in the object.
	\return A reference to the tag
	*/    
	IrisImportTag		&GetTag(int i) {return	_tag[i];}
	//! GetName
	/*!
		Return the name of the object.
		\return The name of the object.
	*/
	char				*GetName(){return _name;};
	//! GetFileName
	/*! 
		Return the file name from where the object 
		was loaded.
	*/
    char				*GetFileName(){return _file_name;}
    //	set
    void		SetNumMaterial(int i)
    {
        _num_material = i;
        if (_material != NULL)
            free (_material);
        _material = new IrisMaterial*[i]; // (IrisMaterial **) malloc(sizeof(IrisMaterial *) * i);
    }
    void		SetNumMesh(int i)
    {
        _num_mesh = i;
        if (_mesh != NULL)
            delete [] _mesh;
        _mesh = new IrisImportMesh[i];
    }
    
	void		SetNumTag(int i, int num_anim)
    {
		if (_tag != NULL)
            delete [] _tag;
        _tag = new IrisImportTag[i];
		for (int idx = 0; idx < i; ++idx)
			_tag[idx].SetNumAnim(num_anim);
        _num_tag = i;
    }

	void		SetName(char *name)
    {
        memcpy(_name, name, IMD_MAX_NAME);
    }

	//! GetSize
	/*!
	Return size in byte taken by the object in memory.
	\return size of the object in memory.
	*/
    int			GetSize();
    int			_total_num_vertex;
    int			_total_num_strip;
    int			_total_num_indice;
protected:
    char		*GetFilePath(char *file_path, char *file_name)
    {
        char *file_path_and_name = new char[strlen(file_path) + strlen(file_name) + 1];
        strcpy(file_path_and_name, file_path);
        strcat(file_path_and_name, file_name);
        return file_path_and_name;
    };
	// imd file translation ...
	void	LoadImdMesh(imd_object_t *imd_object, float scale);
	void	LoadImd2Mesh(imd2_object_t *imd_object, float scale);

	bool	LoadFromImdFile(char *file_path, char *file_name,float scale);
	bool	LoadFromImd2File(char *file_path, char *file_name,float scale);

    int								_num_tag;
    int								_num_mesh;
    int								_num_anim;
    int								_num_material;
    char							_name[IMD_MAX_NAME];
    IrisImportTag					*_tag;
    IrisMaterial					**_material;
    IrisImportMesh					*_mesh;
    char							*_file_name;
};

#endif

