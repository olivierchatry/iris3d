/*********************************************************
**  File name : matrix.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

class   vect3d;

//! Matrix class.
/*!
	Class for the matrix objects.
	Includes methodes to manipulate a matrix object.
*/
class	matrix
{
public:
	union 
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		}; /*!< Struct of a matrix.*/
		matrix_t	_m; /*!< matrix_t type of a matrix.*/
		float		_linear[16]; /*!< representation of matrix in an array of 16 floats.*/
		double		_linear_double[8]; /*!< representation of matrix in an array of 8 double, ONLY USED FOR ALIGNEMENT.*/
	};
	//! Constructor of the Matrix class.
	/*!
		Create a matrix identity.
	*/
	matrix() 
	{ 
		Identity(); 
	}
	//!	CreateProjection
	/*!
		Create a matrix for the projection.
		\param near_plane : Distance to near clipping IrisPlane.
		\param far_plane : Distance to far clipping IrisPlane.
		\param fov_horiz : Horizontal field of view angle, in radians.
		\param fov_vert : Vertical field of view angle, in radians.
		\return a matrix of the projection.
	*/
	inline static	matrix CreateProjection(
					const float near_plane,	// Distance to near clipping 
											// IrisPlane
					const float far_plane,	// Distance to far clipping 
								            // IrisPlane
					const float fov_horiz,  // Horizontal field of view 
						                    // angle, in radians
					const float fov_vert)   // Vertical field of view 
							                // angle, in radians
	{
		float    h, w, Q;
 
		w = (float)1 / ftan(fov_horiz*0.5f);  // 1/tan(x) == cot(x)
		h = (float)1 / ftan(fov_vert*0.5f);   // 1/tan(x) == cot(x)
		Q = far_plane / (far_plane - near_plane);
		matrix ret;
		ret.Zero();

		ret._11 = w;
		ret._22 = h;
		ret._33 = Q;
		ret._43 = -Q * near_plane;
		ret._34 = 1;
		return ret;
	}
	//! CreateLookAt function.
	/*!
		Create a matrix for a vision
		\param eye : a reference on a vect3D, the position of the eye.
		\param at : a reference on a vect3D, the position where the eye is pointing.
		\param up : a reference on a vect3D, the rotation of the eye.
		\return a matrix object;
	*/
	static	matrix CreateLookAt(const vect3d &eye, const vect3d &at, const vect3d &up);
	//! CreateLookAt function.
	/*!
		Create a matrix for a camera viewport.
		\param x : a float, the x position of the viewport.
		\param y : a float, the y position of the viewport.
		\param width : a float, the width of the viewport.
		\param height : a float, the height of the viewport.
		\param min_z : a float, the min focus.
		\param max_z : a float, the max focus.
		\return a matrix object;
	*/
	inline static	matrix CreateViewPort(const float x, const float y, const float width, const float height, const float min_z, const float max_z)
	{
		matrix	ret;
		ret._11 = width / 2;
		ret._22 = -height / 2;
		ret._33 = max_z - min_z;
		ret._41 = x + ret._11;
		ret._42 = y - ret._22;
		ret._43 = min_z;
		return ret;
	}
	matrix	operator * (const matrix &other) const
	{
		matrix result;

		for (uint32 i = 0; i < 4; ++i)
			for (uint32 j = 0; j < 4; ++j)
			{
				result._m[i][j] = 0;
				for (uint32 k = 0; k < 4; ++k)
					result._m[i][j] += _m[i][k] * other._m[k][j];
			}
		return result;
	}
	//!	SaveTranslation function.
	/*!
		Used to save the translation of the matrix object in an array of 3 floats and restores them to 0.0f.
		\return an array of 3 floats witch contains the translation.
	*/
	float*	SaveTranslation()
	{
		static float f[3];
		f[0] = _41;
		f[1] = _42;
		f[2] = _43;
		_41 = _42 = _43 = 0.0f;
		return f;
	}	
	//! RestoreTranslation function.
	/*!
		Applies a translation to the matrix object 
		\param trans : an array of 3 floats witch contain a translation.
		\return nothing.
	*/
	void	RestoreTranslation(const float* trans)
	{
		_41 = trans[0];
		_42 = trans[1];
		_43 = trans[2];
	}
	matrix	& operator *= (const matrix & mat) 
	{ 
		return *this = *this * mat; 
	}
	//! Zero function
	/*!
		Set a marix to zero.
		\return a refrence on a matrix object.
	*/
	matrix	& Zero() 
	{ 
		_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = 0.0f;
		return (*this);
	}
	//! Indentity function
	/*!
		Set a marix to indentity, an empty matrix.
		\return a refrence on a matrix object.
	*/
	matrix	& Identity()
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
		return (*this);
	}
	//! RotateX function
	/*!
		Do a rotation to X axis.
		\param angle : a float, the angle of the rotation, in radians.
		\return nothing.
	*/
	void	RotateX(const float angle)
	{
		matrix ret;
		ret._22 = ret._33 = fcos(angle);
		ret._23 = fsin(angle);
		ret._32 = -ret._23;
		ret.Load();
		Apply();
		Store();
	}
	//! RotateY function
	/*!
		Do a rotation to Y axis.
		\param angle : a float, the angle of the rotation, in radians.
		\return nothing.
	*/
	void	RotateY(const float angle)
	{
		matrix ret;
		ret._11 = ret._33 = fcos(angle);
		ret._31 = fsin(angle);
		ret._13 = -ret._31;
		ret.Load();
		Apply();
		Store();
	}
	//! RotateZ function
	/*!
		Do a rotation to Z axis.
		\param angle : a float, the angle of the rotation, in radians.
		\return nothing.
	*/
	void	RotateZ(const float angle)
	{
		matrix ret;
		ret._11 = ret._22 = fcos(angle);
		ret._12 = fsin(angle);
		ret._21 = -ret._12;
		ret.Load();
		Apply();
		Store();
	}
	//! Print function.
	/*!
		Print the matrix object values to the standard output.
		\return nothing.
	*/
	void	Print()
	{
		for (int i = 0; i < 4; i ++)
		{
			printf("[ ");
			for (int j = 0; j < 4; j ++)
				printf("\t%3.3f ", (double) _m[i][j]);
			printf("]\n");
		}
	}
	//! Translate function.
	/*!
		Applies a translation to the matrix object.
		\param tx :	a float, translation to the X axis.
		\param ty :	a float, translation to the y axis.
		\param tz : a float, translation to the Z axis.
		\return nothing.
	*/
	void	Translate(const float tx, const float ty, const float tz)
	{
		matrix ret;
		ret._41 = tx;
		ret._42 = ty;
		ret._43 = tz;
		ret.Load();
		Apply();
		Store();
	}
	//! Translate function.
	/*!
		Applies a translation to the matrix object from a vector.
		\param t : a reference on a vect3D.
		\return nothing.
	*/
	void	Translate(const vect3d &t);

	//! Scale function
	/*!
	change scale of the matrix object by multipliers in each axis. A 1.0f value makes no change.
	\warning DON'T USE THIS FUNCTION. Bounding sphere around object are not scaled, so IrisFrustrum clipping, collide, etc.. can be wrong.
	\param sx :	a float, scale multiplier of the X axis.
	\param sy :	a float, scale multiplier of the y axis.
	\param sz : a float, scale multiplier of the Z axis.
	\return nothing.
	*/
	void	Scale(const float sx, const float sy, const float sz)
	{
		matrix ret;
		ret._11 = sx;
		ret._22 = sy;
		ret._33 = sz;
		ret.Load();
		Apply();
		Store();
	}
	//! Load matrix into sh4 register function
	/*!
	this function load current matrix to the sh4 register.
	\todo : try to find why gcc doesn't wan't to align our matrix in certain case ...
	\return nothing.
	*/
	void	Load()
	{
#if 1	// old version
		static matrix_t bourinnage __attribute__((aligned(32)));
		if (((uint32)_m) & 0xF)
		{
			bourinnage = _m;
			mat_load(&bourinnage);
		}
		else
			mat_load(&_m);
#else 
		mat_load(&_m);
#endif
	}

	//! Multiply matrix in sh4 register by current matrix
	/*!
	this function multiply sh4 register by current matrix.
	\todo : try to find why gcc doesn't wan't to align our matrix in certain case ...
	\return nothing.
	*/
	void	Apply()
	{
#if 1 // old version 
		static matrix_t bourinnage __attribute__((aligned(32)));
		if (((uint32)_m) & 0xF)
		{
			bourinnage = _m;
			mat_apply(&bourinnage);
		}
		else
			mat_apply(&_m);
#else
		mat_apply(&_m);
#endif
	}

	//! Get matrix in sh4 register and store it in current matrix
	/*!
	this function store the sh4 matrix to our current matrix
	\todo : try to find why gcc doesn't wan't to align our matrix in certain case ...
	\return nothing.
	*/
	void	Store()
	{
#if 1 // old version 
		static matrix_t bourinnage __attribute__((aligned(32)));
		if (((uint32)_m) & 0xF)
		{
			mat_store(&bourinnage);
			_m = bourinnage;
		}
		else
			mat_store(&_m);
#else
		mat_store(&_m);
#endif
	}
};

