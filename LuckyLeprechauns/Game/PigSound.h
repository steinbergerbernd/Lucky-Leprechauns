#pragma once
#include "SoundComponent.h"
#include "PigController.h"

class LLSoundManager;

class PigSound :
	public SoundComponent<PigController>
{
public:
	PigSound(LLSoundManager* manager, PigController* controller);

	void init();
	void update(const GameTime& gameTime);
};

