#include "stdafx.h"
#include "efbbox.h"
#include "magic/MgcBox3.h"
#include "magic/MgcVector3.h"

static imdviewer_plugin_info_t	plugins_info =
{
	"Epifighter BoundingBox generator.",
	"Generate bounding box around bones.",
	"Chatry Oliver, Julien Barbier.",
	true	//	display box for octree.
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


void	PluginEfBox::PluginDestroy()
{
	if (_bbox)
		delete _bbox;
	_bbox = 0;
	_bbox_count = 0;
}

#define DEFAULT_FILE_NAME	"\\bones_bbox.dat"
#define NB_POINTS_PER_BBOX	8
#define NB_COORD_PER_POINT	3

void			PluginEfBox::SaveToFile(const char *path)
{
	int			i;
	std::string	file_name(path);
	
	file_name += DEFAULT_FILE_NAME;
	FILE		*fd = fopen(file_name.c_str(), "wb");
	if (fd == NULL)
		return;
	fwrite(&_bbox_count, sizeof(_bbox_count), 1, fd);		
	for (i = 0; i < _bbox_count; i++)
		fwrite(&_bbox[i], sizeof(_bbox[i]), 1, fd);
	fclose(fd);
}

bool	PluginEfBox::PluginGo(const char *path, imd2_object_t *object, imd2_bone_file_t *bones)
{
	if (object == 0 || !object->imd2_object_header.have_skin)
		return false;
	PluginDestroy();
	_bbox = new BoundingBox[object->imd2_object_header.num_bones];
	_bbox_count = object->imd2_object_header.num_bones;
	for (int bi = 0; bi < _bbox_count; bi ++)
	{
		std::vector<Mgc::Vector3> mesh_point;
		mesh_point.clear();
		for (int mi = 0; mi < object->imd2_object_header.num_mesh; mi++)
		{
			imd2_mesh_t		*mesh = object->imd2_mesh + mi;
			if (mesh->imd2_mesh_header.have_skin)
			{
				for (int vi = 0; vi < mesh->imd2_mesh_header.num_vertex; vi++)
				{
					imd2_skin_t		*skin = mesh->imd2_skin + vi;
					imd2_vertex_t	*vertex = mesh->imd2_vertex + vi;
					int				bone_index = -1;
					float			max_weight = 0.0f;
					for (int wi = 0; wi < skin->num_bones_assigned; wi++)
					{
						imd2_weight_t *weight = skin->weight + wi;
						if (weight->weight > max_weight)
						{
							bone_index = weight->bone_index;
							max_weight = weight->weight;
						}
					}
					if (bone_index != -1 && bone_index == bi)
					{
						
						Mgc::Vector3	v(vertex->pos);
						mesh_point.push_back(v);
					}
				}
			}
		}
		size_t		count = mesh_point.size();
	
		Mgc::Vector3	*pt = new Mgc::Vector3[count];
		for (size_t i = 0; i < count ; ++i)
			pt[i] = mesh_point[i];
		// compute bounding box.
		Mgc::Box3	magic_bbox;
		BoundingBox	*bbox = _bbox + bi;
		if (mesh_point.size() > 0)
		{
			bbox->box = ContOrientedBox((int) mesh_point.size(), pt);
			bbox->bone_index = bi;
		}
		else
			bbox->bone_index = -1;
		delete pt;
	}
	SaveToFile(path);
	return false;
}

void	PluginEfBox::PluginInit()
{
	_bbox = 0;
	_bbox_count = 0;
}

void	PluginEfBox::PluginRender(unsigned int current_anim, imd2_bone_file_t *bones)
{
	for (int i = 0; i < _bbox_count; ++i)
	{
		BoundingBox	*bbox = _bbox + i;
		int	pair[] = {3, 2, 2, 1, 1, 0, 0, 3, 1, 5, 2, 6, 3, 7, 0, 4, 4, 7, 7, 6, 6, 5, 5, 4};
		Mgc::Vector3	p[8];
		bbox->box.ComputeVertices(p);
		// search for bone index
		glPushMatrix();
		if (bones)
			for (int i = 0; i < bones->imd2_bone_file_header.bone_count; ++i)
			{
				if (bones->bones[i].imd2_bone_header.bone_index == bbox->bone_index)
				{
					glMultMatrixf(bones->bones[i].imd2_bone_anim[current_anim].matrix);
					continue;
				}
			}
		glBegin(GL_LINES);
		for (int i = 0; i < 24; i += 2)
		{
			glVertex3fv(p[pair[i]]);		
			glVertex3fv(p[pair[i + 1]]);		
		}	
		glEnd();
		glPopMatrix();
	}
}


DLLAPI imdviewer_plugin_info_t	*GetPluginInfo()
{
	return &plugins_info;
}

DLLAPI ImdViewerPlugins			*CreateInstance()
{
	return new PluginEfBox;
}

