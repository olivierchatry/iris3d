/*********************************************************
**  File name : polar.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __POLAR_HPP__
#define __POLAR_HPP__

class polar
{
public:
  void Init()
  {
    for (int i = 0; i < 360; i++)
      for (int j = 0; j < 360; j++)
      {
        float alpha = (2.0f * (float)i * F_PI) / 360.0f;
        float beta = (2.0f * (float)j * F_PI) / 360.0f;

        _lookup[i][j]._x = (float)(fcos(beta) * fsin(alpha));
        _lookup[i][j]._y = (float)(fsin(beta) * fsin(alpha));
        _lookup[i][j]._z = (float)(fcos(alpha));
      }
  }
  void Transform(int u, int v, vect3d &dst)
  {
    dst._x = _lookup[u][v]._x;
    dst._y = _lookup[u][v]._y;
    dst._z = _lookup[u][v]._z;
  }

protected:
  vect3d _lookup[360][360];
};

#if TAMERE

polar::get(51, 86)

    class polar
{
public:
  static vect3d &get(int u, int v)
  {
    static polar p;
    return /*...*/;
  }

private:
  polar()
  {
    for (int i = 0; i < 360; i++)
      for (int j = 0; j < 360; j++)
      {
        float alpha = (2.0f * (float)i * F_PI) / 360.0f;
        float beta = (2.0f * (float)j * F_PI) / 360.0f;

        _lookup[i][j]._x = (float)(fcos(beta) * fsin(alpha));
        _lookup[i][j]._y = (float)(fsin(beta) * fsin(alpha));
        _lookup[i][j]._z = (float)(fcos(alpha));
      }
  }

  vect3d _lookup[360][360];
};

#endif

#endif
