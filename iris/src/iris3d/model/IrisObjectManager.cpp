/*********************************************************
**  File name : IrisObjectManager.cpp
** Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

//////////////////////////////////////////////////////////////////////////
//	TODO: create a template that define a basic 
//	reference counter.
//////////////////////////////////////////////////////////////////////////

#include	"Iris.hpp"



IrisImportObject	*IrisObjectManager::LoadObject(char *file_path, char *file_name,float scale)
{
    char			buffer_tmp[1024] = {0};
	reference_it	r = _references.begin();
	
	strcpy(buffer_tmp, file_path);
    strcat(buffer_tmp, file_name);
    for (data_it i = _pointers.begin(); i < _pointers.end(); ++i)
    {
        IrisImportObject	*object = *i;
        if (strcmp(buffer_tmp, object->GetFileName()) == 0)
        {
            (*r) ++;
            return (object);
        }
		++ r;
    }
    IrisImportObject	*new_one = new IrisImportObject;
    new_one->LoadFromFile(file_path, file_name,scale);
    _pointers.push_back(new_one);
    _references.push_back(1);
    return (new_one);
}

