#pragma once

#include <SDL3/SDL.h>

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Pelvis
{

class Input
{
public:

    static void initialize();

    static void processEvent(const SDL_Event& event);

    static void update();

    static bool isKeyDown(Key key);
    static bool isKeyPressed(Key key);
    static bool isKeyReleased(Key key);

    static bool isMouseDown(MouseButton button);
    static bool isMousePressed(MouseButton button);
    static bool isMouseReleased(MouseButton button);

    static float getMouseDeltaX();
    static float getMouseDeltaY();

    static float getMouseScroll();

private:

    static bool s_keys[SDL_SCANCODE_COUNT];
    static bool s_previousKeys[SDL_SCANCODE_COUNT];

    static bool s_mouseButtons[3];
    static bool s_previousMouseButtons[3];

    static float s_mouseDeltaX;
    static float s_mouseDeltaY;
    static float s_mouseScroll;

};

}
