#include "TestScene.h"
#include "quantum/Game.h"
#include "quantum/Game2D.h"
#include "quantum/Math.h"
#include "quantum/components/ComponentsList.h"
#include "quantum/AnimatedSprite.h"

using namespace components2D;

TestScene::TestScene() :
    Scene("Test Scene")
{
    // Player input
    this->playerKeyDown = false;

    // This Scene will have a Physics and rendering system
    this->physicsSystem = (systems2D::Physics*) addSystem(new systems2D::Physics());
    this->renderingSystem = (systems2D::AssetRendering*) addSystem(new systems2D::AssetRendering());
    this->collisionsSystem = (systems2D::Collisions*) addSystem(new systems2D::Collisions());

    // Add physics to the camera so we can move it
    mainCamera->addComponent(new components2D::Physics(*mainCamera));

    // Create objects
    this->background = createGameObject("background");
    this->background->addComponent(new AssetRenderer(*background,
                                                     mainCamera,
                                                     Game::resourceManager.getSprite("background.png"),
                                                     0,
                                                     0));
    ((Transform2D*)this->background->getComponent<Transform2D>())->setScale(4.0f, 4.0f);

    this->bonfire = createGameObject("bonfire");
    this->bonfire->addComponent(new AssetRenderer(*bonfire,
                                                  mainCamera,
                                                  Game::resourceManager.getAnimatedSprite("bonfire.png"),
                                                  10,
                                                  0));
    this->bonfire->addComponent(new CircleCollider(*bonfire));

    Transform2D* transformComp;

    transformComp = (Transform2D*) bonfire->getComponent<Transform2D>();
    transformComp->setScale(3.0f, 3.0f);
    transformComp->setPosition(0.0f, 130.0f);

    this->woodCrate = createGameObject("woodCrate");
    this->woodCrate->addComponent(new AssetRenderer(*woodCrate,
                                                    mainCamera,
                                                    Game::resourceManager.getSprite("wood-crate.png"),
                                                    5,
                                                    0));
    this->woodCrate->addComponent(new BoxCollider(*woodCrate));
    transformComp = (Transform2D*) woodCrate->getComponent<Transform2D>();
    transformComp->setPosition(80.0f, 120.0f);
    transformComp->setScale(2.0f, 2.0f);

    this->player = createGameObject("Player");
    this->player->addComponent(new AssetRenderer(*player,
                                                 mainCamera,
                                                 Game::resourceManager.getAnimatedSprite("zero.png"),
                                                 5,
                                                 0));
    this->player->addComponent(new CircleCollider(*player));
    this->player->addComponent(new Physics(*player));
    transformComp = (Transform2D*) player->getComponent<Transform2D>();
    transformComp->setScale(2.0f, 2.0f);
    transformComp->setPosition(-100.0f, 120.0f);

    BoxCollider* colliderComp;
    colliderComp = (BoxCollider*) this->player->getComponent<BoxCollider>();

    Collisions* collisionsComp;
    collisionsComp = (Collisions*) this->player->getComponent<Collisions>();

    collisionsComp->setIsTrigger(true);
    collisionsComp->setScriptToTrigger("collision.lua");
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
    collisionsSystem->process();

    // If there is no directional key pressed, smoot the stop of the camera
    components2D::Physics* p;
    p = (components2D::Physics*) mainCamera->getComponent<components2D::Physics>();
    if (keyDown == false && p->getVelocity() != Vector2(0,0))
    {
        // Camera Smoothing
        Vector2 velocity;

        velocity = math::interpolate(math::interpolation::EasyIn,
                                     p->getVelocity(),
                                     Vector2(0,0),
                                     math::Normalize(timeKeyUp, timeKeyUp + 1000, Game::getTime()));
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

    case SDLK_a:
        if (playerKeyDown == false)
        {
            ((assets2D::AnimatedSprite*)((AssetRenderer*)this->player->getComponent<AssetRenderer>())->getRenderizableAsset())->changeAnimation("Walk Left");
            Physics* physicsComp;
            physicsComp = (Physics*) player->getComponent<Physics>();
            physicsComp->addVelocity(-100.0f, 0.0f);

            this->playerKeyDown = true;
        }
        break;

    case SDLK_d:
        if (playerKeyDown == false)
        {
            ((assets2D::AnimatedSprite*)((AssetRenderer*)this->player->getComponent<AssetRenderer>())->getRenderizableAsset())->changeAnimation("Walk Right");
            Physics* physicsComp;
            physicsComp = (Physics*) player->getComponent<Physics>();
            physicsComp->addVelocity(100.0f, 0.0f);
            playerKeyDown = true;
        }
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

    if (playerKeyDown == true)
    {
        if (key == SDLK_a)
        {
            ((assets2D::AnimatedSprite*)((AssetRenderer*)this->player->getComponent<AssetRenderer>())->getRenderizableAsset())->changeAnimation("Stand Left");
            Physics* physicsComp;
            physicsComp = (Physics*) player->getComponent<Physics>();
            physicsComp->addVelocity(100.0f, 0.0f);
            playerKeyDown = false;
        }
        else if (key == SDLK_d)
        {
            ((assets2D::AnimatedSprite*)((AssetRenderer*)this->player->getComponent<AssetRenderer>())->getRenderizableAsset())->changeAnimation("Stand Right");
            Physics* physicsComp;
            physicsComp = (Physics*) player->getComponent<Physics>();
            physicsComp->addVelocity(-100.0f, 0.0f);
            playerKeyDown = false;
        }
    }
    keyDown = false;
}
