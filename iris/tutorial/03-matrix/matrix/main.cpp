/*********************************************************
** File name : main.cpp
** Iris Sample V0.6 [Object matrix sample]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"


int	main(int ac, char **av)
{
	// the object to display
	IrisObject	sample;
	// get the context
	Context &c = Context::Get();
	c.InitRender(1024 * 1024);
	// load Iris Object (must be done after Init)
	sample.LoadFromFile("/rd/", "sphere.imd");
	sample.SetLoopBegin(0);
	sample.SetLoopEnd(3);
	sample.SetCurrentAnim(0);
	sample.SetFPS(60);
	// look for paddle
	if (c.GetInputManager().PaddleCount() == 0)
		{
			printf("no paddle found\n");
			return (-1);
		}
	// setup project, viewport and camera(view) matrix
	c.GetGeometryPipeline().SetProjection(90.0f, 1, 10000.0f);
	c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
	c.GetGeometryPipeline().SetView(vect3d(0.0, 100.0f, 0.0),
																vect3d(0, 0.0, 1.0f),
																vect3d(0.0, 1.0, 0.0));
	c.GetGeometryPipeline().Generate();
	// set ambient color
  	c.GetLightingPipeline().SetAmbientColor(Color(1.0f, 1.0f, 1.0f));
  	c.GetLightingPipeline().Update();

	// get the first paddle in mapple bus
	Paddle &p = c.GetInputManager().GetPaddle(0);
	vect3d	angle(0.0);
	vect3d	pos(0.0);

	while (!p.Start())
		{
			// do some movement
			sample.GetMatrix().Identity();
			sample.GetMatrix().RotateY(angle._y);
			sample.GetMatrix().RotateX(angle._x);
			sample.GetMatrix().Translate(pos._x, pos._y, pos._z);
			// ---
			angle._y += p.AnalogHorizontal() / 2;
			angle._x += p.AnalogVertical() / 2;
			pos._x += p.a() ? 0.1 : p.b() ? -0.1 : 0.0;
			pos._y += p.x() ? 0.1 : p.y() ? -0.1 : 0.0;
			c.BeginScene();
			// render object
			sample.Render();
			c.EndScene();
			sample.NextAnim();
		}
	return (0);
}
