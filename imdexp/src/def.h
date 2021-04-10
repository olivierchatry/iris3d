#pragma once
#include <max.h>
#include <vector>
#include <map>
#include <stdio.h>

typedef unsigned int uint;
typedef unsigned char uchar;

struct vect3d
{
  float _x;
  float _y;
  float _z;
};

struct matrix
{
  float _m[4 * 4];
};
enum element_type
{
  none_element = 0,
  tag_element,
  skin_element,
  bone_element,
  mesh_element,
  light_element,
  material_element

};
class ImportedElements
{
public:
  ImportedElements()
  {
    _type = none_element;
  };
  virtual ~ImportedElements(){};
  virtual void Save(FILE *file){};
  virtual void Load(FILE *file){};

  std::string _name;
  element_type _type;

protected:
};

//////////////////////////////////////////////////////////////////////////
// light
//////////////////////////////////////////////////////////////////////////
struct LightData
{
  LightData()
  {
    _pos.Set(0.0f, 0.0f, 0.0f);
    _dir.Set(0.0f, 0.0f, 0.0f);
  }
  LightData(Point3 pos, Point3 dir) : _pos(pos), _dir(dir) {}
  Point3 _pos;
  Point3 _dir;
};

class ImportedLight : public ImportedElements
{
public:
  enum light_type
  {
    light_omni,
    light_directionnal
  };
  ImportedLight()
  {
    _type = light_element;
  };
  virtual ~ImportedLight()
  {
    _data.resize(0);
  }
  virtual void Save(FILE *file);
  virtual void Load(FILE *file);
  uint _color;
  light_type _light_type;
  bool _have_target;
  std::vector<LightData> _data;
};

inline void ImportedLight::Save(FILE *file)
{
  fprintf(file, "--- OBJECT LIGHT\n");
  fprintf(file, "type	   : %s\n", _type == light_omni ? "light_omni" : "light_directionnal");
  fprintf(file, "color     : %2d %2d %2d", (_color >> 16) & 0xff, (_color >> 8) & 0xff, (_color & 0xff));
  fprintf(file, "animation : %2d\n", _data.size());
  for (uint i = 0; i < _data.size(); ++i)
  {
    fprintf(file, "\t[%f %f %f]", _data[i]._pos.x, _data[i]._pos.y, _data[i]._pos.z);
    fprintf(file, " dir [%f %f %f]\n", _data[i]._dir.x, _data[i]._dir.y, _data[i]._dir.z);
  }
}

inline void ImportedLight::Load(FILE *file)
{
}

struct UVMapping
{
  UVVert _uv;
  bool _filled;
  int _tvertex_index;
};

typedef std::vector<Point3> point3_list_t;
typedef std::vector<UVMapping> uvvert_list_t;
typedef std::vector<VertColor> vertcolor_list_t;
typedef std::vector<Point2> point2_list_t;
typedef std::vector<uint> uint_list_t;
typedef signed char sbyte;
typedef unsigned char ubyte;
typedef signed short sword;
typedef unsigned short uword;
typedef signed int sdword;
typedef unsigned int udword;
typedef signed __int64 sqword;
typedef unsigned __int64 uqword;
typedef float sfloat;
typedef std::vector<uword> uword_list_t;

//////////////////////////////////////////////////////////////////////////
// Material def
//////////////////////////////////////////////////////////////////////////

struct MaterialData
{
  char *_name;
  char *_diffuse_map;
  char *_env_map;
  void *_data; // used for texturing.

  bool operator==(const MaterialData &to_cmp)
  {
    return (strcmp(to_cmp._name, _name) == 0); // same max id
  }

  MaterialData &operator=(const struct MaterialData &from)
  {
    _name = from._name;
    _diffuse_map = from._diffuse_map;
    _env_map = from._env_map;
  }
};

class ImportedMaterial : public ImportedElements
{
public:
  ImportedMaterial()
  {
    _type = material_element;
  };
  virtual ~ImportedMaterial()
  {
    material_data_list_it_t it;
    for (it = _material_data.begin(); it != _material_data.end(); ++it)
    {
      delete *it;
    }
    _material_data.clear();
  };
  virtual void Save(FILE *file){};
  virtual void Load(FILE *file){};
  typedef std::vector<MaterialData *> material_data_list_t;
  typedef material_data_list_t::iterator material_data_list_it_t;
  material_data_list_t _material_data;
};

