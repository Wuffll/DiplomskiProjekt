#pragma once

#include <string>

class aiScene;
class aiMesh;
class aiBone;

class MeshV2
{
public:

	MeshV2();
	MeshV2(const std::string& filePath);

	void Initialize();

	const std::string& GetFilePath() const;
	void SetFilePath(const std::string& filePath);

	void ParseScene(const aiScene* pScene);
	void ParseMeshes(const aiScene* pScene);
	void ParseMeshBones(const aiMesh* pMesh);
	void ParseSingleBone(const unsigned int& boneIndex, const aiBone* pBone);

	void Init(const std::string& filePath);

private:

	std::string mFilePath;

};