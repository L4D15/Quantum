#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "quantum/Scene.h"
#include "quantum/Vector2.h"
#include "quantum/Libraries.h"
#include "quantum/systems/2D/Physics.h"
#include "quantum/systems/2D/AssetRendering.h"
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
    systems2D::Physics* physicsSystem;
    systems2D::AssetRendering* renderingSystem;

    // Control related
    int timeKeyUp;
    bool keyDown;

};

#endif // TESTSCENE_H
