
/********************************************************************
created:	2003/02/03
created:	4:2:2003   20:17
filename: 	e:\_dev\tmp\imdexp\src\imdexp.cpp
file path:	e:\_dev\tmp\imdexp\src
file base:	imdexp
file ext:	cpp
author:		Chatry Olivier alias gruiiik

purpose:	Main exporter function.
*********************************************************************/
#include <stdafx.h>
#include <imd2/imd2.hpp>
/*
 *	consttructor destructor
 */
ImdExp::ImdExp() : _material_list(0), _log(0)
{

}

ImdExp::~ImdExp()
{
}

//////////////////////////////////////////////////////////////////////////

/*
*	return pointer to max interface
*/
Interface *ImdExp::GetInterface()
{
	return ip;
}
/*
*	give number of file extension handled by the plugin
*	for instance, we only handle imd file.
*/
int	ImdExp::ExtCount()
{
	
return 1;
}
/*
*	give the string of given extension.
*	Because we handle only one extension, we
*	just return a string without any test.
*/
const TCHAR *ImdExp::Ext(int n)
{
	return GetString(IDS_IMDEXTENSION);
}

/*
*	Long description of our plugin
*/
const TCHAR *ImdExp::LongDesc()
{
	return GetString(IDS_LONGDESCRIPTION);
}

/*
*	Short description of our plugin
*/
const TCHAR *ImdExp::ShortDesc()
{
	return GetString(IDS_SHORTDESCRIPTION);
}

/*
*	My name :)
*/
const TCHAR *ImdExp::AuthorName()
{
	return GetString(IDS_ME);
}

/*
*	Stupid copyright (i hate copyright :)) message
*/
const TCHAR *ImdExp::CopyrightMessage()
{
	return GetString(IDS_COPYRIGHT);
}

/*
*	Version of our plugin
*/
unsigned int ImdExp::Version()
{
	return IMDEXP_VERSION;
}

//////////////////////////////////////////////////////////////////////////
// callback def.
#include "callback.h"
//////////////////////////////////////////////////////////////////////////

/*
*	About our plugin, just create a sample dialog box 
*	from IDD_ABOUTBOX resource.
*/
void ImdExp::ShowAbout(HWND hwnd)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOGABOUT), hwnd, AboutBoxDlgProc, 0);
}


/*
*	Other message function, return empty string
*/
const TCHAR *ImdExp::OtherMessage1()
{
	return _T("");
}
const TCHAR *ImdExp::OtherMessage2()
{
	return _T("");
}

