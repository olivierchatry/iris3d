/*********************************************************
** File name : ColSphereModel.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLSPHEREMODEL_HPP__
#define __COLSPHEREMODEL_HPP__


class	ColSphereModel
{
public:
	ColSphereModel();
	~ColSphereModel();
	/*
	**	query collison beetween model and bounding sphere 
	*/
	bool		Query(ColModel &model, const matrix& mat_model, const matrix& mat_sphere, bool report_all = false);
	/*
	** colliding faces
	*/
	uint		Count() const;
	uint		Index(uint i) const;
	// void	info(uint i, float& dist, float& u, float& v) const;
	void		Build(const vect3d	&pos, float radius);
	void		Info(uint i, float& dist) const;
	Sphere		&GetSphere() { return _sphere;}
private:
	Opcode::SphereCollider*			_sc;
	Sphere							_sphere;
	Point							_point_center;
};


#endif //__COLSPHEREMODEL_HPP__

