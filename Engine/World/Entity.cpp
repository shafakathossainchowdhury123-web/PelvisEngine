#include "Entity.h"

#include "../Graphics/Model.h"

namespace Pelvis
{

static EntityID nextEntityID = 1;


Entity::Entity()
    : m_id(nextEntityID++)
    , m_name("Entity")
    , m_model(nullptr)
{
}


Entity::Entity(
    const std::string& name
)
    : m_id(nextEntityID++)
    , m_name(name)
    , m_model(nullptr)
{
}


Entity::Entity(
    EntityID id,
    const std::string& name
)
    : m_id(id)
    , m_name(name)
    , m_model(nullptr)
{
    if (id >= nextEntityID)
        nextEntityID = id + 1;
}


EntityID Entity::getID() const
{
    return m_id;
}


const std::string& Entity::getName() const
{
    return m_name;
}


void Entity::setName(
    const std::string& name
)
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


void Entity::setModel(
    Model* model
)
{
    m_model = model;
}


Model* Entity::getModel()
{
    return m_model;
}


const Model* Entity::getModel() const
{
    return m_model;
}

}
