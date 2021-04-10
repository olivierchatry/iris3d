/*********************************************************
**  File name : IrisMaterialManager.cpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"

IrisMaterial *IrisMaterialManager::LoadMaterial(char *file_name, TextureType tex)
{
  reference_it r = _references.begin();
  for (data_it i = _pointers.begin(); i < _pointers.end(); ++i)
  {
    IrisMaterial *material = *i;
    if (strcmp(file_name, material->GetTexture(0)->GetFileName()) == 0)
    {
      (*r)++;
      return (material);
    }
    r++;
  }
  IrisMaterial *new_one = new IrisMaterial;
  new_one->LoadFromFile(file_name, tex);
  _pointers.push_back(new_one);
  _references.push_back(1);
  return (new_one);
}
