/*********************************************************
**  File name : IrisObjectManager.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IRIS_OBJECT_MANAGER__
#define __IRIS_OBJECT_MANAGER__

//!	IrisObjectManager class.
/*!
	The IrisObjectManager class handle objet loading / managing. 
	It allow you to load model without carrying if this model is already loaded in memory. 
	Normally you don't use directly this class. It use is abstracted by IrisObject::LoadFromFile();. 
*/
class IrisObjectManager : public IrisManager<IrisImportObject>
{
public:
  //!	LoadObject function.
  /*!
		\param file_path : directory where the file is located. IMD file and textures must be located in the same directory.
		\param file_name : file name of IMD file.
		\param scale : scale object by value.
		\return a pointer to a new IrisImportObject.
	*/
  IrisImportObject *LoadObject(char *file_path, char *file_name, float scale);
};

#endif
