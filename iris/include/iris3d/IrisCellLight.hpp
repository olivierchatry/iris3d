/*********************************************************
**  File name : OmniLight.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __CELLLIGHT_HPP__
#define __CELLLIGHT_HPP__

class IrisCellLight : public IrisLight
{
protected:
	float	_radius;
public:	
	IrisCellLight(){}
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
		vect_lum.Normalize(len);
		float	result = vect_lum.Dot(normal);
		if (result < 0.0f)
		{
			result = -result * intensity;
			vout->u = result;
			vout->v = 0.0f;
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

