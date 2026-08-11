#include "World.h"

#include <algorithm>

namespace Pelvis
{

World::World()
    : m_nextEntityID(1)
{
}

Entity* World::createEntity(const char* name)
{
    const EntityID id = m_nextEntityID++;

    auto entity = std::make_unique<Entity>(
        id,
        name ? name : "Entity"
    );

    Entity* result = entity.get();

    m_entities.push_back(
        std::move(entity)
    );

    return result;
}

void World::destroyEntity(EntityID id)
{
    m_entities.erase(
        std::remove_if(
            m_entities.begin(),
            m_entities.end(),
            [id](const std::unique_ptr<Entity>& entity)
            {
                return entity &&
                       entity->getID() == id;
            }
        ),
        m_entities.end()
    );
}

Entity* World::getEntity(EntityID id)
{
    for (auto& entity : m_entities)
    {
        if (entity &&
            entity->getID() == id)
        {
            return entity.get();
        }
    }

    return nullptr;
}

const Entity* World::getEntity(EntityID id) const
{
    for (const auto& entity : m_entities)
    {
        if (entity &&
            entity->getID() == id)
        {
            return entity.get();
        }
    }

    return nullptr;
}

void World::update(float deltaTime)
{
    (void)deltaTime;
}

void World::clear()
{
    m_entities.clear();
    m_nextEntityID = 1;
}

std::size_t World::getEntityCount() const
{
    return m_entities.size();
}

}
