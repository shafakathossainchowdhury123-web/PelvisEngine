#pragma once

#include "Transform.h"

namespace Pelvis
{

using EntityID = unsigned int;

class Entity
{
public:

    Entity(
        EntityID id,
        const char* name
    );

    EntityID getID() const;

    const char* getName() const;

    Transform& getTransform();

    bool isActive() const;

    void setActive(bool active);

private:

    EntityID m_id;

    const char* m_name;

    Transform m_transform;

    bool m_active;
};

}
