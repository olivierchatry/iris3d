/*********************************************************
** File name : ColSphereModel.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLSPHEREMODEL_HPP__
#define __COLSPHEREMODEL_HPP__

//! Collider used for mesh vs sphere collision test
class	ColSphereModel
{
public:
	ColSphereModel();
	~ColSphereModel();

	//! Query a collision between a sphere and mesh.
	/*!
	\param model ColModel that represent the model to test with.
	\param mat_model 3d transformation matrix of the model.
	\param mat_sphere 3d transformation matrix of the sphere.
	\param report_all report single or all collision.
	\return true if collision.
	*/
	bool		Query(ColModel &model, const matrix& mat_model, const matrix& mat_sphere, bool report_all = false);

	//! Return number of collision
	uint		Count() const;
	//! Return face index of a collision.
	uint		Index(uint i) const;
	//! Build sphere collider.
	/*!
	\param pos center of the sphere.
	\param radius radius of the sphere.
	*/
	void		Build(const vect3d	&pos, float radius);
	//! Get information on a collision.
	/*!
	\param i index of collision
	\param dist distance of collision from center of the sphere to
	the point of impact.
	*/
	void		Info(uint i, float& dist) const;
	Sphere		&GetSphere() { return _sphere;}
private:
	Opcode::SphereCollider*			_sc;
	Sphere							_sphere;
	Point							_point_center;
};


#endif //__COLSPHEREMODEL_HPP__

