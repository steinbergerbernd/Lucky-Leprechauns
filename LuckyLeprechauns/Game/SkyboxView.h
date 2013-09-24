#pragma once

#include "LLGraphicsComponent.h"

class SkyboxView : public LLGraphicsComponent
{
public:
	SkyboxView(GraphicsManager* graphicsManager);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};