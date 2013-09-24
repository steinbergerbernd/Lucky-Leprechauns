#include "MushroomSound.h"
#include "LLSoundManager.h"

MushroomSound::MushroomSound(LLSoundManager* manager, MushroomController* controller) : SoundComponent(manager, controller)
{
}


void MushroomSound::init()
{
	SoundComponent::init();

	loadSoundBank(Config::getValue(LLConfigKeys::soundMushroomSoundPath));
}


void MushroomSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);

	MushroomController& mushroom = getController();
	setEmitter(mushroom.getPosition(), mushroom.getForward(), Vector3::up);

	if (mushroom.isOvertaken())
		play3DSound("yummy");
}