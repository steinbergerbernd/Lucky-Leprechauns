#pragma once

#include "LLControlledGraphicsComponent.h"
#include "LeprechaunController.h"

class BubbleView : public LLControlledGraphicsComponent<LeprechaunController>
{
public:
	BubbleView(LLGraphicsManager* manager, LeprechaunController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};