#pragma once

#include "STL.h"
#include "FBXModel.h"
#include "GraphicsDevice.h"
#include "Config.h"
#include "TinyXML\tinyxml.h"

class FBXModelLoader
{
	typedef std::map<std::string, FBXModel> ModelMap;

public:
	static void init(const std::string& path, const GraphicsDevice& device);
	static void release();

	static FBXModel getModelClone(const std::string& name);

	static void onResetDevice(const GraphicsDevice& device);
	static void onLostDevice();
	
private:
	static TiXmlDocument doc;

	static const std::string xmlTrue;
	static const std::string xmlFalse;

	static ModelMap models;
};