/*********************************************************
**  File name : IrisFrustrum.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**					 Maes francis
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"


/*void	dbg_printf(int x, int y, char *str, ...)
{

	static char			buffer[4096];
	static kl_font	dbg_font(0, 0, "",8, "/rd/font/lucida.txf", 1.0, 1.0, 1.0);
	printf(buffer);
  va_list arglist;      
	va_start (arglist, str);            
	vsprintf(buffer, str, arglist);
	va_end (arglist);   
	dbg_font.SetText(buffer);
	dbg_font.SetSize(10);
	dbg_font.SetPos(x, y);
	dbg_font.Draw();
}
*/
void IrisFrustrum::SetView(const vect3d &eye, const vect3d &at, const vect3d &up)
{
	_eye = eye;
	_at = at;
	_up = up;
}

void IrisFrustrum::SetProjection(float znear, float zfar, float fov)
{
	_znear = znear;
	_zfar = zfar;
	_fov = fov;
	_c = fcos(_fov / 2);
	_s = fsin(_fov / 2);

}

void IrisFrustrum::Compute()
{
	_up = vect3d(0.0f, 0.0f, 1.0f);
	vect3d dir = (_at - _eye);
	vect3d l = -dir.Cross(_up);
	dir.Normalize();
	l.Normalize();
	_up.Normalize();
	_p[left].Set(_eye, l * _c + dir * _s);
	_p[right].Set(_eye, l * -_c + dir * _s);
	_p[top].Set(_eye,	_up * -_c + dir * _s);
	_p[bottom].Set(_eye, _up * _c + dir * _s);
	_p[back].Set(_eye + dir * _zfar, -dir);
	_p[front].Set(_eye + dir * _znear, dir);
}

IrisFrustrum::e_res IrisFrustrum::TestPoint(const vect3d& p)
{
	static const float	epsilon = 0.001f;
	e_res res = inside;

	for (unsigned i = 0; i < size; ++i)
	{
		float d = _p[i].Distance(p);
		if (d < -epsilon)
			return outside;
		else if (d < epsilon)
			res = intersect;
	}
	return res;
}

IrisFrustrum::e_res IrisFrustrum::TestSphere(const vect3d& center, float radius)
{
	unsigned i = size;
	while (i--)
	{
		float d = _p[i].Distance(center);
		if (d < -radius)
			return outside;	
		if ((d < (radius))) // just clip far plane / near plane.
			return intersect;
	}
	return (inside);
}
