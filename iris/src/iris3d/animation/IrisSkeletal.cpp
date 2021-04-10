/*********************************************************
**  File name : IrisSkeletal.hpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 25/05/2003
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#include "iris.hpp"
//////////////////////////////////////////////////////////////////////////
// IrisBone Class
//////////////////////////////////////////////////////////////////////////
IrisBone::~IrisBone()
{
  Destroy();
}

void IrisBone::Destroy()
{
  if (this->_bone_anim)
    delete[] _bone_anim;
  _bone_anim = 0;
  _index = 0;
}

//////////////////////////////////////////////////////////////////////////
// IrisSkeletal Class
//////////////////////////////////////////////////////////////////////////
IrisSkeletal::IrisSkeletal()
{
  _current_anim = 0;
  _num_anim = 0;
  _bones = 0;
  _num_bones = 0;
  _context = &(IrisContext::Get());
}

bool IrisSkeletal::LoadFromFile(char *file)
{
  printf("[IrisSkeletal::LoadFromFile] %s\n", file);
  Destroy();
  imd2_bone_file_t *imd2_bone = load_imd2_bone(file);
  if (imd2_bone == 0)
    return false;
  _num_anim = imd2_bone->imd2_bone_file_header.anim_count;
  int num_bones = imd2_bone->imd2_bone_file_header.bone_count;
  _bones = new IrisBone[num_bones];
  _num_bones = num_bones;
  for (int i = 0; i < num_bones; ++i)
  {
    IrisBone *b = _bones + i;
    imd2_bone_t *ib = imd2_bone->bones + i;
    b->_index = ib->imd2_bone_header.bone_index;
    b->_parent = _bones + ib->imd2_bone_header.bone_parent;
    b->_bone_anim = new IrisBoneAnim[_num_anim];
    // load animation
    for (int a = 0; a < _num_anim; ++a)
    {
      imd2_bone_anim_t *anim = ib->imd2_bone_anim + a;
      memcpy(b->_bone_anim[a]._matrix._linear, anim->matrix, sizeof(anim->matrix));
    }
  }
  free_imd2_bone(imd2_bone);
  _loop_begin = 0;
  _loop_end = _num_anim - 1;
  printf("[IrisSkeletal::LoadFromFile] finished\n");
  return true;
}

char *IrisSkeletal::GetFileName()
{
  return _file_name;
}

void IrisSkeletal::Destroy()
{
  if (_bones)
    delete[] _bones;
  _bones = 0;
  _num_anim = 0;
}

IrisBoneAnim *IrisSkeletal::GetBone(int index)
{
  return _bones[index]._bone_anim + _current_anim;
}

IrisBoneAnim *IrisSkeletal::GetBone(int index, int anim_index)
{
  return _bones[index]._bone_anim + anim_index;
}

void IrisSkeletal::Update(IrisImportObject *iobject)
{
  int mesh_count = iobject->GetNumMesh();

  while (mesh_count--)
  {
    IrisImportMesh &imesh = iobject->GetMesh(mesh_count);
    int *skin_index = imesh.GetSkinIndex();
    int *skin_indices = imesh.GetSkinIndices();
    int *erase_skin_index = skin_index;
    float *weight = imesh.GetSkinWeight();
    if (skin_index == 0 || skin_indices == 0 || weight == 0)
      continue;
    int bone_count = _num_bones;
    IrisBone *bone = _bones;
    int count = imesh.GetNumSkinned();
    vect3d *dst_vertex = imesh.GetVertex();
    vect3d *src_vertex = imesh.GetSkinnedVertex();
    while (count--)
    {
      vect3d *v = dst_vertex + *erase_skin_index++;
      v->x = 0.0f;
      v->y = 0.0f;
      v->z = 0.0f;
    }
    while (bone_count--)
    {
      IrisBoneAnim *banim = bone->_bone_anim + _current_anim;
      banim->_matrix.Load();
      int skin_count_for_bones = *skin_indices;
      while (skin_count_for_bones--)
      {
        const int vi = *skin_index++;
        // asm("pref @%0" : : "r" (skin_index));
        vect3d *src = src_vertex + vi;
        vect3d *dst = dst_vertex + vi;
        vect3d pos(*src);
        mat_trans_single_iris(pos.x, pos.y, pos.z);
        float w = *weight++;
        *dst += pos * w;
      }
      skin_indices++;
      bone++;
    }
  }
}
