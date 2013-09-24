#pragma once
#include "LLControlledGraphicsComponent.h"
#include "LLGraphicsManager.h"
#include "PigController.h"

#include "FBXModel.h"

class PigView : public LLControlledGraphicsComponent<PigController>
{
public:
	PigView(LLGraphicsManager* manager, PigController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;
};

