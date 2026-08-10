#include "Model.h"
#include "../Renderer/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

namespace Pelvis
{

Model::Model()
    : m_loaded(false)
{
}

Model::~Model()
{
    destroy();
}

bool Model::load(
    const std::string& path
)
{
    destroy();

    m_path = path;

    Assimp::Importer importer;

    const aiScene* scene =
        importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality
        );

    if (!scene ||
        !scene->mRootNode ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cerr
            << "ModelLoader: failed to load model:\n"
            << path
            << '\n'
            << "Assimp: "
            << importer.GetErrorString()
            << '\n';

        return false;
    }

    std::cout
        << "ModelLoader: loading "
        << path
        << '\n';

    for (unsigned int i = 0;
         i < scene->mNumMeshes;
         ++i)
    {
        aiMesh* sourceMesh =
            scene->mMeshes[i];

        if (!sourceMesh ||
            sourceMesh->mNumVertices == 0)
        {
            continue;
        }

        std::vector<float> vertices;

        vertices.reserve(
            sourceMesh->mNumFaces * 9
        );

        for (unsigned int faceIndex = 0;
             faceIndex < sourceMesh->mNumFaces;
             ++faceIndex)
        {
            const aiFace& face =
                sourceMesh->mFaces[faceIndex];

            for (unsigned int index = 0;
                 index < face.mNumIndices;
                 ++index)
            {
                const unsigned int vertexIndex =
                    face.mIndices[index];

                const aiVector3D& vertex =
                    sourceMesh->mVertices[vertexIndex];

                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            }
        }

        if (vertices.empty())
            continue;

        std::vector<Vertex> meshVertices;
        meshVertices.reserve(vertices.size() / 3);

        for (std::size_t vertexIndex = 0;
             vertexIndex < vertices.size();
             vertexIndex += 3)
        {
            Vertex vertex;

            vertex.x = vertices[vertexIndex];
            vertex.y = vertices[vertexIndex + 1];
            vertex.z = vertices[vertexIndex + 2];

            meshVertices.push_back(vertex);
        }

        auto mesh =
            std::make_unique<Mesh>();

        if (!mesh->create(meshVertices))
        {
            std::cerr
                << "ModelLoader: failed to create GPU mesh.\n";

            destroy();

            return false;
        }

        m_meshes.push_back(
            std::move(mesh)
        );
    }

    if (m_meshes.empty())
    {
        std::cerr
            << "ModelLoader: model contains no usable meshes.\n";

        destroy();

        return false;
    }

    m_loaded = true;

    std::cout
        << "ModelLoader: loaded successfully.\n"
        << "Meshes: "
        << m_meshes.size()
        << '\n';

    return true;
}

void Model::draw() const
{
    if (!m_loaded)
        return;

    for (const auto& mesh : m_meshes)
    {
        if (mesh)
            mesh->draw();
    }
}

void Model::destroy()
{
    m_meshes.clear();

    m_path.clear();

    m_loaded = false;
}

bool Model::isLoaded() const
{
    return m_loaded;
}

const std::string& Model::getPath() const
{
    return m_path;
}

std::size_t Model::getMeshCount() const
{
    return m_meshes.size();
}

}
