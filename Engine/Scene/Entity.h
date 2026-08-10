#pragma once

#include "Transform.h"

#include <cstdint>
#include <string>

namespace Pelvis
{

class Entity
{
public:

    Entity();
    explicit Entity(const std::string& name);

    std::uint32_t getID() const;

    const std::string& getName() const;
    void setName(const std::string& name);

    Transform& getTransform();
    const Transform& getTransform() const;

private:

    std::uint32_t m_id;
    std::string m_name;
    Transform m_transform;
};

}