//////////////////////////////////////////////////////////////////////////
// mesh
//////////////////////////////////////////////////////////////////////////
struct MeshStrip
{
  ~MeshStrip()
  {
    _face_index.clear();
  }
  void Allocate(uint count)
  {
    _face_index.resize(count);
  }
  uword_list_t _face_index;
};

typedef std::vector<MeshStrip> meshstrip_list_t;

struct MeshData
{
  ~MeshData()
  {
  }
  void Allocate(uint count)
  {
    _vertex.resize(count);
    _normal.resize(count);
  }
  Matrix3 _matrix;
  point3_list_t _vertex;
  point3_list_t _normal;
};

typedef std::vector<MeshData> meshdata_list_t;
typedef std::vector<MeshData>::iterator meshdata_list_it_t;

struct MeshColorMapping
{
  uvvert_list_t _mapping;
  vertcolor_list_t _color;
  void Allocate(uint count)
  {
    VertColor p;
    p.x = p.y = p.z = 1.0f;
    _mapping.resize(count);
    _color.resize(count, p);
  }
};
typedef std::vector<MeshColorMapping> meshcolormapping_list_t;
typedef std::vector<MeshColorMapping>::iterator meshcolormapping_list_it_t;

// vertice added for one one correspondance
// veetween vertice and mapping.
struct MapVertice
{
  MapVertice(int original, int i)
  {
    _original_indice = original;
    _indice = i;
  }

  int _original_indice;
  int _indice;
};
class ImportedSkin;
class ImportedMesh : public ImportedElements
{
public:
  ImportedMesh() : _material(0)
  {
    _type = mesh_element;
  };
  virtual ~ImportedMesh()
  {
    _mesh_data.resize(0);
    _strip.resize(0);
  };
  virtual void Save(FILE *file);
  virtual void Load(FILE *file){};
  int _face_total;
  int _vertex_total;
  meshdata_list_t _mesh_data;
  meshstrip_list_t _strip;
  MeshColorMapping _mesh_color_mapping;
  MaterialData *_material;
  ImportedSkin *_skin;
  std::vector<MapVertice> _map_vertices;
  std::string _user_properties;
};

inline void ImportedMesh::Save(FILE *file)
{
  fprintf(file, "--- OBJECT MESH\n");
  fprintf(file, "number of strips : %d\n", _strip.size());
  fprintf(file, "number of face : %d\n", _face_total);
}

struct TagData
{
  Point3 _pos;
};

class ImportedTag : public ImportedElements
{
public:
  ImportedTag()
  {
    _type = tag_element;
  }
  ~ImportedTag()
  {
    tag_data_list_it_t it;
    for (it = _tag_data.begin(); it != _tag_data.end(); ++it)
    {
      delete *it;
    }
    _tag_data.clear();
  }
  virtual void Save(FILE *file) {}
  virtual void Load(FILE *file) {}
  typedef std::vector<TagData *> tag_data_list_t;
  typedef tag_data_list_t::iterator tag_data_list_it_t;
  tag_data_list_t _tag_data;
  std::string _user_properties;
};

//////////////////////////////////////////////////////////////////////////
// Bones definition
//////////////////////////////////////////////////////////////////////////

struct PointWeight
{
  int _point_index;
  float _weight;
};

struct SkinData
{
  std::vector<PointWeight> _point_weight;
  int _bone_index;
};

struct BoneAnim
{
  Matrix3 matrix;
  Point3 pos;
};

typedef std::vector<BoneAnim> boneanim_list_t;
typedef std::vector<BoneAnim>::iterator boneanim_list_it_t;

struct BoneData
{
  // animated bone.
  boneanim_list_t _animation;
  // bone index.
  int _bone_index;
  std::string _name;
  // child bones.
  typedef std::vector<BoneData> bone_data_list_t;
  typedef bone_data_list_t::iterator bone_data_list_it_t;
  bone_data_list_t _bone_child;
};

class ImportedSkin : public ImportedElements
{
public:
  ImportedSkin()
  {
    _type = skin_element;
  }
  ~ImportedSkin()
  {
    _skin_data.clear();
  }
  virtual void Save(FILE *file) {}
  virtual void Load(FILE *file) {}
  typedef std::map<INode *, SkinData> skin_data_list_t;
  typedef skin_data_list_t::iterator skin_data_list_it_t;
  skin_data_list_t _skin_data;
};

class ImportedBone : public ImportedElements
{
public:
  ImportedBone()
  {
    _type = bone_element;
  }
  ~ImportedBone()
  {
    _bone_data.clear();
  }
  virtual void Save(FILE *file) {}
  virtual void Load(FILE *file) {}
  BoneData::bone_data_list_t _bone_data;
};
