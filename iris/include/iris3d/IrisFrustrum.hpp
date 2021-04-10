/*********************************************************
**  File name : IrisFrustrum.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**					 Maes francis
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __FRUSTRUM_HPP__
#define __FRUSTRUM_HPP__

class IrisFrustrum
{
public:
  /**
     * Define the viewport for rendering 
     */
  void SetView(const vect3d &eye, const vect3d &at, const vect3d &up);
  void SetProjection(float znear, float zfar, float fov);
  void Compute();
  void Get(vect3d &eye, vect3d &at, vect3d &up)
  {
    eye = _eye;
    at = _at;
    up = _up;
  }
  enum e_res
  {
    inside,
    outside,
    intersect
  };

  e_res TestPoint(const vect3d &p);
  e_res TestSphere(const vect3d &center, float radius);

private:
  enum
  {
    size = 6
  };
  IrisPlane _p[size];

  enum
  {
    left,
    right,
    top,
    bottom,
    back,
    front
  };

  vect3d _eye, _at, _up;
  float _fov, _znear, _zfar;
  float _c, _s;
};

#endif /* not __FRUSTRUM_HPP__ */
