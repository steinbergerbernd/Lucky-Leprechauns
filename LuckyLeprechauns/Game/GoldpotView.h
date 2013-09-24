#pragma once
#include "LLControlledGraphicsComponent.h"
#include "GoldpotController.h"

class GoldpotView :
	public LLControlledGraphicsComponent<GoldpotController>
{
public:
	GoldpotView(LLGraphicsManager* manager, GoldpotController* controller);

	void init();
	void draw(const GameTime& gameTime);
private:
	FBXModel model;
};

