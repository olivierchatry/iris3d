/*********************************************************
** File name : main.cpp
** Iris Sample V0.9 'alllaiii' 
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
    IrisContext &c = IrisContext::Get();
    c.InitContext(1024 * 1024);
    // load Iris Object (must be done after Init)
    sample.LoadFromFile("/rd/", "anim.imd");
    // look for paddle
    if (c.GetInputManager().ControllerCount() == 0)
    {
        printf("no controller found\n");
        return (-1);
    }
    // setup project, viewport and camera(view) matrix
    c.GetGeometryPipeline().SetProjection(90.0f, 1, 10000.0f);
    c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
    c.GetGeometryPipeline().SetView(vect3d(0.0, 500.0f, 0.0),
                                    vect3d(0, 0.0, 1.0f),
                                    vect3d(0.0, 1.0, 0.0));
    c.GetGeometryPipeline().Update();
    // set ambient color
    c.GetLightingPipeline().SetAmbientColor(IrisColor(0.7f, 0.7f, 0.7f));
    c.GetLightingPipeline().Update();
    // get the first paddle in mapple bus
    IrisController &p = c.GetInputManager().GetController(0);
    // define begin and end anim frame
    sample.SetLoopBegin(0);
    sample.SetLoopEnd(11);
    float	z = 0.0f;
    int		count = 0;
    while (!p.Start())
    {
        c.BeginScene();
        // render object
        sample.GetMatrix().Identity();
        sample.GetMatrix().RotateZ(z);
        sample.Render();
        sample.NextAnim();
        c.EndScene();
        z += 0.01;
        count ++;
    }
    return (0);
}
