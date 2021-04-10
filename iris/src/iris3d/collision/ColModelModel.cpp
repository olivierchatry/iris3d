/*********************************************************
** File name : ColModelModel.cpp
** Iris Engine V0.7 "presque"
** Date of creation: 26/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"
#include "../opcode/Opcode.h"

using namespace Opcode;
#include "collision/ColModel.hpp"
#include "collision/ColModelModel.hpp"

ColModelModel::ColModelModel()
{
  _tc = new AABBTreeCollider;
  _cache = new BVTCache;
}

ColModelModel::~ColModelModel()
{
  delete _tc;
  delete _cache;
}

void ColModelModel::Build(ColModel &first, ColModel &second, bool report_all)
{
  if (!report_all)
  {
    _tc->SetFirstContact(true);
    _tc->SetTemporalCoherence(true);
  }
  else
  {
    _tc->SetFirstContact(false);
    _tc->SetTemporalCoherence(false);
  }
  _tc->SetFullBoxBoxTest(false);  // use coarse BV-BV tests
  _tc->SetFullPrimBoxTest(false); // use coarse primitive-BV tests
  _tc->SetCallback0(ColModel::callback, first.userdata());
  _tc->SetCallback1(ColModel::callback, second.userdata());

  _cache->Model0 = first._model;
  _cache->Model1 = second._model;
}

bool ColModelModel::Query(const matrix &first, const matrix &second)
{
  return _tc->Collide(*_cache, (Matrix4x4 *)&first, (Matrix4x4 *)&second) && _tc->GetContactStatus();
}

uint ColModelModel::Count() const
{
  return _tc->GetNbPairs();
}

void ColModelModel::Get(unsigned i, unsigned long &first, unsigned long &second) const
{
  first = _tc->GetPairs()[i].id0;
  second = _tc->GetPairs()[i].id1;
}
