#include "MeshV2.h"

#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Debug.h"

MeshV2::MeshV2()
{
}

MeshV2::MeshV2(const std::string& filePath)
	:
	mFilePath(filePath)
{
	Init(mFilePath);
}

void MeshV2::Initialize()
{
	if (mFilePath.empty())
		Debug::ThrowException("MeshV2 => File path not set!");

	Init(mFilePath);
}

const std::string& MeshV2::GetFilePath() const
{
    return mFilePath;
}

void MeshV2::SetFilePath(const std::string& filePath)
{
	mFilePath = filePath;
}

void MeshV2::ParseScene(const aiScene* pScene)
{
    ParseMeshes(pScene);
}

void MeshV2::ParseMeshes(const aiScene* pScene)
{
    printf("-------------------\n");
    printf("Parsing %d meshes\n\n", pScene->mNumMeshes);

    int total_verts = 0, total_indices = 0, total_bones = 0;

    for (int j = 0; j < pScene->mNumMeshes; j++)
    {
        const aiMesh* pMesh = pScene->mMeshes[j];
        printf("Mesh %d '%s': Vertoces %d indices %d bones %d\n\n", j, pMesh->mName.C_Str(), pMesh->mNumVertices, pMesh->mNumFaces * 3, pMesh->mNumBones);

        total_verts += pMesh->mNumVertices;
        total_indices += pMesh->mNumFaces * 3;
        total_bones += pMesh->mNumBones;

        if (pMesh->HasBones())
            ParseMeshBones(pMesh);

        printf("\n");
    }

    printf("\n Total vertices %d total indices %d total bones %d\n", total_verts, total_indices, total_bones);
}

void MeshV2::ParseMeshBones(const aiMesh* pMesh)
{
    for (int j = 0; j < pMesh->mNumBones; j++)
    {
        ParseSingleBone(j, pMesh->mBones[j]);
    }
}

void MeshV2::ParseSingleBone(const unsigned int& boneIndex, const aiBone* pBone)
{
    printf("\tBone %d: '%s' num vertices affected by this bone: %d\n", boneIndex, pBone->mName.C_Str(), pBone->mNumWeights);

    for (int j = 0; j < pBone->mNumWeights; j++)
    {
        if (j == 0)
            printf("\n");

        const aiVertexWeight& vWeight = pBone->mWeights[j];
        printf("\t\t %d: Vertex id %d weight %.2f\n", j, vWeight.mVertexId, vWeight.mWeight);
    }
}

void MeshV2::Init(const std::string& filePath)
{
    /*
    Assimp::Importer importer;
    
    const aiScene* pScene = importer.ReadFile(filePath.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GenNormals);
    
    if (pScene == nullptr)
    {
        Debug::ThrowException("Unable to import from defined file! (" + filePath + ")");
    }

    ParseScene(pScene);

    auto mesh = pScene->mMeshes[0];

    if (pScene->HasAnimations())
    {
        for (int j = 0; j < pScene->mNumAnimations; j++)
        {
            Debug::Print(pScene->mAnimations[j]->mName.C_Str());
        }
    }
    */

    /*

    mVertices.reserve(mesh->mNumVertices);
    mIndices.reserve(mesh->mNumFaces * 3);

    // std::cout << "Processing vertices...(" << mesh->mNumVertices << ")" << std::endl;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        mVertices.push_back({ {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}, {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z} });
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

    // mTransformMatrix.Translation(glm::vec3(center[0], center[1], center[2]));

    // std::cout << "Processing indices..." << std::endl;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            mIndices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    mVBO.FillBuffer(mVertices.data(), mVertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    mIBO.FillBuffer(mIndices.data(), mIndices.size(), GL_STATIC_DRAW);
    */
}
