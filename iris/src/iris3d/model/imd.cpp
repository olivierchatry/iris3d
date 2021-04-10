/*********************************************************
**  File name : imd.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "model/imd.hpp"

float magnitude(imd_vertex_t *vertex)
{
  return (float)sqrt(vertex->normal[0] * vertex->normal[0] + vertex->normal[1] * vertex->normal[1] + vertex->normal[2] * vertex->normal[2]);
}

void normalize_all(imd_vertex_t *vertex, int num_vertex)
{
  for (int i = 0; i < num_vertex; ++i)
  {
    float mag = magnitude(&(vertex[i]));
    vertex[i].normal[0] /= mag;
    vertex[i].normal[1] /= mag;
    vertex[i].normal[2] /= mag;
  }
}

imd_object_t *load_imd(const char *file_name)
{
  FILE *file;
  int iIndex;
  char magic[4];
  imd_object_t *pImdObject;

  file = fopen(file_name, "rb");
  if (file == NULL)
    return (NULL);
  fread(magic, 1, 4, file);
  if (memcmp(magic, IMD_MAGIC_TAG, 4) != 0)
  {
    fclose(file);
    return (NULL);
  }
  pImdObject = new imd_object_t;
  fread(&(pImdObject->imd_object_header), sizeof(imd_object_header_t), 1, file);
  if (pImdObject->imd_object_header.num_object_link > 0)
  {
    pImdObject->imd_object_links = new imd_object_link_t[pImdObject->imd_object_header.num_object_link];
    fread(pImdObject->imd_object_links, sizeof(imd_object_link_t), pImdObject->imd_object_header.num_object_link, file);
  }
  if (pImdObject->imd_object_header.num_material > 0)
  {
    pImdObject->imd_material = new imd_material_t[pImdObject->imd_object_header.num_material];
    fread(pImdObject->imd_material, sizeof(imd_material_t), pImdObject->imd_object_header.num_material, file);
  }
  if (pImdObject->imd_object_header.num_tag > 0)
  {
    pImdObject->imd_tag = new imd_tag_t[pImdObject->imd_object_header.num_tag * pImdObject->imd_object_header.num_anim];
    fread(pImdObject->imd_tag, sizeof(imd_tag_t), pImdObject->imd_object_header.num_tag * pImdObject->imd_object_header.num_anim, file);
  }
  if (pImdObject->imd_object_header.num_anim_part > 0)
  {
    pImdObject->imd_anim_part = new imd_anim_part_t[pImdObject->imd_object_header.num_anim_part];
    fread(pImdObject->imd_anim_part, sizeof(imd_anim_part_t), pImdObject->imd_object_header.num_anim_part, file);
  }
  /*	if (pImdObject->imd_object_header.num_light > 0)
		{
			pImdObject->imd_light = new imd_light_t[pImdObject->imd_object_header.num_light];
			fread(pImdObject->imd_light , sizeof(imd_light_t), pImdObject->imd_object_header.num_light, file);
		}*/
  pImdObject->imd_mesh = new imd_mesh_t[pImdObject->imd_object_header.num_mesh];
  for (iIndex = 0; iIndex < pImdObject->imd_object_header.num_mesh; iIndex++)
  {
    imd_mesh_t *imd_mesh = &(pImdObject->imd_mesh[iIndex]);

    // Reading mesh header
    fread(&(imd_mesh->imd_mesh_header), sizeof(imd_mesh_header_t), 1, file);
    // Reading vertex data
    imd_mesh->imd_vertex = new imd_vertex_t[imd_mesh->imd_mesh_header.num_vertex * pImdObject->imd_object_header.num_anim];
    fread(imd_mesh->imd_vertex, sizeof(imd_vertex_t), imd_mesh->imd_mesh_header.num_vertex * pImdObject->imd_object_header.num_anim, file);
    //normalize_all(imd_mesh->imd_vertex, imd_mesh->imd_mesh_header.num_vertex * pImdObject->imd_object_header.num_anim);
    fread(&(imd_mesh->imd_face.num_section), sizeof(imd_mesh->imd_face.num_section), 1, file);
    imd_mesh->imd_face.imd_section = new imd_face_section_t[imd_mesh->imd_face.num_section];
    for (int iIndex_section = 0; iIndex_section < imd_mesh->imd_face.num_section; iIndex_section++)
    {
      fread(&(imd_mesh->imd_face.imd_section[iIndex_section].num_indice), sizeof(imd_mesh->imd_face.imd_section[iIndex_section].num_indice), 1, file);
      imd_mesh->imd_face.imd_section[iIndex_section].indice = new unsigned short[imd_mesh->imd_face.imd_section[iIndex_section].num_indice];
      fread(imd_mesh->imd_face.imd_section[iIndex_section].indice, sizeof(unsigned short), imd_mesh->imd_face.imd_section[iIndex_section].num_indice, file);
    }
  }
  fclose(file);
  return (pImdObject);
}

