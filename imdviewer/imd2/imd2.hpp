/*********************************************************
**  File name : imd2.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: [4/14/2003]
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IMD2_H__
#define __IMD2_H__

#define	IMD2_MAX_NAME			64
#define	IMD2_MAGIC_TAG			"IM2\0"
#define	IMD2_BONE_MAGIC_TAG			"IB2\0"


typedef	struct	imd2_object_header_s
{
	char	    name[IMD2_MAX_NAME];
	unsigned	short	num_anim;
	unsigned	short	num_mesh;
	unsigned	short	num_material;
	unsigned	short	num_tag;
	unsigned	short	num_light;
	bool		have_skin;
	bool		matrix_sampling;
}								imd2_object_header_t;


typedef	struct	imd2_vertex_s
{
	float			pos[3];
	float			uv[2];
	float			normal[3];
	unsigned int    color;
}								imd2_vertex_t;

typedef struct	imd2_weight_s
{
	float			weight;
	int				bone_index;
}				imd2_weight_t;

typedef struct imd2_skin_s
{
	int				num_bones_assigned;
	imd2_weight_t	*weight;
}				imd2_skin_t;

typedef	struct	imd2_material_s
{
	char	file_name[IMD2_MAX_NAME];
	int		flag;
	int		material_id;
}								imd2_material_t;

typedef struct	imd2_face_section_s
{
	unsigned	short	num_indice;		// number of indice in this section
	unsigned	short	*indice;			// the indice of face 	
}								imd2_face_section_t;

typedef struct	imd2_face_s
{
	unsigned	short			num_section;	
	imd2_face_section_t	*imd2_section;
}								imd2_face_t;


typedef struct  imd2_tag_data_s
{
	float	pos[3];
}				imd2_tag_data_t;

typedef	struct	imd2_tag_s
{
	char	name[IMD2_MAX_NAME]; // can be replaced by id ?? -> had to be seen later
	char	*user_data;
	imd2_tag_data_t	*tag_data;
}								imd2_tag_t;

typedef struct	imd2_mesh_header_s
{
	char	name[IMD2_MAX_NAME];
	short	num_vertex;
	int		material_id;
	bool	have_skin;
}								imd2_mesh_header_t;

typedef struct	imd2_matrix_s
{
	float	m[16];
}				imd2_matrix_t;

typedef struct	imd2_mesh_s
{
	imd2_mesh_header_t	    imd2_mesh_header;
	imd2_vertex_t			*imd2_vertex;
	imd2_matrix_t			*imd2_matrix;
	imd2_face_t				imd2_face;
	imd2_skin_t				*imd2_skin;
	char					*user_data;     // user data.
}							imd2_mesh_t;

enum imd2_light_type_t
{
    type_omni = 0,
    type_dir = 1
};

typedef	struct	imd2_light_s
{
	float	            pos[3];
	float	            radius;
    float               dir[3];
	float	            color[3];
    imd2_light_type_t   type;
	unsigned char		flag;
}								imd2_light_t;

typedef	struct	imd2_object_s
{
	imd2_object_header_t	    imd2_object_header;
	imd2_material_t				*imd2_material;
	imd2_tag_t					*imd2_tag;
	imd2_mesh_t					*imd2_mesh;
	imd2_light_t				*imd2_light;
}				imd2_object_t;							

//////////////////////////////////////////////////////////////////////////
// imd2 mesh file format.
imd2_object_t	*load_imd2(const char *file_name);
void			save_imd2(imd2_object_t *pImdObject, const char *file_name);
void			free_imd2(imd2_object_t *pImdObject);
unsigned long	sizeof_imd2(imd2_object_t *pImdObject);

//////////////////////////////////////////////////////////////////////////
// imd2 bones file format.
typedef struct imd2_bone_anim_s
{
	// should be an imd2_matrix_t
	float	matrix[16];
	float			pos[3];
}				imd2_bone_anim_t;

typedef	struct imd2_bone_header_s
{
	char	name[IMD2_MAX_NAME];
	int		bone_index;
	int		bone_parent;
}				imd2_bone_header_t;

typedef struct imd2_bone_s
{
	imd2_bone_header_t	imd2_bone_header;
	imd2_bone_anim_t	*imd2_bone_anim;
}				imd2_bone_t;

typedef struct imd2_bone_file_header_s
{
	int			bone_count;
	int			anim_count;	
}				imd2_bone_file_header_t;

typedef	struct imd2_bone_file_s
{
	imd2_bone_file_header_t	imd2_bone_file_header;
	imd2_bone_t				*bones;
}				imd2_bone_file_t;

imd2_bone_file_t	*load_imd2_bone(const char *file_name);
void				save_imd2_bone(imd2_bone_file_t *imd2_bones, const char *file_name);
void				free_imd2_bone(imd2_bone_file_t *imd2_bones);
unsigned long		sizeof_imd2_bone(imd2_bone_file_t *imd2_bones);

#endif
