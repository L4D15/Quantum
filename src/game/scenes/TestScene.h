#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "quantum/Scene.h"
#include "quantum/Vector2.h"
#include "quantum/Libraries.h"
#include "quantum/systems/2D/Physics.h"
#include "quantum/systems/2D/AssetRendering.h"
#include "quantum/systems/2D/Collisions.h"
#include "quantum/Sprite.h"
#include "quantum/AnimatedSprite.h"

class TestScene : public Scene
{
public:
    TestScene();
    ~TestScene();

    void onActivate();

    void onDeactivate();

    void onRender();

    void onLoop();

    void onKeyDown(SDL_Keycode key, Uint16 mod);
    void onKeyUp(SDL_Keycode key, Uint16 mod);

private:
    // Systems
    systems2D::Physics* physicsSystem;
    systems2D::AssetRendering* renderingSystem;
    systems2D::Collisions* collisionsSystem;

    // Control related
    int timeKeyUp;
    bool keyDown;
    bool playerKeyDown;

    // Objects
    GameObject* background;
    GameObject* bonfire;
    GameObject* woodCrate;
    GameObject* player;
    GameObject* zero;

};

#endif // TESTSCENE_H
