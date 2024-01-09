#include "Debug.h"



void Debug::Print(const std::string msg)
{
	std::cout << "DEBUG: " << msg << std::endl;
}

void Debug::ThrowException(const std::string msg)
{
	// temporary!
	std::cout << "DEBUG EXCEPTION: " << msg << std::endl;
	throw false && msg.c_str();
}
