/*********************************************************
** File name : ColModelModel.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __COLMODELMODEL_HPP__
#define __COLMODELMODEL_HPP__

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
	void	Build(ColModel& first, ColModel& second, bool report_all = false);
	bool	Query(const matrix& first, const matrix& second);
	
	/*
	** colliding pairs
	*/
	unsigned	Count() const;
	void		Get(unsigned i, unsigned long& first, unsigned long& second) const;

private:
	Opcode::AABBTreeCollider*	_tc;
	Opcode::BVTCache*			_cache;
};


#endif //__COLMODELMODEL_HPP__
