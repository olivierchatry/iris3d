/*********************************************************
**  File name : IrisMaterialManager.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef CIRISMATERIALMANAGER_H
#define CIRISMATERIALMANAGER_H

//!	IrisMaterialManager class.
/*!
	The IrisMaterialManager class handle texture loading / managing. 
	It allow you to load texture without carrying if this texture is already loaded in memory.
	You can acces material manager by calling IrisContext::GetMaterialManager. 

	Source code : Texture loading with manual object (sample/manualobject/).
	Html source code : Texture loading with manual object (sample/manualobject/).
*/
class IrisMaterialManager
{
public:
	//! Destructor of the IrisMaterialManager class.
	/*!
	*/
  ~IrisMaterialManager()
		{
			DeleteAll();
		}
		//! LoadMaterial function.
		/*!
			\param file_name : file name with complete path of bitmap file. Must be a PNG file.
			\return a pointer to a new IrisMaterial.
		*/
		IrisMaterial	*LoadMaterial(char *file_name, TextureType tex = texture_noalpha);
	/*
	* Free material from memory using it pointer for search
	*/
	//! UnloadMaterial function.
	/*!
		Free a texture from texture manager regarding if it loaded more than one time.
		\param material : pointer to an IrisMaterial.
		\return a boolean, true if texture have been freed, false if not.
	*/
	bool		    UnloadMaterial(IrisMaterial *material);
	/*
	* Free material from memory using file_name for search
	*/
	/*!
		\overload bool  UnloadMaterial(const char *file_name);
		\param file_name : file name of the Texture (with complete path).
		\return a boolean, true if texture have been freed, false if not.
	*/
  bool			UnloadMaterial(const char *file_name);
  /*
   * Load a PNG file. 
   */
	//! DeleteAll function.
	/*!
		Free all texture regardless how many time objects in list have been instancied.
		\return a boolean, true if xxx, false if not.
	*/
	bool	DeleteAll();

protected:
	std::vector<IrisMaterial *>   _materials;
	std::vector<uint>						_used;
};

#endif //CIRISMATERIALMANAGER_H
