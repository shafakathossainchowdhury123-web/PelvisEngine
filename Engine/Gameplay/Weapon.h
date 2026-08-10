#pragma once

#include "GameplayAction.h"

namespace Pelvis
{

class Weapon : public GameplayAction
{
public:

    virtual ~Weapon() = default;

    virtual void fire() {}
    virtual void altFire() {}
    virtual void reload() {}

    virtual bool canFire() const
    {
        return true;
    }

    virtual const char* getName() const
    {
        return "Weapon";
    }

    void primary() override
    {
        fire();
    }

    void secondary() override
    {
        altFire();
    }

};

}
