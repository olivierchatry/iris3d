/*********************************************************
**  File name : SmousseLight.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __SMOUSSELIGHT_HPP_
#define __SMOUSSELIGHT_HPP_

class IrisSmousseLight : public IrisLight
{
protected:
	float	_radius;

public:	
	IrisSmousseLight(){}
	virtual void		Update()
	{
		 _transformation.Load();
		_transformed_pos.Set(_pos._x, _pos._y, _pos._z);
		_transformed_pos = _pos;
		mat_trans_single_iris(_transformed_pos._x, _transformed_pos._y, _transformed_pos._z);
	}

	virtual void		Print()
	{
		_pos.Print();
		_color.Print();
		printf("Radius = %.3f\n", (double) _radius);
	}
	
	virtual void		Compute(const vect3d &vin, const vect3d &normal, IrisColor &color, vertex2dtl *vout)
	{
		float		len;
		
		vect3d	    vertex_tmp = vin - _transformed_pos;
		vect3d		vect_lum(vertex_tmp._x, vertex_tmp._y, vertex_tmp._z, 1.0f);
		len = vect_lum.Magnitude();
		if (len > _radius)
			return;
		float	intensity = 1.0f - (len / _radius);
		if (intensity > 0.0f)
		{
			color._r += (intensity * _color._r * 255.0f);
			color._g += (intensity * _color._g * 255.0f);
			color._b += (intensity * _color._b * 255.0f);
		}
	}

	void	SetRadius(const float radius)
	{
		_radius = radius;
	}

	float	GetRadius(void)
	{
		return _radius;
	}
	
	virtual bool	IsInLight(const vect3d &center, float radius)
	{
		vect3d	vect_tmp(_transformed_pos._x, _transformed_pos._y, _transformed_pos._z, 1.0f);
		vect_tmp -= center;
		return vect_tmp.Magnitude() <= (_radius + radius);
	}
};
#endif

