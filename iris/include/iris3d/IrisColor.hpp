/*********************************************************
**  File name : IrisColor.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __COLOR_HPP__
#define __COLOR_HPP__

//! Class IrisColor handle color operation for Iris3D.
class IrisColor
{
public:
  union
  {
    struct
    {
      float _r, _g, _b, _a;
    };
    float _argb[4];
  };

  IrisColor()
  {
    _r = _g = _b = 0.0f;
  }

  IrisColor(const float r, const float g, const float b, const float a = 0.0f)
  {
    _r = r;
    _g = g;
    _b = b;
    _a = a;
    for (int i = 0; i < 4; i++)
      if (_argb[i] > 1.0)
        _argb[i] = 1.0f;
  }
  IrisColor(uint32 argb)
  {
    Unpack(argb);
  }

  void Print()
  {
    printf("r=%.3f, g=%.3f, b=%.3f a=%.3f\n", (double)_r, (double)_g, (double)_b, (double)_a);
  }

  //! Pack current IrisColor to an ARGB (8888) value.
  uint32 Pack() const
  {
    uint32 r, g, b, a;
    r = (uint32)(_r * 255);
    g = (uint32)(_g * 255);
    b = (uint32)(_b * 255);
    a = (uint32)(_a * 255);
    return (a << 24 | r << 16 | g << 8 | b);
  }

  //! Unpack an ARGB value to an IrisColor.
  void Unpack(uint32 argb)
  {
    _a = (float)((argb >> 24) & 0xff) / 255.0f;
    _r = (float)((argb >> 16) & 0xff) / 255.0f;
    _g = (float)((argb >> 8) & 0xff) / 255.0f;
    _b = (float)((argb)&0xff) / 255.0f;
  }

  //! += Operator, simpli add value.
  IrisColor &operator+=(const IrisColor &col)
  {
    int i = 4;
    while (i--)
    {
      this->_argb[i] += col._argb[i];
      // bound check
      if (this->_argb[i] > 1.0)
        this->_argb[i] = 1.0f;
      if (this->_argb[i] < 0.0)
        this->_argb[i] = 0.0f;
    }
    return (*this);
  }

  //! + Operateor, simpli add value.
  IrisColor operator+(const IrisColor &col)
  {
    IrisColor ret;
    int i = 4;
    while (i--)
    {
      ret._argb[i] = this->_argb[i] + col._argb[i];
      // bound check
      if (ret._argb[i] > 1.0)
        ret._argb[i] = 1.0f;
      if (ret._argb[i] < 0.0)
        ret._argb[i] = 0.0f;
    }
    return (ret);
  }
};
#endif
