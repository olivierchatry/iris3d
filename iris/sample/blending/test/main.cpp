/*********************************************************
** File name : main.cpp
** Iris Sample V0.6 [Display sample]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"
KOS_INIT_FLAGS(INIT_DEFAULT);

int	main(int ac, char **av)
{
    IrisContext &c = IrisContext::Get();
    IrisMaterial        *material_test;
    IrisManualObject    manual_object;
    c.InitContext(1024 * 1024);
    material_test = c.GetMaterialManager().LoadMaterial("/rd/fire.png", texture_alpha);
    if (c.GetInputManager().ControllerCount() == 0)
    {
        printf("[ERROR] No controller found\n");
        return (-1);
    }
    // pvr_set_bg_color(1.0f, 0.0f, 1.0f);
    const float size = 40.0f;
    manual_object.Allocate(4);
    manual_object.GetVertex(0) = vect3d(-size, 0.0f, size);
    manual_object.GetVertex(1) = vect3d(size, 0.0f, size);
    manual_object.GetVertex(2) = vect3d(-size, 0.0f, -size);
    manual_object.GetVertex(3) = vect3d(size, 0.0f, -size);
    manual_object.SetColor(IrisColor(1.0f, 1.0f, 1.0f, 1.0f));
    manual_object.SetUV(0,  0.0f, 0.0f);
    manual_object.SetUV(1,  1.0f, 0.0f);
    manual_object.SetUV(2,  0.0f, 1.0f);				
    manual_object.SetUV(3,  1.0f, 1.0f);
    manual_object.PrecalculateBoundingSphere(); 
    manual_object.SetMaterial(material_test);
    // setup project, viewport and camera(view) matrix
    c.GetGeometryPipeline().SetProjection(90.0f, 1, 10000.0f);
    c.GetGeometryPipeline().SetViewport(0.0f, 0.0f, 640.0f, 480.0f);
    c.GetGeometryPipeline().SetView(vect3d(0.0, 100.0f, 0.0), 
                                    vect3d(0, 0.0, 1.0f), 
                                    vect3d(0.0, 1.0, 0.0));
    c.GetGeometryPipeline().Update();
    // set ambient color
    c.GetLightingPipeline().SetAmbientColor(IrisColor(0.0f, 0.0f, 0.0f));
    c.GetLightingPipeline().Update();
    // get the first paddle in mapple bus
    IrisController &p = c.GetInputManager().GetController(0);
    float angle = 0.0f;
    float alpha = 0.0f;
    while (!p.Start())
    {
        c.BeginScene();
        c.BeginBlending();
        c.SetBlendSrc(src_alpha);
        c.SetBlendDst(one);
        alpha += 0.0005f;
        if (alpha > 1.0f)
            alpha = 0.0f;
        manual_object.SetColor(IrisColor(0.5f, 0.5f, 0.5f, alpha));
        for (int i = 0; i < 10; ++i)
        {
            manual_object.GetMatrix().Identity();           
            manual_object.GetMatrix().RotateZ(angle + i * 0.4f);
            manual_object.GetMatrix().RotateY(angle + i * 0.4f);
            manual_object.GetMatrix().Translate(0.0f, i * -size, 0.0f);
            manual_object.Render();
        }
        angle += 0.01f;
        c.EndScene();
    }
    return (0);
}
