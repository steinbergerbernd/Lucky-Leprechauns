#include "FBXModelLoader.h"

const std::string FBXModelLoader::xmlTrue = "true";
const std::string FBXModelLoader::xmlFalse = "false";
TiXmlDocument FBXModelLoader::doc;
FBXModelLoader::ModelMap FBXModelLoader::models;

void FBXModelLoader::init(const std::string& path, const GraphicsDevice& device)
{
	doc.LoadFile(path.c_str());

	TiXmlElement* xmlModel = doc.FirstChildElement("Models")->FirstChildElement();

	while (xmlModel)
	{
		const char* name = xmlModel->Value();
		const char* path = xmlModel->Attribute("path");
		const char* keyframes = xmlModel->Attribute("keyframes");

		unsigned kf = (keyframes) ? Converter::convert<unsigned>(keyframes) : 0;

		FBXModel model = FBXModel::createFromFile(device, path, kf);

		TiXmlElement* animation = xmlModel->FirstChildElement("Animation");

		while (animation)
		{
			const char* name = animation->Attribute("name");

			FBXAnimation fbxAnimation;

			fbxAnimation.name = name;
			fbxAnimation.start = Converter::convert<unsigned>(animation->Attribute("start"));
			fbxAnimation.length = Converter::convert<unsigned>(animation->Attribute("length"));
			fbxAnimation.originalFps = Converter::convert<unsigned>(animation->Attribute("fps"));
			fbxAnimation.currentFps = fbxAnimation.originalFps;
			fbxAnimation.action = animation->Attribute("action") ? Converter::convert<unsigned>(animation->Attribute("action")) : -1;
			fbxAnimation.priority = Converter::convert<unsigned>(animation->Attribute("priority"));

			model.addAnimation(name, fbxAnimation);

			animation = animation->NextSiblingElement();
		}

		models[name] = model;

		xmlModel = xmlModel->NextSiblingElement();
	}
}


void FBXModelLoader::release()
{
	for (ModelMap::iterator it = models.begin(); it != models.end(); ++it)
		it->second.unload();
}


void FBXModelLoader::onResetDevice(const GraphicsDevice& device)
{
	for (ModelMap::iterator it = models.begin(); it != models.end(); ++it)
		it->second.onResetDevice(device);
}


void FBXModelLoader::onLostDevice()
{
	for (ModelMap::iterator it = models.begin(); it != models.end(); ++it)
		it->second.onLostDevice();
}


FBXModel FBXModelLoader::getModelClone(const std::string& name)
{
	ModelMap::iterator it = models.find(name);

	if (it == models.end())
		throw Exception(Exceptions::invalidArgument);

	return it->second;
}