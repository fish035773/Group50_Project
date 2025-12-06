#include "GameWindow.h"

int main()
{
    GameWindow game("Group 50 Final Project");
   
    if (!game.Init())
        return -1;

    game.Run();

    return 0;
}
