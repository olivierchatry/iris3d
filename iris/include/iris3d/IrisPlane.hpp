/*********************************************************
**  File name : Plane.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __PLANE_HPP__
#define __PLANE_HPP__

class IrisPlane
{
public:
  IrisPlane(const vect3d &point, const vect3d &normal)
      : _a(point), _n(normal) { _n.Normalize(); }
  IrisPlane(const vect3d &a, const vect3d &b, const vect3d &c, bool inv = false);

  IrisPlane() {}

  void Set(const vect3d &point, const vect3d &normal)
  {
    _a = point;
    _n = normal;
    _n.Normalize();
  }

  /*
	** const accessors
	*/
  const vect3d &Normal() const { return _n; }
  const vect3d &Point() const { return _a; }

  /*
	** non-const accessors
	*/
  vect3d &Normal() { return _n; }
  vect3d &Point() { return _a; }

  /*
	** projection
	*/
  float Distance(const vect3d &p) { return (p - _a).Dot(_n); }
  vect3d Project(const vect3d &p) { return (vect3d)p - _n * Distance(p); }

private:
  vect3d _a;
  vect3d _n;
};

inline vect3d GetNormal(const vect3d &v1, const vect3d &v2, const vect3d &v3)
{
  vect3d ve1, ve2;
  vect3d ret;
  vect3d tmp;
  tmp = v2 - v1;
  ve1._x = tmp._x;
  ve1._y = tmp._y;
  ve1._z = tmp._z;
  tmp = v3 - v1;
  ve2._x = tmp._x;
  ve2._y = tmp._y;
  ve2._z = tmp._z;
  ret = ve1.Cross(ve2);
  ret.Normalize();
  return ret;
}
inline IrisPlane::IrisPlane(const vect3d &a, const vect3d &b, const vect3d &c, bool inv)
{
  vect3d sum;
  vect3d normal[3];
  sum = a + b + c;
  sum /= 3.0f;
  _a = sum;
  // normal
  if (inv)
  {
    normal[0] = GetNormal(a, b, c);
    normal[1] = GetNormal(b, c, a);
    normal[2] = GetNormal(c, a, b);
  }
  else
  {
    normal[0] = GetNormal(a, c, b);
    normal[1] = GetNormal(b, a, c);
    normal[2] = GetNormal(c, b, a);
  }
  _n = (normal[0] + normal[1] + normal[2]) / 3.0f;
}

#endif /* not __PLANE_HPP__ */
