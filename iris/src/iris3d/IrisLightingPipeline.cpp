/*********************************************************
**  File name : IrisLightingPipeline.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"

void IrisLightingPipeline::Update()
{
  LightIterator j;
  for (j = _light.begin(); j != _light.end(); ++j)
    (*j)->Update();
}

void IrisLightingPipeline::Compute(vect3d *vin, vertex2dtl *vout, vect3d *normal, int num, uint32 *color)
{
  int i = num;
  LightIterator j;
  IrisColor color_tmp;
  while (i--)
  {

    color_tmp._r = (((*color) >> 16) & 0xff) + _ambient_color._r;
    color_tmp._g = (((*color) >> 8) & 0xff) + _ambient_color._g;
    color_tmp._b = ((*color) & 0xff) + _ambient_color._b;
    for (j = _light.begin(); j != _light.end(); ++j)
      (*j)->Compute(*vin, *normal, color_tmp, vout);
    vin++;
    normal++;
    color_tmp._r = color_tmp._r > 255.0f ? 255.0f : color_tmp._r;
    color_tmp._g = color_tmp._g > 255.0f ? 255.0f : color_tmp._g;
    color_tmp._b = color_tmp._b > 255.0f ? 255.0f : color_tmp._b;
    vout->oargb = ((uint32)(color_tmp._r) & 0xff) << 16 |
                  ((uint32)(color_tmp._g) & 0xff) << 8 |
                  ((uint32)(color_tmp._b) & 0xff) | (*color & 0xff000000) << 24;
    color++;
    vout++;
  }
}

//HT

#define ARGB_A(c) (c >> 24)
#define ARGB_R(c) ((c >> 16) & 0xff)
#define ARGB_G(c) ((c >> 8) & 0xff)
#define ARGB_B(c) (c & 0xff)

void IrisLightingPipeline::ComputeInterpol(int fps, int count, vect3d *vin, vertex2dtl *vout, vect3d *normal, int num, uint32 *color, uint32 *color2)
{
  int i = num;
  LightIterator j;
  IrisColor color_tmp;
  uint32 c1, c2, c;
  while (i--)
  {
    c1 = *color++;
    c2 = *color2++;
    //interpol between color
    if (c1 == c2)
    {
      c = c1;
    }
    else
    {

      //FIXME??????to lerpcolor()
      int a, r, g, b;

      a = ARGB_A(c1) - (int)(((ARGB_A(c1) - ARGB_A(c2)) / (float)count) * fps);
      r = ARGB_R(c1) - (int)(((ARGB_R(c1) - ARGB_R(c2)) / (float)count) * fps);
      g = ARGB_G(c1) - (int)(((ARGB_G(c1) - ARGB_G(c2)) / (float)count) * fps);
      b = ARGB_B(c1) - (int)(((ARGB_B(c1) - ARGB_B(c2)) / (float)count) * fps);

      c = (((a << 8) | r) << 16) | (g << 8) | (b);
    }
    color_tmp._r = (((c) >> 16) & 0xff) + _ambient_color._r;
    color_tmp._g = (((c) >> 8) & 0xff) + _ambient_color._g;
    color_tmp._b = ((c)&0xff) + _ambient_color._b;
    for (j = _light.begin(); j != _light.end(); ++j)
      (*j)->Compute(*vin, *normal, color_tmp, vout);
    vin++;
    normal++;

    color_tmp._r = color_tmp._r > 255.0f ? 255.0f : color_tmp._r;
    color_tmp._g = color_tmp._g > 255.0f ? 255.0f : color_tmp._g;
    color_tmp._b = color_tmp._b > 255.0f ? 255.0f : color_tmp._b;
    vout->oargb = (uint32)(color_tmp._r) << 16 |
                  (uint32)(color_tmp._g) << 8 |
                  (uint32)(color_tmp._b);
    vout++;
  }
}
bool IrisLightingPipeline::IsInLight(const vect3d &center, float radius)
{
  bool ret = false;
  LightIterator j;
  for (j = _light.begin(); j != _light.end(); ++j)
    ret |= (*j)->IsInLight(center, radius);
  return (ret);
}

int IrisLightingPipeline::AddLight(IrisLight *light)
{
  int last_id;
  if (_light.size() == 0)
    last_id = 0;
  else
    last_id = _light[_light.size() - 1]->GetId() + 1;
  _light.push_back(light);
  light->SetId(last_id);
  return last_id;
}

void IrisLightingPipeline::DelLight(int id)
{
  LightIterator j;
  for (j = _light.begin(); j != _light.end(); ++j)
    if ((*j)->GetId() == id)
    {
      _light.erase(j);
      return;
    }
}

void IrisLightingPipeline::DelLight(IrisLight *light)
{
  LightIterator j;
  for (j = _light.begin(); j != _light.end(); ++j)
  {
    IrisLight *tmp = *j;
    if (tmp == light)
    {
      _light.erase(j);
      return;
    }
  }
}

IrisLight *IrisLightingPipeline::GetLight(int id)
{
  LightIterator j;
  for (j = _light.begin(); j != _light.end(); ++j)
    if ((*j)->GetId() == id)
      return (*j);
  return (NULL);
}

void IrisLightingPipeline::SetAmbientColor(const IrisColor &_color)
{
  _ambient_color = _color;
  _ambient_color_packed = _color.Pack();
  _ambient_color._r *= 255.0f;
  _ambient_color._g *= 255.0f;
  _ambient_color._b *= 255.0f;
}
