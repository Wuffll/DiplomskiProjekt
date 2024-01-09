#include "Parser.h"

#include <fstream>

#include "Debug.h"

void Parser::ReadFile(const std::string& filePath, std::vector<glm::vec3>& storeVector)
{
	std::ifstream file(filePath);

	if (!file.is_open())
		Debug::ThrowException("Can't open file that is supposed to be parsed!");

	std::string line;

	int i = -1;

	glm::vec3 p;
	std::vector<std::string> lineSplit;

	while (std::getline(file, line))
	{
		if (i == -1)
		{
			i = atoi(line.c_str());
			storeVector.reserve(i);
		}
		else
		{
			StringSplit(line, 3, " ", lineSplit);

			for (i = 0; i < 3; i++)
			{
				p[i] = strtof(lineSplit[i].c_str(), NULL);
			}

			storeVector.push_back(p);

			lineSplit.clear();
		}
	}
}

void Parser::StringSplit(std::string& str, const unsigned int& count, const std::string& delimiter, std::vector<std::string>& output)
{
	size_t pos = 0;
	std::string token;

	output.reserve(count);

	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		output.push_back(token);
		// std::cout << token << std::endl;
		str.erase(0, pos + delimiter.length());
	}

	output.push_back(str);
}
