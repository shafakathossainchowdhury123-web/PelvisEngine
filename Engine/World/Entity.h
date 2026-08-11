#pragma once

#include "Transform.h"

#include <cstdint>
#include <string>

namespace Pelvis
{

class Model;

using EntityID = std::uint32_t;

class Entity
{
public:

    Entity();
    explicit Entity(const std::string& name);
    Entity(EntityID id, const std::string& name);

    EntityID getID() const;

    const std::string& getName() const;
    void setName(const std::string& name);

    Transform& getTransform();
    const Transform& getTransform() const;

    void setModel(Model* model);
    Model* getModel();
    const Model* getModel() const;

private:

    EntityID m_id;

    std::string m_name;

    Transform m_transform;

    Model* m_model = nullptr;
};

}
