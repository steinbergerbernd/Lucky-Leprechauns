#include "CaneSound.h"
#include "LLSoundManager.h"
#include "LeprechaunController.h"

CaneSound::CaneSound(LLSoundManager* manager, CaneController* controller) : SoundComponent(manager, controller)
{
}


void CaneSound::init()
{
	SoundComponent::init();

	loadSoundBank(Config::getValue(LLConfigKeys::soundCaneSoundPath));
}


void CaneSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);

	CaneController& cane = getController();
	setEmitter(cane.getPosition(), cane.getForward(), Vector3::up);

	if (cane.getLeprechaun().isBeginCane())
		play3DSound("cane throw");
	else if(!cane.isEnabled())
		stopSound("cane throw");
}