/*********************************************************
**  File name : IrisGeometryPipeline.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __GEOMETRYPIPELINE_HPP__
#define __GEOMETRYPIPELINE_HPP__

//! IrisGeometryPipeline
/*!
	Geometry pipeline is the core of iris. It handle
	geometry transformation tought four matrix:
	- The view matrix, represanting the camera.
	- The projection matrix, representing how 3d vertex are projected to 2d screen.
	- The viewport matrix, that handle where the vertex will be projected on screen.
	\note Normally, you do not use directly the geometry pipeline. 
*/
class IrisGeometryPipeline
{
protected:
	//! The view matrix, representing the camera 
	matrix		_view			__attribute__((aligned(32)));
	//! The viewport matrix, representing the screen window where the dreamcast will render the scene. 
	matrix		_viewport		__attribute__((aligned(32)));
	//! The projection matrix, representing how 3d vertex will be projected to the 2d screen.
	matrix		_projection		__attribute__((aligned(32)));
	//! The concatenation matrix, projection * viewport * view.
	matrix		_concatenation	__attribute__((aligned(32)));
	//! The transformation matrix, concatenation * object matrix.
	matrix		_transformation __attribute__((aligned(32)));
	//! The frustrum of the camera.
	IrisFrustrum	_frustrum;
	//! The camera direction. Basically at - eye vector
	vect3d		_camera_dir;
public:
	//! IrisGeometryPipeline
	/*!
	Default constructor. Initialize all tranformation matrix.
	*/
	IrisGeometryPipeline()
	{
		_view.Identity();
		_viewport.Identity();
		_projection.Identity();
		_transformation.Identity();
		mat_identity();
	}
	//! SetProjection
	/*!
	Define the projection matrix.
		\param fov Horizontal field of view. The field of view is basically the angle
		beetwen frustrum pane.
		\param znear The near plane clipping.
		\param zfar The far plane clipping.
		\return Nothing.
	*/
	void	SetProjection(float fov, float znear, float zfar)
	{
		float radiant_fov = IRIS_DEGTORAD(fov);
		_projection = matrix::CreateProjection(znear, zfar, radiant_fov, radiant_fov * 2 / 3);
		_frustrum.SetProjection(znear, zfar, fov);
	}

	//! SetProjection
	/*!
	Define the projection matrix.
	\param projection A matrix that directly represent the projection matrix.
	\return Nothing.
	*/
	void	SetProjection(const matrix &projection)
	{
		_projection = projection;
	}

	//! SetViewport
	/*!
	Define the viewport matrix.
	\param m A matrix that directly represent the viewport matrix.
	\return Nothing.
	*/
	void	SetViewport(const matrix &m)
	{
		_viewport = m;
	}

	//! SetViewport
	/*!
	Define the viewport matrix.
	\param x Top coordinate of rendering window.
	\param y Left coordinate of rendering window.
	\param width Width of the render window.
	\param height Height of the render window.
	\return Nothing.
	*/
	void	SetViewport(float x, float y, float width, float height, float min_z = 0.0f, float max_z = 1.0f)
	{
		// glScissor((int) x, (int) y, (int) width, (int) height);
		_viewport =	matrix::CreateViewPort(x, y, width, height, min_z, max_z);
	}

	//! SetView
	/*!
	Define the view (Camera) matrix.
	\param m A matrix that directly represent the view (Camera) matrix.
	\return Nothing.
	*/
	void	SetView(const matrix &m)
	{
		_view = m;
	}

	//! SetView
	/*!
	Define the view (Camera) matrix.
	\param camera An IrisCamera.
	\return Nothing.
	*/
	void	SetView(IrisCamera &camera)
	{
		SetView(camera.GetEye(), camera.GetAt(), camera.GetUp());
	}

	//! SetView
	/*!
	Define the view (Camera) matrix.
	\param eye Position of the camera.
	\param at Position of were the camera look.
	\param up Perpendicular vector of the camera. Generaly (0.0f,1.0f,0.0f)
	\return Nothing.
	*/
	void	SetView(const vect3d &eye, const vect3d &at, const vect3d &up)
	{
		_view = matrix::CreateLookAt(eye, at, up);
		_camera_dir = at - eye;
		_camera_dir.Normalize();
		_frustrum.SetView(eye, at, up);
	}
	//! GetProjectionMatrix
	/*!
		Get the projection matrix
		\param projection The matrix that will receive the projection matrix.
		\return Nothing.
	*/
	void	GetProjectionMatrix(matrix &projection)
	{
		projection = _projection;
	}

	//! GetTransformationMatrix
	/*!
	Get the transformation matrix
	\param transformation The matrix that will receive the transformation matrix.
	\return Nothing.
	*/
	void	GetTransformationMatrix(matrix	&transformation)
	{
		transformation = _transformation;
	}

