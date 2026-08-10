#include "ActionSystem.h"

namespace Pelvis
{

bool ActionSystem::s_current[14] = {};
bool ActionSystem::s_previous[14] = {};

static int indexOf(Action action)
{
    return static_cast<int>(action);
}

void ActionSystem::initialize()
{
    for (int i = 0; i < 14; ++i)
    {
        s_current[i] = false;
        s_previous[i] = false;
    }
}

void ActionSystem::update()
{
    for (int i = 0; i < 14; ++i)
    {
        s_previous[i] = s_current[i];
    }
}

bool ActionSystem::isPressed(Action action)
{
    const int i = indexOf(action);

    return s_current[i] && !s_previous[i];
}

bool ActionSystem::isDown(Action action)
{
    return s_current[indexOf(action)];
}

bool ActionSystem::isReleased(Action action)
{
    const int i = indexOf(action);

    return !s_current[i] && s_previous[i];
}

void ActionSystem::trigger(Action action)
{
    s_current[indexOf(action)] = true;
}

void ActionSystem::release(Action action)
{
    s_current[indexOf(action)] = false;
}

}
