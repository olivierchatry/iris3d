/*********************************************************
**  File name : IrisMaterialManager.cpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include	"Iris.hpp"
#include	"IrisContext.hpp"

typedef std::vector<IrisMaterial	*>::iterator	MaterialIterator;
typedef std::vector<uint>::iterator	UsedIterator;


IrisMaterial	*IrisMaterialManager::LoadMaterial(char *file_name, TextureType tex)
{
	for (unsigned i = 0; i < _materials.size(); ++i)
	{
		IrisMaterial	*material = _materials[i];
		if (strcmp(file_name, material->GetTexture(0)->GetFileName()) == 0)
		{
			_used[i] ++;
			return (material);
		}
	}
	IrisMaterial	*new_one = new IrisMaterial; 	
	new_one->LoadFromFile(file_name, tex);
	_materials.push_back(new_one);
	_used.push_back(1);
	return (new_one);
}


bool	IrisMaterialManager::UnloadMaterial(const char *file_name)
{
	unsigned	idx = 0; 
	for (MaterialIterator i = _materials.begin(); i != _materials.end(); ++i)
	{
		IrisMaterial	*material = *i;
		if (strcmp(file_name, material->GetTexture(0)->GetFileName()) == 0)
		{
			if (--_used[idx] == 0)
			{
				delete material;
				_materials.erase(i);
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
			idx ++;
		}
	}
	return (false);
}


bool	IrisMaterialManager::UnloadMaterial(IrisMaterial *material)
{	
	unsigned	idx = 0; 
	for (MaterialIterator i = _materials.begin(); i != _materials.end(); ++i)
	{
		IrisMaterial	*material_test  = *i;
		if (material_test == material)
		{
			if (--_used[idx] == 0)
			{
				delete material;
				_materials.erase(i);
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
			idx ++;
		}
	}
	return (false);
}


bool	IrisMaterialManager::DeleteAll()
{
	MaterialIterator	i;
	for (i = _materials.begin(); i != _materials.end(); ++i)
	{
		IrisMaterial	*material = *i;
		delete material;
	}
	for (i = _materials.begin(); i != _materials.end();)
		i = _materials.erase(i);
	_materials.resize(0);
	return true;
}
