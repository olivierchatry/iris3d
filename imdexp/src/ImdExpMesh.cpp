#include <stdafx.h>

//////////////////////////////////////////////////////////////////////////
// triangular importer
//////////////////////////////////////////////////////////////////////////
bool	ImdExp::StripTriangularZappy(uword *face, int count, ImportedMesh *imported_mesh)
{
	// thanks pierre tardiman :)
	_log->Print("  + Stripping with ZAPPY");
	STRIPERCREATE sc;
	sc.WFaces			= face;
	sc.DFaces			= 0;
	sc.NbFaces			= count;
	sc.AskForWords		= true;
	sc.ConnectAllStrips	= false;
	sc.OneSided			= false;
	sc.SGIAlgorithm		= true;
	Striper Strip;
	if (Strip.Init(sc) == false)
		return false;
	STRIPERRESULT sr;
	if (Strip.Compute(sr) == false)
		return false;
	imported_mesh->_face_total = 0;
	uword* refs = (uword*)sr.StripRuns;
	imported_mesh->_strip.resize(sr.NbStrips);
	for(udword i = 0; i < sr.NbStrips; i++)
	{
		udword nb_refs = sr.StripLengths[i];
		MeshStrip &strip = imported_mesh->_strip[i];
		imported_mesh->_face_total += nb_refs - 2;
		strip._face_index.resize(nb_refs);
		for(udword j = 0; j < nb_refs; j++)
		{
			strip._face_index[j] = *refs;
			refs++;
		}
	}
	return true;
}

#include "strip/nvidia/NvTriStrip.h"

