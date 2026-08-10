#include "Firearm.h"

#include <iostream>

namespace Pelvis
{

Firearm::Firearm(
    int magazineSize,
    float damage
)
    : m_ammo(magazineSize)
    , m_magazineSize(magazineSize)
    , m_damage(damage)
{
}

void Firearm::fire()
{
    if (m_ammo <= 0)
    {
        std::cout
            << "[Firearm] Empty magazine\n";

        return;
    }

    --m_ammo;

    std::cout
        << "[Firearm] FIRE | Damage: "
        << m_damage
        << " | Ammo: "
        << m_ammo
        << '/'
        << m_magazineSize
        << '\n';
}

void Firearm::altFire()
{
    std::cout
        << "[Firearm] Secondary fire\n";
}

void Firearm::reload()
{
    m_ammo = m_magazineSize;

    std::cout
        << "[Firearm] Reloaded\n";
}

const char* Firearm::getName() const
{
    return "Firearm";
}

int Firearm::getAmmo() const
{
    return m_ammo;
}

int Firearm::getMagazineSize() const
{
    return m_magazineSize;
}

}
