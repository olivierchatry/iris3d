/*********************************************************
**  File name : IrisMaterial.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef CIRISMATERIAL_H
#define CIRISMATERIAL_H

//! TextureType enum.
/*!
	This enum give information about texture type.
*/
enum TextureType
{
	texture_alpha = PNG_FULL_ALPHA,
	texture_noalpha = PNG_NO_ALPHA,
	texture_punchtru = PNG_MASK_ALPHA,
	texture_vqf
};

/*
 * Iris Texture class.
 */
//! IrisTexture class.
/*!
	Class used for operations on textures, to load and manipulate them.
	IrisTexure objects are difined by an id and the file name of the used texture.
*/
class IrisTexture
{
public:
	//! Constructor of the IrsisTexture class.
	/*!
		Create an empty IrisTexture object.
	*/
	IrisTexture(void)
	{
		_id = 0;
		_file_name = NULL;
	}
	//! Destructor of the IrsisTexture class.
	/*!
		Create an empty IrisTexture object.
	*/
	~IrisTexture(void);
	//! TODO SetTexture function.
	/*!
		Does nothing.
		\return a boolean, true if the assignement is successfull else false.
	*/
	bool			SetTexture(unsigned short *buffer, int size_x, int size_y)
	{
		return (_id >= 0);
	}
	//! SetFileName function.
	/*!
		Set the file name of a texture in IrisTexture object.
		\param file_name : a string, the file name of a texture.
		\return nothing.
	*/
	void			SetFileName(char *file_name)	
	{ 
		if (_file_name) 
			delete [] _file_name;
		_file_name = new char [strlen(file_name) + 1];
		strcpy(_file_name, file_name); 
	}
	//! GetFileName function.
	/*!
		Get the file name of the texture in IrisTexture object.
		\return a string, the file name of the texture.
	*/
	char *			GetFileName(void)
	{ 
		return _file_name; 
	}
	//! GetId function.
	/*!
		\return an uint32, the id of the IrisTexture object.
	*/
	uint32				GetId(void)
	{ 
		return _id;
	}
	//! SetId function.
	/*!
		Set the id of the IrisTexture object.
		\param id : an uint32, the id of the IrisTexture object.
		\return nothing.
	*/
	void				SetId(uint32 id)
	{ 
		_id = id;
	}
	//! SetType function.
	/*!
		Set the type of texture in the IrisTexture object.
		\param  type : the TextureType of the IrisTexture object.
		\return nothing.
	*/
	void		SetType(TextureType type)
	{
		type = _type;
	}
protected:
	char *		_file_name;
	uint32		_id;
	TextureType	_type;
};


/*cxt.txr.format = PVR_TXRFMT_RGB565;
cxt.txr.format = PVR_TXRFMT_ARGB4444;
cxt.txr.format = PVR_TXRFMT_ARGB1555;

#define ARGB4444(a, r, g, b) ((a >> 4) << 12 )
#define ARGB1555(a, r, g, b)
#define RGB565(a, r, g, b)
*/
/**
 * Iris Material class. 
 */
class IrisMaterial
{
public:
	IrisMaterial () 
	{
		_texture = NULL;
	};
	
	~IrisMaterial ()
	{
		Destroy();
	}
	void	IrisMaterial::Generate(void (*fct_generate)(unsigned short *buffer, int size_x, int size_y), int size_x, int size_y, TextureType type = texture_noalpha);
	
	/*! Load material from a .mat file (for now, .mat is in fact .PNG) 
	 * \param file_name file name of the texture to load. must be a PNG file.
	 * \param tex texture type (noalpha, alpha, punchthru.)
     */
    bool			LoadFromFile(char *file_name, TextureType tex = texture_noalpha);
	
	/*! Return a pointer to a given texture index
	 * \param id index of the texture, for now, always 0.
	 * \return a pointer to an IrisTexture obejesct.
	 */
	IrisTexture *	GetTexture(int id) { return &(_texture[id]); }
    
	/*! Return number of texture in the material*/
    int				GetNumTexture() {return _num_texture;};

	void			SetTempId(int temp_id){_temp_id = temp_id;};
	int				GetTempId(void) {return (_temp_id);}
	pvr_poly_cxt_t	*GetCxt(){return &_cxt;}
protected:
	void	Destroy();
	IrisTexture		*_texture;
	int				_num_texture;
	int				_temp_id;
	pvr_poly_cxt_t	_cxt;
};
#endif //CIRISMATERIAL_H
