/*********************************************************
** File name : main.cpp
** Iris Engine V0.9 "alllaiii" [init]
** Date of creation: 30/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"
#define PUSHED(button) button?"pushed":"not pushed"

// display all info of paddle p 
void	display_paddle_info(const IrisController &p)
{
    printf("----------------------------- \n");
    printf("Paddle %x\n", p.Id());
    printf("a = %s, b = %s, x = %s, y = %s \n", PUSHED(p.a()), PUSHED(p.b()), PUSHED(p.x()), PUSHED(p.y()));
    printf("left = %s, right = %s, up = %s, down = %s \n", PUSHED(p.Left()), PUSHED(p.Right()), PUSHED(p.Up()), PUSHED(p.Down()));
    printf("start = %s \n", PUSHED(p.Start()));
    printf("ltrig = %f, rtrig = %f \n", (double) p.LeftTrigger(), (double) p.RightTrigger());
    printf("joyx = %f, joyy = %f \n", (double) p.AnalogHorizontal(), (double) p.AnalogVertical());
    printf("---------------------------------- \n");
}

int	main(int ac, char **av)
{
    // get the context
    Context &c = Context::Get();
    c.InitContext(1024 * 1024);
    InputManager	&im = c.GetInputManager();
    // look for paddle
    if (im.ControllerCount() == 0)
    {
        printf("no paddle found\n");
        return (-1);
    }
    // display all information of paddle in mapple bus
    bool done = false;
    while (!done)
    {
        // request for update input, normally this automaticaly done in 
        // Contex::BeginScene();
        im.Update();
        done = true;
        // for each paddle display information.
        for (unsigned i = 0; i < im.ControllerCount(); ++i)
        {
            IrisController &p = im.GetController(i);
            display_paddle_info(p);
            // start from all paddle must be pushed for exiting sample
            done &= p.Start();
        }   
    }
    return (0);
}
