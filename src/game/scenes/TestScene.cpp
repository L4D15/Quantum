#include "TestScene.h"
#include "quantum/Game.h"
#include "quantum/Math.h"
#include "quantum/components/ComponentsList.h"

TestScene::TestScene() :
    Scene("Test Scene")
{
    // This Scene will have a Physics and rendering system
    this->physicsSystem = (systems2D::Physics*) addSystem(new systems2D::Physics());
    this->renderingSystem = (systems2D::AssetRendering*) addSystem(new systems2D::AssetRendering());

    // Resources
    sprite = Game::resourceManager.getAnimatedSprite("bonfire.png");
    bgImage = Game::resourceManager.getSprite("bg.png");

    // Objects
    object = createGameObject("Object");
    object->setPosition(0, 0);
    object->addComponent(new components2D::Physics(*object));
    object->addComponent(new components2D::AssetRenderer(*object, mainCamera, sprite));

    background = createGameObject("Background");
    background->addComponent(new components2D::AssetRenderer(*background, mainCamera, bgImage, -1, 1000));

    mainCamera->addComponent(new components2D::Physics(*mainCamera));
}

TestScene::~TestScene()
{

}

void TestScene::onActivate()
{
    keyDown = false;
}

void TestScene::onDeactivate()
{

}

void TestScene::onLoop()
{
    Scene::onLoop();

    // Process systems
    physicsSystem->process();

    // If there is no directional key pressed, smoot the stop of the camera
    components2D::Physics* p;
    p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
    if (keyDown == false && p->getVelocity() != Vector2D(0,0))
    {
        // Camera Smoothing
        Vector2D velocity;

        velocity = math::interpolate(math::interpolation::EasyIn, p->getVelocity(), Vector2D(0,0), math::Normalize(timeKeyUp, timeKeyUp + 1000, Game::getTime()));
        p->setVelocity(velocity);
    }
}

void TestScene::onRender()
{
    this->renderingSystem->process();
    Scene::onRender();
}


void TestScene::onKeyDown(SDL_Keycode key, Uint16 mod)
{
    components2D::Physics* p;
    switch (key)
    {
    case SDLK_ESCAPE:
        Game::terminate();
        break;

    case SDLK_RIGHT:
        p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
        p->addVelocity(5.0f, 0.0f);
        break;

    case SDLK_LEFT:
        p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
        p->addVelocity(-5.0f, 0.0f);
        break;

    case SDLK_UP:
        p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
        p->addVelocity(0.0f, -5.0f);
        break;

    case SDLK_DOWN:
        p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
        p->addVelocity(0.0f, 5.0f);
        break;
    }
    this->keyDown = true;
}

void TestScene::onKeyUp(SDL_Keycode key, Uint16 mod)
{
    // If we transition between pressed and unpressed
    if (keyDown == true)
    {
        timeKeyUp = Game::getTime();
    }

    keyDown = false;
}
