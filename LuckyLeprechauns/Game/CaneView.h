#pragma once

#include "LLControlledGraphicsComponent.h"
#include "CaneController.h"

class CaneView : public LLControlledGraphicsComponent<CaneController>
{
public:
	CaneView(LLGraphicsManager* manager, CaneController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};