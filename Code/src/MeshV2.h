#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "Debug.h"
#include "Transform.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define MAX_NUM_OF_BONES_PER_VERTEX 8 // for the mixamo rig, 6 is enough, but i made it pretty flexible
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct VertexBoneData
{
	unsigned int mBoneIDs[MAX_NUM_OF_BONES_PER_VERTEX] = { 0 };
	float mWeights[MAX_NUM_OF_BONES_PER_VERTEX] = { 0.0f };

	VertexBoneData()
	{
	}

	void AddBoneData(const unsigned int& boneID, const float& weight)
	{
		for (unsigned int j = 0; j < ARRAY_SIZE_IN_ELEMENTS(mBoneIDs); j++)
		{
			if (mWeights[j] == 0.0f)
			{
				mBoneIDs[j] = boneID;
				mWeights[j] = weight;
				// printf("bone %d weight %f index %i\n", boneID, weight, j);
				return;
			}
		}

		Debug::ThrowException("All bone spots taken!");
	}
};

struct BoneInfo
{
	aiMatrix4x4 mOffsetMatrix;
	aiMatrix4x4 mFinalTransformation;

	BoneInfo(const aiMatrix4x4& offset)
	{
		mOffsetMatrix = offset;
		mFinalTransformation = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	}
};

struct VertexV2
{
	glm::vec3 mPos;
	glm::vec3 mNormal;
	VertexBoneData mBoneData;
};

class MeshV2
{
public:

	MeshV2();
	MeshV2(const std::string& filePath);

	void Initialize();

	const std::string& GetFilePath() const;
	void SetFilePath(const std::string& filePath);

	void Draw(Shader& shader);

	void Init(const std::string& filePath);
	void LoadMesh(aiMesh* mesh);

	void SelectNextAnimation();

	Transform& GetTransform();
	void GetBoneTransforms(const double& timeInSeconds, std::vector<aiMatrix4x4>& transforms);

private:

	void ParseScene(const aiScene* pScene);
	void ParseHierarchy(const aiScene* pScene);

	void ParseMeshes(const aiScene* pScene);
	void ParseMeshBones(const unsigned int& meshIndex, const aiMesh* pMesh);
	void ParseSingleBone(const unsigned int& meshIndex, const aiBone* pBone);

	void ParseNode(const aiNode* pNode);

	void CalculateInterpolatedScaling(aiVector3D& scaling, const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);

	void CalculateInterpolatedRotation(aiQuaternion& rotationQ, const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);

	void CalculateInterpolatedPosition(aiVector3D& translation, const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(const float& animationTimeTicks, const aiNodeAnim* pNodeAnim);

	int GetBoneID(const aiBone* pBone);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& nodeName);
	void ReadNodeHeirarchy(const float& animationTimeTicks, const aiNode* pNode, const aiMatrix4x4& parentTransform);

	void PrintAnimations(const aiScene* pScene);
	void PrintAssimpMatrix(const aiMatrix4x4& matrix);

	void ConfigureVAOLayout();



	std::string mFilePath;

	std::vector<VertexBoneData> mVertexToBonesVector; // mapping from vertex to bones (which bones affect a certain vertex)
	std::vector<int> mMeshBaseVector; // Offset for each mesh (when there are more than one mesh for a model)
	std::map<std::string, unsigned int> mBoneNameToIndexMap; // Mapping bone name to its index

	VertexArray mVAO;
	VertexBuffer mVBO;
	IndexBuffer mIBO;

	Transform mTransform;
	aiMatrix4x4 mGlobalInverseTransform;

	unsigned int mActiveAnimation = 0;

	std::vector<unsigned int> mIndices;
	std::vector<VertexV2> mVertices;
	std::vector<BoneInfo> mBoneInfo; // one struct per bone

	Assimp::Importer mImporter;
	const aiScene* mPScene = nullptr;

};