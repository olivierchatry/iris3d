/*********************************************************
** File name : main.cpp
** Iris Sample V0.9 'alllaiii' 
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_IRQ | INIT_MALLOCSTATS);

IrisObject              objTable[100];

int	main(int ac, char **av)
{
    // the object to display
    IrisObject	        sample;
    
    
    // get the context
    IrisContext &c = IrisContext::Get();
    c.InitContext(1024 * 1024);
    if (c.GetInputManager().ControllerCount() == 0)
    {
        printf("no controller found\n");
        return (-1);
    }
    c.GetGeometryPipeline().SetProjection(90.0f, 1, 10000.0f);
    c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
    c.GetGeometryPipeline().SetView(vect3d(500.0f, 0.0f, 0.0f),
                                    vect3d(0.0f, 0.0f, 1.0f),
                                    vect3d(0.0f, 1.0f, 0.0f));
    c.GetGeometryPipeline().Update();
    // set ambient color
    c.GetLightingPipeline().SetAmbientColor(IrisColor(0.7f, 0.7f, 0.7f));
    c.GetLightingPipeline().Update();
    // get the first paddle in mapple bus
    IrisController &p = c.GetInputManager().GetController(0);

    
    malloc_stats();
    float angle = 0.0f;
    while (!p.a())
    {
        
        c.GetGeometryPipeline().SetView(vect3d(fcos(angle) * 200.0f, 0.0f, fsin(angle) * 200.0f),
                                        vect3d(0.0f, 0.0f, 1.0f),
                                        vect3d(0.0f, 1.0f, 0.0f));
        c.GetGeometryPipeline().Update();
        for (int i = 0 ; i < 10; i++)
            objTable[i].LoadFromFile("/rd/", "perso.IMD");
        angle += 0.1f;
        c.BeginScene();
        objTable[0].Render();
        c.EndScene();
        for (int i = 0; i < 10; i++)
            objTable[i].Destroy();
    }
    // setup project, viewport and camera(view) matrix
    // define begin and end anim frame
/*    skeletal->SetLoopBegin(0);
    skeletal->SetLoopEnd(60);
    while (!p.Start())
    {
        c.BeginScene();
        // render object
        skeletal->Update(&(sample.GetObject()));
        sample.Render();
        skeletal->NextAnim();
        c.EndScene();
    }*/
    return (0);
}
