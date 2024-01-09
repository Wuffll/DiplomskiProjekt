#pragma once

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <string>

#define STRING(value) (std::to_string(value))
#define GLM_TOSTRING(value) (glm::to_string(value))

class Debug
{
public:
	template<typename T>
	static std::string GlmString(const T& glmVariable);
	static void Print(const std::string msg);
	static void ThrowException(const std::string msg);

private:



};

template<typename T>
static std::string Debug::GlmString(const T& glmVariable)
{
	return glm::to_string(glmVariable);
}