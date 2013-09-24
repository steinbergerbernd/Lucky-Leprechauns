#pragma once

#include "LLControlledGraphicsComponent.h"
#include "StoneController.h"

class StoneView : public LLControlledGraphicsComponent<StoneController>
{
public:
	StoneView(LLGraphicsManager* manager, const StoneController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};