#include "Entity.h"

namespace Pelvis
{

static std::uint32_t nextEntityID = 1;

Entity::Entity()
    : m_id(nextEntityID++)
    , m_name("Entity")
{
}

Entity::Entity(const std::string& name)
    : m_id(nextEntityID++)
    , m_name(name)
{
}

std::uint32_t Entity::getID() const
{
    return m_id;
}

const std::string& Entity::getName() const
{
    return m_name;
}

void Entity::setName(const std::string& name)
{
    m_name = name;
}

Transform& Entity::getTransform()
{
    return m_transform;
}

const Transform& Entity::getTransform() const
{
    return m_transform;
}

}
