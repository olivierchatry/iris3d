/*********************************************************
** File name : main.cpp
** Iris Sample V0.6 [Display sample]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

int main(int ac, char **av)
{
  // the object to display
  IrisObject sample;
  IrisOmniLight light;
  // get the context
  IrisContext &c = IrisContext::Get();
  c.InitContext(1024 * 1024);
  // load Iris Object (must be done after Init)
  sample.LoadFromFile("/rd/", "ecrou.imd");
  // look for paddle
  if (c.GetInputManager().ControllerCount() == 0)
  {
    printf("no paddle found\n");
    return (-1);
  }
  // setup project, viewport and camera(view) matrix
  c.GetGeometryPipeline().SetProjection(90.0f, 1, 10000.0f);
  c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
  c.GetGeometryPipeline().SetView(vect3d(0.0, 2000.0f, 0.0),
                                  vect3d(0, 0.0, 1.0f),
                                  vect3d(0.0, 1.0, 0.0));
  c.GetGeometryPipeline().Update();
  c.SetLighting(true);
  // set ambient color
  c.GetLightingPipeline().SetAmbientColor(IrisColor(0.0f, 0.0f, 0.0f));
  c.GetLightingPipeline().AddLight(&light);
  light.SetRadius(100.0f);
  light.SetColor(IrisColor(1.0f, 1.0f, 1.0f));
  c.GetLightingPipeline().Update();
  // get the first paddle in mapple bus
  IrisController &p = c.GetInputManager().GetController(0);
  float angle = 0.001f;
  IrisImportMesh &m = sample.GetObject().GetMesh(0);
  uint32 *color = m.GetColorPtr(0);
  int count = m.GetNumVertex();
  while (count--)
  {
    *color = 0x50444444;
    color++;
  }
  while (!p.Start())
  {
    c.GetGeometryPipeline().SetView(vect3d(0.0, cos(angle) * 75.0f, 0.0),
                                    vect3d(0, 0.0, 1.0f),
                                    vect3d(0.0, 1.0, 0.0));
    c.GetGeometryPipeline().Update();
    c.BeginScene();
    c.BeginBlending();
    c.SetBlendSrc(src_alpha);
    c.SetBlendDst(one);
    // render object
    light.SetPos(vect3d(0.0f, 0.0f, 0.0f));
    c.GetLightingPipeline().Update();
    angle += 0.008f;
    float old_radius = m.GetRadius();
    sample.GetMatrix().Identity();
    for (int i = 0; i < 6; ++i)
    {
      sample.GetMatrix().RotateX(angle);
      sample.GetMatrix().RotateY(angle);
      sample.GetMatrix().RotateZ(angle);
      sample.GetMatrix().Scale(1.5f, 1.5f, 1.5f);
      m.SetRadius(m.GetRadius() * 1.5f);
      sample.Render();
    }
    m.SetRadius(old_radius);
    c.EndScene();
  }
  return (0);
}
