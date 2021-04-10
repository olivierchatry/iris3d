/*********************************************************
**  File name : IrisMaterialManager.hpp
**	Iris Engine V0.9 "alaiii"
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
class IrisMaterialManager : public IrisManager<IrisMaterial>
{
public:
  //! LoadMaterial function.
  /*!
		\param file_name : file name with complete path of bitmap file. Must be a PNG file.
		\return a pointer to a new IrisMaterial.
	*/
  IrisMaterial *LoadMaterial(char *file_name, TextureType tex = texture_noalpha);
};

#endif //CIRISMATERIALMANAGER_H
