/*********************************************************
**	File name : IrisManualObject.cpp
**  Iris Engine V0.9 "alllaiii"
**	Date of creation: 01/07/2002
**	Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

void IrisManualObject::PrecalculateBoundingSphere()
{
  vect3d center(0.0f, 0.0f, 0.0f);
  uint32 i = _count;
  vect3d *v = _vertex_buffer;
  while (i--)
  {
    center += *_vertex_buffer / (float)_count;
    v++;
  }
  _center = center;
  v = _vertex_buffer;
  i = _count;
  while (i--)
  {
    vect3d tmp = (center - *_vertex_buffer);
    vect3d vect(tmp._x, tmp._y, tmp._z);
    if (vect.Magnitude() > _radius)
      _radius = vect.Magnitude();
    v++;
  }
}

void IrisManualObject::Precalculate2D()
{
  uint i = _count;
  vertex2dtl *v2d = _2d_vertex;
  vect3d *v3d = _vertex_buffer;
  while (i--)
  {
    v2d->x = v3d->_x;
    v2d->y = v3d->_y;
    v2d->z = v3d->_z;
    v2d++;
    v3d++;
  }
}

void IrisManualObject::Destroy()
{
  printf("[INFO] Deleting manual object\n");
  if (_vertex_buffer)
    delete[] _vertex_buffer;
  if (_trans_vertex)
    delete[] _trans_vertex;
  if (_index)
    delete[] _index;
  if (_2d_vertex)
    delete[] _2d_vertex;
  if (_normal)
    delete[] _normal;
  if (_trans_normal)
    delete[] _trans_normal;
  if (_color)
    delete[] _color;
  if (_material)
  {
    printf("[INFO] Deleting material '%s'\n", _material->GetTexture(0)->GetFileName());
    IrisContext::Get().GetMaterialManager().Unload(_material);
  }
  _color = NULL;
  _normal = NULL;
  _trans_normal = NULL;
  _index = NULL;
  _2d_vertex = NULL;
  _vertex_buffer = NULL;
  _trans_vertex = NULL;
  _count = 0;
}

void IrisManualObject::Allocate(unsigned int count)
{
  _vertex_buffer = new vect3d[count];
  _trans_vertex = new vect3d[count];
  _index = new uint16[count];
  _2d_vertex = new vertex2dtl[count];
  _trans_normal = new vect3d[count];
  _normal = new vect3d[count];
  _color = new uint32[count];
  for (unsigned int i = 0; i < count; ++i)
  {
    _index[i] = i; // face index
    _2d_vertex[i].oargb = 0xff000000;
    _2d_vertex[i].argb = 0xff000000;
    _color[i] = 0xffffffff;
  }
  _count = count;
}

void IrisManualObject::TransformAndLightAndDraw()
{
  static IrisContext &context = IrisContext::Get();
  // normal tranfo
  // 3d convertion
  if (_clipped == IrisFrustrum::outside)
    return;
  context.SetMaterial(_material);
  if (_lighted)
  {
    _matrix.Load();
    context.ProcessPrimitive(_vertex_buffer, _trans_vertex, _count);
    float *trans = _matrix.SaveTranslation();
    _matrix.Load();
    _matrix.RestoreTranslation(trans);

    context.ProcessNormal(_normal, _trans_normal, _count);
    context.GetLightingPipeline().Compute(_trans_vertex, _2d_vertex, _trans_normal, _count, _color);
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    context.ProcessPrimitive(_trans_vertex, _2d_vertex, _count);
  }
  else
  {
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    _matrix.Apply();
    context.ProcessPrimitive(_vertex_buffer, _2d_vertex, _count, _color);
  }
  if (_clipped == IrisFrustrum::inside)
    context.DrawPrimitive(_2d_vertex, _index, _count);
  else
    context.DrawPrimitiveIntersect(_trans_vertex, _2d_vertex, _index, _count);
}

void IrisManualObject::ClippedAndLighted()
{
  static IrisContext &context = IrisContext::Get();
  _matrix.Load();
  vect3d center = _center;
  mat_trans_single_iris(center._x, center._y, center._z);
  if (!context._soft_clip)
  {
    IrisFrustrum &f = context.GetGeometryPipeline().GetFrustrum();
    _clipped = f.TestSphere(center, _radius);
  }
  if (context._lighting && _clipped != IrisFrustrum::outside)
    _lighted = context.GetLightingPipeline().IsInLight(center, _radius);
}

void IrisManualObject::Transform()
{
  static IrisContext &context = IrisContext::Get();
  if (_clipped != IrisFrustrum::outside)
  {
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    context.ProcessPrimitive(_vertex_buffer, _trans_vertex, _count);
  }
}

void IrisManualObject::Draw()
{
  if (_clipped != IrisFrustrum::outside)
  {
    static IrisContext &context = IrisContext::Get();
    context.SetMaterial(_material);
    context.DrawPrimitive(_2d_vertex, _index, _count);
  }
}
