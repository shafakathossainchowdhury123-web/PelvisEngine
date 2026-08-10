#include "Inventory.h"

namespace Pelvis
{

Inventory::Inventory()
    : m_equippedSlot(0)
{
    for (int i = 0; i < MAX_SLOTS; ++i)
    {
        m_slots[i] = nullptr;
    }
}

void Inventory::setWeapon(
    int slot,
    Weapon* weapon
)
{
    if (slot < 0 || slot >= MAX_SLOTS)
        return;

    m_slots[slot] = weapon;
}

Weapon* Inventory::getWeapon(int slot) const
{
    if (slot < 0 || slot >= MAX_SLOTS)
        return nullptr;

    return m_slots[slot];
}

Weapon* Inventory::getEquippedWeapon() const
{
    return m_slots[m_equippedSlot];
}

int Inventory::getEquippedSlot() const
{
    return m_equippedSlot;
}

bool Inventory::equip(int slot)
{
    if (slot < 0 || slot >= MAX_SLOTS)
        return false;

    if (m_slots[slot] == nullptr)
        return false;

    m_equippedSlot = slot;

    return true;
}

void Inventory::clearSlot(int slot)
{
    if (slot < 0 || slot >= MAX_SLOTS)
        return;

    m_slots[slot] = nullptr;

    if (m_equippedSlot == slot)
    {
        for (int i = 0; i < MAX_SLOTS; ++i)
        {
            if (m_slots[i] != nullptr)
            {
                m_equippedSlot = i;
                return;
            }
        }
    }
}

}
