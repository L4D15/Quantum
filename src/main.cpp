#include <iostream>
#include "Game/TestGame2D.h"

int main(int argc, char** argv)
{
    TestGame2D* game;

    game = new TestGame2D();
    game->Start();

    delete game;

    return 0;
}
