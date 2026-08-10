#pragma once

#include "Weapon.h"

namespace Pelvis
{

class Inventory
{
public:

    static constexpr int MAX_SLOTS = 9;

    Inventory();

    void setWeapon(
        int slot,
        Weapon* weapon
    );

    Weapon* getWeapon(int slot) const;

    Weapon* getEquippedWeapon() const;

    int getEquippedSlot() const;

    bool equip(int slot);

    void clearSlot(int slot);

private:

    Weapon* m_slots[MAX_SLOTS];

    int m_equippedSlot;
};

}
