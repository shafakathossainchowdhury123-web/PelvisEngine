#pragma once

#include "Weapon.h"

namespace Pelvis
{

class MeleeWeapon : public Weapon
{
public:

    explicit MeleeWeapon(
        float damage = 25.0f
    );

    void fire() override;

    const char* getName() const override;

    float getDamage() const;

private:

    float m_damage;
};

}
