/*********************************************************
** File name : main.cpp
** Iris Sample V0.9 'alllaiii' 
** [Animation sampling with interpolation sample]
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
    sample.LoadFromFile("/rd/", "anim_test_bd.IMD");
    // look for paddle
    if (c.GetInputManager().ControllerCount() == 0)
    {
        printf("[ERROR] No controller found\n");
        return (-1);
    }
    // setup project, viewport and camera(view) matrix
    c.GetGeometryPipeline().SetProjection(90.0f, 1.0f, 10000.0f);
    c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
    c.GetGeometryPipeline().SetView(vect3d(0.0f, 50.0f, 0.0f),
                                    vect3d(0.0f, 0.0f, 1.0f),
                                    vect3d(0.0f, 1.0f, 0.0f));
    c.GetGeometryPipeline().Update();
    // set ambient color
    c.GetLightingPipeline().SetAmbientColor(IrisColor(0.7f, 0.7f, 0.7f));
    c.GetLightingPipeline().Update();
    // get the first paddle in mapple bus
    IrisController &p = c.GetInputManager().GetController(0);
    // define number of interpolated frame between satatic frame
    int fps = 10;
    sample.SetFPS(fps);
    int		count = 0;
    while (!p.Start())
    {
        c.BeginScene();
        if (p.a())
            fps ++;
        if (p.b())
            fps --;
        if (fps < 1)
            fps = 1;
        if (fps > 60)
            fps = 60;
        sample.SetFPS(fps);
        sample.Render();
        sample.NextAnim();
        c.EndScene();
        count ++;
    }
    return (0);
}
