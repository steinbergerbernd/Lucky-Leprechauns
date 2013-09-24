#pragma once
#include "SoundComponent.h"
#include "LeprechaunController.h"

class LLSoundManager;
class CaneSound;

class LeprechaunSound : public SoundComponent<LeprechaunController>
{
public:
	LeprechaunSound(LLSoundManager* manager, LeprechaunController* controller);

	virtual void init();
	virtual void update(const GameTime& gameTime);

private:
	CaneSound* cane;
};

