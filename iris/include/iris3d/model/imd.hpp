/*********************************************************
**  File name : imd.h
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IMD_H__
#define __IMD_H__

#define	IMD_MAX_NAME			64
#define	IMD_MAGIC_TAG			"IMD\0"

// typedef unsigned int uint32;
typedef	struct	imd_object_link_s
{
	char	file_name[IMD_MAX_NAME];
	char	tag_in_other[IMD_MAX_NAME];
	char	tag_in_me[IMD_MAX_NAME];
}								imd_object_link_t;

typedef	struct	imd_object_header_s
{
	char	name[IMD_MAX_NAME];
	unsigned	short	num_anim;
	unsigned	short	num_anim_part;
	unsigned	short	num_mesh;
	unsigned	short	num_material;
	unsigned	short	num_tag;
	unsigned	short	num_object_link;
}								imd_object_header_t;


typedef	struct	imd_vertex_s
{
	float		pos[3];
	float		uv[2];
	float		normal[3];
	uint32	    color;
}								imd_vertex_t;

typedef	struct	imd_material_s
{
	char	file_name[IMD_MAX_NAME];
	int		flag;
	int		material_id;
}								imd_material_t;

typedef struct	imd_face_section_s
{
	unsigned	short	num_indice;		// number of indice in this section
	unsigned	short	*indice;			// the indice of face 	
}								imd_face_section_t;

typedef struct	imd_face_s
{
	unsigned	short			num_section;	
	imd_face_section_t	*imd_section;
}								imd_face_t;


typedef	struct	imd_tag_s
{
	char	name[IMD_MAX_NAME]; // can be replaced by id ?? -> had to be seen later
	float	pos[3];
	float	rot_matrix[3][3];
}								imd_tag_t;

typedef struct	imd_mesh_header_s
{
	char	name[IMD_MAX_NAME];
	short	num_vertex;
	short	material_id;
}								imd_mesh_header_t;

typedef struct	imd_mesh_s
{
	imd_mesh_header_t	imd_mesh_header;
	imd_vertex_t			*imd_vertex;
	imd_face_t				imd_face;
}								imd_mesh_t;

typedef	struct	imd_anim_part_s
{
	unsigned	short	begin;
	unsigned	short	end;
	char						name[IMD_MAX_NAME];
}								imd_anim_part_t;

typedef	struct	imd_light_s
{
	float	pos[3];
	float	radius;
	float	color[3];
	unsigned char	flag;
}								imd_light_t;

typedef	struct	imd_object_s
{
	imd_object_header_t	imd_object_header;
	imd_object_link_t		*imd_object_links;
	imd_material_t			*imd_material;
	imd_tag_t						*imd_tag;
	imd_mesh_t					*imd_mesh;
	imd_anim_part_t			*imd_anim_part;
}								imd_object_t;							



imd_object_t	*load_imd(const char *file_name);
void					save_imd(imd_object_t *pImdObject, const char *file_name);
void					free_imd(imd_object_t *pImdObject);
unsigned long	sizeof_imd(imd_object_t *pImdObject);

#endif
