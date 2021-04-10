/*********************************************************
**  File name : vert3d.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry / Omar Cornut - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

// Note: there's a inconsistancies regarding the _w attributes
// which is or not affected by the different operators.
// _w is reseted to either 1.0f or 0.f0 by most operators!
// So it's pretty useless on user side, and waste CPU.
// Should we remove it ?

// Note; there's few inconsistancies regarding usage of the 'const'
// attribute.

#ifndef __VECT3D_HPP__
#define __VECT3D_HPP__

//! vect3d Class.
/*!
	Class for the vertex, includes methodes to manipulate the vertexes.
	A vect3d object is a vertex in Iris3D.
*/
class vect3d
{
public: // Attributes ************************************
  union
  {
    struct
    {
      float _x, _y, _z, _w; // FIXME: those should be removed if possible
    };
    struct
    {
      float x, y, z, w;
    };
    float _pos[4];
  };

public: // Constructors/Assignators *********************
  //! Constructor of the vect3d class.
  /*!
		Create a vect3d object whith x, y, z values of the vertex to 0.0f.
	*/
  vect3d()
  {
    _x = 0.0f;
    _y = 0.0f;
    _z = 0.0f;
    _w = 1.0f;
  }

  //! Constructor of the vect3d class.
  /*!
		Create a vect3d object.
		\param x : a float, the x value of the vertex.
		\param y : a float, the y value of the vertex.
		\param z : a float, the z value of the vertex.
	*/
  vect3d(const float x, const float y, const float z)
  {
    Set(x, y, z);
  }

  //! Constructor of the vect3d class.
  /*!
		Create a vect3d object.
		\param x : a float, the x value of the vertex.
		\param y : a float, the y value of the vertex.
		\param z : a float, the z value of the vertex.
		\param w : a float, , by default is set to 1.0f.
	*/
  vect3d(const float x, const float y, const float z, const float w)
  {
    Set(x, y, z, w);
  }

  //! Constructor of the vect3d class.
  /*!
	Create a vect3d object.
	\param value : a float, the x, y, z value of the vertex.
	*/
  vect3d(const float value)
  {
    _x = _y = _z = value;
    _w = value; // FIXME: Is that what we want ?
  }

  //! Constructor of the vect3d class.
  /*!
	Create a vect3d object.
	\param value : a pointer to a table of 3 float
	*/
  vect3d(const float *value)
  {
    _x = *value++;
    _y = *value++;
    _z = *value++;
    _w = 1.0f;
  }

  vect3d(const vect3d &v)
  {
    _x = v._x;
    _y = v._y;
    _z = v._z;
    _w = v._w;
  }

  //! Set function.
  /*!
		Assign the x, y and z value to a vect3d object.
		\param x : a float, the x value of the vertex.
		\param y : a float, the y value of the vertex.
		\param z : a float, the z value of the vertex.
		\return nothing.
	*/
  void Set(const float x, const float y, const float z)
  {
    _x = x;
    _y = y;
    _z = z;
    _w = 1.0f;
  }

  //! Set function.
  /*!
		Assign the x, y and z value to a vect3d object.
		\param x : a float, the x value of the vertex.
		\param y : a float, the y value of the vertex.
		\param z : a float, the z value of the vertex.
		\param w : a float, ,usaly set to 1.0f.
		\return nothing.
	*/
  void Set(const float x, const float y, const float z, const float w)
  {
    _x = x;
    _y = y;
    _z = z;
    _w = w;
  }

public: // Methods ***************************************
#include "matrix.hpp"

  // Matrix Arithmetic Operators
  vect3d operator*(const matrix &m)
  {
    return vect3d(_x * m._11 + _y * m._21 + _z * m._31 + _w * m._41,
                  _x * m._12 + _y * m._22 + _z * m._32 + _w * m._42,
                  _x * m._13 + _y * m._23 + _z * m._33 + _w * m._43,
                  _x * m._14 + _y * m._24 + _z * m._34 + _w * m._44);
  }

  // Arithmetic Operator : Add
  vect3d operator+(const vect3d &v) const
  {
    return vect3d(_x + v._x, _y + v._y, _z + v._z);
  }

  vect3d operator+(const float f) const
  {
    return vect3d(_x + f, _y + f, _z + f);
  }

  vect3d &operator+=(const vect3d &v)
  {
    _x += v._x;
    _y += v._y;
    _z += v._z;
    return (*this);
  }

  vect3d &operator+=(const float v)
  {
    _x += v;
    _y += v;
    _z += v;
    return (*this);
  }

  // Arithmetic Operator : Subtract
  vect3d operator-(const vect3d &v) const
  {
    return vect3d(_x - v._x, _y - v._y, _z - v._z);
  }

  vect3d operator-(const float f) const
  {
    return vect3d(_x - f, _y - f, _z - f);
  }

  vect3d &operator-=(const vect3d &v)
  {
    _x -= v._x;
    _y -= v._y;
    _z -= v._z;
    return (*this);
  }

  vect3d &operator-=(const float f)
  {
    _x -= f;
    _y -= f;
    _z -= f;
    return (*this);
  }

  // Arithmetic Operator : Multiply
  vect3d operator*(const vect3d &v)
  {
    return vect3d(_x * v._x, _y * v._y, _z * v._z);
  }

  vect3d operator*(const float f)
  {
    return vect3d(_x * f, _y * f, _z * f);
  }

  vect3d &operator*=(const float v)
  {
    _x *= v;
    _y *= v;
    _z *= v;
    return (*this);
  }

  // Arithmetic Operator : Divide
  vect3d operator/(const float f)
  {
    return vect3d(_x / f, _y / f, _z / f);
  }

  vect3d &operator/=(const float f)
  {
    _x /= f;
    _y /= f;
    _z /= f;
    return (*this);
  }

  // Arithmetic Operator: Unary -
  vect3d operator-() const
  {
    return vect3d(-_x, -_y, -_z);
  }

  // Comparaison Operators
  bool operator==(const vect3d &v) const
  {
    return (_x == v._x) && (_y == v._y) && (_z == v._z);
  }

  bool operator!=(const vect3d &v) const
  {
    return (_x != v._x) || (_y != v._y) || (_z != v._z);
  }

  // Printing
  //!	Print function.
  /*!
		Print on standard out informations on the vect3d object. 
		\return nothing.
	*/
  void Print(void)
  {
    printf("x = %f, y = %f, z = %f\n",
           (double)_x, (double)_y, (double)_z);
  }

  // Dot Product
  float Dot(const vect3d &v) const
  {
    return (_x * v._x) + (_y * v._y) + (_z * v._z);
  }

  // Cross Product
  vect3d Cross(const vect3d &v) const
  {
    return vect3d(_y * v._z - _z * v._y,
                  _z * v._x - _x * v._z,
                  _x * v._y - _y * v._x);
  }

  // Magnitude (Length)
  float Magnitude(void) const
  {
    // FIXME: should we use fsqrt() there ?
    return mat_sqrtipr(_x, _y, _z);
  }

  // Normalize
  vect3d &Normalize(void)
  {
    float mag = Magnitude();

    _x /= mag;
    _y /= mag;
    _z /= mag;
    return (*this);
  }

  vect3d &Normalize(float mag)
  {
    _x /= mag;
    _y /= mag;
    _z /= mag;
    return (*this);
  }

  // Homogenize
  vect3d &Homogenize(void)
  {
    assert(_w != 0.0f);
    _x /= _w;
    _y /= _w;
    _z /= _w;
    _w = 1.0f;
    return (*this);
  }
};

#endif //__VECT3D_HPP__
