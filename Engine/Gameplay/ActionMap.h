#pragma once

#include "ActionSystem.h"

namespace Pelvis
{

class ActionMap
{
public:

    static void initialize();

    static void update();

private:

    static void mapKeyboardActions();
    static void mapMouseActions();
};

}
