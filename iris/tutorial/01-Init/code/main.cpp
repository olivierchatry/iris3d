/*********************************************************
** File name : main.cpp
** Iris Engine V0.9 "alllaiii" [init]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"

int	main(int ac, char **av)
{
    // get the context
    IrisContext &c = IrisContext::Get();
    c.InitContext(1024 * 1024);
    // load Iris Object (must be done after Init)
    // look for paddle
    if (c.GetInputManager().ControllerCount() == 0)
    {
        printf("[ERROR] No controller found\n");
        return (-1);
    }
    // get the first paddle in mapple bus
    IrisController &p = c.GetInputManager().GetController(0);
    while (!p.Start())
    {
        // request for update input, normally this automaticaly done in
        // Contex::BeginScene();
        c.GetInputManager().Update();
        printf("Hello world\n");
    }
    return (0);
}
