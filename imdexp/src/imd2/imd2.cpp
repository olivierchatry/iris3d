/*********************************************************
**  File name : imd2.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: [4/14/2003]
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdafx.h>
#include "imd2.hpp"

//////////////////////////////////////////////////////////////////////////
// imd2 object file format.

imd2_object_t	*load_imd2(const char *file_name)
{
	FILE					*file;
	int						iIndex;
	char					magic[4];
	imd2_object_t	*pImdObject;

	file = fopen(file_name, "rb");
	if (file == NULL)
		return (NULL);
	fread(magic, 1, 4, file); 
	if (memcmp(magic, IMD2_MAGIC_TAG, 4) != 0)
	{
		fclose(file);
		return (NULL);
	}
	pImdObject = new imd2_object_t;
	fread(&(pImdObject->imd2_object_header), sizeof(imd2_object_header_t), 1, file);
	if (pImdObject->imd2_object_header.num_material > 0)
	{
		pImdObject->imd2_material= new imd2_material_t[pImdObject->imd2_object_header.num_material];
		fread(pImdObject->imd2_material, sizeof(imd2_material_t), pImdObject->imd2_object_header.num_material, file);
	}
	if (pImdObject->imd2_object_header.num_tag > 0)
	{
		pImdObject->imd2_tag = new imd2_tag_t[pImdObject->imd2_object_header.num_tag];
		for (iIndex = 0; iIndex < pImdObject->imd2_object_header.num_tag ; iIndex ++)
		{
			int	size;
			fread(pImdObject->imd2_tag[iIndex].name, sizeof(char), IMD2_MAX_NAME, file);
			fread(&size, sizeof(size), 1, file);
			pImdObject->imd2_tag[iIndex].user_data = new char[size];
			fread(pImdObject->imd2_tag[iIndex].user_data, sizeof(char), size, file);
			pImdObject->imd2_tag[iIndex].tag_data = new imd2_tag_data_t[pImdObject->imd2_object_header.num_anim];
			fread(pImdObject->imd2_tag[iIndex].tag_data, sizeof(imd2_tag_data_t), pImdObject->imd2_object_header.num_anim , file);
		}
	}
	if (pImdObject->imd2_object_header.num_light > 0)
	{
		pImdObject->imd2_light = new imd2_light_t[pImdObject->imd2_object_header.num_light];
		fread(pImdObject->imd2_light , sizeof(imd2_light_t), pImdObject->imd2_object_header.num_light, file);
	}
	pImdObject->imd2_mesh = new imd2_mesh_t[pImdObject->imd2_object_header.num_mesh];
	for (iIndex = 0; iIndex < pImdObject->imd2_object_header.num_mesh; iIndex ++)
	{
		imd2_mesh_t *imd2_mesh = 	&(pImdObject->imd2_mesh[iIndex]);
		int size;
		// Reading mesh header 
		fread(&(imd2_mesh->imd2_mesh_header), sizeof(imd2_mesh_header_t), 1, file);
		// Reading user data.
		fread(&size, sizeof(size), 1, file);
		imd2_mesh->user_data = new char[size];
		fread(imd2_mesh->user_data, sizeof(char), size, file);
		// Reading vertex data
		if (pImdObject->imd2_object_header.matrix_sampling)
		{
			imd2_mesh->imd2_vertex = new imd2_vertex_t[imd2_mesh->imd2_mesh_header.num_vertex];
			fread(imd2_mesh->imd2_vertex, sizeof(imd2_vertex_t), imd2_mesh->imd2_mesh_header.num_vertex, file);
			imd2_mesh->imd2_matrix = new imd2_matrix_t[pImdObject->imd2_object_header.num_anim];
			fread(imd2_mesh->imd2_matrix, sizeof(imd2_matrix_t), pImdObject->imd2_object_header.num_anim , file);
		}
		else
		{
			imd2_mesh->imd2_vertex = new imd2_vertex_t[imd2_mesh->imd2_mesh_header.num_vertex * pImdObject->imd2_object_header.num_anim];
			fread(imd2_mesh->imd2_vertex, sizeof(imd2_vertex_t), imd2_mesh->imd2_mesh_header.num_vertex * pImdObject->imd2_object_header.num_anim , file);
		}
		//normalize_all(imd2_mesh->imd2_vertex, imd2_mesh->imd2_mesh_header.num_vertex * pImdObject->imd2_object_header.num_anim);
		fread(&(imd2_mesh->imd2_face.num_section) , sizeof(imd2_mesh->imd2_face.num_section), 1, file);
		imd2_mesh->imd2_face.imd2_section = new imd2_face_section_t[imd2_mesh->imd2_face.num_section];
		for (int iIndex_section = 0;iIndex_section < imd2_mesh->imd2_face.num_section; iIndex_section ++)
		{
			fread(&(imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice), sizeof(imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice), 1, file);
			imd2_mesh->imd2_face.imd2_section[iIndex_section].indice = new unsigned short[imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice];
			fread(imd2_mesh->imd2_face.imd2_section[iIndex_section].indice, sizeof(unsigned short), imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice, file);
		}
		imd2_mesh->imd2_skin = 0;
		if (imd2_mesh->imd2_mesh_header.have_skin)
		{
			// reading skinning data.
			imd2_mesh->imd2_skin = new imd2_skin_t[imd2_mesh->imd2_mesh_header.num_vertex];
			for(int iIndexVertex = 0; iIndexVertex < imd2_mesh->imd2_mesh_header.num_vertex; ++iIndexVertex)
			{
				imd2_skin_t *skin = &(imd2_mesh->imd2_skin[iIndexVertex]);
				fread(&(skin->num_bones_assigned), sizeof(int), 1, file);
				skin->weight = new imd2_weight_t[skin->num_bones_assigned];
				fread(skin->weight, sizeof(imd2_weight_t), skin->num_bones_assigned, file);
			}
		}
	}
	fclose(file);
	return (pImdObject);
}

void	save_imd2(imd2_object_t *pImdObject, const char *file_name)
{
	FILE	*file;
	int		iIndex;

	file = fopen(file_name, "wb");
	if (file == NULL)
		return;
	// look for light.
	if (pImdObject->imd2_light == 0)
		pImdObject->imd2_object_header.num_light = 0;

	// First, we write IMD magic number
	fwrite(IMD2_MAGIC_TAG, 1, 4, file);
	fwrite(&(pImdObject->imd2_object_header), sizeof(imd2_object_header_t), 1, file); // next we write object header
	// next, we write material data
	if (pImdObject->imd2_object_header.num_material > 0)
		fwrite(pImdObject->imd2_material, sizeof(imd2_material_t), pImdObject->imd2_object_header.num_material, file);
	// next, we write tag data
	if (pImdObject->imd2_object_header.num_tag > 0)
	{
		for (iIndex = 0; iIndex < pImdObject->imd2_object_header.num_tag ; iIndex ++)
		{
			fwrite(pImdObject->imd2_tag[iIndex].name, sizeof(char), IMD2_MAX_NAME, file);
			size_t	size = strlen(pImdObject->imd2_tag[iIndex].user_data) + 1; // include '\0'
			fwrite(&size, sizeof(size), 1, file);
			fwrite(pImdObject->imd2_tag[iIndex].user_data, sizeof(char), size, file);
			fwrite(pImdObject->imd2_tag[iIndex].tag_data, sizeof(imd2_tag_data_t), pImdObject->imd2_object_header.num_anim , file);
		}
	}
	// next, we write static light data
	if (pImdObject->imd2_object_header.num_light > 0)
		fwrite(pImdObject->imd2_light, sizeof(imd2_light_t), pImdObject->imd2_object_header.num_light, file);
	// next, we write all the mesh data
	for (iIndex = 0; iIndex < pImdObject->imd2_object_header.num_mesh; iIndex ++)
	{
		imd2_mesh_t *imd2_mesh = 	&(pImdObject->imd2_mesh[iIndex]);
		// writing mesh header 
		fwrite(&(imd2_mesh->imd2_mesh_header), sizeof(imd2_mesh_header_t), 1, file);
		size_t	size = strlen(imd2_mesh->user_data) + 1;
		fwrite(&size, sizeof(size), 1, file);
		fwrite(imd2_mesh->user_data, sizeof(char), size, file);
		// writing vertex data
		if (pImdObject->imd2_object_header.matrix_sampling)
		{
			fwrite(imd2_mesh->imd2_vertex, sizeof(imd2_vertex_t), imd2_mesh->imd2_mesh_header.num_vertex, file);
			fwrite(imd2_mesh->imd2_matrix, sizeof(imd2_matrix_t), pImdObject->imd2_object_header.num_anim , file);
		}
		else
			fwrite(imd2_mesh->imd2_vertex, sizeof(imd2_vertex_t), imd2_mesh->imd2_mesh_header.num_vertex * pImdObject->imd2_object_header.num_anim , file);
		//	now, writing face data
		//	note that vertex buffer must be generated at file loading
		//	first number of section
		fwrite(&(imd2_mesh->imd2_face.num_section) , sizeof(imd2_mesh->imd2_face.num_section), 1, file);
		for (int iIndex_section = 0;iIndex_section < imd2_mesh->imd2_face.num_section; iIndex_section ++)
		{
			// number of indice in current session
			fwrite(&(imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice), sizeof(imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice), 1, file);
			//	next the indice
			fwrite(imd2_mesh->imd2_face.imd2_section[iIndex_section].indice, sizeof(unsigned short), imd2_mesh->imd2_face.imd2_section[iIndex_section].num_indice, file);
		}
		if (imd2_mesh->imd2_mesh_header.have_skin)
		{
			// writting skinning data.
			for(int iIndexVertex = 0; iIndexVertex < imd2_mesh->imd2_mesh_header.num_vertex; ++iIndexVertex)
			{
				imd2_skin_t *skin = &(imd2_mesh->imd2_skin[iIndexVertex]);
				fwrite(&(skin->num_bones_assigned), sizeof(int), 1, file);
				fwrite(skin->weight, sizeof(imd2_weight_t), skin->num_bones_assigned, file);
			}
		}
	}
	fclose(file);
}

void	free_imd2(imd2_object_t *pImdObject)
{
	if (pImdObject == NULL)
		return;
	if (pImdObject->imd2_object_header.num_material > 0)
		delete [] pImdObject->imd2_material;
	if (pImdObject->imd2_object_header.num_tag > 0)
	{
		for (int i = 0; i < pImdObject->imd2_object_header.num_tag; ++i)
		{
			delete [] pImdObject->imd2_tag[i].user_data;
			delete [] pImdObject->imd2_tag[i].tag_data;
		}
		delete [] pImdObject->imd2_tag;
	}
	if (pImdObject->imd2_object_header.num_light > 0)
		delete [] pImdObject->imd2_light;
	for (int iIndex = 0; iIndex < pImdObject->imd2_object_header.num_mesh; iIndex ++)
	{
		imd2_mesh_t *imd2_mesh = 	&(pImdObject->imd2_mesh[iIndex]);
		if (pImdObject->imd2_object_header.matrix_sampling)
			delete [] imd2_mesh->imd2_matrix;
		delete [] imd2_mesh->imd2_vertex;
		delete [] imd2_mesh->user_data;
		for (int iIndex_section = 0;iIndex_section < imd2_mesh->imd2_face.num_section; iIndex_section ++)
			delete [] imd2_mesh->imd2_face.imd2_section[iIndex_section].indice;
		if (imd2_mesh->imd2_skin && pImdObject->imd2_object_header.have_skin)
		{
			for (int iIndexVertex = 0; iIndexVertex < imd2_mesh->imd2_mesh_header.num_vertex; ++iIndexVertex)
				delete [] imd2_mesh->imd2_skin[iIndexVertex].weight;
			delete [] imd2_mesh->imd2_skin;
		}
		delete [] imd2_mesh->imd2_face.imd2_section;
	}

	delete [] pImdObject->imd2_mesh;
	delete pImdObject;
}

//////////////////////////////////////////////////////////////////////////
// imd2 bone file format.

imd2_bone_file_t	*load_imd2_bone(const char *file_name)
{
	FILE				*file;
	char				magic[4];
	imd2_bone_file_t	*imd2_bones;
	
	file = fopen(file_name, "rb");
	if (file == NULL)
		return NULL;
	fread(magic, 1, 4, file); 
	if (memcmp(magic, IMD2_BONE_MAGIC_TAG, 4) != 0)
	{
		fclose(file);
		return (NULL);
	}
	imd2_bones = new imd2_bone_file_t;
	fread(&(imd2_bones->imd2_bone_file_header), sizeof(imd2_bone_file_header_t), 1, file);
	int	iBoneCount = imd2_bones->imd2_bone_file_header.bone_count;
	int	iAnimCount = imd2_bones->imd2_bone_file_header.anim_count;
	imd2_bones->bones = new imd2_bone_t[iBoneCount];
	for (int iIndexBone = 0; iIndexBone < iBoneCount; ++iIndexBone)
	{
		imd2_bone_t	*bone = &(imd2_bones->bones[iIndexBone]);
		fread(&(bone->imd2_bone_header), sizeof(imd2_bone_header_t), 1, file);
		bone->imd2_bone_anim = new imd2_bone_anim_t[iAnimCount];
		fread(bone->imd2_bone_anim, sizeof(imd2_bone_anim_t), iAnimCount, file);
	}
	fclose(file);
	return imd2_bones;
}

void			save_imd2_bone(imd2_bone_file_t *imd2_bones, const char *file_name)
{
	FILE				*file;
	
	file = fopen(file_name, "wb");
	if (file == NULL)
		return;
	fwrite(IMD2_BONE_MAGIC_TAG, 1, 4, file); 
	fwrite(&(imd2_bones->imd2_bone_file_header), sizeof(imd2_bone_file_header_t), 1, file);
	int	iBoneCount = imd2_bones->imd2_bone_file_header.bone_count;
	int	iAnimCount = imd2_bones->imd2_bone_file_header.anim_count;
	for (int iIndexBone = 0; iIndexBone < iBoneCount; ++iIndexBone)
	{
		imd2_bone_t	*bone = &(imd2_bones->bones[iIndexBone]);
		fwrite(&(bone->imd2_bone_header), sizeof(imd2_bone_header_t), 1, file);
		fwrite(bone->imd2_bone_anim, sizeof(imd2_bone_anim_t), iAnimCount, file);
	}
	fclose(file);
}

void			free_imd2_bone(imd2_bone_file_t *imd2_bones)
{
	int	iBoneCount = imd2_bones->imd2_bone_file_header.bone_count;
	int	iAnimCount = imd2_bones->imd2_bone_file_header.anim_count;
	for (int iIndexBone = 0; iIndexBone < iBoneCount; ++iIndexBone)
	{
		imd2_bone_t	*bone = &(imd2_bones->bones[iIndexBone]);
		delete [] 	bone->imd2_bone_anim;
	}
	delete [] imd2_bones->bones;
	delete imd2_bones;
}

