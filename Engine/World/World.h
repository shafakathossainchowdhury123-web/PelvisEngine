#pragma once

#include "Entity.h"

#include <vector>
#include <memory>

namespace Pelvis
{

class World
{
public:

    World();

    Entity* createEntity(
        const char* name
    );

    void destroyEntity(EntityID id);

    Entity* getEntity(EntityID id);
    const Entity* getEntity(EntityID id) const;

    void update(float deltaTime);

    void clear();

    std::size_t getEntityCount() const;

private:

    std::vector<std::unique_ptr<Entity>> m_entities;

    EntityID m_nextEntityID;
};

}
