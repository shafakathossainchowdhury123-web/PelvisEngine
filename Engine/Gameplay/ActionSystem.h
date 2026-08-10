#pragma once

namespace Pelvis
{

enum class Action
{
    Primary,
    Secondary,
    Interact,
    Reload,
    Use,
    Slot1,
    Slot2,
    Slot3,
    Slot4,
    Slot5,
    Slot6,
    Slot7,
    Slot8,
    Slot9
};

class ActionSystem
{
public:

    static void initialize();

    static void update();

    static bool isPressed(Action action);
    static bool isDown(Action action);
    static bool isReleased(Action action);

    static void trigger(Action action);
    static void release(Action action);

private:

    static bool s_current[14];
    static bool s_previous[14];
};

}
