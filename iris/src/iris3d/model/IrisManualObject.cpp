/*********************************************************
**	File name : IrisManualObject.cpp
**	Iris Engine V0.7 "presque"
**	Date of creation: 01/07/2002
**	Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

void	IrisManualObject::PrecalculateBoundingSphere()
{
    vect3d	center(0.0, 0.0, 0.0);
    for (unsigned i = 0; i < _count; ++i)
        center += _vertex_buffer[i] / (float) _count;
    _center = center;
    for (unsigned j = 0; j < _count; ++j)
    {
        vect3d tmp = (center - _vertex_buffer[j]);
        vect3d	vect(tmp._x, tmp._y, tmp._z);
        if (vect.Magnitude() > _radius)
            _radius = vect.Magnitude();
    }
}

void	IrisManualObject::Precalculate2D()
{
    for (unsigned i = 0; i < _count; ++i)
    {
        _2d_vertex[i].x = _vertex_buffer[i]._x;
        _2d_vertex[i].y = _vertex_buffer[i]._y;
        _2d_vertex[i].z = _vertex_buffer[i]._z;
    }
}

void	IrisManualObject::Delete()
{
    if (_vertex_buffer)
        delete [] _vertex_buffer;
    if (_trans_vertex)
        delete [] _trans_vertex;
    if (_index)
        delete [] _index;
    if(_2d_vertex)
        delete [] _2d_vertex;
    if(_normal)
        delete [] _normal;
    if(_trans_normal)
        delete [] _trans_normal;
    if (_color)
        delete [] _color;
    _color	= NULL;
    _normal = NULL;
    _trans_normal = NULL;
    _index = NULL;
    _2d_vertex = NULL;
    _vertex_buffer = NULL;
    _trans_vertex = NULL;
    _count = 0;
}

void	IrisManualObject::Allocate(unsigned int count)
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

void	IrisManualObject::TransformAndLightAndDraw()
{
    IrisContext		&context = IrisContext::Get();
    // normal tranfo
    // 3d convertion
    if (_clipped == IrisFrustrum::outside)
        return;
    context.SetMaterial(_material);
    if (_lighted)
    {
        _matrix.Load();
        context.ProcessPrimitive(_vertex_buffer, _trans_vertex, _count);
        float* trans = _matrix.SaveTranslation();
        _matrix.Load();
        _matrix.RestoreTranslation(trans);

        context.ProcessNormal(_normal, _trans_normal, _count);
        context.GetLightingPipeline().Compute(_trans_vertex, _2d_vertex , _trans_normal, _count, _color);
        context.GetGeometryPipeline().LoadTrasformationMatrix();
        context.ProcessPrimitive(_trans_vertex, _2d_vertex, _count);
        if (_clipped == IrisFrustrum::inside)
            context.DrawPrimitive(_2d_vertex, _index, _count);
        else
            context.DrawPrimitiveIntersect(_trans_vertex, _2d_vertex, _index, _count);
    }
    else
    {
        context.GetGeometryPipeline().LoadTrasformationMatrix();
        _matrix.Apply();
        context.ProcessPrimitive(_vertex_buffer, _2d_vertex, _count, _color);
        if (_clipped == IrisFrustrum::inside)
            context.DrawPrimitive(_2d_vertex, _index, _count);
        else
            context.DrawPrimitiveIntersect(_vertex_buffer, _2d_vertex, _index, _count);
    }

}


void	IrisManualObject::TransformAndLightAndDrawNoClip()
{
    IrisContext		&context = IrisContext::Get();
    // normal tranfo
    // 3d convertion
    context.SetMaterial(_material);
    if (_lighted)
    {
        _matrix.Load();
        context.ProcessPrimitive(_vertex_buffer, _trans_vertex, _count);
        float* trans = _matrix.SaveTranslation();
        _matrix.Load();
        _matrix.RestoreTranslation(trans);
        context.ProcessNormal(_normal, _trans_normal, _count);
        context.GetLightingPipeline().Compute(_trans_vertex, _2d_vertex , _trans_normal, _count, _color);
        context.GetGeometryPipeline().LoadTrasformationMatrix();
        context.ProcessPrimitive(_trans_vertex, _2d_vertex, _count);
        if (_clipped == IrisFrustrum::inside)
            context.DrawPrimitive(_2d_vertex, _index, _count);
        else
            context.DrawPrimitiveIntersect(_trans_vertex, _2d_vertex, _index, _count);
    }
    else
    {
        context.GetGeometryPipeline().LoadTrasformationMatrix();
        _matrix.Apply();
        context.ProcessPrimitive(_vertex_buffer, _2d_vertex, _count,  _color);
        if (_clipped == IrisFrustrum::inside)
            context.DrawPrimitive(_2d_vertex, _index, _count);
        else
            context.DrawPrimitiveIntersect(_vertex_buffer, _2d_vertex, _index, _count);
    }
}


void	IrisManualObject::ClippedAndLighted()
{
    IrisContext		&context = IrisContext::Get();
    IrisFrustrum& f = context.GetGeometryPipeline().GetFrustrum();

    _matrix.Load();
    vect3d	center = _center;
    mat_trans_single_iris(center._x, center._y, center._z);
    _clipped = f.TestSphere(center, _radius);
    if (_clipped != IrisFrustrum::outside)
        _lighted = context.GetLightingPipeline().IsInLight(center, _radius);
}

void	IrisManualObject::Transform()
{
    if (_clipped != IrisFrustrum::outside)
    {
        IrisContext::Get().GetGeometryPipeline().LoadTrasformationMatrix();
        IrisContext::Get().ProcessPrimitive(_vertex_buffer, _trans_vertex, _count);
    }
}

void	IrisManualObject::Draw()
{
    if (_clipped != IrisFrustrum::outside)
    {
        IrisContext		&context = IrisContext::Get();
        context.SetMaterial(_material);
        context.DrawPrimitive(_2d_vertex, _index, _count);
    }
}

