#include "VegetationController.h"
#include "TerrainController.h"

#include "TinyXML\tinyxml.h"

VegetationController::VegetationController(LuckyLeprechauns* game) : LLDrawableGameComponent(game)
{
}


void VegetationController::init()
{
	TerrainController& terrain = ((LuckyLeprechauns&)getManager()).getTerrain();

	std::string path = Config::getValue(LLConfigKeys::vegetationConfigPath);

	TiXmlDocument doc;
	doc.LoadFile(path.c_str());

	TiXmlElement* root = doc.FirstChildElement("Vegetation");
	TiXmlElement* object = root->FirstChildElement("Object");

	while (object)
	{
		VegetationObject vo;

		vo.modelPath = object->Attribute("modelPath");
		vo.alphaCutoff = convert(object->Attribute("alphaCutoff"));

		TiXmlElement* transform = object->FirstChildElement("Transform");

		while (transform)
		{
			const char* translateX = transform->Attribute("translateX");
			const char* translateZ = transform->Attribute("translateZ");
			const char* rotateY = transform->Attribute("rotateY");
			const char* scale = transform->Attribute("scale");

			Vector3 translate = terrain.getHeight(Vector3(convert(translateX), 0, convert(translateZ)));
			Vector3 rotate = Vector3(0, convert(rotateY), 0);
			float scaling = convert(scale);

			vo.transforms.push_back(
				Matrix::createScale(scaling) * 
				Matrix::createRotation(rotate) * 
				Matrix::createTranslation(translate));

			transform = transform->NextSiblingElement();
		}

		vegetation.push_back(vo);

		object = object->NextSiblingElement();
	}
}


float VegetationController::convert(const std::string& s)
{
	return Converter::convert<float>(s);
}


const VegetationController::Vegetation& VegetationController::getVegetation() const
{
	return vegetation;
}