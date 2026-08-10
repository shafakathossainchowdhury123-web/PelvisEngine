#pragma once

#include "Action.h"

namespace Pelvis
{

class ActionMap
{
public:

    static bool isDown(Action action);
    static bool isPressed(Action action);
    static bool isReleased(Action action);

};

}
