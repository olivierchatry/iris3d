#ifndef __PLUGINSAPI_H__
#define __PLUGINSAPI_H__

#ifdef EXPORTS
	#define DLLAPI	__declspec(dllexport)
#else
	#define DLLAPI	__declspec(dllimport)
#endif

struct imdviewer_plugin_info_t
{
	char	*name;
	char	*description;
	char	*author;
	bool	display_data;
};

class	ImdViewerPlugins	
{
public:
	virtual bool PluginGo(const char *path, imd2_object_t *object, imd2_bone_file_t *bones) = 0;
	virtual	 void PluginRender(unsigned int current_anim, imd2_bone_file_t *bones) = 0;
	virtual void PluginInit() = 0;
	virtual void PluginDestroy() = 0;
};

DLLAPI imdviewer_plugin_info_t	*GetPluginInfo();
DLLAPI ImdViewerPlugins			*CreateInstance();

#endif