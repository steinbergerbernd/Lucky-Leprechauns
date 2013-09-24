#pragma once
#include "SoundComponent.h"
#include "LLGameLogic.h"

class LLSoundManager;

class LLGameSound : public SoundComponent<LLGameLogic>
{
public:
	LLGameSound(LLSoundManager* manager, LLGameLogic* controller);

	void init();
	void update(const GameTime& gameTime);

	void toggleBackgroundMusic();

private:
	bool backgroundPlay;
};