void	ImdExp::PopulateTreeView(HWND hwnd, INode* node, HTREEITEM parent)
{
	_node_count++;
	if (node == 0)
		node = ip->GetRootNode();
	char	buffer[512];
	ObjectState os = node->EvalWorldState(0);
	if (os.obj)
		sprintf(buffer, "%s [ID = %0.2X]", node->GetName(), os.obj->SuperClassID());
	else
		sprintf(buffer, "%s", node->GetName());
	HTREEITEM me = AddNode(hwnd, IDC_TREESCENE, buffer, parent);
	for (int c = 0; c < node->NumberOfChildren(); c++)
		PopulateTreeView(hwnd, node->GetChildNode(c), me);	
}
/*
*	Suported option, only export for instance.
*/
BOOL ImdExp::SupportsOptions(int ext, DWORD options) 
{
	assert(ext == 0);
	return (options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}



//////////////////////////////////////////////////////////////////////////
// utility function
//////////////////////////////////////////////////////////////////////////
Modifier *ImdExp::FindModifier(INode *node, Class_ID	&class_id)
{
	Object* object = node->GetObjectRef();
	if (object == 0) 
		return 0;
	while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* derived_object = static_cast<IDerivedObject*>(object);
		for (int mi = 0; mi < derived_object->NumModifiers(); ++mi)
		{
			Modifier* m = derived_object->GetModifier(mi);
			Loger::Get().Printf("[ImdExp::FindModifier] Find Modifier %s", m->GetName());
			if (m->ClassID() == class_id)
				return m;
		}
		object = derived_object->GetObjRef();
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// mesh importer
//////////////////////////////////////////////////////////////////////////
void ImdExp::ImportMeshData(INode *node, ObjectState &os)
{
	// import only triangular data.
	// bones are processed in another loop.
	if (os.obj->ClassID() != Class_ID(BONE_OBJ_CLASSID))
	{
		ImportTriangularData(node, os);
	}
}

Matrix3   ImdExp::GetNodeOffsetTM(INode* node)
{
	Matrix3 offset_tm(1);
	Point3 pos = node->GetObjOffsetPos();
	offset_tm.PreTranslate(pos);
	Quat quat = node->GetObjOffsetRot();
	PreRotateMatrix(offset_tm, quat);
	ScaleValue scale_value = node->GetObjOffsetScale();
	ApplyScaling(offset_tm, scale_value);
	return offset_tm;
}

/*
 *	Export data
 */
void ImdExp::ImportDataFromMax(INode *node)
{
	// node->Selected()
	ObjectState os = node->EvalWorldState(_plugin_config._begin_frame);
	if (os.obj)
	{
		switch(os.obj->SuperClassID()) 
		{
		case GEOMOBJECT_CLASS_ID: 
			ImportMeshData(node, os);
			break;
		case CAMERA_CLASS_ID:
			break;
		case LIGHT_CLASS_ID:
			ImportLightData(node, os);
			break;
		case SHAPE_CLASS_ID:
			break;
		case HELPER_CLASS_ID:
		case BONE_CLASS_ID:
			break;
		}
	}
	for (int c = 0; c < node->NumberOfChildren(); c++)
		ImportDataFromMax(node->GetChildNode(c));
}

/*
*	Free all allocated data
*/
void	ImdExp::FreeAll()
{
	element_list_it_t	it;
	Loger::Get().Print("++ [ImdExp::FreeAll]");
	for (it = _elements.begin(); it != _elements.end(); ++it)
		delete (*it);
	_elements.resize(0);
	Loger::Get().Print("-- [ImdExp::FreeAll]");
}

void	PrintBone(BoneData *data, std::string tab)
{
	BoneData::bone_data_list_it_t it;	
	Loger::Get().Printf("%s%d", tab.c_str(), data->_bone_index);
	tab += "  ";
	for (it = data->_bone_child.begin(); it != data->_bone_child.end(); ++it)
		PrintBone(&(*it), tab);
}

unsigned short	ImdExp::CountElementOf(element_type type)
{
	element_list_it_t	it;
	unsigned short		count = 0;

	for (it = _elements.begin(); it != _elements.end(); ++it)
	{
		if ((*it)->_type == type) 
			count ++;
	}
	return count;
}

std::string	ImdExp::GetFilePath(std::string str)
{
	size_t	pos_sep = str.find_last_of("\\/");
	return str.substr(0, pos_sep + 1);
}

std::string	ImdExp::GetFileName(std::string str)
{
	size_t	pos_sep = str.find_last_of("\\/") + 1;
	return str.substr(pos_sep);
}
std::string	ImdExp::GetFileNameWithoutExtension(std::string str)
{
	size_t	pos_sep = str.find_last_of("\\/") + 1;
	size_t	pos_point = str.find_last_of(".");
	return str.substr(pos_sep,  pos_point - pos_sep);	
}

int	ImdExp::TruncateValueToPower2(int value)
{
	int	bit = 32;
	while (bit--)
		if (value >> bit)
			return (1 << bit);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// write our imd2 file.
void	ImdExp::ExportImd2Material(imd2_object_t &object, std::string &path)
{
	ImportedMaterial::material_data_list_it_t	it;
	int		count = 0;
	ilInit();
	iluInit();
	ilutInit();
	iluImageParameter(ILU_FILTER, ILU_BILINEAR);
	for (it = _material_list->_material_data.begin(); it != _material_list->_material_data.end(); ++it)
	{
		MaterialData	*material_data = *it;
		char			*file_name_path;
		memset(object.imd2_material[count].file_name, 0, IMD2_MAX_NAME);
		file_name_path = material_data->_diffuse_map;
		if (file_name_path == 0)
			file_name_path = material_data->_env_map;
		if (file_name_path == 0)
		{
			_log->Printf("!!! Invalid texture file name for material %s\n", material_data->_name);
			continue;
		}
		// copy bitmap file to imd2 folder.
		std::string file_name = GetFileNameWithoutExtension(file_name_path);
		std::string new_file_name_path (path);
		file_name += ".png";
		new_file_name_path += file_name;
		ILuint	image_id;
		ilGenImages(1, &image_id);
		ilBindImage(image_id);
		iluLoadImage(file_name_path);
		int convert_width = TruncateValueToPower2(ilGetInteger(IL_IMAGE_WIDTH));
		int convert_height = TruncateValueToPower2(ilGetInteger(IL_IMAGE_WIDTH));
		int	middle = 0;
		if (convert_height != convert_width)
			middle = TruncateValueToPower2((convert_width + convert_height) / 2);
		else
			middle = convert_width;
		if (middle > 512)
			middle = 512;
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		iluScale(middle, middle, 1);
		DeleteFile(new_file_name_path.c_str());
		ilSaveImage((ILstring) new_file_name_path.c_str());
		ilDeleteImages(1, &image_id);
		strncpy(object.imd2_material[count].file_name, file_name.c_str(), IMD2_MAX_NAME - 1);
		// defin material id !!!
		object.imd2_material[count].material_id = (int) material_data;
		_log->Printf("inserting material file [%s]", object.imd2_material[count].file_name);
		material_data->_diffuse_map = strdup(new_file_name_path.c_str());
		material_data->_env_map = 0;
		count ++;
	}
}

#define MatrixToFloatP(m, max_mat)\
{\
	Point4	p = max_mat.GetRow(0);\
	m[0] = p.x; m[1] = p.y; m[2] = p.z; m[3] = 0.0f;\
	p = max_mat.GetRow(1);\
	m[4] = p.x; m[5] = p.y; m[6] = p.z; m[7] = 0.0f;\
	p = max_mat.GetRow(2);\
	m[8] = p.x; m[9] = p.y; m[10] = p.z; m[11] = 0.0f;\
	p = max_mat.GetRow(3);\
	m[12] = p.x; m[13] = p.y; m[14] = p.z; m[15] = 1.0f;\
}

void	ImdExp::ExportImd2Mesh(imd2_object_t &object)
{
	element_list_it_t it;
	// allocate data
	object.imd2_object_header.have_skin = false;
	object.imd2_mesh = new imd2_mesh_t[object.imd2_object_header.num_mesh];
	imd2_mesh_t *mesh = object.imd2_mesh;
	memset(mesh, 0, object.imd2_object_header.num_mesh * sizeof(imd2_mesh_t));
	for (it = _elements.begin(); it != _elements.end(); ++it)
	{
		ImportedElements *el = *it;
		if (el->_type == mesh_element)
		{
			ImportedMesh *imesh = (ImportedMesh *) el;
			strncpy(mesh->imd2_mesh_header.name, imesh->_name.c_str(), IMD2_MAX_NAME - 1);
			size_t vertex_count = imesh->_mesh_data[0]._vertex.size();
			mesh->imd2_mesh_header.num_vertex = vertex_count;
			object.imd2_object_header.have_skin |= imesh->_skin != 0;
			mesh->imd2_mesh_header.have_skin = imesh->_skin != 0;
			mesh->user_data = new char [imesh->_user_properties.size() + 1];
			strcpy(mesh->user_data, imesh->_user_properties.c_str());
			mesh->imd2_mesh_header.material_id = (int) imesh->_material;
			mesh->imd2_face.num_section = (unsigned short) imesh->_strip.size();
			mesh->imd2_face.imd2_section = new imd2_face_section_t[mesh->imd2_face.num_section];
			for (int index_strip = 0; index_strip < mesh->imd2_face.num_section; ++index_strip)
			{
				mesh->imd2_face.imd2_section[index_strip].num_indice = (unsigned short) imesh->_strip[index_strip]._face_index.size();
				mesh->imd2_face.imd2_section[index_strip].indice = new unsigned short[mesh->imd2_face.imd2_section[index_strip].num_indice];
				for (size_t index_face = 0; index_face < imesh->_strip[index_strip]._face_index.size(); ++index_face)
					mesh->imd2_face.imd2_section[index_strip].indice[index_face] = imesh->_strip[index_strip]._face_index[index_face]; 	
			}
			//////////////////////////////////////////////////////////////////////////
			// copy vertex data
			if (!_plugin_config._sample_matrix)
			{
				mesh->imd2_vertex = new imd2_vertex_t[vertex_count * object.imd2_object_header.num_anim];
				mesh->imd2_matrix = 0;
				// save object matrix.
				for (int index_anim = 0; index_anim < object.imd2_object_header.num_anim; ++index_anim)
					for (size_t index_vertex = 0; index_vertex < vertex_count; ++index_vertex)
					{
						int offset = vertex_count * index_anim + index_vertex;
						MeshData &mesh_data = imesh->_mesh_data[index_anim];
												mesh_data._vertex[index_vertex] = mesh_data._matrix * mesh_data._vertex[index_vertex];
						mesh_data._normal[index_vertex] = mesh_data._matrix * mesh_data._normal[index_vertex];
						memcpy(mesh->imd2_vertex[offset].normal, mesh_data._normal[index_vertex], sizeof(float) * 3);
						memcpy(mesh->imd2_vertex[offset].pos, mesh_data._vertex[index_vertex], sizeof(float) * 3);
						uint r = (uint) (imesh->_mesh_color_mapping._color[index_vertex].x * 255) & 0xff;
						uint g = (uint) (imesh->_mesh_color_mapping._color[index_vertex].y * 255) & 0xff;
						uint b = (uint) (imesh->_mesh_color_mapping._color[index_vertex].z * 255) & 0xff;
						mesh->imd2_vertex[offset].color = (r << 16 | g << 8 | b );
						mesh->imd2_vertex[offset].uv[0] = imesh->_mesh_color_mapping._mapping[index_vertex]._uv.x;
						mesh->imd2_vertex[offset].uv[1] = 1.0f - imesh->_mesh_color_mapping._mapping[index_vertex]._uv.y;
					}
			}
			else
			{
				mesh->imd2_vertex = new imd2_vertex_t[vertex_count];
				mesh->imd2_matrix = new imd2_matrix_t[object.imd2_object_header.num_anim];
				for (size_t index_vertex = 0; index_vertex < vertex_count; ++index_vertex)
				{
					memcpy(mesh->imd2_vertex[index_vertex].normal, imesh->_mesh_data[0]._normal[index_vertex], sizeof(float) * 3);
					memcpy(mesh->imd2_vertex[index_vertex].pos, imesh->_mesh_data[0]._vertex[index_vertex], sizeof(float) * 3);
					uint r = (uint) (imesh->_mesh_color_mapping._color[index_vertex].x * 255) & 0xff;
					uint g = (uint) (imesh->_mesh_color_mapping._color[index_vertex].y * 255) & 0xff;
					uint b = (uint) (imesh->_mesh_color_mapping._color[index_vertex].z * 255) & 0xff;
					mesh->imd2_vertex[index_vertex].color = (r << 16 | g << 8 | b );
					mesh->imd2_vertex[index_vertex].uv[0] = imesh->_mesh_color_mapping._mapping[index_vertex]._uv.x;
					mesh->imd2_vertex[index_vertex].uv[1] = 1.0f - imesh->_mesh_color_mapping._mapping[index_vertex]._uv.y;
				}
				for (int index_anim = 0; index_anim < object.imd2_object_header.num_anim; ++index_anim)
				{
					Matrix3 &max_mat = imesh->_mesh_data[index_anim]._matrix;
					float	*m = mesh->imd2_matrix[index_anim].m;
					MatrixToFloatP(m, max_mat);
				}
			}


			//////////////////////////////////////////////////////////////////////////
			// exporting skining information if needed;
			if (object.imd2_object_header.have_skin && imesh->_skin)
			{	
				int	count_bones = 0;
				// first count all bone linked to this vertex.
				int	vertex_count = mesh->imd2_mesh_header.num_vertex;
				mesh->imd2_skin = new imd2_skin_t[vertex_count];
				for (int index_vertex = 0; index_vertex != vertex_count; ++index_vertex)
				{
					count_bones = 0;
					ImportedSkin *skin = imesh->_skin;
					ImportedSkin::skin_data_list_t &s_list = skin->_skin_data; 
					ImportedSkin::skin_data_list_it_t it(s_list.begin());
					for (;it != s_list.end(); ++it)
					{
						std::vector<PointWeight>::iterator	it_w(it->second._point_weight.begin());
						for (;it_w != it->second._point_weight.end(); ++it_w)
							if (it_w->_point_index == index_vertex)
								count_bones ++;
					}		
					mesh->imd2_skin[index_vertex].num_bones_assigned = count_bones;
					mesh->imd2_skin[index_vertex].weight = new imd2_weight_t[count_bones];
					count_bones = 0;
					it = s_list.begin();
					for (;it != s_list.end(); ++it)
					{
						std::vector<PointWeight>::iterator	it_w(it->second._point_weight.begin());
						for(;it_w != it->second._point_weight.end(); ++it_w)
							if (it_w->_point_index == index_vertex)
							{
								mesh->imd2_skin[index_vertex].weight[count_bones].weight = it_w->_weight;
								mesh->imd2_skin[index_vertex].weight[count_bones].bone_index = it->second._bone_index;
								count_bones ++;
							}
					}		
				}
				
			}

			mesh ++;
		}
	}
}

void	ImdExp::ExportImd2Tag(imd2_object_t &object)
{
	element_list_it_t it;

	// allocate data
	object.imd2_tag = new imd2_tag_t[object.imd2_object_header.num_tag];
	imd2_tag_t *tag = object.imd2_tag;
	memset(tag, 0, object.imd2_object_header.num_tag * sizeof(imd2_tag_t));
	for (it = _elements.begin(); it != _elements.end(); ++it)
	{
		ImportedElements *el = *it;
		if (el->_type == tag_element)
		{
			ImportedTag *itag = (ImportedTag *) el;
			strncpy(tag->name, itag->_name.c_str(), IMD2_MAX_NAME - 1);
			tag->user_data = new char [itag->_user_properties.size() + 1];
			tag->tag_data = new imd2_tag_data_t[object.imd2_object_header.num_anim];
			strcpy(tag->user_data, itag->_user_properties.c_str());
			for (int index_anim = 0; index_anim < object.imd2_object_header.num_anim; ++index_anim )
			{
				tag->tag_data[index_anim].pos[0] = itag->_tag_data[index_anim]->_pos.x;
				tag->tag_data[index_anim].pos[1] = itag->_tag_data[index_anim]->_pos.y;
				tag->tag_data[index_anim].pos[2] = itag->_tag_data[index_anim]->_pos.z;
			}	
			tag ++;
		}
	}
}


void	ImdExp::SaveObjectFile(const TCHAR *c_file_name)
{
	std::string		file_name(c_file_name);
	std::string		name;
	std::string		path;
	imd2_object_t	*object = new imd2_object_t;
	memset(object, 0, sizeof(imd2_object_t));
	// got path.
	_file_name = file_name;
	object->imd2_object_header.matrix_sampling = _plugin_config._sample_matrix;
	_log->Printf("---------------------------");
	name = GetFileNameWithoutExtension(file_name);
	_log->Printf("Object name = %s", name.c_str());
	path = GetFilePath(file_name);
	_log->Printf("Path = %s", path.c_str());
	strncpy(object->imd2_object_header.name, name.c_str(), IMD2_MAX_NAME - 1);
	object->imd2_object_header.num_anim = _plugin_config._end_frame - _plugin_config._begin_frame + 1;
	object->imd2_object_header.num_mesh = CountElementOf(mesh_element);
	if (_material_list == 0)
		object->imd2_object_header.num_material = 0;
	else
	{
		object->imd2_object_header.num_material = (unsigned short) _material_list->_material_data.size();
		object->imd2_material = new imd2_material_t[object->imd2_object_header.num_material];
		ExportImd2Material(*object, path);
	}
	object->imd2_object_header.num_tag = CountElementOf(tag_element);
	object->imd2_object_header.num_light = 0; // ountElementOf(light_element); // TODO.
	ExportImd2Mesh(*object);
	ExportImd2Tag(*object);
	save_imd2(object, c_file_name);
	free_imd2(object);
}

int	ImdExp::GetNumBones(INode *node)
{
	int count = 0;
	if(IsNodeBone(node))
		count += 1;
	for(int i = 0; i < node->NumberOfChildren(); i++)
		count += GetNumBones(node->GetChildNode(i));
	return count;
}

Matrix3& ImdExp::FixCoordSys(Matrix3 &tm)
{
	// swap 2nd and 3rd rows
	Point3 row = tm.GetRow(1);
	tm.SetRow(1, tm.GetRow(2));
	tm.SetRow(2, row);

	// swap 2nd and 3rd columns
	Point4 column = tm.GetColumn(1);
	tm.SetColumn(1, tm.GetColumn(2));
	tm.SetColumn(2, column);

	tm.SetRow(0, tm.GetRow(0));
	tm.SetColumn(0, tm.GetColumn(0));

	return tm;
}

Point3& ImdExp::FixCoordSys(Point3 &pnt)
{
	float	tmp;
	tmp = pnt.y;
	pnt.y = pnt.z;
	pnt.z = tmp;
	return pnt;
}

void	ImdExp::RecursiveSaveBone(imd2_bone_file_t *imd2_bone, BoneData *data, int &index, int parent)
{
	imd2_bone->bones[index].imd2_bone_header.bone_index = data->_bone_index;
	imd2_bone->bones[index].imd2_bone_header.bone_parent = parent;
	memset(imd2_bone->bones[index].imd2_bone_header.name, 0, IMD2_MAX_NAME);
	strncpy(imd2_bone->bones[index].imd2_bone_header.name, data->_name.c_str(), IMD2_MAX_NAME - 1);
	imd2_bone->bones[index].imd2_bone_header.name[IMD2_MAX_NAME - 1] = 0;
	int anim_count = imd2_bone->imd2_bone_file_header.anim_count;
	imd2_bone->bones[index].imd2_bone_anim = new imd2_bone_anim_t[anim_count];
	int	parent_index = imd2_bone->bones[index].imd2_bone_header.bone_index;
	for (int i = 0; i < anim_count; ++i)
	{
		imd2_bone_anim_t	*anim = &(imd2_bone->bones[index].imd2_bone_anim[i]);
		MatrixToFloatP(anim->matrix, data->_animation[i].matrix);
		anim->pos[0] = data->_animation[i].pos.x;
		anim->pos[1] = data->_animation[i].pos.y;
		anim->pos[2] = data->_animation[i].pos.z;
	}
	index ++;
	size_t	child_count = data->_bone_child.size();
	for (size_t b = 0; b < child_count; ++b)
		RecursiveSaveBone(imd2_bone, &(data->_bone_child[b]), index, parent_index);
}

void	ImdExp::SaveBoneFile(const TCHAR *c_file_name, ImportedBone *bones)
{
	int					bone_count	= GetNumBones(ip->GetRootNode());
	imd2_bone_file_t	*imd2_bone = new imd2_bone_file_t;
	int					index = 0;

	imd2_bone->imd2_bone_file_header.anim_count = _plugin_config._end_bone_frame - _plugin_config._begin_bone_frame + 1;
	imd2_bone->imd2_bone_file_header.bone_count = bone_count;
	imd2_bone->bones = new imd2_bone_t[bone_count];
	for (size_t i = 0; i < bones->_bone_data.size(); ++i)
	{
		RecursiveSaveBone(imd2_bone, &(bones->_bone_data[i]), index);
		continue;
	}
	save_imd2_bone(imd2_bone, c_file_name);
	free_imd2_bone(imd2_bone);
	// count bones.
}

/*
*	Main export function
*/
int ImdExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts/* =FALSE */, DWORD options/* =0 */)
{	
	BOOL showPrompts = suppressPrompts ? FALSE : TRUE;
	ip = i;
	_file_name = name;
	_bone_file_name = _file_name.substr(0,  _file_name.find_last_of("."));	
	_bone_file_name += ".imdbone";
	LoadConfig();
	if (DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOGCONFIG), ip->GetMAXHWnd(), ExportDlgProc, (LPARAM)this) != IDOK)
		return 0;
	SaveConfig();
	if (_plugin_config._log_window)
	{
		Loger::Get().Create(hInstance, _plugin_config._dos_log_window);
		Loger::Get().Show();
		Loger::Get().EnableOk(false);
	}
	_log = &(Loger::Get());
	if (_plugin_config._export_object)
		ImportDataFromMax(ip->GetRootNode());
	// bones import
	if (_plugin_config._export_bones)
	{
		ImportedBone *bones = ImportBoneData();
		if (bones != 0)
		{
			_elements.push_back(bones);
			SaveBoneFile(_bone_file_name.c_str(), bones);
		}
	}
	if (_plugin_config._export_object)
		SaveObjectFile(name);
	Loger::Get().EnableOk();
	FreeAll();
	return 1;
}