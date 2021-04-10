/*********************************************************
**  File name : IrisSkeletalManager.cpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 25/05/2003
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "iris.hpp"

IrisSkeletal *IrisSkeletalManager::LoadSkeletal(char *file_name)
{
  reference_it r = _references.begin();
  for (data_it i = _pointers.begin(); i < _pointers.end(); ++i)
  {
    IrisSkeletal *skeletal = *i;
    if (strcmp(file_name, skeletal->GetFileName()) == 0)
    {
      (*r)++;
      return (skeletal);
    }
    r++;
  }
  IrisSkeletal *new_one = new IrisSkeletal;
  if (!new_one->LoadFromFile(file_name))
  {
    new_one->Destroy();
    delete new_one;
    return 0;
  }
  _pointers.push_back(new_one);
  _references.push_back(1);
  return (new_one);
}
