/*********************************************************
**  File name : IrisObject.cpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************
**	MODIFIED gruiiik : replaced for i = 0 by while (i--)
*********************************************************/
#include "Iris.hpp"


void	IrisObject::TransformAndLightAndDrawMesh(int mesh_index, matrix *mat)
{
	IrisContext	&context = IrisContext::Get();
	IrisImportMesh	&mesh = _object->GetMesh(mesh_index);
	if (mesh.GetClipped() == IrisFrustrum::outside)
		return;
	context.SetMaterial(mesh.GetMaterial());
	uint32 vertex_index = _current_animation * mesh.GetNumVertex();
	uint32 *color = mesh.GetColor();
	if (mesh.GetLighted())
	{
		mat->Load();
		context.ProcessPrimitive(mesh.GetVertex(vertex_index), mesh.GetTransformedVertex(), mesh.GetNumVertex());
		float* trans = mat->SaveTranslation();
		mat->Load();
		mat->RestoreTranslation(trans);
		context.ProcessNormal(mesh.GetNormal(_current_animation), mesh.GetTransformedNormal(), mesh.GetNumVertex());
		context.GetLightingPipeline().Compute(mesh.GetTransformedVertex(), mesh.Get2DVertex() ,mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
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
		while (section --)
			context.DrawPrimitive(	mesh.Get2DVertex(),
			mesh.GetSection(section).GetIndicePtr(),
			mesh.GetSection(section).GetNumIndice());
	}
	else
	{
		while (section --)
			context.DrawPrimitiveIntersect(mesh.GetVertex(), mesh.Get2DVertex(),
			mesh.GetSection(section).GetIndicePtr(),
			mesh.GetSection(section).GetNumIndice());
	}
}

void	IrisObject::TransformAndLightAndDraw()
{
	int			i;
	i = _object->GetNumMesh();
	while (i--)
	{
		TransformAndLightAndDrawMesh(i, &_object_matrix);
	}
}

//MODIFIED by Heinrich Tillack
void	IrisObject::TransformAndLightAndDrawInterpolMesh(int mesh_index, matrix *mat)
{
	IrisContext		&context = IrisContext::Get();
	IrisImportMesh	&mesh = _object->GetMesh(mesh_index);
	if (mesh.GetClipped() == IrisFrustrum::outside)
		return;
	context.SetMaterial(mesh.GetMaterial());
	uint32 vertex_index = _current_animation * mesh.GetNumVertex();
	uint32 vertex_index_next = _current_animation_next * mesh.GetNumVertex();
	uint32 *color = mesh.GetColor();
	if (mesh.GetLighted())
	{
		_object_matrix.Load();
		context.ProcessPrimitive(_fps,_framecount,mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next),
								mesh.GetTransformedVertex(), mesh.GetNumVertex());
		float* trans = mat->SaveTranslation();
		mat->Load();
		mat->RestoreTranslation(trans);
		context.ProcessNormal(	_fps,_framecount,mesh.GetNormal(_current_animation),mesh.GetNormal(_current_animation_next), 
								mesh.GetTransformedNormal(), mesh.GetNumVertex());
		context.GetLightingPipeline().Compute(mesh.GetTransformedVertex(), mesh.Get2DVertex() ,mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
		context.GetGeometryPipeline().LoadTrasformationMatrix();
		context.ProcessPrimitive(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetNumVertex());
	}
	else
	{
		context.GetGeometryPipeline().LoadTrasformationMatrix();
		mat->Apply();
		context.ProcessPrimitiveInterpol(	_fps,_framecount,mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next), 
											mesh.Get2DVertex(), mesh.GetNumVertex(), color);
	}
	int section = mesh.GetNumSection();
	if (mesh.GetClipped() == IrisFrustrum::inside)
	{
		while (section --)
			context.DrawPrimitive(	mesh.Get2DVertex(),
			mesh.GetSection(section).GetIndicePtr(),
			mesh.GetSection(section).GetNumIndice());
	}
	else
	{
		while (section --)
			context.DrawPrimitiveIntersect(mesh.GetVertex(), mesh.Get2DVertex(),
			mesh.GetSection(section).GetIndicePtr(),
			mesh.GetSection(section).GetNumIndice());
	}

}

void	IrisObject::TransformAndLightAndDrawInterpol()
{
    int			i(_object->GetNumMesh());
    while (i--)
		TransformAndLightAndDrawInterpolMesh(i, &_object_matrix);
}

