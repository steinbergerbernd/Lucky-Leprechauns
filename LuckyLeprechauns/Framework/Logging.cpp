#include "Logging.h"


std::fstream Logging::file;
std::string Logging::path = "Log.txt";


void Logging::init()
{
	path = Config::getValue<std::string>(ConfigKeys::logPath);
}


void Logging::log(const std::string& text)
{	
	file.open(path, std::ios::app);
	file << text.c_str() << std::endl;
	file.close();
}


const std::string& Logging::getPath()
{
	return path;
}


void Logging::setPath(const std::string& path)
{
	Logging::path = path;
}
