/*********************************************************
**  File name : IrisCamera.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

class IrisCamera
{
protected:
	vect3d	_eye;
	vect3d	_at;
	vect3d	_up;
public:
    IrisCamera(){Set(vect3d(0.0f, 0.0f, 0.0f), vect3d(0.0f, 0.0f, 0.0f), vect3d(0.0f, 0.0f, 0.0f));}
	/* -- */
    IrisCamera(const vect3d &the_eye, const vect3d &the_at, const vect3d &the_up)
	{
		Set(the_eye, the_at, the_up);
	}
	/* -- */
    void Set(const vect3d &the_eye, const vect3d &the_at, const vect3d &the_up)
	{
		_eye = the_eye;
		_at = the_at;
		_up = the_up;
	}
	/* -- */
	vect3d		&GetUp() {return _up;}
	vect3d		&GetAt()  {return _at;}
	vect3d		&GetEye() {return _eye;}
	void			SetUp(const vect3d &the_up){_up = the_up;}
	void			SetAt(const vect3d &the_at){_at = the_at;}
	void			SetEye(const vect3d &the_eye){_eye = the_eye;}
	
};

class	IrisCameraSpline : public IrisCamera
{
public:
	bool	LoadFromFile(char *file_name)
	{
		_current = 0;
		_up = vect3d(0.0f, 1.0f, 1.0f);
		return _spline.LoadFromFile(file_name);
	}
	bool	End()
	{
		return _spline[0].len == _current;
	}
	
	void	Reset()
	{
		_current = 0;
	}
	
	void	GoTo(unsigned where)
	{
		_current = where;
	}

	void	Update()
	{
		//	no spline present, just does nothing
		if (_spline.get().size() == 0)
			return;
		//	check for end of the spline. If so, look if we
		//	have to loop, else simply return;
		if (_spline[0].v.size() <= _current)
		{
			if (_spline[0].closed)
				_current = 0;
			else
				return;
		}
		// check if eye and at use the same spline, 
		// if so, at are one vector above eye, like a track
		// else one spline is used for eye and one for at
		// assuming that the spline had the same length
		if (_spline.get().size() == 1)
		{
			_at = _spline[0].v[_current];
			if (_current == _spline[0].len - 1)
				_eye = _spline[0].v[0];
			else
				_eye = _spline[0].v[_current + 1];
		}
		else
		{
			_eye = _spline[0].v[_current];
			if (_current < _spline[1].v.size())
				_at = _spline[1].v[_current];
			else
				_at = _spline[1].v[_spline[1].v.size() - 1];
		}
		_current ++;
	}
protected:
	Spline		_spline;
	unsigned	_current;
};
#endif //__CAMERA_HPP__

