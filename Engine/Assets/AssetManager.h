#pragma once

#include "../Graphics/Model.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Pelvis
{

class AssetManager
{
public:

    AssetManager();
    ~AssetManager();

    Model* loadModel(
        const std::string& path
    );

    Model* getModel(
        const std::string& path
    );

    bool hasModel(
        const std::string& path
    ) const;

    void clear();

    std::size_t getModelCount() const;

private:

    std::unordered_map<
        std::string,
        std::unique_ptr<Model>
    > m_models;
};

}
