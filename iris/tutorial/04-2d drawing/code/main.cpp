/*********************************************************
** File name : main.cpp
** Iris Engine V0.9 "alllaiii" [2d]
** Date of creation: 22/07/2003
** Author : Clement BOURGEON - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

void UpdateVertex(IrisManualObject &image, int index, vect3d &motion)
{
  vect3d &pos = image.GetVertex(index);

  // move the vertex with the motion vector
  pos += motion;
  // color the vertex
  float red = pos._x / 150.0f;
  red = (red < 0.0f) ? 0.0f : (red > 1.0f) ? 1.0f
                                           : red;
  float green = (pos._y - 360.0f) / 120.0f;
  green = (green < 0.0f) ? 0.0f : (green > 1.0f) ? 1.0f
                                                 : green;
  image.SetColor(index, IrisColor(red, green, 1.0f, 1.0f));

  // modify the motion vector if vertex is out of the screen
  if (pos._x <= 0.0f || 340.0f <= pos._x)
    motion._x *= -1.0f;
  if (pos._y <= 240.0f || 480.0f <= pos._y)
    motion._y *= -1.0f;
}

int main(int ac, char **av)
{
  // INIT IRIS 3D //////////////////////////////////
  // get IrisContext
  IrisContext &context = IrisContext::Get();
  context.InitContext(1024 * 1024);
  // set IrisGeometryPipeline
  context.GetGeometryPipeline().SetProjection(90.0f, 1.0f, 1000.0f);

  context.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
  context.GetGeometryPipeline().SetView(vect3d(0.0f, 250.0f, 0.0f),
                                        vect3d(0.0f, 0.0f, 0.0f),
                                        vect3d(0.0f, 0.0f, 1.0f));
  context.GetGeometryPipeline().Update();
  // set IrisLightingPipeline
  context.GetLightingPipeline().SetAmbientColor(IrisColor(0.0f, 0.0f, 0.0f, 0.0f));
  context.GetLightingPipeline().Update();

  // INIT CONTROLLER ///////////////////////////////
  // get InputManager
  IrisInputManager &im = context.GetInputManager();
  // look for Controllers
  if (im.ControllerCount() == 0)
  {
    printf("[ERROR] No controller found\n");
    return (-1);
  }
  // get first controller
  IrisController &controller1 = im.GetController(0);

  // 3D OBJECT FOR VISUAL REFERENCE
  IrisObject mesh;
  mesh.LoadFromFile("/rd/", "mesh.imd");

  // INIT IRIS MANUAL OBJECT ///////////////////////
  // create manual object
  IrisManualObject image;
  // alloc memory for the 4 vertices
  image.Allocate(4);
  // set vertices start positions
  image.GetVertex(0) = vect3d(25.0f, 385.0f, 1.0f);
  image.GetVertex(1) = vect3d(75.0f, 385.0f, 1.0f);
  image.GetVertex(2) = vect3d(25.0f, 435.0f, 1.0f);
  image.GetVertex(3) = vect3d(75.0f, 435.0f, 1.0f);
  // set vertices color
  image.SetColor(IrisColor(1.0f, 1.0f, 1.0f, 1.0f));
  // alloc material (the picture we want draw)
  IrisMaterial *material = context.GetMaterialManager().LoadMaterial("/rd/gruiiik.png");
  // set image material
  image.SetMaterial(material);
  // set UV texture mapping
  image.SetUV(0, 0.0f, 0.0f);
  image.SetUV(1, 1.0f, 0.0f);
  image.SetUV(2, 0.0f, 1.0f);
  image.SetUV(3, 1.0f, 1.0f);
  // precalculate 2D
  image.Precalculate2D();

  vect3d s1(1.0f, -3.0f, 0.0f);
  vect3d s2(3.0f, 1.0f, 0.0f);
  vect3d s3(-3.0f, -1.0f, 0.0f);
  vect3d s4(-1.0f, 3.0f, 0.0f);

  float a = 0.0f;
  while (!controller1.Start())
  {
    a += 0.01f;
    context.GetGeometryPipeline().SetView(vect3d(fcos(-a) * fabs(fcos(-a) * 500.0f), 150.0f, fsin(-a) * fabs(fsin(-a) * 500.0f)),
                                          vect3d(0.0f, 0.0f, 0.0f),
                                          vect3d(0.0f, 1.0f, 0.0f));
    context.GetGeometryPipeline().Update();
    context.BeginScene();
    mesh.Render();
    context.SetDepthCmpFct(DF_ALWAYS);
    image.Render2D();
    context.SetDepthCmpFct(DF_LESSER);
    context.EndScene();

    // Update
    UpdateVertex(image, 0, s1);
    UpdateVertex(image, 1, s2);
    UpdateVertex(image, 2, s3);
    UpdateVertex(image, 3, s4);

    image.Precalculate2D();
  }
  return (0);
}
