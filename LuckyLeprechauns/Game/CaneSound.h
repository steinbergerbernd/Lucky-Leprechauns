#pragma once
#include "SoundComponent.h"
#include "CaneController.h"

class LLSoundManager;

class CaneSound : public SoundComponent<CaneController>
{
public:
	CaneSound(LLSoundManager* manager, CaneController* controller);

	void init();
	void update(const GameTime& gameTime);
};

