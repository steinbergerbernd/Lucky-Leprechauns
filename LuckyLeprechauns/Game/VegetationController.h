#pragma once

#include "LLDrawableGameComponent.h"

class VegetationController : public LLDrawableGameComponent
{
	struct VegetationObject
	{
		std::string modelPath;
		float alphaCutoff;
		std::vector<Matrix> transforms;
	};

public:
	typedef std::vector<VegetationObject> Vegetation;

	VegetationController(LuckyLeprechauns* game);

	void init();

	const Vegetation& getVegetation() const;

private:
	Vegetation vegetation;

	float convert(const std::string& s);
};