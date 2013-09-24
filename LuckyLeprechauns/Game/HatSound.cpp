#include "HatSound.h"
#include "LLSoundManager.h"

HatSound::HatSound(LLSoundManager* manager, HatController* controller) : SoundComponent(manager, controller)
{
}


void HatSound::init()
{
	SoundComponent::init();

	loadSoundBank(Config::getValue(LLConfigKeys::soundHatSoundPath));
}


void HatSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);

	HatController& hat = getController();
	setEmitter(hat.getPosition(), hat.getForward(), Vector3::up);

	if (hat.isLanded())
		play3DSound("flop");
}