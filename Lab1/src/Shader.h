#pragma once

#include <string>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

struct ShaderProgramSource
{
	std::string Vertex{};
	std::string Fragment{};
	std::string Geometry{};
};

enum ShaderType
{
	UNDEFINED = -1,
	VERT = 0,
	FRAG = 1,
	GEOM = 2
};

class Shader
{
public:

	Shader(const std::string& filePath);

	void Bind() const;
	void Unbind() const;

	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniform4fv(const std::string& name, const std::vector<float>& vec4f);

	~Shader();

private:
	
	static unsigned int ActiveShader; // careful if implementing multithreading (mutex needed)

	void Init(const std::string& filePath);

	ShaderProgramSource ReadShaderFile(const std::string& filePath);
	unsigned int CompileShader(unsigned int shaderType, const std::string& source);

	int GetUniformLocation(const std::string& name);

	unsigned int mRendererID = 0;
	std::unordered_map<std::string, int> mUniformLocationCache{};
	std::string mFilePath{};

};