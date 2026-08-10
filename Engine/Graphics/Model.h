#pragma once

#include "../Renderer/Mesh.h"

#include <memory>
#include <string>
#include <vector>

namespace Pelvis
{

class Model
{
public:

    Model();
    ~Model();

    bool load(
        const std::string& path
    );

    void draw() const;

    void destroy();

    bool isLoaded() const;

    const std::string& getPath() const;

    std::size_t getMeshCount() const;

private:

    std::vector<std::unique_ptr<Mesh>> m_meshes;

    std::string m_path;

    bool m_loaded;
};

}
