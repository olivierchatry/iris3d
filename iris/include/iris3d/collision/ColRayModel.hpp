/*********************************************************
** File name : ColRayModel.hpp
** Iris Engine V0.8 "tada"
** Date of creation: 11/11/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLRAYMODEL_HPP__
#define __COLRAYMODEL_HPP__


class	ColRayModel
{
public:
	ColRayModel();
	~ColRayModel();
	/*
	**	query collison beetween model and bounding sphere 
	*/
	bool		Query(ColModel &model, const matrix& mat, float max_len, bool report_all);
	/*
	** colliding faces
	*/
	uint		Count() const;
	uint		Index(uint i) const;
	void		Build(const vect3d	&pos, const vect3d	&dir);
	void		Info(uint i, float& dist);
private:
	Opcode::RayCollider				*_rc;
	Opcode::CollisionFaces			*_faces;
	Ray								_ray;
};


#endif //__COLRAYMODEL_HPP__