// Include definition of 'vect3d' class since we're 
// using it for some operator.
#include "vect3d.hpp"

inline void matrix::Translate(const vect3d &t)
{
	matrix ret;
	ret._41 = t.x;
	ret._42 = t.y;
	ret._43 = t.z;
	ret.Load();
	Apply();
	Store();	
}

inline matrix matrix::CreateLookAt(const vect3d &eye, const vect3d &at, const vect3d &up)
{
	// Get the z basis vector, which points straight ahead; the
	// difference from the eye point to the look-at point. This is the 
	// direction of the gaze (+z).
	vect3d z_axis = at - eye;
	z_axis.Normalize();
	// Compute the orthogonal axes from the cross product of the gaze 
	// and the pUp vector.
	vect3d x_axis = up.Cross(z_axis);
	x_axis.Normalize();
	vect3d y_axis = z_axis.Cross(x_axis);
	y_axis.Normalize();
	// Start building the matrix. The first three rows contain the 
	// basis vectors used to rotate the view to point at the look-at 
	// point. The fourth row contains the translation values. 
	// Rotations are still about the eyepoint.
	matrix result;
	result._11 = x_axis._x;
	result._21 = x_axis._y;
	result._31 = x_axis._z;
	result._41 = - x_axis.Dot(eye);
	result._12 = y_axis._x;
	result._22 = y_axis._y;
	result._32 = y_axis._z;
	result._42 = - y_axis.Dot(eye);
	result._13 = z_axis._x;
	result._23 = z_axis._y;
	result._33 = z_axis._z;
	result._43 = - z_axis.Dot(eye);
	result._14 = 0.0f;
	result._24 = 0.0f;
	result._34 = 0.0f;
	result._44 = 1.0f;
	return result;
}


#endif //__MATRIX_HPP__
