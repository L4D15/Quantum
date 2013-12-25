#include "TestScene.h"
#include "quantum/Game.h"
#include "quantum/Game2D.h"
#include "quantum/Math.h"
#include "quantum/components/ComponentsList.h"

TestScene::TestScene() :
    Scene("Test Scene")
{
    // Allow the camera to move
    mainCamera->addComponent(new components2D::Physics(*mainCamera));

    // Systems
    this->physicsSystem = this->addSystem<systems2D::Physics>();
    this->renderingSystem = this->addSystem<systems2D::AssetRendering>();

    // Background
    GameObject* background;
    background = createGameObject("Background");

    GameObject* starsStatic;
    GameObject* stars01;
    GameObject* stars02;
    GameObject* stars03;

    starsStatic = createGameObject("Stars-Static");
    stars01 = createGameObject("Stars-01");
    stars02 = createGameObject("Stars-02");
    stars03 = createGameObject("Stars-03");

    starsStatic->makeChildOfObject(background);
    stars01->makeChildOfObject(background);
    stars02->makeChildOfObject(background);
    stars03->makeChildOfObject(background);

    starsStatic->addComponent(new components2D::AssetRenderer(*starsStatic, mainCamera, Game::resourceManager.getSprite("stars-static.png"), 0, Game2D::parallaxScrolling.getBackgroundDistance()));
    stars01->addComponent(new components2D::AssetRenderer(*stars01, mainCamera, Game::resourceManager.getSprite("stars-01.png"), 1, Game2D::parallaxScrolling.getBackgroundDistance() * 0.99f));
    stars02->addComponent(new components2D::AssetRenderer(*stars02, mainCamera, Game::resourceManager.getSprite("stars-02.png"), 2, Game2D::parallaxScrolling.getBackgroundDistance() * 0.95f));
    stars03->addComponent(new components2D::AssetRenderer(*stars03, mainCamera, Game::resourceManager.getSprite("stars-03.png"), 3, Game2D::parallaxScrolling.getBackgroundDistance() * 0.91f));
    //---

    // Player ship
    GameObject* playerShip;
    playerShip = createGameObject("Player Ship");

    playerShip->addComponent(new components2D::AssetRenderer(*playerShip, mainCamera, Game::resourceManager.getSprite("Ship.png"), 99, 0));

}

TestScene::~TestScene()
{


}

void TestScene::onActivate()
{

}

void TestScene::onDeactivate()
{

}

void TestScene::onLoop()
{
    Scene::onLoop();

    physicsSystem->process();
}

void TestScene::onRender()
{
    this->renderingSystem->process();
    Scene::onRender();
}

void TestScene::onKeyDown(SDL_Keycode key, Uint16 mod)
{
    if (key == SDLK_ESCAPE)
    {
        Game::terminate();
    }

    switch (key)
    {
    case SDLK_RIGHT:
        mainCamera->getComponent<components2D::Physics>()->setVelocity(100.0f, 0.0f);
        break;

    case SDLK_LEFT:
        mainCamera->getComponent<components2D::Physics>()->setVelocity(-100.0f, 0.0f);
        break;

    case SDLK_UP:
        mainCamera->getComponent<components2D::Physics>()->setVelocity(0.0f, -100.0f);
        break;

    case SDLK_DOWN:
        mainCamera->getComponent<components2D::Physics>()->setVelocity(0.0f, 100.0f);
        break;
    }
}

void TestScene::onKeyUp(SDL_Keycode key, Uint16 mod)
{

}
