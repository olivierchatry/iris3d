/*********************************************************
** File name : ColSphereModel.cpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/


#include	"Iris.hpp"
using namespace Opcode;

/*
**	constructor, allocate memory
*/
ColSphereModel::ColSphereModel()
{
	_sc = new SphereCollider;
}

/*
**	destructor, delete allocated memory
*/
ColSphereModel::~ColSphereModel()
{
	delete _sc;
}

/*
**	query collison beetween model and bounding sphere 
*/
bool	ColSphereModel::Query(ColModel &model, const matrix& mat_model, const matrix& mat_sphere, bool report_all)
{
	static	SphereCache	cache;
	if (!report_all)
	{
		_sc->SetFirstContact(true);
		_sc->SetTemporalCoherence(true);
	}
	else
	{
		_sc->SetFirstContact(false);
		_sc->SetTemporalCoherence(false);
	}
	_sc->SetCallback(ColModel::callback, (udword) &model);
	return _sc->Collide(cache, _sphere, model._model, (Matrix4x4* )&mat_sphere, (Matrix4x4* )&mat_model) && Count() != 0;
}

/*
**	query collison beetween model and bounding sphere 
*/
void	ColSphereModel::Build(const vect3d &pos, float radius)
{	

	VERTEXTOPOINT(pos, _point_center);
	_sphere.SetCenter(_point_center);
	_sphere.SetRadius(radius);
}


/*
**	Return number of face that collide with the bounding sphere
*/
uint ColSphereModel::Count() const
{
	return _sc->GetNbTouchedPrimitives();
}

/*
** colliding faces
*/
uint	ColSphereModel::Index(uint i) const
{
	return _sc->GetTouchedPrimitives()[i];
}


void ColSphereModel::Info(uint i, float& dist) const
{
	dist = fsqrt(_sc->GetDistance());
}

