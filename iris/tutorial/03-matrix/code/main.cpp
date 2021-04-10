/*********************************************************
** File name : main.cpp
** Iris Engine V0.9 "alllaiii" [matrix]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

int main(int ac, char **av)
{
  // the object to display
  IrisObject sample;
  // get the context
  IrisContext &c = IrisContext::Get();
  c.InitContext(1024 * 1024);
  // load Iris Object (must be done after Init)
  sample.LoadFromFile("/rd/", "mesh.imd");
  // look for paddle
  if (c.GetInputManager().ControllerCount() == 0)
  {
    printf("[ERROR] No controller found\n");
    return (-1);
  }
  // setup project, viewport and camera(view) matrix
  c.GetGeometryPipeline().SetProjection(90.0f, 1.0f, 1000.0f);
  c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
  c.GetGeometryPipeline().SetView(vect3d(0.0f, 100.0f, 0.0f),
                                  vect3d(0.0f, 0.0f, 1.0f),
                                  vect3d(0.0f, 1.0f, 0.0f));
  c.GetGeometryPipeline().Update();
  // set ambient color
  c.GetLightingPipeline().SetAmbientColor(IrisColor(1.0f, 1.0f, 1.0f));
  c.GetLightingPipeline().Update();
  // get the first paddle in mapple bus
  IrisController &p = c.GetInputManager().GetController(0);
  vect3d angle(0.0f);
  vect3d pos(0.0f);
  while (!p.Start())
  {
    // do some movement
    sample.GetMatrix().Identity();
    sample.GetMatrix().RotateY(angle._y);
    sample.GetMatrix().RotateX(angle._x);
    sample.GetMatrix().Translate(pos._x, pos._y, pos._z);

    angle._y += p.AnalogHorizontal() / 2.0f;
    angle._x += p.AnalogVertical() / 2.0f;
    pos._x += p.a() ? 0.1f : p.b() ? -0.1f
                                   : 0.0f;
    pos._y += p.x() ? 0.1f : p.y() ? -0.1f
                                   : 0.0f;
    c.BeginScene();
    // render object
    sample.Render();
    c.EndScene();
  }
  return (0);
}
