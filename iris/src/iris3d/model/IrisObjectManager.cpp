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
#include	"IrisContext.hpp"

typedef std::vector<IrisImportObject	*>::iterator	ObjectIterator;
typedef std::vector<uint>::iterator	UsedIterator;


IrisImportObject	*IrisObjectManager::LoadObject(char *file_path, char *file_name,float scale)
{
    char	buffer_tmp[1024] = {0};
    strcpy(buffer_tmp, file_path);
    strcat(buffer_tmp, file_name);
    for (unsigned i = 0; i < _objects.size(); ++i)
    {
        IrisImportObject	*object = _objects[i];
        if (strcmp(buffer_tmp, object->GetFileName()) == 0)
        {
            _used[i] ++;
            return (object);
        }
    }
    IrisImportObject	*new_one = new IrisImportObject;
    new_one->LoadFromFile(file_path, file_name,scale);
    _objects.push_back(new_one);
    _used.push_back(1);
    return (new_one);
}

bool	IrisObjectManager::UnloadObject(const char *file_name)
{
    unsigned	idx = 0;
    for (ObjectIterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        IrisImportObject	*object  = *i;
        if (strcmp(file_name, object->GetFileName()) == 0)
        {
            if (--_used[idx] == 0)
            {
                delete object;
                _objects.erase(i);
                for (UsedIterator j = _used.begin(); j != _used.end(); ++j)
                    if (idx == 0)
                    {
                        _used.erase(j);
                        break;
                    }
                    else
                        idx --;
                return (true);
            }
            else
                return (true);
            idx ++;
        }
    }
    return (false);
}

bool	IrisObjectManager::UnloadObject(IrisImportObject *object)
{
    unsigned	idx = 0;
    for (ObjectIterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        IrisImportObject	*object_test  = *i;
        if (object_test == object)
        {
            if (--_used[idx] == 0)
            {
                delete object;
                _objects.erase(i);
                for (UsedIterator j = _used.begin(); j != _used.end(); ++j)
                    if (idx == 0)
                    {
                        _used.erase(j);
                        break;
                    }
                    else
                        idx --;
                return (true);
            }
            else
                return (true);
            idx ++;
        }
    }
    return (false);
}


bool	IrisObjectManager::DeleteAll()
{
    ObjectIterator	i;
    for (i = _objects.begin(); i != _objects.end(); ++i)
    {
        IrisImportObject	*object = *i;
        delete object;
    }
    _objects.resize(0);
    _used.resize(0);
    return true;
}
