/*********************************************************
** File name : ColModelModel.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLMODELMODEL_HPP__
#define __COLMODELMODEL_HPP__
//! Collider used for mesh vs mesh collision test.
class ColModelModel
{
public:
	/*
	** ctor - dtor
	*/
	ColModelModel();
	~ColModelModel();

	/*
	** making queries
	*/
	//! Build a mesh-mesh collider.
	/*!
	\param first mesh for building collider.
	\param second  mesh for building collider.
	\param report_all report single or all collision.
	*/
	void	Build(ColModel& first, ColModel& second, bool report_all = false);
	//! Query a collision from collider.
	/*!
	\param first matrix of the first object.
	\param second matrix of the second object.
	\return true if the two object collide, false if not.
	\sa Get
	*/
	bool	Query(const matrix& first, const matrix& second);
	
	/*
	** colliding pairs
	*/
	//! Return number of collision between the two object.
	unsigned	Count() const;
	//! Get information on a collision
	/*!
	\param i index of collision.
	\param first face index in the first mesh of face that collide.
	\param second face index in the second mesh of face that collide.
	*/
	void		Get(unsigned i, unsigned long& first, unsigned long& second) const;

private:
	Opcode::AABBTreeCollider*	_tc;
	Opcode::BVTCache*			_cache;
};


#endif //__COLMODELMODEL_HPP__
