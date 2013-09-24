#pragma once

#include <fstream>
#include <sstream>

#include "Config.h"

class Logging
{
public:
	static void log(const std::string& text);
	static void init();

	static const std::string& getPath();
	static void setPath(const std::string& path);
private:
	static std::fstream file;
	static std::string path;
};