#include "Shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <fstream>

#include "Debug.h"

unsigned int Shader::ActiveShader = 0;

Shader::Shader(const std::string& filePath)
	:
	mFilePath(filePath)
{
	Init(filePath);
}

void Shader::Bind() const
{
	if (mRendererID == 0)
		Debug::ThrowException("ERROR: Shader mRendererID not set or shader not initiated!!!");

	if(ActiveShader != mRendererID)
		glUseProgram(mRendererID);

	ActiveShader = mRendererID;
}

void Shader::Unbind() const
{
	glUseProgram(0);

	ActiveShader = 0;
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	Bind();

	int location = GetUniformLocation(name);

	if (location == -1)
		return;

	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	Bind();

	int location = GetUniformLocation(name);

	if (location == -1)
		return;

	glUniform4f(location, f0, f1, f2, f3);
}

void Shader::SetUniform4fv(const std::string& name, const std::vector<float>& vec4f)
{
	Bind();

	int location = GetUniformLocation(name);

	if (location == -1)
		return;

	glUniform4fv(location, 4, vec4f.data());
}

Shader::~Shader()
{
	glDeleteProgram(mRendererID);
}

void Shader::Init(const std::string& filePath)
{
	ShaderProgramSource source = ReadShaderFile(filePath);

	mRendererID = glCreateProgram();
	unsigned int shader;
	std::vector<unsigned int> shaders{};

	if (source.Vertex.size() != 0)
	{
		shader = CompileShader(GL_VERTEX_SHADER, source.Vertex);
		glAttachShader(mRendererID, shader);
		shaders.push_back(shader);
	}

	if (source.Fragment.size() != 0)
	{
		shader = CompileShader(GL_FRAGMENT_SHADER, source.Fragment);
		glAttachShader(mRendererID, shader);
		shaders.push_back(shader);
	}

	if (source.Geometry.size() != 0)
	{
		shader = CompileShader(GL_GEOMETRY_SHADER, source.Geometry);
		glAttachShader(mRendererID, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(mRendererID);
	glValidateProgram(mRendererID);

	for (auto& s : shaders)
	{
		glDeleteShader(s);
	}
}

ShaderProgramSource Shader::ReadShaderFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
		throw false && "File was not able to be opened!";


	ShaderType type = ShaderType::UNDEFINED;
	ShaderProgramSource source;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("VERT") != std::string::npos)
				type = ShaderType::VERT;
			else if (line.find("FRAG") != std::string::npos)
				type = ShaderType::FRAG;
			else if (line.find("GEOM") != std::string::npos)
				type = ShaderType::GEOM;
		}
		else
		{
			switch (type)
			{
				case VERT:
					source.Vertex.append(line + "\n");
					break;
				case FRAG:
					source.Fragment.append(line + "\n");
					break;
				case GEOM:
					source.Geometry.append(line + "\n");
					break;
			}
		}
	}

	return source;
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& source)
{
	unsigned int id = glCreateShader(shaderType);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result, length;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, &msg[0]);

		std::cout << "Failed to compile shader! (shader = " << shaderType << ")" << std::endl;
		std::cout << msg << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
		return mUniformLocationCache[name];

	int location = glGetUniformLocation(mRendererID, name.c_str());

	if (location == -1)
	{
		std::cout << "There is no uniform with name \"" << name << "\"! (mRendererID = " << mRendererID << "; check whether the uniform is used in GLSL shader)" << std::endl;
		return location;
	}

	mUniformLocationCache[name] = location;
	return location;
}
