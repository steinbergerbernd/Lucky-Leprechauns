#pragma once

#include "LLControlledGraphicsComponent.h"
#include "HatController.h"

class HatView : public LLControlledGraphicsComponent<HatController>
{
public:
	HatView(LLGraphicsManager* manager, HatController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};