/*********************************************************
** File name : ColModel.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLMODEL_HPP__
#define __COLMODEL_HPP__

#define VERTEXTOPOINT(vertex, point)\
point.x = vertex._x;\
point.y = vertex._y;\
point.z = vertex._z

class ColModelModel;
class ColSphereModel;
class ColRayModel;

class ColModel
{
friend class ColModelModel;
friend class ColSphereModel;
friend class ColRayModel;
public:
	/*
	** ctor - dtor
	*/
	ColModel();
	~ColModel();


	/*
	** from/to mesh
	*/
	void					Build(IrisImportMesh& mesh);
	IrisImportMesh&			GetMesh() {return *_mesh;}
	udword					SectionFaces(int i){return _section_faces[i];}
	bool					GetFace(unsigned long l, vect3d& a, vect3d& b, vect3d& c);
private:
	Opcode::OPCODE_Model	*_model;
	IrisImportMesh			*_mesh;
	udword					*_section_faces;
	uint	userdata()		
	{
		return (uint)this;
	}
	static void		callback(uint i, Opcode::VertexPointers& triangle, uint user);
	static Point	_vertex_collide[3];
};


#endif //__COLMODEL_HPP__
