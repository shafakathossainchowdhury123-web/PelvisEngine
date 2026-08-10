#include "World.h"

namespace Pelvis
{

World::World()
    : m_nextEntityID(1)
{
}

Entity* World::createEntity(
    const char* name
)
{
    const EntityID id = m_nextEntityID++;

    auto entity =
        std::make_unique<Entity>(
            id,
            name
        );

    Entity* result = entity.get();

    m_entities.push_back(
        std::move(entity)
    );

    return result;
}

void World::destroyEntity(EntityID id)
{
    for (auto it = m_entities.begin();
         it != m_entities.end();
         ++it)
    {
        if ((*it)->getID() == id)
        {
            m_entities.erase(it);
            return;
        }
    }
}

Entity* World::getEntity(EntityID id)
{
    for (auto& entity : m_entities)
    {
        if (entity->getID() == id)
            return entity.get();
    }

    return nullptr;
}

void World::update(float deltaTime)
{
    // Entity gameplay/component updates
    // will be added here later.

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
