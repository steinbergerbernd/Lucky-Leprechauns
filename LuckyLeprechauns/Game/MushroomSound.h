#pragma once
#include "SoundComponent.h"
#include "MushroomController.h"

class LLSoundManager;

class MushroomSound :
	public SoundComponent<MushroomController>
{
public:
	MushroomSound(LLSoundManager* manager, MushroomController* controller);
	
	void init();
	void update(const GameTime& gameTime);
};

