#include "ActionMap.h"

#include "../Input/Input.h"

namespace Pelvis
{

void ActionMap::initialize()
{
    ActionSystem::initialize();
}

void ActionMap::update()
{
    ActionSystem::update();

    mapKeyboardActions();
    mapMouseActions();
}

void ActionMap::mapKeyboardActions()
{
    if (Input::isKeyDown(Key::E))
        ActionSystem::trigger(Action::Interact);
    else
        ActionSystem::release(Action::Interact);

    if (Input::isKeyDown(Key::R))
        ActionSystem::trigger(Action::Reload);
    else
        ActionSystem::release(Action::Reload);

    if (Input::isKeyDown(Key::F))
        ActionSystem::trigger(Action::Use);
    else
        ActionSystem::release(Action::Use);

    if (Input::isKeyDown(Key::Num1))
        ActionSystem::trigger(Action::Slot1);
    else
        ActionSystem::release(Action::Slot1);

    if (Input::isKeyDown(Key::Num2))
        ActionSystem::trigger(Action::Slot2);
    else
        ActionSystem::release(Action::Slot2);

    if (Input::isKeyDown(Key::Num3))
        ActionSystem::trigger(Action::Slot3);
    else
        ActionSystem::release(Action::Slot3);

    if (Input::isKeyDown(Key::Num4))
        ActionSystem::trigger(Action::Slot4);
    else
        ActionSystem::release(Action::Slot4);

    if (Input::isKeyDown(Key::Num5))
        ActionSystem::trigger(Action::Slot5);
    else
        ActionSystem::release(Action::Slot5);

    if (Input::isKeyDown(Key::Num6))
        ActionSystem::trigger(Action::Slot6);
    else
        ActionSystem::release(Action::Slot6);

    if (Input::isKeyDown(Key::Num7))
        ActionSystem::trigger(Action::Slot7);
    else
        ActionSystem::release(Action::Slot7);

    if (Input::isKeyDown(Key::Num8))
        ActionSystem::trigger(Action::Slot8);
    else
        ActionSystem::release(Action::Slot8);

    if (Input::isKeyDown(Key::Num9))
        ActionSystem::trigger(Action::Slot9);
    else
        ActionSystem::release(Action::Slot9);
}

void ActionMap::mapMouseActions()
{
    if (Input::isMouseDown(MouseButton::Left))
        ActionSystem::trigger(Action::Primary);
    else
        ActionSystem::release(Action::Primary);

    if (Input::isMouseDown(MouseButton::Right))
        ActionSystem::trigger(Action::Secondary);
    else
        ActionSystem::release(Action::Secondary);
}

}
