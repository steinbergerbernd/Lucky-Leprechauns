#pragma once

#include "LLControlledGraphicsComponent.h"
#include "VegetationController.h"

class VegetationView : public LLControlledGraphicsComponent<VegetationController>
{
	struct VegetationObject
	{
		FBXModel model;
		std::vector<Matrix> transforms;
	};

	typedef std::vector<VegetationObject> Vegetation;

public:
	VegetationView(GraphicsManager* graphicsManager, VegetationController* controller);

	void init();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

	void onResetDevice(const GraphicsDevice& device);
	void onLostDevice();

private:
	Vegetation vegetation;
};