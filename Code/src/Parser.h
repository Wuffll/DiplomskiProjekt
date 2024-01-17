#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

class Parser
{
public:

	static void ReadFile(const std::string& filePath, std::vector<glm::vec3>& storeVector);

private:

	static void StringSplit(std::string& str, const unsigned int& count,const std::string& delimiter, std::vector<std::string>& output);

};