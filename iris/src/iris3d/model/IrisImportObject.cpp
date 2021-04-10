/*********************************************************
**  File name : IrisImportObject.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"
//////////////////////////////////////////////////////////////////////////
// IrisImportTag
//////////////////////////////////////////////////////////////////////////
IrisImportTag::IrisImportTag() : _user_data(0), _pos(0)
{
}

IrisImportTag::~IrisImportTag()
{
  printf("[INFO] Deleting tag '%s'", _name);
  if (_pos)
    delete[] _pos;
  if (_user_data)
    delete[] _user_data;
}

const vect3d &IrisImportTag::GetPos(int current_anim) const
{
  return _pos[current_anim];
}

vect3d &IrisImportTag::GetPos(int current_anim)
{
  return _pos[current_anim];
}

void IrisImportTag::Destroy()
{
  printf("[INFO] Deleting tag '%s'", _name);
  if (_user_data)
    delete[] _user_data;
  if (_pos)
    delete[] _pos;
  _user_data = 0;
  _pos = 0;
}

void IrisImportTag::SetUserDataSize(int size)
{
  if (_user_data)
    delete[] _user_data;
  _user_data = new char[size];
}

void IrisImportTag::SetNumAnim(int num_anim)
{
  if (_pos)
    delete[] _pos;
  _pos = new vect3d[num_anim];
}

//////////////////////////////////////////////////////////////////////////
// IrisImportMesh
//////////////////////////////////////////////////////////////////////////

IrisImportMesh::IrisImportMesh()
{
  _user_data = NULL;
  _vertex = NULL;
  _section = NULL;
  _2d_vertex = NULL;
  _trans_normal = NULL;
  _normal = NULL;
  _trans_vertex = NULL;
  _color = NULL;
  _num_vertex = 0;
  _num_section = 0;
  _matrix = 0;
  _skinned_vertex = NULL;
  _skinned_normal = NULL;
  _skin_index = NULL;
  _skin_weight = NULL;
  _skin_indices = NULL;
  _num_skinned = 0;
  _lighted = false;
}

IrisImportMesh::~IrisImportMesh()
{
  Destroy();
}

void IrisImportMesh::Allocate(int num_vertex, int num_anim, bool matrix_sampling, bool have_skin)
{
  int count = num_vertex * num_anim;
  _num_vertex = num_vertex;
  if (matrix_sampling)
  {
    count = num_vertex;
    _matrix = new matrix[count];
  }
  else
    _matrix = NULL;
  _vertex = new vect3d[count];
  _normal = new vect3d[count];
  _trans_normal = new vect3d[_num_vertex];
  _trans_vertex = new vect3d[_num_vertex];
  _2d_vertex = new vertex2dtl[_num_vertex];
  _color = new uint32[_num_vertex];
  if (have_skin)
  {
    _skinned_vertex = new vect3d[num_vertex];
    _skinned_normal = new vect3d[num_vertex];
  }
}

void IrisImportMesh::Destroy(void)
{
  printf("[INFO] Deleting mesh '%s'\n", _name);
  if (_vertex != NULL)
    delete[] _vertex;
  if (_skin_index != NULL)
    delete[] _skin_index;
  if (_skin_indices != NULL)
    delete[] _skin_indices;
  if (_skin_weight != NULL)
    delete[] _skin_weight;
  if (_skinned_normal != NULL)
    delete[] _skinned_normal;
  if (_skinned_vertex != NULL)
    delete[] _skinned_vertex;
  if (_matrix != NULL)
    delete[] _matrix;
  if (_trans_vertex != NULL)
    delete[] _trans_vertex;
  if (_normal != NULL)
    delete[] _normal;
  if (_trans_normal != NULL)
    delete[] _trans_normal;
  if (_2d_vertex != NULL)
    delete[] _2d_vertex;
  if (_section != NULL)
  {
    delete[] _section[0]._indice;
    delete[] _section;
  }
  if (_color != NULL)
    delete[] _color;
  if (_user_data != NULL)
    delete[] _user_data;

  _matrix = NULL;
  _skin_index = NULL;
  _skin_indices = NULL;
  _skin_weight = NULL;
  _skinned_vertex = NULL;
  _skinned_normal = NULL;
  _color = NULL;
  _vertex = NULL;
  _section = NULL;
  _2d_vertex = NULL;
  _trans_normal = NULL;
  _normal = NULL;
  _trans_vertex = NULL;
  _num_vertex = 0;
  _num_section = 0;
  _num_skinned = 0;
  _material = 0;
}

void IrisImportMesh::SetUserDataSize(int size)
{
  if (_user_data)
    delete[] _user_data;
  _user_data = new char[size];
}

void IrisImportMesh::ComputeCenter()
{
  _center._x = _center._y = _center._z = 0.0f;
  int num_vertex = GetNumVertex();
  if (num_vertex)
  {
    for (int j = 0; j < GetNumVertex(); ++j)
      _center += *GetVertex(j);
    _center._x /= num_vertex;
    _center._y /= num_vertex;
    _center._z /= num_vertex;
  }
}
void IrisImportMesh::ComputeRadius()
{
  _radius = 0.0f;
  for (int j = 0; j < GetNumVertex(); ++j)
  {
    //FIXME?
    //vect3d tmp = (_center - *GetVertex(j));
    vect3d tmp = (*GetVertex(j) - _center);
    vect3d vect(tmp._x, tmp._y, tmp._z);
    if (vect.Magnitude() > _radius)
      _radius = vect.Magnitude();
  }
}

void IrisImportMesh::UpdateColorBuffer()
{
  vertex2dtl *v = _2d_vertex;
  uint32 *color = _color;

  for (int i = 0; i < _num_vertex; ++i)
  {
    v->argb = *color;
    v->oargb = *color;
    color++;
    v++;
  }
}

IrisImportObject::IrisImportObject()
{
  _total_num_vertex = 0;
  _total_num_strip = 0;
  _total_num_indice = 0;
  _num_tag = 0;
  _num_mesh = 0;
  _num_anim = 0;
  _tag = NULL;
  _mesh = NULL;
  _material = NULL;
  _num_material = 0;
  _file_name = NULL;
}

IrisImportObject::~IrisImportObject()
{
  Destroy();
}

void IrisImportObject::Destroy()
{
  printf("[INFO] Deleting object %s\n", _name);
  if (_tag != NULL)
    delete[] _tag;
  if (_mesh != NULL)
    delete[] _mesh;
  if (_num_material)
  {

    for (int i = 0; i < _num_material; ++i)
    {
      printf("[INFO] Deleting material '%s'\n", _material[i]->GetTexture(0)->GetFileName());
      IrisContext::Get().GetMaterialManager().Unload(_material[i]);
    }
    delete[] _material;
  }
  if (_file_name)
    delete[] _file_name;
  _file_name = NULL;
  _tag = NULL;
  _mesh = NULL;
  _num_material = 0;
  _material = NULL;
  _total_num_vertex = 0;
  _total_num_strip = 0;
  _total_num_indice = 0;
  _num_tag = 0;
  _num_mesh = 0;
  _num_anim = 0;
}

//////////////////////////////////////////////////////////////////////////
// imd v1.0 loading.
//////////////////////////////////////////////////////////////////////////
void IrisImportObject::LoadImdMesh(imd_object_t *imd_object, float scale)
{
  SetNumMesh(imd_object->imd_object_header.num_mesh);
  for (int mesh_index = 0; mesh_index < imd_object->imd_object_header.num_mesh; ++mesh_index)
  {
    imd_mesh_t *mesh = imd_object->imd_mesh + mesh_index;
    IrisImportMesh &imesh = GetMesh(mesh_index);
    int count = mesh->imd_mesh_header.num_vertex * imd_object->imd_object_header.num_anim;
    strcpy(imesh.GetName(), mesh->imd_mesh_header.name);
    imesh.Allocate(mesh->imd_mesh_header.num_vertex, imd_object->imd_object_header.num_anim);
    _total_num_vertex += imesh.GetNumVertex();
    vect3d *vertex = imesh.GetVertex();
    vect3d *normal = imesh.GetNormal();
    for (int i = 0; i < count; ++i)
    {

      vertex[i]._pos[0] = scale * mesh->imd_vertex[i].pos[0];
      vertex[i]._pos[1] = scale * mesh->imd_vertex[i].pos[1];
      vertex[i]._pos[2] = scale * mesh->imd_vertex[i].pos[2];
      normal[i]._pos[0] = mesh->imd_vertex[i].normal[0];
      normal[i]._pos[1] = mesh->imd_vertex[i].normal[1];
      normal[i]._pos[2] = mesh->imd_vertex[i].normal[2];
      normal[i]._pos[3] = 1.0f;
      normal[i].Normalize();
    }

    vertex2dtl *vertex_2d = imesh.Get2DVertex();
    uint32 *color = imesh.GetColor();
    for (int i = 0; i < imesh.GetNumVertex(); i++)
    {
      vertex_2d[i].u = mesh->imd_vertex[i].uv[0];
      vertex_2d[i].v = mesh->imd_vertex[i].uv[1];
      vertex_2d[i].argb = mesh->imd_vertex[i].color;
      vertex_2d[i].oargb = mesh->imd_vertex[i].color;
      color[i] = mesh->imd_vertex[i].color;
    }
    imesh.SetNumSection(mesh->imd_face.num_section);
    // assign good material to mesh
    for (int i = 0; i < GetNumMaterial(); ++i)
      if (GetMaterial(i)->GetTempId() == mesh->imd_mesh_header.material_id)
        imesh.SetMaterial(GetMaterial(i));

    if (imesh.GetNumSection() > 0)
    {
      int num_indice;
      unsigned short *indices;
      imd_face_section_s *src_section;
      IrisSection *dst_section;
      src_section = &mesh->imd_face.imd_section[0];
      num_indice = 0;
      for (int i = 0; i < imesh.GetNumSection(); ++i) // Total size should be precalculated in IMD
      {
        num_indice += src_section->num_indice;
        _total_num_strip++;
        src_section++;
      }
      _total_num_indice += num_indice;
      indices = new unsigned short[num_indice];
      src_section = &(mesh->imd_face.imd_section[0]);
      // !! WARNING TO BE TESTES i don't remenber if
      // address of a referenece is the real
      // address :(.
      dst_section = &(imesh.GetSection(0));
      num_indice = 0;
      for (int i = 0; i < imesh.GetNumSection(); ++i)
      {
        dst_section->_indice = indices + num_indice;
        dst_section->_num_indice = src_section->num_indice;
        memcpy(dst_section->_indice, src_section->indice, dst_section->_num_indice * sizeof(unsigned short));
        num_indice += dst_section->_num_indice;
        src_section++;
        dst_section++;
      }
    }
  }
}

bool IrisImportObject::LoadFromImdFile(char *file_path, char *file_name, float scale)
{
  imd_object_t *imd_object = load_imd(_file_name);
  if (imd_object == NULL)
    return (false);
  memcpy(_name, imd_object->imd_object_header.name, IMD_MAX_NAME);
  _num_anim = imd_object->imd_object_header.num_anim;
  //////////////////////////////////////////////////////////////////////////
  // material loading.
  SetNumMaterial(imd_object->imd_object_header.num_material);
  for (int i = 0; i < imd_object->imd_object_header.num_material; i++)
  {
    char *file_path_and_name = GetFilePath(file_path, imd_object->imd_material[i].file_name);
    IrisMaterial *the_mat = IrisContext::Get().GetMaterialManager().LoadMaterial(file_path_and_name);
    delete[] file_path_and_name;
    the_mat->SetTempId(imd_object->imd_material[i].material_id);
    _material[i] = the_mat;
  }
  //////////////////////////////////////////////////////////////////////////
  // Mesh converting.
  LoadImdMesh(imd_object, scale);
  //////////////////////////////////////////////////////////////////////////
  // Tag converting.
  SetNumTag(imd_object->imd_object_header.num_tag, imd_object->imd_object_header.num_anim);
  int offset = 0;
  for (int i = 0; i < imd_object->imd_object_header.num_tag; i++)
  {
    strcpy(_tag[i].GetName(), imd_object->imd_tag[offset].name);
    for (int anim = 0; anim < imd_object->imd_object_header.num_anim; ++anim)
    {
      vect3d &tag_pos = _tag[i].GetPos(anim);
      tag_pos.x = imd_object->imd_tag[offset].pos[0];
      tag_pos.y = imd_object->imd_tag[offset].pos[1];
      tag_pos.z = imd_object->imd_tag[offset].pos[2];
      offset++;
    }
  }
  free_imd(imd_object);
  return true;
}

//////////////////////////////////////////////////////////////////////////
// imd v2.0 loading.
//////////////////////////////////////////////////////////////////////////
int GetSkinOffset(int bone_index, int *skin_index)
{
  int offset = 0;
  while (bone_index--)
    offset += skin_index[bone_index];
  return offset;
}

void IrisImportObject::LoadImd2Mesh(imd2_object_t *imd_object, float scale)
{
  SetNumMesh(imd_object->imd2_object_header.num_mesh);
  for (int mesh_index = 0; mesh_index < imd_object->imd2_object_header.num_mesh; ++mesh_index)
  {
    imd2_mesh_t *mesh = imd_object->imd2_mesh + mesh_index;
    IrisImportMesh &imesh = GetMesh(mesh_index);
    int count = mesh->imd2_mesh_header.num_vertex * imd_object->imd2_object_header.num_anim;
    strcpy(imesh.GetName(), mesh->imd2_mesh_header.name);
    imesh.Allocate(mesh->imd2_mesh_header.num_vertex, imd_object->imd2_object_header.num_anim,
                   imd_object->imd2_object_header.matrix_sampling,
                   mesh->imd2_mesh_header.have_skin && mesh->imd2_skin);
    if (strlen(mesh->user_data) > 0)
    {
      imesh.SetUserDataSize(strlen(mesh->user_data) + 1);
      strcpy(imesh.GetUserData(), mesh->user_data);
      printf("[INFO] User data for mesh '%s' = '%s'\n", mesh->imd2_mesh_header.name, imesh.GetUserData());
    }
    _total_num_vertex += imesh.GetNumVertex();
    vect3d *vertex = imesh.GetVertex();
    vect3d *normal = imesh.GetNormal();
    vect3d *skinned_vertex = imesh.GetSkinnedVertex();
    vect3d *skinned_normal = imesh.GetSkinnedNormal();
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < count; ++i)
    {
      vertex[i]._pos[0] = scale * mesh->imd2_vertex[i].pos[0];
      vertex[i]._pos[1] = scale * mesh->imd2_vertex[i].pos[1];
      vertex[i]._pos[2] = scale * mesh->imd2_vertex[i].pos[2];
      vertex[i]._pos[3] = 1.0f;
      normal[i]._pos[0] = mesh->imd2_vertex[i].normal[0];
      normal[i]._pos[1] = mesh->imd2_vertex[i].normal[1];
      normal[i]._pos[2] = mesh->imd2_vertex[i].normal[2];
      normal[i]._pos[3] = 1.0f;
      normal[i].Normalize();
      if (mesh->imd2_mesh_header.have_skin && mesh->imd2_skin)
      {
        skinned_vertex[i] = vertex[i];
        skinned_normal[i] = normal[i];
      }
    }
    vertex2dtl *vertex_2d = imesh.Get2DVertex();
    uint32 *color = imesh.GetColor();
    for (int i = 0; i < imesh.GetNumVertex(); i++)
    {
      vertex_2d[i].u = mesh->imd2_vertex[i].uv[0];
      vertex_2d[i].v = mesh->imd2_vertex[i].uv[1];
      vertex_2d[i].argb = mesh->imd2_vertex[i].color;
      vertex_2d[i].oargb = mesh->imd2_vertex[i].color;
      color[i] = mesh->imd2_vertex[i].color;
    }
    imesh.SetNumSection(mesh->imd2_face.num_section);
    // assign good material to mesh
    for (int i = 0; i < GetNumMaterial(); ++i)
    {
      if (GetMaterial(i)->GetTempId() == mesh->imd2_mesh_header.material_id)
        imesh.SetMaterial(GetMaterial(i));
    }
    if (imesh.GetNumSection() > 0)
    {
      int num_indice;
      unsigned short *indices;
      imd2_face_section_s *src_section;
      IrisSection *dst_section;
      src_section = &mesh->imd2_face.imd2_section[0];
      num_indice = 0;
      for (int i = 0; i < imesh.GetNumSection(); ++i) // Total size should be precalculated in IMD
      {
        num_indice += src_section->num_indice;
        _total_num_strip++;
        src_section++;
      }
      _total_num_indice += num_indice;
      indices = new unsigned short[num_indice];
      src_section = &(mesh->imd2_face.imd2_section[0]);
      dst_section = &(imesh.GetSection(0));
      num_indice = 0;
      for (int i = 0; i < imesh.GetNumSection(); ++i)
      {
        dst_section->_indice = indices + num_indice;
        dst_section->_num_indice = src_section->num_indice;
        memcpy(dst_section->_indice, src_section->indice,
               dst_section->_num_indice * sizeof(unsigned short));
        num_indice += dst_section->_num_indice;
        src_section++;
        dst_section++;
      }
    }
    if (mesh->imd2_mesh_header.have_skin && mesh->imd2_skin)
    {
      int skinned_count = mesh->imd2_mesh_header.num_skinned;
      int bone_count = imd_object->imd2_object_header.num_bones;
      imesh.AllocateSkin(skinned_count, bone_count);
      int *skin_index = imesh.GetSkinIndex();
      float *skin_weight = imesh.GetSkinWeight();
      int *skin_indices = imesh.GetSkinIndices();

      memset(skin_index, 0, sizeof(*skin_index) * skinned_count);
      memset(skin_weight, 0, sizeof(*skin_weight) * skinned_count);
      memset(skin_indices, 0, sizeof(*skin_indices) * bone_count);

      // for each bones ... count max index for each bones.
      int *tmp = new int[bone_count];
      memset(tmp, 0, sizeof(*tmp) * bone_count);
      for (int i = 0; i < mesh->imd2_mesh_header.num_vertex; ++i)
      {
        imd2_skin_t *skin = mesh->imd2_skin + i;
        for (int b = 0; b < skin->num_bones_assigned; ++b)
        {
          imd2_weight_t *weight = skin->weight + b;
          skin_indices[weight->bone_index]++;
        }
      }
      for (int i = 0; i < mesh->imd2_mesh_header.num_vertex; ++i)
      {
        imd2_skin_t *skin = mesh->imd2_skin + i;
        for (int b = 0; b < skin->num_bones_assigned; ++b)
        {
          imd2_weight_t *weight = skin->weight + b;
          int bone_index = weight->bone_index;
          int offset = GetSkinOffset(bone_index, skin_indices) + tmp[bone_index];
          skin_weight[offset] = weight->weight;
          skin_index[offset] = i;
          tmp[bone_index]++;
        }
      }
      delete tmp;
    }
  }
}

bool IrisImportObject::LoadFromImd2File(char *file_path, char *file_name, float scale)
{
  imd2_object_t *imd_object = load_imd2(_file_name);
  if (imd_object == NULL)
    return (false);
  memcpy(_name, imd_object->imd2_object_header.name, IMD_MAX_NAME);
  _num_anim = imd_object->imd2_object_header.num_anim;
  //////////////////////////////////////////////////////////////////////////
  // material loading.
  SetNumMaterial(imd_object->imd2_object_header.num_material);
  for (int i = 0; i < imd_object->imd2_object_header.num_material; i++)
  {
    char *file_path_and_name = GetFilePath(file_path, imd_object->imd2_material[i].file_name);
    IrisMaterial *the_mat = IrisContext::Get().GetMaterialManager().LoadMaterial(file_path_and_name);
    delete[] file_path_and_name;
    the_mat->SetTempId(imd_object->imd2_material[i].material_id);
    _material[i] = the_mat;
  }
  //////////////////////////////////////////////////////////////////////////
  // Mesh converting.
  printf("[INFO] Loading mesh ...\n");
  LoadImd2Mesh(imd_object, scale);
  //////////////////////////////////////////////////////////////////////////
  // Tag converting.
  printf("[INFO] Loading tag ...\n");
  SetNumTag(imd_object->imd2_object_header.num_tag,
            imd_object->imd2_object_header.num_anim);
  int offset = 0;
  for (int i = 0; i < imd_object->imd2_object_header.num_tag; i++)
  {
    strcpy(_tag[i].GetName(), imd_object->imd2_tag[i].name);
    _tag[i].SetUserDataSize(strlen(imd_object->imd2_tag[i].user_data) + 1);
    strcpy(_tag[i].GetUserData(), imd_object->imd2_tag[i].user_data);
    for (int anim = 0; anim < imd_object->imd2_object_header.num_anim; ++anim)
    {
      vect3d &tag_pos = _tag[i].GetPos(anim);
      tag_pos.x = imd_object->imd2_tag[i].tag_data[anim].pos[0];
      tag_pos.y = imd_object->imd2_tag[i].tag_data[anim].pos[1];
      tag_pos.z = imd_object->imd2_tag[i].tag_data[anim].pos[2];
      offset++;
    }
  }
  free_imd2(imd_object);
  return true;
}

bool IrisImportObject::LoadFromFile(char *file_path, char *file_name, float scale)
{
  _total_num_indice = 0;
  _total_num_strip = 0;
  _total_num_vertex = 0;
  _file_name = GetFilePath(file_path, file_name);
  if (!LoadFromImdFile(file_path, file_name, scale))      // try to load an imd v1 file.
    return LoadFromImd2File(file_path, file_name, scale); // try to load an imd v2 file.
  return true;
}
