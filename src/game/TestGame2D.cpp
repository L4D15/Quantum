#include "TestGame2D.h"
#include "scenes/TestScene.h"

TestGame2D::TestGame2D() :
    Game2D("Quantum")
{

}

TestGame2D::~TestGame2D()
{

}

void TestGame2D::start()
{
    createWindow(1366, 780, false);
    this->sceneManager.addScene(new TestScene());
    this->sceneManager.changeToScene("Test Scene");
    Game2D::start();
}
