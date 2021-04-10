#include "stdafx.h"
#include "../../imd2/imd2.hpp"
#include "../../pluginsapi/pluginsapi.h"
#include "octree.h"

static imdviewer_plugin_info_t plugins_info =
    {
        "Octree generator plugins.",
        "Generate an octree from an imd file."
        "This Octree will be saved into a separate file."
        "This file can be used with the octree lib gived as sample for iris.",
        "Chatry Olivier",
        true //	display box for octree.
};

BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

void PluginOctree::PluginDestroy()
{
}

bool PluginOctree::PluginGo(const char *path, imd2_object_t *object, imd2_bone_file_t *bones)
{
  return false;
}

void PluginOctree::PluginInit()
{
}

void PluginOctree::PluginRender(unsigned int current_anim, imd2_bone_file_t *bones)
{
}
DLLAPI imdviewer_plugin_info_t *GetPluginInfo()
{
  return &plugins_info;
}

DLLAPI ImdViewerPlugins *CreateInstance()
{
  return new PluginOctree;
}
