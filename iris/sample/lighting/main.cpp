/*********************************************************
** File name : main.cpp
** Iris Sample V0.6 [Lighting sample]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/
#include "Iris.hpp"
KOS_INIT_FLAGS(INIT_DEFAULT);

int main(int ac, char **av)
{
  // the object to display
  IrisObject sample;
  IrisOmniLight light1;
  IrisSmousseLight light2;
  // get the context
  IrisContext &c = IrisContext::Get();
  c.InitContext(1024 * 1024);
  c.SetLighting(true);
  // load Iris Object (must be done after Init)
  sample.LoadFromFile("/rd/", "lighting.imd");
  // look for paddle
  if (c.GetInputManager().ControllerCount() == 0)
  {
    printf("no controller found\n");
    return (-1);
  }
  // setup project, viewport and camera(view) matrix
  c.GetGeometryPipeline().SetProjection(90.0f, 0.001f, 10000.0f);
  c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
  c.GetGeometryPipeline().SetView(vect3d(0.1f, 0.1f, 500.0f),
                                  vect3d(0.0f, 0.0f, 1.0f),
                                  vect3d(0.0f, 0.0f, 1.0f));
  c.GetGeometryPipeline().Update();
  // set ambient color
  // ligh setting
  light1.SetRadius(250.0f);
  light1.SetPos(vect3d(0.0f, 0.0f, 50.0f));
  light1.SetColor(IrisColor(0.8f, 0.8f, 0.8f));

  light2.SetRadius(250.0f);
  light2.SetPos(vect3d(0.0f, 0.0f, 50.0f));
  light2.SetColor(IrisColor(0.8f, 0.8f, 0.8f));
  // --
  c.GetLightingPipeline().SetAmbientColor(IrisColor(0.0f, 0.0f, 0.0f));
  c.GetLightingPipeline().AddLight(&light1);
  c.GetLightingPipeline().AddLight(&light2);
  c.GetLightingPipeline().Update();
  // get the first paddle in mapple bus
  IrisController &p = c.GetInputManager().GetController(0);
  vect3d angle(0.0f);
  while (!p.Start())
  {
    light1.SetPos(vect3d(fcos(angle._z) * 150.0f, 50.0f, fsin(angle._z) * 150.0f));
    light1.SetColor(IrisColor((1.0f + fcos(angle._z)) / 2.0, 0.5, (1.0 + fsin(angle._z)) / 2.0));
    light2.SetPos(vect3d(fcos(-angle._z) * 150.0f, 50.0f, fsin(angle._z) * 150.0f));
    light2.SetColor(IrisColor((1.0f + fcos(angle._z)) / 2.0f, 0.5f, 0.5f + (1.0f + fcos(angle._z)) / 4.0f));
    c.GetLightingPipeline().Update(); // don't forget to update the pipeline
    c.GetGeometryPipeline().SetView(vect3d(fcos(-angle._z) * fabs(fcos(-angle._z) * 500.0f), 150.0f,
                                           fsin(-angle._z) * fabs(fsin(-angle._z) * 500.0f)),
                                    vect3d(0.0f, 0.0f, 0.0f),
                                    vect3d(0.0f, 1.0f, 0.0f));
    c.GetGeometryPipeline().Update();

    c.BeginScene();
    sample.Render();
    c.EndScene();
    angle._z += 0.02f;
  }
  return (0);
}