void	IrisObject::TransformAndLightAndDrawNoClip()
{
    int			i(_object->GetNumMesh());
    IrisContext		&context = IrisContext::Get();

	while(i--)
    {	
        IrisImportMesh	&mesh = _object->GetMesh(i);
        uint32 vertex_index = _current_animation * mesh.GetNumVertex();
        uint32 vertex_index_next = _current_animation_next * mesh.GetNumVertex();
        uint32 *color = mesh.GetColor() + vertex_index;
        context.SetMaterial(mesh.GetMaterial());
        if (mesh.GetLighted())
        {
            _object_matrix.Load();
            context.ProcessPrimitive(mesh.GetVertex(vertex_index), mesh.GetTransformedVertex(), mesh.GetNumVertex());
            float* trans = _object_matrix.SaveTranslation();
            _object_matrix.Load();
            _object_matrix.RestoreTranslation(trans);
            context.ProcessNormal(mesh.GetNormal(_current_animation), mesh.GetTransformedNormal(), mesh.GetNumVertex());
            context.GetLightingPipeline().Compute(	mesh.GetTransformedVertex(), mesh.Get2DVertex(), 
													mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
            context.GetGeometryPipeline().LoadTrasformationMatrix();
            context.ProcessPrimitive(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetNumVertex());
        }
        else
        {
            context.GetGeometryPipeline().LoadTrasformationMatrix();
            _object_matrix.Apply();
            context.ProcessPrimitiveInterpol(	_fps,_framecount,mesh.GetVertex(vertex_index), 
												mesh.GetVertex(vertex_index_next),mesh.Get2DVertex(), 
												mesh.GetNumVertex(), color);
        }
		int section = mesh.GetNumSection();
		while (section --)
			context.DrawPrimitive(	mesh.Get2DVertex(),
									mesh.GetSection(section).GetIndicePtr(),
									mesh.GetSection(section).GetNumIndice());
	}
}


void	IrisObject::TransformAndLightAndDrawNoClipInterpol()
{
    int			i;
    IrisContext		&context = IrisContext::Get();
    // normal tranfo
    // 3d convertion
    i = _object->GetNumMesh();
    while (i--)
    {
        IrisImportMesh	&mesh = _object->GetMesh(i);

        context.SetMaterial(mesh.GetMaterial());
        uint32 vertex_index = _current_animation * mesh.GetNumVertex();
        uint32 vertex_index_next = _current_animation_next * mesh.GetNumVertex();
        uint32 *color = mesh.GetColor();
        if (mesh.GetLighted())
        {
				_object_matrix.Load();
				//MODIFIED by Heinrich Tillack
				context.ProcessPrimitive(	_fps,_framecount,mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next), 
											mesh.GetTransformedVertex(), mesh.GetNumVertex());
				float* trans = _object_matrix.SaveTranslation();
				_object_matrix.Load();
				_object_matrix.RestoreTranslation(trans);
				context.ProcessNormal(	_fps, _framecount, mesh.GetNormal(_current_animation), 
										mesh.GetNormal(_current_animation_next), mesh.GetTransformedNormal(), 
										mesh.GetNumVertex());
				context.GetLightingPipeline().Compute(	mesh.GetTransformedVertex(), mesh.Get2DVertex(), 
														mesh.GetTransformedNormal(), mesh.GetNumVertex(), color);
				context.GetGeometryPipeline().LoadTrasformationMatrix();
				context.ProcessPrimitive(mesh.GetTransformedVertex(), mesh.Get2DVertex(), mesh.GetNumVertex());
        }
        else
        {
            context.GetGeometryPipeline().LoadTrasformationMatrix();
            _object_matrix.Apply();
            context.ProcessPrimitiveInterpol(	_fps, _framecount, mesh.GetVertex(vertex_index), mesh.GetVertex(vertex_index_next), 
												mesh.Get2DVertex(), mesh.GetNumVertex(), color);
        }
		int section = mesh.GetNumSection();
		while (section --)
			context.DrawPrimitive(mesh.Get2DVertex(),
			mesh.GetSection(section).GetIndicePtr(),
			mesh.GetSection(section).GetNumIndice());
    }
}


void	IrisObject::ClippedAndLighted()
{
    // clip test
    int	i = _object->GetNumMesh();
    while (i--)
		ClippedAndLightedMesh(i, &_object_matrix);
}

void	IrisObject::ClippedAndLightedMesh(int mesh_index, matrix *mat)
{
    IrisContext		&context = IrisContext::Get();
	IrisImportMesh	&mesh = _object->GetMesh(mesh_index);
    vect3d	center(mesh.GetCenter());
	mat->Load();
    mat_trans_single_iris(center._x, center._y, center._z);
    IrisFrustrum& f = context.GetGeometryPipeline().GetFrustrum();
    mesh.SetClipped(f.TestSphere(center, mesh.GetRadius()));
    if (mesh.GetClipped() != IrisFrustrum::outside && context.GetLightingPipeline().Count())
        mesh.SetLighted(context.GetLightingPipeline().IsInLight(center, mesh.GetRadius()));
    else
        mesh.SetLighted(false);
}


void	IrisObject::Transform()
{
    IrisContext::Get().GetGeometryPipeline().LoadTrasformationMatrix();
    int	i = _object->GetNumMesh();
    while (i--)
    {
        IrisImportMesh	&mesh = _object->GetMesh(i);
        if (mesh.GetClipped() == IrisFrustrum::outside)
            continue;
		IrisContext::Get().ProcessPrimitiveInterpol(_fps,_framecount,mesh.GetVertex(_current_animation * mesh.GetNumVertex()),
		mesh.GetVertex(_current_animation_next * mesh.GetNumVertex()),mesh.Get2DVertex(), mesh.GetNumVertex());
    }
}

void	IrisObject::Draw()
{
    IrisContext		&context = IrisContext::Get();
    int	i = _object->GetNumMesh();
    while (i--)
    {
        IrisImportMesh	&mesh = _object->GetMesh(i);
        if (mesh.GetClipped() == IrisFrustrum::outside)
            continue;
        context.SetMaterial(mesh.GetMaterial());
        int section = mesh.GetNumSection();
        while (section --)
            context.DrawPrimitive(mesh.Get2DVertex(),
                                  mesh.GetSection(section).GetIndicePtr(),
                                  mesh.GetSection(section).GetNumIndice());
    }
}
