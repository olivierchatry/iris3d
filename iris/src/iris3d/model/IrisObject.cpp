/*********************************************************
**  File name : IrisObject.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"

void IrisObject::TransformAndLightAndDrawMesh(int mesh_index, matrix *mat)
{
  IrisContext &context = IrisContext::Get();
  IrisImportMesh &mesh = _object->GetMesh(mesh_index);
  if (mesh.GetClipped() == IrisFrustrum::outside)
    return;
  context.SetMaterial(mesh.GetMaterial());
  uint32 vertex_index = _current_animation * mesh.GetNumVertex();
  uint32 *color = mesh.GetColor();
  if (mesh.GetLighted())
  {
    mat->Load();
    context.ProcessPrimitive(mesh.GetVertex(vertex_index), mesh.GetTransformedVertex(), mesh.GetNumVertex());
    float *trans = mat->SaveTranslation();
    mat->Load();
    mat->RestoreTranslation(trans);
    context.ProcessNormal(mesh.GetNormal(_current_animation), mesh.GetTransformedNormal(), mesh.GetNumVertex());
    context.GetLightingPipeline().Compute(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    context.ProcessPrimitive(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetNumVertex());
  }
  else
  {
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    mat->Apply();
    context.ProcessPrimitive(mesh.GetVertex(vertex_index), mesh.Get2DVertex(), mesh.GetNumVertex(), color);
  }
  int section = mesh.GetNumSection();
  if (mesh.GetClipped() == IrisFrustrum::inside)
  {
    while (section--)
      context.DrawPrimitive(mesh.Get2DVertex(),
                            mesh.GetSection(section).GetIndicePtr(),
                            mesh.GetSection(section).GetNumIndice());
  }
  else
  {
    while (section--)
      context.DrawPrimitiveIntersect(mesh.GetVertex(), mesh.Get2DVertex(),
                                     mesh.GetSection(section).GetIndicePtr(),
                                     mesh.GetSection(section).GetNumIndice());
  }
}

void IrisObject::TransformAndLightAndDraw()
{
  int i;

  i = _object->GetNumMesh();
  while (i--)
  {
    TransformAndLightAndDrawMesh(i, &_object_matrix);
  }
}

//MODIFIED by Heinrich Tillack
void IrisObject::TransformAndLightAndDrawInterpolMesh(int mesh_index, matrix *mat)
{
  IrisContext &context = IrisContext::Get();
  IrisImportMesh &mesh = _object->GetMesh(mesh_index);
  if (mesh.GetClipped() == IrisFrustrum::outside)
    return;
  context.SetMaterial(mesh.GetMaterial());
  uint32 vertex_index = _current_animation * mesh.GetNumVertex();
  uint32 vertex_index_next = _current_animation_next * mesh.GetNumVertex();
  uint32 *color = mesh.GetColor();
  if (mesh.GetLighted())
  {
    _object_matrix.Load();
    context.ProcessPrimitive(_fps, _framecount, mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next),
                             mesh.GetTransformedVertex(), mesh.GetNumVertex());
    float *trans = mat->SaveTranslation();
    mat->Load();
    mat->RestoreTranslation(trans);
    context.ProcessNormal(_fps, _framecount, mesh.GetNormal(_current_animation), mesh.GetNormal(_current_animation_next),
                          mesh.GetTransformedNormal(), mesh.GetNumVertex());
    context.GetLightingPipeline().Compute(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    context.ProcessPrimitive(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetNumVertex());
  }
  else
  {
    context.GetGeometryPipeline().LoadTrasformationMatrix();
    mat->Apply();
    context.ProcessPrimitiveInterpol(_fps, _framecount, mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next),
                                     mesh.Get2DVertex(), mesh.GetNumVertex(), color);
  }
  int section = mesh.GetNumSection();
  if (mesh.GetClipped() == IrisFrustrum::inside)
  {
    while (section--)
      context.DrawPrimitive(mesh.Get2DVertex(),
                            mesh.GetSection(section).GetIndicePtr(),
                            mesh.GetSection(section).GetNumIndice());
  }
  else
  {
    while (section--)
      context.DrawPrimitiveIntersect(mesh.GetVertex(), mesh.Get2DVertex(),
                                     mesh.GetSection(section).GetIndicePtr(),
                                     mesh.GetSection(section).GetNumIndice());
  }
}
void IrisObject::Destroy()
{
  IrisContext::Get().GetObjectManager().Unload(_object);
  _current_animation = 0;
  _current_animation_next = 0;
  _loop_begin = 0;
  _loop_end = 0;
  _framecount = 0;
  _fps = 1;
  _animation = false;
}

void IrisObject::TransformAndLightAndDrawInterpol()
{
  int i(_object->GetNumMesh());
  while (i--)
  {
    TransformAndLightAndDrawInterpolMesh(i, &_object_matrix);
  }
}

void IrisObject::ClippedAndLighted()
{
  // clip test
  int i = _object->GetNumMesh();
  while (i--)
    ClippedAndLightedMesh(i, &_object_matrix);
}

void IrisObject::ClippedAndLightedMesh(int mesh_index, matrix *mat)
{
  IrisContext &context = IrisContext::Get();
  IrisImportMesh &mesh = _object->GetMesh(mesh_index);
  vect3d center(mesh.GetCenter());
  mat->Load();
  mat_trans_single_iris(center._x, center._y, center._z);
  IrisFrustrum &f = context.GetGeometryPipeline().GetFrustrum();
  if (!context._soft_clip)
    mesh.SetClipped(f.TestSphere(center, mesh.GetRadius()));
  if (context._lighting && mesh.GetClipped() != IrisFrustrum::outside && context.GetLightingPipeline().Count())
    mesh.SetLighted(context.GetLightingPipeline().IsInLight(center, mesh.GetRadius()));
  else
    mesh.SetLighted(false);
}
