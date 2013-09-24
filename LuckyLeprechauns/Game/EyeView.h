#pragma once

#include "LLControlledGraphicsComponent.h"
#include "LeprechaunController.h"

class EyeView : public LLControlledGraphicsComponent<LeprechaunController>
{
public:
	EyeView(LLGraphicsManager* manager, LeprechaunController* controller);

	void init();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

private:
	FBXModel model;

	Light lightCopy;
	bool specialLight;
};