void save_imd(imd_object_t *pImdObject, const char *file_name)
{
  FILE *file;
  int iIndex;

  file = fopen(file_name, "wb");

  // First, we write IMD magic number
  fwrite(IMD_MAGIC_TAG, 1, 4, file);
  fwrite(&(pImdObject->imd_object_header), sizeof(imd_object_header_t), 1, file); // next we write object header
  // next,	we write object link (file name of other imd taht are linked);
  if (pImdObject->imd_object_header.num_object_link > 0)
    fwrite(pImdObject->imd_object_links, sizeof(imd_object_link_t), pImdObject->imd_object_header.num_object_link, file);
  // next, we write material data
  if (pImdObject->imd_object_header.num_material > 0)
    fwrite(pImdObject->imd_material, sizeof(imd_material_t), pImdObject->imd_object_header.num_material, file);
  // next, we write tag data
  if (pImdObject->imd_object_header.num_tag > 0)
    fwrite(pImdObject->imd_tag, sizeof(imd_tag_t), pImdObject->imd_object_header.num_tag * pImdObject->imd_object_header.num_anim, file);
  if (pImdObject->imd_object_header.num_anim_part > 0)
    fwrite(pImdObject->imd_anim_part, sizeof(imd_anim_part_t), pImdObject->imd_object_header.num_anim_part, file);
  // next, we write static light data
  //	if (pImdObject->imd_object_header.num_light > 0)
  //		fwrite(pImdObject->imd_light, sizeof(imd_light_t), pImdObject->imd_object_header.num_light, file);
  // next, we write all the mesh data
  for (iIndex = 0; iIndex < pImdObject->imd_object_header.num_mesh; iIndex++)
  {
    imd_mesh_t *imd_mesh = &(pImdObject->imd_mesh[iIndex]);

    // writing mesh header
    fwrite(&(imd_mesh->imd_mesh_header), sizeof(imd_mesh_header_t), 1, file);
    // writing vertex data
    fwrite(imd_mesh->imd_vertex, sizeof(imd_vertex_t), imd_mesh->imd_mesh_header.num_vertex * pImdObject->imd_object_header.num_anim, file);
    //	now, writing face data
    //	note that vertex buffer must be generated at file loading
    //	first number of section
    fwrite(&(imd_mesh->imd_face.num_section), sizeof(imd_mesh->imd_face.num_section), 1, file);
    for (int iIndex_section = 0; iIndex_section < imd_mesh->imd_face.num_section; iIndex_section++)
    {
      // number of indice in current session
      fwrite(&(imd_mesh->imd_face.imd_section[iIndex_section].num_indice), sizeof(imd_mesh->imd_face.imd_section[iIndex_section].num_indice), 1, file);
      //	next the indice
      fwrite(imd_mesh->imd_face.imd_section[iIndex_section].indice, sizeof(unsigned short), imd_mesh->imd_face.imd_section[iIndex_section].num_indice, file);
    }
  }
  fclose(file);
}

void free_imd(imd_object_t *pImdObject)
{
  if (pImdObject == NULL)
    return;
  if (pImdObject->imd_object_header.num_material > 0)
    delete pImdObject->imd_material;
  if (pImdObject->imd_object_header.num_object_link > 0)
    delete pImdObject->imd_object_links;
  if (pImdObject->imd_object_header.num_tag > 0)
    delete pImdObject->imd_tag;
  if (pImdObject->imd_object_header.num_anim_part > 0)
    delete pImdObject->imd_anim_part;
  /*	if (pImdObject->imd_object_header.num_light > 0)
		delete pImdObject->imd_light;*/
  for (int iIndex = 0; iIndex < pImdObject->imd_object_header.num_mesh; iIndex++)
  {
    imd_mesh_t *imd_mesh = &(pImdObject->imd_mesh[iIndex]);

    delete imd_mesh->imd_vertex;
    for (int iIndex_section = 0; iIndex_section < imd_mesh->imd_face.num_section; iIndex_section++)
      delete imd_mesh->imd_face.imd_section[iIndex_section].indice;
    delete imd_mesh->imd_face.imd_section;
  }
  delete pImdObject->imd_mesh;
  delete pImdObject;
}

unsigned long sizeof_imd(imd_object_t *pImdObject)
{
  unsigned long ulRet = 0;
  ulRet = sizeof(pImdObject->imd_object_header);
  ulRet += pImdObject->imd_object_header.num_object_link * sizeof(imd_object_link_t);
  ulRet += pImdObject->imd_object_header.num_material * sizeof(imd_material_t);
  ulRet += pImdObject->imd_object_header.num_tag * pImdObject->imd_object_header.num_anim * sizeof(imd_tag_t);
  ulRet += pImdObject->imd_object_header.num_anim_part * sizeof(imd_anim_part_t);
  ulRet += pImdObject->imd_object_header.num_mesh * sizeof(imd_mesh_t);
  //	ulRet += pImdObject->imd_object_header.num_light				* sizeof(imd_light_t);
  for (int iIndex = 0; iIndex < pImdObject->imd_object_header.num_mesh; iIndex++)
  {
    imd_mesh_t *imd_mesh = &(pImdObject->imd_mesh[iIndex]);

    ulRet += imd_mesh->imd_mesh_header.num_vertex * pImdObject->imd_object_header.num_anim * sizeof(imd_vertex_t);
    ulRet += imd_mesh->imd_face.num_section * sizeof(imd_face_section_t);
    for (int iIndex_section = 0; iIndex_section < imd_mesh->imd_face.num_section; iIndex_section++)
      ulRet += imd_mesh->imd_face.imd_section[iIndex_section].num_indice * sizeof(unsigned short);
  }
  return (ulRet);
}
