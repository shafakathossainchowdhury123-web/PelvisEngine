#include "AssetManager.h"

#include <iostream>
#include <utility>

namespace Pelvis
{

AssetManager::AssetManager() = default;

AssetManager::~AssetManager()
{
    clear();
}

Model* AssetManager::loadModel(
    const std::string& path
)
{
    if (path.empty())
        return nullptr;

    auto existing = m_models.find(path);

    if (existing != m_models.end())
    {
        std::cout
            << "AssetManager: using cached model: "
            << path
            << '\n';

        return existing->second.get();
    }

    auto model = std::make_unique<Model>();

    if (!model->load(path))
    {
        std::cerr
            << "AssetManager: failed to load model: "
            << path
            << '\n';

        return nullptr;
    }

    Model* result = model.get();

    m_models.emplace(
        path,
        std::move(model)
    );

    std::cout
        << "AssetManager: model registered: "
        << path
        << '\n';

    return result;
}

Model* AssetManager::getModel(
    const std::string& path
)
{
    auto it = m_models.find(path);

    if (it == m_models.end())
        return nullptr;

    return it->second.get();
}

bool AssetManager::hasModel(
    const std::string& path
) const
{
    return m_models.find(path) != m_models.end();
}

void AssetManager::clear()
{
    m_models.clear();
}

std::size_t AssetManager::getModelCount() const
{
    return m_models.size();
}

}
