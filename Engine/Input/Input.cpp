#include "Input.h"

namespace Pelvis
{

bool Input::s_keys[SDL_SCANCODE_COUNT] = {};
bool Input::s_previousKeys[SDL_SCANCODE_COUNT] = {};

bool Input::s_mouseButtons[3] = {};
bool Input::s_previousMouseButtons[3] = {};

float Input::s_mouseDeltaX = 0.0f;
float Input::s_mouseDeltaY = 0.0f;
float Input::s_mouseScroll = 0.0f;


void Input::initialize()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        s_keys[i] = false;
        s_previousKeys[i] = false;
    }

    for (int i = 0; i < 3; ++i)
    {
        s_mouseButtons[i] = false;
        s_previousMouseButtons[i] = false;
    }

    s_mouseDeltaX = 0.0f;
    s_mouseDeltaY = 0.0f;
    s_mouseScroll = 0.0f;
}


void Input::processEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_EVENT_KEY_DOWN:
        {
            if (event.key.repeat)
                break;

            const SDL_Scancode scancode = event.key.scancode;

            if (scancode >= 0 && scancode < SDL_SCANCODE_COUNT)
            {
                s_keys[scancode] = true;
            }

            break;
        }

        case SDL_EVENT_KEY_UP:
        {
            const SDL_Scancode scancode = event.key.scancode;

            if (scancode >= 0 && scancode < SDL_SCANCODE_COUNT)
            {
                s_keys[scancode] = false;
            }

            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            const int index =
                static_cast<int>(event.button.button) - 1;

            if (index >= 0 && index < 3)
            {
                s_mouseButtons[index] = true;
            }

            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            const int index =
                static_cast<int>(event.button.button) - 1;

            if (index >= 0 && index < 3)
            {
                s_mouseButtons[index] = false;
            }

            break;
        }

        case SDL_EVENT_MOUSE_MOTION:
        {
            s_mouseDeltaX += event.motion.xrel;
            s_mouseDeltaY += event.motion.yrel;

            break;
        }

        case SDL_EVENT_MOUSE_WHEEL:
        {
            s_mouseScroll += event.wheel.y;

            break;
        }

        default:
            break;
    }
}


void Input::update()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        s_previousKeys[i] = s_keys[i];
    }

    for (int i = 0; i < 3; ++i)
    {
        s_previousMouseButtons[i] = s_mouseButtons[i];
    }

    s_mouseDeltaX = 0.0f;
    s_mouseDeltaY = 0.0f;
    s_mouseScroll = 0.0f;
}


static SDL_Scancode getScancode(Key key)
{
    switch (key)
    {
        case Key::W:             return SDL_SCANCODE_W;
        case Key::A:             return SDL_SCANCODE_A;
        case Key::S:             return SDL_SCANCODE_S;
        case Key::D:             return SDL_SCANCODE_D;

        case Key::Space:         return SDL_SCANCODE_SPACE;
        case Key::LeftControl:   return SDL_SCANCODE_LCTRL;
        case Key::LeftShift:     return SDL_SCANCODE_LSHIFT;

        case Key::E:             return SDL_SCANCODE_E;
        case Key::F:             return SDL_SCANCODE_F;
        case Key::R:             return SDL_SCANCODE_R;

        case Key::Num1:          return SDL_SCANCODE_1;
        case Key::Num2:          return SDL_SCANCODE_2;
        case Key::Num3:          return SDL_SCANCODE_3;
        case Key::Num4:          return SDL_SCANCODE_4;
        case Key::Num5:          return SDL_SCANCODE_5;
        case Key::Num6:          return SDL_SCANCODE_6;
        case Key::Num7:          return SDL_SCANCODE_7;
        case Key::Num8:          return SDL_SCANCODE_8;
        case Key::Num9:          return SDL_SCANCODE_9;

        case Key::Escape:        return SDL_SCANCODE_ESCAPE;
    }

    return SDL_SCANCODE_UNKNOWN;
}


bool Input::isKeyDown(Key key)
{
    const SDL_Scancode scancode = getScancode(key);

    return s_keys[scancode];
}


bool Input::isKeyPressed(Key key)
{
    const SDL_Scancode scancode = getScancode(key);

    return s_keys[scancode] &&
           !s_previousKeys[scancode];
}


bool Input::isKeyReleased(Key key)
{
    const SDL_Scancode scancode = getScancode(key);

    return !s_keys[scancode] &&
           s_previousKeys[scancode];
}


bool Input::isMouseDown(MouseButton button)
{
    const int index = static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return s_mouseButtons[index];
}


bool Input::isMousePressed(MouseButton button)
{
    const int index = static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return s_mouseButtons[index] &&
           !s_previousMouseButtons[index];
}


bool Input::isMouseReleased(MouseButton button)
{
    const int index = static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return !s_mouseButtons[index] &&
           s_previousMouseButtons[index];
}


float Input::getMouseDeltaX()
{
    return s_mouseDeltaX;
}


float Input::getMouseDeltaY()
{
    return s_mouseDeltaY;
}


float Input::getMouseScroll()
{
    return s_mouseScroll;
}

}
