/*********************************************************
**  File name : IrisObjectManager.hpp
**	Iris Engine V0.7 "presque"
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
class IrisObjectManager
{
public:
	//!	Constructor of the IrisObjectManager class.
	/*!
	*/
    IrisObjectManager()
    {
        _objects.resize(0);
    }
	//!	Destructor of the IrisObjectManager class.
	/*!
	*/
    ~IrisObjectManager()
    {
        DeleteAll();
    }
	//!	LoadObject function.
	/*!
		\param file_path : directory where the file is located. IMD file and textures must be located in the same directory.
		\param file_name : file name of IMD file.
		\param scale : scale object by value.
		\return a pointer to a new IrisImportObject.
	*/
    IrisImportObject	*LoadObject(char *file_path, char *file_name,float scale);
    /*
    * Free object from manager
    */
	/*!	\fn bool  UnloadObject(const char *file_name);
		\brief UnloadObject function

		Free an object from object manager regarding if it loaded more than one time.
		\param file_name of the IMD (with path).
		\return a boolean, true if object have been freed, false if not. 
	*/
    bool		UnloadObject(const char *file_name);
	/*!
		\overload bool  UnloadObject(IrisImportObject *object);
		\param a pointer to an IrisImportObject.
		\return a boolean, true if object have been freed, false if not. 
	*/
    bool		UnloadObject(IrisImportObject *object);
	/*
     * Unload all object
     */
	//!	DeleteAll function.
	/*!
		Free all object regardless how many time objects in list have been instancied.
		\return a boolean.
	*/
    bool		DeleteAll();
	//!	Count function.
	/*!
		\return an uint32 representating how many object are loaded.
	*/
    uint32	Count() {return _objects.size();}
	//!	GetObject function.
	/*!
		\param an index value.
		\return a pointer to an IrisImportObject using index value.
	*/
    IrisImportObject *GetObject(unsigned index)
    {return _objects[index];};
protected:
    std::vector<IrisImportObject *>	_objects;
    std::vector<uint>								_used;
};


#endif
