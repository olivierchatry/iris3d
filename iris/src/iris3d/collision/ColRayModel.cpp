/*********************************************************
** File name : ColRayModel.cpp
** Iris Engine V0.8 "tada"
** Date of creation: 11/11/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include	"Iris.hpp"
using namespace Opcode;

/*
**	constructor, allocate memory
*/
ColRayModel::ColRayModel()
{
	_rc = new RayCollider;
	_faces = new CollisionFaces;
}

/*
**	destructor, delete allocated memory
*/
ColRayModel::~ColRayModel()
{
	delete _rc;
	delete _faces;
}

/*
**	query collison beetween model and ray
*/

bool	ColRayModel::Query(ColModel &model, const matrix& mat, float max_len, bool report_all)
{
	static	udword	cache;
	if (!report_all)
	{
		_rc->SetFirstContact(true);
		_rc->SetTemporalCoherence(true);
	}
	else
	{
		_rc->SetFirstContact(false);
		_rc->SetTemporalCoherence(false);
	}
	_faces->Reset();
	_rc->SetClosestHit(true);
	_rc->SetCulling(false);
	_rc->SetDestination(_faces);
	_rc->SetCallback(ColModel::callback, (udword) &model);
	_rc->SetMaxDist(max_len);
	return _rc->Collide(_ray, model._model, (Matrix4x4* )&mat, &cache) && Count() != 0;
}

/*
**	Build opcode from iris value
*/
void	ColRayModel::Build(const vect3d &pos, const vect3d &dir)
{
	VERTEXTOPOINT(pos, _ray.mOrig);
	VERTEXTOPOINT(dir, _ray.mDir);
}

/*
**	return number of colliding face with the ray
*/
uint	ColRayModel::Count() const
{
	return _faces->GetNbFaces();
}

/*
**	return face's index of collision [i]
*/
uint	ColRayModel::Index(uint i) const
{
	return _faces->GetFaces()[i].mFaceID;
}

/*
**	return information about collision [i]
*/
void	ColRayModel::Info(uint i, float& dist)
{
	dist = fsqrt(_faces->GetFaces()[i].mDistance);
}
