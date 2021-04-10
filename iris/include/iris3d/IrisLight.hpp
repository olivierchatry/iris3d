/*********************************************************
**  File name : IrisLight.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

//! IrisLight class.
/*!
	Common class for the different light types.
	Used for the different class of lights like cell lights or omni lights.
	\sa IrisCellLight, IrisOmniLight, IrisSmousseLight.
*/
class IrisLight
{
protected:
  char _light_type;
  vect3d _pos;
  vect3d _transformed_pos;
  matrix _transformation __attribute__((aligned(32)));
  int _id;
  IrisColor _color;

public:
  //! SetColor function.
  /*!
		Set the color of the light.
		\param color a reference on a IrisColor object.
		\return nothing.
		\sa GetColor.
	*/
  void SetColor(const IrisColor &color)
  {
    _color = color;
  }
  //! GetColor function.
  /*!
		Get the color of the light.
		\return a reference on an IrisColor object.
		\sa SetColor.
	*/
  IrisColor &GetColor()
  {
    return (_color);
  }

  //! Constructor of the IrisColor class.
  /*!
	*/
  IrisLight()
  {
  }
  //! IsInLight function test if a bounding sphere is lighted by the light
  /*!
		\param center center of the bounding sphere
		\param radius radius of the bounding spher
		\return true if lighted by current light, false if not.
	*/
  virtual bool IsInLight(const vect3d &center, float radius)
  {
    return (false);
  }

  //! Constructor of the IrisColor class.
  /*!
		Copy a IrisLight object into a new one.
		\param copy a reference on the IrisLight object to copy.
	*/
  IrisLight(const IrisLight &copy)
  {
    _pos = copy._pos;
    _color = copy._color;
    _id = copy._id;
    _light_type = copy._light_type;
    _transformation = copy._transformation;
  }

  //! SetId function.
  /*!
		Set the id of the light.
		\param id an int, the id of the light.
		\return nothing.
		\sa GetId.
	*/
  void SetId(int id)
  {
    _id = id;
  }

  //! GetId function.
  /*!
		Get the id of the light.
		\return an int, the id of the light.
		\sa SetId.
	*/
  int GetId()
  {
    return _id;
  }
  //! Print function.
  /*!
		Print infomations on the IrisLight object on the standard out.
	*/
  virtual void Print()
  {
    printf("Fake lighting\n");
  }

  virtual void Update()
  {
  }

  virtual void Compute(const vect3d &vin, const vect3d &normal, IrisColor &color, vertex2dtl *vout)
  {
  }
  //! GetMatrix function.
  /*!
		\return a reference on the matrix transformation of the IrisLight object.
		\sa SetMatrix.
	*/
  matrix &GetMatrix()
  {
    return _transformation;
  }
  //! SetMatrix function.
  /*!
		Applies transformation from a matrix to a IrisLight object.
		\param transformation a reference on a matrix.
		\return nothing.
		\sa GetMatrix.
	*/
  void SetMatrix(const matrix &transformation)
  {
    _transformation = transformation;
  }
  //! SetPos function.
  /*!
		Set the position of the light.
		\param pos a vect3d with the position of the light.
		\return  nothing.
		\sa GetPos.
	*/
  void SetPos(vect3d pos)
  {
    _pos = pos;
  }
  //! GetPos function.
  /*!
		\return a vect3d, the position of the light.
		\sa SetPos.
	*/
  vect3d &GetPos()
  {
    return (_pos);
  }
  //! GetType function.
  /*!
		\return a char, the type of the light used.
	*/
  char GetType()
  {
    return (_light_type);
  }
};

#endif //__LIGHT_HPP__
