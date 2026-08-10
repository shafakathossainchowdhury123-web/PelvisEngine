#include "ActionMap.h"

#include "Input.h"

namespace Pelvis
{

bool ActionMap::isDown(Action action)
{
    switch (action)
    {
        case Action::MoveForward:
            return Input::isKeyDown(Key::W);

        case Action::MoveBackward:
            return Input::isKeyDown(Key::S);

        case Action::MoveLeft:
            return Input::isKeyDown(Key::A);

        case Action::MoveRight:
            return Input::isKeyDown(Key::D);

        case Action::Jump:
            return Input::isKeyDown(Key::Space);

        case Action::Crouch:
            return Input::isKeyDown(Key::LeftControl);

        case Action::Sprint:
            return Input::isKeyDown(Key::LeftShift);

        case Action::PrimaryAction:
            return Input::isMouseDown(MouseButton::Left);

        case Action::SecondaryAction:
            return Input::isMouseDown(MouseButton::Right);

        case Action::Interact:
            return Input::isKeyDown(Key::E);

        case Action::Reload:
            return Input::isKeyDown(Key::R);

        case Action::Slot1:
            return Input::isKeyDown(Key::Num1);

        case Action::Slot2:
            return Input::isKeyDown(Key::Num2);

        case Action::Slot3:
            return Input::isKeyDown(Key::Num3);

        case Action::Slot4:
            return Input::isKeyDown(Key::Num4);

        case Action::Slot5:
            return Input::isKeyDown(Key::Num5);

        case Action::Slot6:
            return Input::isKeyDown(Key::Num6);

        case Action::Slot7:
            return Input::isKeyDown(Key::Num7);

        case Action::Slot8:
            return Input::isKeyDown(Key::Num8);

        case Action::Slot9:
            return Input::isKeyDown(Key::Num9);

        case Action::Pause:
            return Input::isKeyDown(Key::Escape);
    }

    return false;
}


bool ActionMap::isPressed(Action action)
{
    switch (action)
    {
        case Action::MoveForward:
            return Input::isKeyPressed(Key::W);

        case Action::MoveBackward:
            return Input::isKeyPressed(Key::S);

        case Action::MoveLeft:
            return Input::isKeyPressed(Key::A);

        case Action::MoveRight:
            return Input::isKeyPressed(Key::D);

        case Action::Jump:
            return Input::isKeyPressed(Key::Space);

        case Action::Crouch:
            return Input::isKeyPressed(Key::LeftControl);

        case Action::Sprint:
            return Input::isKeyPressed(Key::LeftShift);

        case Action::PrimaryAction:
            return Input::isMousePressed(MouseButton::Left);

        case Action::SecondaryAction:
            return Input::isMousePressed(MouseButton::Right);

        case Action::Interact:
            return Input::isKeyPressed(Key::E);

        case Action::Reload:
            return Input::isKeyPressed(Key::R);

        case Action::Slot1:
            return Input::isKeyPressed(Key::Num1);

        case Action::Slot2:
            return Input::isKeyPressed(Key::Num2);

        case Action::Slot3:
            return Input::isKeyPressed(Key::Num3);

        case Action::Slot4:
            return Input::isKeyPressed(Key::Num4);

        case Action::Slot5:
            return Input::isKeyPressed(Key::Num5);

        case Action::Slot6:
            return Input::isKeyPressed(Key::Num6);

        case Action::Slot7:
            return Input::isKeyPressed(Key::Num7);

        case Action::Slot8:
            return Input::isKeyPressed(Key::Num8);

        case Action::Slot9:
            return Input::isKeyPressed(Key::Num9);

        case Action::Pause:
            return Input::isKeyPressed(Key::Escape);
    }

    return false;
}


bool ActionMap::isReleased(Action action)
{
    switch (action)
    {
        case Action::MoveForward:
            return Input::isKeyReleased(Key::W);

        case Action::MoveBackward:
            return Input::isKeyReleased(Key::S);

        case Action::MoveLeft:
            return Input::isKeyReleased(Key::A);

        case Action::MoveRight:
            return Input::isKeyReleased(Key::D);

        case Action::Jump:
            return Input::isKeyReleased(Key::Space);

        case Action::Crouch:
            return Input::isKeyReleased(Key::LeftControl);

        case Action::Sprint:
            return Input::isKeyReleased(Key::LeftShift);

        case Action::PrimaryAction:
            return Input::isMouseReleased(MouseButton::Left);

        case Action::SecondaryAction:
            return Input::isMouseReleased(MouseButton::Right);

        case Action::Interact:
            return Input::isKeyReleased(Key::E);

        case Action::Reload:
            return Input::isKeyReleased(Key::R);

        case Action::Slot1:
            return Input::isKeyReleased(Key::Num1);

        case Action::Slot2:
            return Input::isKeyReleased(Key::Num2);

        case Action::Slot3:
            return Input::isKeyReleased(Key::Num3);

        case Action::Slot4:
            return Input::isKeyReleased(Key::Num4);

        case Action::Slot5:
            return Input::isKeyReleased(Key::Num5);

        case Action::Slot6:
            return Input::isKeyReleased(Key::Num6);

        case Action::Slot7:
            return Input::isKeyReleased(Key::Num7);

        case Action::Slot8:
            return Input::isKeyReleased(Key::Num8);

        case Action::Slot9:
            return Input::isKeyReleased(Key::Num9);

        case Action::Pause:
            return Input::isKeyReleased(Key::Escape);
    }

    return false;
}

}
