/*********************************************************
** File name : ColModel.cpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include	"Iris.hpp"

using namespace Opcode;

Point	ColModel::_vertex_collide[3];

ColModel::ColModel() : _model(0), _mesh(0), _section_faces(0)
{
	_model = new OPCODE_Model;
}

ColModel::~ColModel()
{
	delete _model;
    if (_section_faces)
        delete [] _section_faces;
}


void ColModel::Build(IrisImportMesh	&mesh)
{
	OPCODECREATE	c;
	udword			num_face;
	int				num_vertex;
	Point			*vertex;
	udword			*face;
	
	num_face = 0;
	_section_faces = new udword [mesh.GetNumSection()];
	for (int section = 0; section < mesh.GetNumSection(); section ++)
	{
		IrisSection &the_section = mesh.GetSection(section);
		_section_faces[section] = num_face;
		num_face += the_section.GetNumIndice() - 2;
	}	
	num_vertex = mesh.GetNumVertex();
	
	// vertex
	vertex = new Point[num_vertex];
	for (int i = 0; i < num_vertex; i++)
	{
		vect3d	*vert = mesh.GetVertex(i);
		VERTEXTOPOINT((*vert), vertex[i]);
	}
	// face
	face   = new udword[num_face * 3];
	int		face_index_tmp = 0;
	for (int section = 0; section < mesh.GetNumSection(); section ++)
	{
		IrisSection &the_section = mesh.GetSection(section);
		for (int index = 0; index < the_section.GetNumIndice() - 2; ++index)
		{
			int	calc;
			calc = (index % 2) * 2;
			face[face_index_tmp++] = the_section.GetIndice(index + calc);
			face[face_index_tmp++] = the_section.GetIndice(index + 1);
			face[face_index_tmp++] = the_section.GetIndice(index + (2 - calc));
		}
	}
	c.NbTris	= num_face;
	c.NbVerts	= num_vertex;
	c.Tris		= face;
	c.Verts		= vertex;
	c.Rules		= SPLIT_COMPLETE | SPLIT_SPLATTERPOINTS; /* SPLIT_LARGESTAXIS */
	c.NoLeaf	= true;
	c.Quantized	= true;
	_model->Build(c);
	delete [] vertex;
	delete [] face;
	_mesh = &mesh;
}

bool ColModel::GetFace(unsigned long i, vect3d& a, vect3d& b, vect3d& c)
{
	int			index;

	for (index = 1; index < _mesh->GetNumSection(); index++)
	{
		if (SectionFaces(index) > i)
			break;
	}
	index --;
	
	IrisSection &section = _mesh->GetSection(index);
	int index_face = i - SectionFaces(index);
	// int calc = (index % 2) * 2;
	a = *_mesh->GetVertex(section.GetIndice(index_face + 2 /*+ calc*/));
	b = *_mesh->GetVertex(section.GetIndice(index_face + 1));
	c = *_mesh->GetVertex(section.GetIndice(index_face + 0 /*(2 - calc)*/));
	return ((index_face % 2) == 0); // false if odd, true if even
}



void ColModel::callback(udword i, VertexPointers& triangle, udword user)
{
	ColModel&	col_model= *((ColModel* )user);
	vect3d		a, b, c;

	
	col_model.GetFace(i, a, b, c);
	VERTEXTOPOINT(a, ColModel::_vertex_collide[0]); 
	VERTEXTOPOINT(b, ColModel::_vertex_collide[1]);
	VERTEXTOPOINT(c, ColModel::_vertex_collide[2]);
	triangle.Vertex[0] = &(ColModel::_vertex_collide[0]);
	triangle.Vertex[1] = &(ColModel::_vertex_collide[1]);
	triangle.Vertex[2] = &(ColModel::_vertex_collide[2]);
}
