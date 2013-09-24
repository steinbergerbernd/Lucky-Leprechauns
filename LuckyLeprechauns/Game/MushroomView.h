#pragma once

#include "LLControlledGraphicsComponent.h"
#include "MushroomController.h"

#include "StoneView.h"

class MushroomView : public LLControlledGraphicsComponent<MushroomController>
{
public:
	MushroomView(LLGraphicsManager* manager, const MushroomController* controller);

	void init();
	void draw(const GameTime& gameTime);

private:
	FBXModel model;

	StoneView* stone;

	std::vector<Texture> playerTextures;
	Texture neutralTexture;
};