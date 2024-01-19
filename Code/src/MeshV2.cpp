#include "MeshV2.h"

#include <glm/gtc/matrix_transform.hpp>

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

    ParseHierarchy(pScene);
}

void MeshV2::ParseHierarchy(const aiScene* pScene)
{
    ParseNode(pScene->mRootNode);
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

    PrintAssimpMatrix(pBone->mOffsetMatrix);

    if (boneId == mBoneInfo.size())
    {
        BoneInfo info(pBone->mOffsetMatrix);
        mBoneInfo.push_back(info);
    }
    
    for (int j = 0; j < pBone->mNumWeights; j++)
    {
        if (j == 0)
            printf("\n");

        const aiVertexWeight& vWeight = pBone->mWeights[j];

        unsigned int globalVertexID = mMeshBaseVector[meshIndex] + vWeight.mVertexId;
        
        if (globalVertexID >= mVertexToBonesVector.size())
            Debug::ThrowException("globalVertexID out of range!");

        mVertexToBonesVector[globalVertexID].AddBoneData(boneId, vWeight.mWeight);
    }
    
}

void MeshV2::ParseNode(const aiNode* pNode)
{
    printf(" Node name: '%s' num children %d num meshes %d\n", pNode->mName.C_Str(), pNode->mNumChildren, pNode->mNumMeshes);
    printf(" Node transformation\n");
    PrintAssimpMatrix(pNode->mTransformation);

    for (int j = 0; j < pNode->mNumChildren; j++)
    {
        printf(" --- %d ---\n", j);
        ParseNode(pNode->mChildren[j]);
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

void MeshV2::ReadNodeHeirarchy(const aiNode* pNode, const aiMatrix4x4& parentTransform)
{
    std::string nodeName(pNode->mName.C_Str());

    aiMatrix4x4 nodeTransformation(pNode->mTransformation);

    // printf("%s - \n", nodeName.c_str());

    aiMatrix4x4 globalTransformation = parentTransform * nodeTransformation;

    if (mBoneNameToIndexMap.find(nodeName) != mBoneNameToIndexMap.end())
    {
        unsigned int boneIndex = mBoneNameToIndexMap[nodeName];
        mBoneInfo[boneIndex].mFinalTransformation = globalTransformation * mBoneInfo[boneIndex].mOffsetMatrix;
    }

    for (int j = 0; j < pNode->mNumChildren; j++)
    {
        ReadNodeHeirarchy(pNode->mChildren[j], globalTransformation);
    }
}

void MeshV2::Init(const std::string& filePath)
{
    mPScene = mImporter.ReadFile(filePath.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GenNormals);
    
    if (mPScene == nullptr)
    {
        Debug::ThrowException("Unable to import from defined file! (" + filePath + ")");
    }
    
    ParseScene(mPScene);

    PrintAnimations(mPScene);

    auto mesh = mPScene->mMeshes[0];

    mVertices.reserve(mesh->mNumVertices);
    mIndices.reserve(mesh->mNumFaces * 3);

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

    // std::cout << "Processing indices..." << std::endl;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            mIndices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    void* ptr = (void*)(mVertices.data());

    for (int i = 0; i <= sizeof(VertexV2) / sizeof(float); i++)
        printf("Value: %f\n", *((float*)(ptr) + i));


    ConfigureVAOLayout();

    mVBO.FillBuffer(mVertices.data(), mVertices.size() * sizeof(VertexV2), GL_STATIC_DRAW);
    mIBO.FillBuffer(mIndices.data(), mIndices.size(), GL_STATIC_DRAW);

    mVBO.Bind<VertexV2>(0);
    mVAO.AddBuffer(mVBO, mIBO);
}

 void MeshV2::GetBoneTransforms(std::vector<aiMatrix4x4>& transforms)
{
    transforms.resize(mBoneInfo.size());

    aiMatrix4x4 Identity;
    
    ReadNodeHeirarchy(mPScene->mRootNode, Identity);

    for (int j = 0; j < mBoneInfo.size(); j++)
    {
        transforms[j] = mBoneInfo[j].mFinalTransformation;
    }
}

void MeshV2::PrintAnimations(const aiScene* pScene)
{
    if (pScene->HasAnimations())
    {
        for (int j = 0; j < pScene->mNumAnimations; j++)
        {
            Debug::Print(pScene->mAnimations[j]->mName.C_Str());
        }
    }
}

void MeshV2::PrintAssimpMatrix(const aiMatrix4x4& matrix)
{
    printf(" %f %f %f %f\n", matrix.a1, matrix.a2, matrix.a3, matrix.a4);
    printf(" %f %f %f %f\n", matrix.b1, matrix.b2, matrix.b3, matrix.b4);
    printf(" %f %f %f %f\n", matrix.c1, matrix.c2, matrix.c3, matrix.c4);
    printf(" %f %f %f %f\n", matrix.d1, matrix.d2, matrix.d3, matrix.d4);
}

void MeshV2::ConfigureVAOLayout()
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