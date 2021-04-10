#ifndef __OCTREE_H__
#define __OCTREE_H__

struct DLLAPI BoundingBox
{
  Mgc::Box3 box;
  int bone_index;
};

class DLLAPI PluginEfBox : public ImdViewerPlugins
{
public:
  bool PluginGo(const char *path, imd2_object_t *object, imd2_bone_file_t *bones);
  void PluginRender(unsigned int current_anim, imd2_bone_file_t *bones);
  void PluginInit();
  void PluginDestroy();
  BoundingBox *_bbox;
  int _bbox_count;

protected:
  void SaveToFile(const char *path);
};

#endif