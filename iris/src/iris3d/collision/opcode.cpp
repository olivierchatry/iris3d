/*-----------------------------.-------------------------------------.
| Filename: opcode.cpp         | using opcode lib                    |
| Author  : francis            |                                     |
| Started : 12/05/2002 17:19   |                                     |
`------------------------------|                                     |
                               |                                     |
                               `------------------------------------*/
#include "Iris.hpp"
#include "opcode.hpp"
#include "opcode/Opcode.h"
using namespace Opcode;
/*
vertices
list de strip				-> nbfaces
strip = indexes			-> 

i		
pair	-> i (i + 1)	(i + 2)
impair -> (i + 2) (i + 1)	i

vector<int>		

n 
*/

#if 0

ColRayMesh::ColRayMesh()
{
	_rc = new AABBRayCollider;
	_faces = new CollisionFaces;
}

ColRayMesh::~ColRayMesh()
{
	delete _rc;
	delete _faces;
}

bool ColRayMesh::query(	ColModel& model, const Matrix& mat,
			const Vector& pos, const Vector& dir, float maxlen)
{
	Ray r;
	r.mOrig = *((::Point* )&pos);
	r.mDir = *((::Point* )&dir);
	r.mDir.Normalize();

	_faces->Reset();
	_rc->SetClosestHit(true);
	_rc->SetCulling(false);
	_rc->SetCallbackObj(ColModel::callback);
	_rc->SetUserData(&model);
	_rc->SetMaxDist(maxlen);
	_rc->SetDestination(_faces);
	return _rc->Collide(r, model._model, *((Matrix4x4* )&mat)) && count() != 0;
}

uint ColRayMesh::count() const
{
	return _faces->GetNbFaces();
}

uint ColRayMesh::index(uint i) const
{
	return _faces->GetFaces()[i].mFaceID;
}

void ColRayMesh::info(uint i, float& dist, float& u, float& v) const
{
	CollisionFace& f = _faces->GetFaces()[i];
	dist = fsqrt(f.mDistance);
	u = f.mU;
	v = f.mV;
}

#endif
