/*********************************************************
** File name : ColRayModel.hpp
** Iris Engine V0.8 "tada"
** Date of creation: 11/11/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLRAYMODEL_HPP__
#define __COLRAYMODEL_HPP__

//! Collider used for mesh vs ray collision test
class	ColRayModel
{
public:
	ColRayModel();
	~ColRayModel();
	//! Query a collision between a ray and mesh.
	/*!
	\param model ColModel that represent the model to test with.
	\param mat 3d transformation matrix of the model.
	\param max_len max length of the ray.
	\param report_all report single or all collision.
	\return true if collision.
	*/
	bool		Query(ColModel &model, const matrix& mat, float max_len, bool report_all);
	/*
	** colliding faces
	*/
	//! Return number of collision
	uint		Count() const;
	//! Return face index of a collision.
	uint		Index(uint i) const;
	//! Build ray collider
	/*!
	\param pos begin of the ray.
	\param dir direction of the ray.
	*/
	void		Build(const vect3d	&pos, const vect3d	&dir);
	//! Get information on a collision.
	/*!
	\param i index of collision
	\param dist distance of collision from begining of the ray to
	the point of impact.
	*/
	void		Info(uint i, float& dist);
private:
	Opcode::RayCollider				*_rc;
	Opcode::CollisionFaces			*_faces;
	Ray								_ray;
};


#endif //__COLRAYMODEL_HPP__
