#pragma once
#include "SoundComponent.h"
#include "HatController.h"

class LLSoundManager;

class HatSound : public SoundComponent<HatController>
{
public:
	HatSound(LLSoundManager* manager, HatController* controller);

	void init();
	void update(const GameTime& gameTime);
};

