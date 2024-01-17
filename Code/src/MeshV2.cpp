#include "MeshV2.h"

#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

MeshV2::MeshV2()
{
}

MeshV2::MeshV2(const std::string& filePath)
	:
	mFilePath(filePath)
{
    Initialize();
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

    mMeshBaseVector.resize(pScene->mNumMeshes);

    for (int j = 0; j < pScene->mNumMeshes; j++)
        total_verts += pScene->mMeshes[j]->mNumVertices;

    mVertexToBonesVector.resize(total_verts);
    total_verts = 0;

    for (int j = 0; j < pScene->mNumMeshes; j++)
    {
        const aiMesh* pMesh = pScene->mMeshes[j];
        printf("Mesh %d '%s': Vertoces %d indices %d bones %d\n\n", j, pMesh->mName.C_Str(), pMesh->mNumVertices, pMesh->mNumFaces * 3, pMesh->mNumBones);

        mMeshBaseVector[j] = total_verts;

        total_indices += pMesh->mNumFaces * 3;
        total_bones += pMesh->mNumBones;
        total_verts += pMesh->mNumVertices;

        if (pMesh->HasBones())
            ParseMeshBones(j, pMesh);

        printf("\n");
    }

    printf("\n Total vertices %d total indices %d total bones %d\n", total_verts, total_indices, total_bones);
}

void MeshV2::ParseMeshBones(const unsigned int& meshIndex, const aiMesh* pMesh)
{
    for (int j = 0; j < pMesh->mNumBones; j++)
    {
        ParseSingleBone(meshIndex, pMesh->mBones[j]);
    }
}

void MeshV2::ParseSingleBone(const unsigned int& meshIndex, const aiBone* pBone)
{
    printf("\tBone '%s' => num vertices affected by this bone: %d\n", pBone->mName.C_Str(), pBone->mNumWeights);

    int boneId = GetBoneID(pBone);
    printf("bone id %d\n", boneId);

    
    for (int j = 0; j < pBone->mNumWeights; j++)
    {
        if (j == 0)
            printf("\n");

        const aiVertexWeight& vWeight = pBone->mWeights[j];

        unsigned int globalVertexID = mMeshBaseVector[meshIndex] + vWeight.mVertexId;
        // printf("Vertex id %d ", globalVertexID);
        
        if (globalVertexID >= mVertexToBonesVector.size())
            Debug::ThrowException("globalVertexID out of range!");

        mVertexToBonesVector[globalVertexID].AddBoneData(boneId, vWeight.mWeight);
    }
    
}

int MeshV2::GetBoneID(const aiBone* pBone)
{
    int boneID = 0;
    std::string boneName(pBone->mName.C_Str());

    if (mBoneNameToIndexMap.find(boneName) == mBoneNameToIndexMap.end())
    {
        boneID = mBoneNameToIndexMap.size();
        mBoneNameToIndexMap[boneName] = boneID;
    }
    else
    {
        boneID = mBoneNameToIndexMap[boneName];
    }

    return boneID;
}

void MeshV2::Init(const std::string& filePath)
{
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    if (MAX_NUM_OF_BONES_PER_VERTEX > 4)
    {
        int numOfBones;
        for (int j = MAX_NUM_OF_BONES_PER_VERTEX; j > 0; j -= 4)
        {
            numOfBones = (j >= 4) ? 4 : j;

            layout.Push<unsigned int>(numOfBones);
        }
        for (int j = MAX_NUM_OF_BONES_PER_VERTEX; j > 0; j -= 4)
        {
            numOfBones = (j >= 4) ? 4 : j;

            layout.Push<float>(numOfBones);
        }
    }
    else
    {
        layout.Push<unsigned int>(MAX_NUM_OF_BONES_PER_VERTEX);
        layout.Push<float>(MAX_NUM_OF_BONES_PER_VERTEX);
    }

    mVAO.Bind();
    mVAO.SetLayout(layout, false);
    mVAO.SetDrawingMode(GL_TRIANGLES);
    mVAO.SetUsage(GL_STATIC_DRAW);

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

    mVertices.reserve(mesh->mNumVertices);
    mIndices.reserve(mesh->mNumFaces * 3);

    // std::cout << "Processing vertices...(" << mesh->mNumVertices << ")" << std::endl;

    struct BoundingBox
    {
        double min = 100000.0;
        double max = -10000.0;
    } mBoundingBox[3];

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        mVertices.push_back({ {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}, {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}, {mVertexToBonesVector[i]}});
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
    mTransform.Scale(glm::vec3(scaleVal, scaleVal, scaleVal));

    // mTransformMatrix.Translation(glm::vec3(center[0], center[1], center[2]));

    // std::cout << "Processing indices..." << std::endl;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            mIndices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    mVBO.FillBuffer(mVertices.data(), mVertices.size() * sizeof(VertexV2), GL_STATIC_DRAW);
    mIBO.FillBuffer(mIndices.data(), mIndices.size(), GL_STATIC_DRAW);

    mVBO.Bind<VertexV2>(0);
    mVAO.AddBuffer(mVBO, mIBO);
}

void MeshV2::Draw(Shader& shader)
{
    shader.Bind();
    shader.SetUniformMatrix4f("model", mTransform.GetMatrix());
    

    mVAO.Bind();
    mVBO.Bind<VertexV2>(0);
    mIBO.Bind();

    glDrawElements(mVAO.GetDrawingMode(), mIBO.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}