bool			ImdExp::StripTriangularNVidia(uword *face, int count, ImportedMesh *imported_mesh)
{
	unsigned short	num_section;

	_log->Print("  + Stripping with NVIDIA");
	SetCacheSize(8);
	SetStitchStrips(false);
	SetMinStripSize(0);
	SetListsOnly(false);
	PrimitiveGroup* old_grp;	
	GenerateStrips(face, count * 3, &old_grp, &num_section);
	imported_mesh->_strip.resize(num_section);
	imported_mesh->_face_total = 0;
	for(uword i = 0; i < num_section; i++)
	{
		MeshStrip &strip = imported_mesh->_strip[i];
		strip._face_index.resize(old_grp[i].numIndices);
		imported_mesh->_face_total += old_grp[i].numIndices - 3;
		for (uword j = 0; j < old_grp[i].numIndices; ++j)
			strip._face_index[j] = old_grp[i].indices[j];
	}
	delete [] old_grp;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// get mapping and color.
// by default, we have same number of mapping 
// and vertex. (it might change cause max uvmap handling)
void	ImdExp::ImportTriangularMapping(Mesh *mesh, ImportedMesh *imesh, uword *face_data)
{
	int next_vertex = mesh->getNumVerts();
	imesh->_mesh_color_mapping.Allocate(next_vertex);
	std::vector<int>	added_vertex; // vertex added for mapping
	bool			warning_fudging = false;	

	//////////////////////////////////////////////////////////////////////////
	// first, we get vertices color !!!
	VertColor* vertColArray = ( mesh->curVCArray ? mesh->curVCArray : mesh->vertCol );
	int	num_vertices = mesh->getNumVerts();
	//////////////////////////////////////////////////////////////////////////
	// try to get one to one correspondance with vertex and mapping	
	if( mesh->getNumTVerts() )
	{
		for(int i = 0; i < mesh->getNumFaces(); i++ )
		{
			for(int j = 0; j < 3; j++ )
			{
				int v	= mesh->faces[i].getVert(j);
				int tv	= mesh->tvFace[i].t[j];
				if( !imesh->_mesh_color_mapping._mapping[v]._filled )
				{
					imesh->_mesh_color_mapping._mapping[v]._uv = mesh->getTVert(tv);
					imesh->_mesh_color_mapping._mapping[v]._filled = true;
					imesh->_mesh_color_mapping._mapping[v]._tvertex_index = tv;
				}
				else
				{
					if (tv == imesh->_mesh_color_mapping._mapping[v]._tvertex_index)
						continue;
					int		uvm = mesh->getNumVerts();
					bool	found_one = false;
					// check if any of the vertices already added are adequate.
					for( std::vector<int>::iterator ev_i = added_vertex.begin(); ev_i != added_vertex.end(); ev_i++ )
					{
						if( *ev_i == v && 
							imesh->_mesh_color_mapping._mapping[uvm]._uv.x == mesh->getTVert(tv).x &&
							imesh->_mesh_color_mapping._mapping[uvm]._uv.y == mesh->getTVert(tv).y )
						{
							face_data[i * 3 + j] = uvm;
							found_one = true;
							break;
						}
						uvm++;
					}
					if( !found_one )
					{
						UVMapping	new_uv;
						added_vertex.push_back(v);
						face_data[i * 3 + j] = next_vertex;
						new_uv._filled = true;
						new_uv._uv = mesh->getTVert(tv);
						new_uv._tvertex_index = tv;
						imesh->_mesh_color_mapping._mapping.push_back(new_uv);
						next_vertex++;
						warning_fudging = true;
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// add new vertex  needed for mapping.
	if (mesh->getNumVertCol())
	{
		imesh->_mesh_color_mapping._color.resize(mesh->getNumVertCol());
		for (int i = 0; i < mesh->getNumVertCol(); ++i)
			imesh->_mesh_color_mapping._color[i] = vertColArray[i];
	}
	size_t	size_to_add = added_vertex.size();
	size_t	original_index = imesh->_mesh_data[0]._vertex.size();
	for (size_t vi = 0; vi < size_to_add; ++vi)
	{
		imesh->_map_vertices.push_back(MapVertice(added_vertex[vi], (int) (vi) + original_index));
		for (size_t j = 0; j < imesh->_mesh_data.size(); ++j)
		{
			imesh->_mesh_data[j]._vertex.push_back(imesh->_mesh_data[j]._vertex[added_vertex[vi]]);
			imesh->_mesh_data[j]._normal.push_back(imesh->_mesh_data[j]._normal[added_vertex[vi]]);
		}
		imesh->_mesh_color_mapping._color.push_back(imesh->_mesh_color_mapping._color[added_vertex[vi]]);
	}
	if (warning_fudging)
		_log->Print("!!! WARNING: possbility of wrong mapping");
}

bool	ImdExp::ImportTriangularFace(Mesh *mesh, ImportedMesh *imported_mesh)
{
	
	int		face_count = mesh->getNumFaces();
	if (face_count == 0)
		return false;
	int		face_count_3 = face_count * 3;
	Face	*faces = mesh->faces;
	int		count = face_count;
	uword	*face_data = new uword[face_count_3];
	int		idx = 0;
	// convert face data to a dword table;
	while  (count--)
	{
		face_data[idx++] = (uword) faces->v[0];
		face_data[idx++] = (uword) faces->v[1];
		face_data[idx++] = (uword) faces->v[2];
		faces ++;
	}
	ImportTriangularMapping(mesh, imported_mesh, face_data);
	//if (StripTriangularZappy(face_data, face_count, imported_mesh) == false)
		if (StripTriangularNVidia(face_data, face_count, imported_mesh) == false)
		{
			_log->Print("  ! can't strip object, sorry :(");
			return false;
		}
		delete [] face_data;
		return true;
}

TriObject* ImdExp::GetTriObjectFromNode(INode *node, TimeValue t)
{
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{ 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		return tri;
	}
	return 0;
}


ImportedMesh *ImdExp::ImportTriangularObject(INode *node, ObjectState &os)
{
	Mesh			*mesh = &GetTriObjectFromNode(node, 0)->GetMesh();
	ImportedMesh	*imported_mesh = new ImportedMesh;
	TimeValue		inc = GetTicksPerFrame();
	Matrix3			offset_matrix = GetNodeOffsetTM(node);
	
	if (imported_mesh == 0)
		return 0;
	// Look for an empty mesh
	if ( ( !mesh->getNumVerts( ) ) || ( !mesh->getNumFaces( ) )  || ( !mesh->getNumTVerts()))
		return 0;
	if (node->GetMtl() != 0)
		imported_mesh->_material = ImportMaterialData(node->GetMtl(), os);
	else
	{
		_log->Print("  !!! Security : Cannot import object with no material");
		delete imported_mesh;
		return 0;
	}
	// read vertex data;
	int vertex_count = mesh->getNumVerts();
	imported_mesh->_vertex_total = vertex_count;
	imported_mesh->_mesh_data.resize(_plugin_config._end_frame - _plugin_config._begin_frame + 1);
	_log->Print("  + Importing Triangular Object");
	int	real_index = 0;
	for (TimeValue i = _plugin_config._begin_frame; i <= _plugin_config._end_frame; i ++)
	{
		AffineParts	ap;
		TimeValue	tv = i * inc;
		Mesh		&the_mesh = GetTriObjectFromNode(node, tv)->GetMesh();
		MeshData	&mesh_data = imported_mesh->_mesh_data[real_index++];
		the_mesh.buildNormals();
		mesh_data._matrix =  FixCoordSys(offset_matrix * node->GetNodeTM(tv) * Inverse(node->GetParentTM(tv)));	
		mesh_data.Allocate(vertex_count);
		for (int v = 0; v < vertex_count; ++v)
		{													
			Point3 point = the_mesh.getVert(v);
			mesh_data._vertex[v] = point;
			mesh_data._normal[v] = the_mesh.getNormal(v);
		}
	}
	mesh = &(GetTriObjectFromNode(node, 0)->GetMesh());
	_log->Printf("Non modified Num vertex  : %d", vertex_count);
	// stripping des faces, mapping et couleur.
	if (ImportTriangularFace(mesh, imported_mesh) == false)
	{
		delete imported_mesh;
		return 0;
	}
	_log->Printf("Num vertex, normal, color : %d", imported_mesh->_mesh_data[0]._vertex.size());
	_log->Printf("Num mapping : %d", imported_mesh->_mesh_color_mapping._mapping.size());
	_log->Printf("Num face : %d", imported_mesh->_face_total);
	// get user properties.
	TSTR	buffer;
	node->GetUserPropBuffer(buffer);
	imported_mesh->_user_properties = buffer.data();
	imported_mesh->_name = node->GetName();
	_log->Printf("Properties = %s", imported_mesh->_user_properties.c_str());
	return imported_mesh;
}

void ImdExp::ImportTriangularData(INode *node, ObjectState &os)
{
	TCHAR *name = node->GetName();	
	if ( node->GetVisibility(0, 0) == 0.0f )
		return;
	// Look for and hidden mesh
	if ( node->IsHidden(0) )
		return;	
	// do not export target data
	if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		return;
	ImportedElements	*imported_element = 0;
	if(os.obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)) == FALSE)
		return;
	TriObject *tri_object = NULL;
	tri_object = (TriObject*) os.obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
	if (tri_object == NULL)
		return;
	//	if object name begin with "tag"
	//	then parse it as a tag.
	_log->Printf("Importing object [%s]", name);
	if (strncmp(name, "tag", 3) == 0)
		imported_element = ImportTagObject(node, os);
	else
	{
		imported_element = ImportTriangularObject(node, os);
		if (imported_element)
			ImportSkinData(node, (ImportedMesh *) imported_element); // try to get skin form our mesh
	}
	if (imported_element)
		_elements.push_back(imported_element);
}
