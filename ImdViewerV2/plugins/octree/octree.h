#ifndef __OCTREE_H__
#define __OCTREE_H__

class  DLLAPI  PluginOctree : public ImdViewerPlugins
{
public:
	bool PluginGo(const char *path,imd2_object_t *object, imd2_bone_file_t *bones);
	void PluginRender(unsigned int current_anim, imd2_bone_file_t *bones);
	void PluginInit();
	void PluginDestroy();
};

#endif