	//! GetViewportMatrix
	/*!
	Get the viewport matrix
	\param viewport The matrix that will receive the viewport matrix.
	\return Nothing.
	*/
	void	GetViewportMatrix(matrix &viewport)
	{
		viewport = _viewport;
	}
	//! GetCameraDir
	/*!
	Return the current camera direction. ie. (at - eye) vector
	\return The vector that represent the camera direction.
	*/
	vect3d	&GetCameraDir()
	{
		return _camera_dir;
	}
	//! GetViewMatrix
	/*!
	Get the view (Camera) matrix
	\param view The matrix that will receive the view matrix.
	\return Nothing.
	*/
	void	GetViewMatrix(matrix &view)
	{
		view = _view;
	}
	//! SetTransformationMatrix
	/*!
	Define the transformation matrix. The transformation matrix
	is the concatenation beetwen all matrix (world, object, etc).
	*/
	void	SetTransformationMatrix(const matrix	&transformation)
	{
		_transformation = transformation;
	}

	//! LoadTrasformationMatrix
	/*!
	Load concatenation matrix ( all matrix without object space matrix )
	into sh4 register.
	\return Nothing.
	*/
	void	LoadTrasformationMatrix()
	{
		_concatenation.Load();
	}
	
	//! ApplyObjectMatrix
	/*!
	Multiply concatenation matrix with object matrix using sh4 instruction,
	store it into transformation matrix.
	\param object_matrix The object transformation matrix.
	\return Nothing.
	*/
	void	ApplyObjectMatrix(matrix &object_matrix)
	{
		_concatenation.Load();
		mat_apply(&(object_matrix._m));
		mat_store(&(_transformation._m));
	}
	//! Update
	/*!
		Update the goemtry pipeline concatenation matrix.
		This function need to be called whenever you
		modfify Project, Viewport or View (Camera) matrix.
		It also compute the camera frustrum.
		\return A reference to the concatenation matrix.
	*/
	matrix	&Update()
	{
		// generate global operation
		_viewport.Load();
		mat_apply(&(_projection._m));
		mat_apply(&(_view._m));
		mat_store(&(_concatenation._m));
		_frustrum.Compute();
		return _concatenation;
	}

	//! GetFrustrum
	/*!
	Return the camera frustrum.
	\return A reference to the camera frustrum.
	*/
	IrisFrustrum&	GetFrustrum()
	{
		return _frustrum;
	}

	//! Transform
	/*!
	Tranform a 3d vertex to a 2d, projected on screen vertex.
	\param source Source 3d vertex.
	\param target Target 2d vertex.
	\return Nothing.
	*/
	void			Transform(const vect3d& source, vertex2dtl* target)
	{
		target->x = source._x;
		target->y = source._y;
		target->z = source._z;
		mat_trans_single_my(target->x, target->y, target->z);
	}

	//! Transform
	/*!
	Tranform a 3d vertex to a 2d, projected on screen vertex.
	This function lerp (Linear interpolation) position beetwen two 
	3D vertex. It's mainly used for animation interpolation.
	\param source Source 3d vertex.
	\param source2 Source 3d vertex.
	\param target Target 2d vertex.
	\return Nothing.
	*/
    void			Transform(const vect3d& source,const vect3d& source2, vertex2dtl* target, int count, int framecount)
    {
		float	x1 = source._x;
		float	x = x1-((x1-source2._x)/count) * framecount;
		float	y1 = source._y;
		float	y = y1 - ((y1-source2._y) / count) * framecount;
		float	z1 = source._z;
		float	z = z1- ((z1-source2._z) / count) * framecount;
		target->x = x;
		target->y = y;
		target->z = z;
        mat_trans_single_my(target->x, target->y, target->z);
    }

	//! Transform
	/*!
	Tranform a 3d vertex to a 3d transformed vertex using transformation
	matrix.
	This function lerp (Linear interpolation) position beetwen two 
	3D vertex. It's mainly used for animation interpolation.
	\param source Source 3d vertex.
	\param source2 Source 3d vertex.
	\param target Target 2d vertex.
	\return Nothing.
	*/
    void			Transform(const vect3d& source,const vect3d& source2, vect3d* target, int count, int framecount)
    {

	        float	x1 = source._x;
            float	x = x1 - ((x1-source2._x) / count) * framecount;


            float	y1 = source._y;
            float	y = y1 - ((y1-source2._y) / count) * framecount;

            float	z1 = source._z;
            float	z = z1 - ((z1-source2._z) / count) * framecount;

            target->x = x;
            target->y = y;
            target->z = z;

        	mat_trans_single_iris(target->_x, target->_y, target->_z);
    }

	//! Transform
	/*!
	Tranform a 3d vertex to a 3d transformed vertex using transformation
	matrix.
	\param source 3d vertex.
	\param target 3d vertex.
	\return Nothing.
	*/
	void			Transform(const vect3d& source, vect3d* target)
	{
		*target = source;
		mat_trans_single_iris(target->_x, target->_y, target->_z);
	}
};
#endif //__GEOMETRYPIPELINE_HPP__
