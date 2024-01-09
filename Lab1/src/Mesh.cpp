#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Debug.h"

Mesh::Mesh(const std::string& filePath)
    :
    mTransformMatrix(),
    mBoundingBox(std::vector<MinMax>(3)),
    mFilePath(filePath)
{
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType |
            aiProcess_GenNormals);

        if (scene == nullptr)
        {
            Debug::ThrowException("Unable to import from defined file! (" + filePath + ")");
        }

        auto mesh = scene->mMeshes[0];

        mVertices.reserve(mesh->mNumVertices);
        mIndices.reserve(mesh->mNumFaces * 3);

        // std::cout << "Processing vertices...(" << mesh->mNumVertices << ")" << std::endl;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            mVertices.push_back({ {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}, {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}});
            // std::cout << mesh->mNormals[i].x << " || " << mesh->mNormals[i].y << " || " << mesh->mNormals[i].z << std::endl;
            if (mesh->mVertices[i].x < mBoundingBox[0].min) mBoundingBox[0].min = mesh->mVertices[i].x;
            if (mesh->mVertices[i].x > mBoundingBox[0].max) mBoundingBox[0].max = mesh->mVertices[i].x;
            if (mesh->mVertices[i].y < mBoundingBox[1].min) mBoundingBox[1].min = mesh->mVertices[i].y;
            if (mesh->mVertices[i].y > mBoundingBox[1].max) mBoundingBox[1].max = mesh->mVertices[i].y;
            if (mesh->mVertices[i].z < mBoundingBox[2].min) mBoundingBox[2].min = mesh->mVertices[i].z;
            if (mesh->mVertices[i].z > mBoundingBox[2].max) mBoundingBox[2].max = mesh->mVertices[i].z;
        }


        double center[3];
        center[0] = (mBoundingBox[0].min + mBoundingBox[0].max) / 2;
        center[1] = (mBoundingBox[1].min + mBoundingBox[1].max) / 2;
        center[2] = (mBoundingBox[2].min + mBoundingBox[2].max) / 2;

        double M = std::max<double>(mBoundingBox[0].max - mBoundingBox[0].min, std::max<double>(mBoundingBox[1].max - mBoundingBox[1].min, mBoundingBox[2].max - mBoundingBox[2].min));

        double scaleVal = 2.0l / M;
        mTransformMatrix.Scale(glm::vec3(scaleVal, scaleVal, scaleVal));

        mTransformMatrix.Translation(glm::vec3(center[0], center[1], center[2]));

        // std::cout << "Processing indices..." << std::endl;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                mIndices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }
    }

    mVBO.FillBuffer(mVertices.data(), mVertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    mIBO.FillBuffer(mIndices.data(), mIndices.size(), GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
}

const VertexBuffer& Mesh::GetVB() const
{
    return mVBO;
}

const IndexBuffer& Mesh::GetIB() const
{
    return mIBO;
}

const std::vector<Vertex> Mesh::GetVertices() const
{
    return mVertices;
}

const std::vector<unsigned int> Mesh::GetIndices() const
{
    return mIndices;
}

const Transform& Mesh::GetTransform() const
{
    return mTransformMatrix;
}
