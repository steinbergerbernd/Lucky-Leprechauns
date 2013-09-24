#pragma once

#include "LLControlledGraphicsComponent.h"
#include "RainbowController.h"

class RainbowView : public LLControlledGraphicsComponent<RainbowController>
{
public:
	RainbowView(LLGraphicsManager* manager, RainbowController* controller);

	void init();
	void draw(const GameTime& gameTime);

	void onLostDevice();
	void onResetDevice(const GraphicsDevice& device);

private:
	FBXModel model;
	FBXEffect effect;
};