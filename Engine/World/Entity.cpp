#include "Entity.h"

namespace Pelvis
{

Entity::Entity(
    EntityID id,
    const char* name
)
    : m_id(id)
    , m_name(name)
    , m_active(true)
{
}

EntityID Entity::getID() const
{
    return m_id;
}

const char* Entity::getName() const
{
    return m_name;
}

Transform& Entity::getTransform()
{
    return m_transform;
}

bool Entity::isActive() const
{
    return m_active;
}

void Entity::setActive(bool active)
{
    m_active = active;
}

}
