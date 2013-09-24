#include "PigSound.h"
#include "LLSoundManager.h"

PigSound::PigSound(LLSoundManager* manager, PigController* controller) : SoundComponent(manager, controller)
{
}


void PigSound::init()
{
	SoundComponent::init();
	
	loadSoundBank(Config::getValue(LLConfigKeys::soundPigSoundPath));
}


void PigSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);

	PigController& pig = getController();
	setEmitter(pig.getPosition(), pig.getForward(), Vector3::up);

	if (pig.isLost() || pig.isCaptured())
		play3DSound("pig squeak");

	if (pig.hasStartedMoving())
		play3DSound("pig mix");
	else if (pig.hasStoppedMoving())
		stopSound("pig mix");
}