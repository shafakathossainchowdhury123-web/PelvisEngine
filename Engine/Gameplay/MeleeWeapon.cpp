#include "MeleeWeapon.h"

#include <iostream>

namespace Pelvis
{

MeleeWeapon::MeleeWeapon(float damage)
    : m_damage(damage)
{
}

void MeleeWeapon::fire()
{
    std::cout
        << "[Melee] Attack | Damage: "
        << m_damage
        << '\n';
}

const char* MeleeWeapon::getName() const
{
    return "Melee Weapon";
}

float MeleeWeapon::getDamage() const
{
    return m_damage;
}

}
