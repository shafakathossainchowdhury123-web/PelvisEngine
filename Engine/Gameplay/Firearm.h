#pragma once

#include "Weapon.h"

namespace Pelvis
{

class Firearm : public Weapon
{
public:

    Firearm(
        int magazineSize = 30,
        float damage = 25.0f
    );

    void fire() override;
    void altFire() override;
    void reload() override;

    const char* getName() const override;

    int getAmmo() const;
    int getMagazineSize() const;

private:

    int m_ammo;
    int m_magazineSize;

    float m_damage;
};

}
