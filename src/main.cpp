#include <iostream>
#include "game/TestGame2D.h"

int main(int argc, char** argv)
{
    TestGame2D* game;

    game = new TestGame2D();
    game->start();

    delete game;

    return 0;
}
