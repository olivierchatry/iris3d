#include "stdafx.h"
#include "../../imd2/imd2.hpp"
#include "../../pluginsapi/pluginsapi.h"
#include "ambient.h"
#include <CommDlg.h>

static imdviewer_plugin_info_t	plugins_info =
{
	"Ambient color.",
	"Set ambient color on vertex color.",
	"Chatry Olivier",
	false
};

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
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



void	PluginAmbient::PluginDestroy()
{
    // does nothing.
}

bool	PluginAmbient::PluginGo(const char *path, imd2_object_t *object, imd2_bone_file_t *bones)
{	
	if (object == 0)
		return false;
	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};
	cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
	cc.hwndOwner = NULL;
	cc.rgbResult = _rgb_current;
	cc.lpCustColors = _rgb_custom;
	if(ChooseColor(&cc))
	{
		for (int imesh = 0; imesh < object->imd2_object_header.num_mesh; ++imesh)
		{
			imd2_mesh_t	*mesh = &(object->imd2_mesh[imesh]);
			for (int a = 0; a < object->imd2_object_header.num_anim; ++a)
			{
				int	offset = mesh->imd2_mesh_header.num_vertex * 0;
				for (int v = 0; v < mesh->imd2_mesh_header.num_vertex; ++v)
					mesh->imd2_vertex[v + offset].color = cc.rgbResult;
			}
		}
		return true;
	}		
	return false;
}

void	PluginAmbient::PluginInit()
{
	_rgb_current = 0;
	memset(_rgb_custom, 0, sizeof(COLORREF) * 16);
}

void	PluginAmbient::PluginRender(unsigned int current_anim, imd2_bone_file_t *bones)
{
    // does nothing.
}
DLLAPI imdviewer_plugin_info_t	*GetPluginInfo()
{
	return &plugins_info;
}

DLLAPI ImdViewerPlugins			*CreateInstance()
{
	return new PluginAmbient;
}

