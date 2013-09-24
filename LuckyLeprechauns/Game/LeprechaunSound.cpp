#include "LeprechaunSound.h"
#include "LLSoundManager.h"
#include "CaneSound.h"
#include "HatSound.h"

LeprechaunSound::LeprechaunSound(LLSoundManager* manager, LeprechaunController* controller) : SoundComponent(manager, controller)
{
	manager->addComponent<CaneSound>(manager, &controller->getCane());
	manager->addComponent<HatSound>(manager, &controller->getHat());
}


void LeprechaunSound::init()
{
	SoundComponent::init();

	loadSoundBank(Config::getValue(LLConfigKeys::soundLeprechaunSoundPath));
}


void LeprechaunSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);

	LeprechaunController& leprechaun = getController();
	setEmitter(leprechaun.getPosition(), leprechaun.getForward(), Vector3::up);

	if (leprechaun.isBeginKick() || leprechaun.isBeginCaneAnimation() || leprechaun.isBeginHatAnimation())
		play3DSound("hey");

	if (leprechaun.isBeginCaneHit() || leprechaun.isBeginFly() || leprechaun.isBeginStoneHit())
		play3DSound("punch");

	if (!leprechaun.isLocal())
		return;

	if (leprechaun.hasUsedItem())
	{
		stopSound("yes hahaha");
		playSound("yes hahaha");
		//play3DSound("yes hahaha");
	}

	if (leprechaun.hasCollectedItem())
	{
		stopSound("door slamming");
		playSound("door slamming");
		//play3DSound("door slamming");
	}
}