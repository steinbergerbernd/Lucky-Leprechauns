#include "LLGameSound.h"
#include "LLSoundManager.h"

LLGameSound::LLGameSound(LLSoundManager* manager, LLGameLogic* controller) : SoundComponent(manager, controller), backgroundPlay(false)
{
}


void LLGameSound::init()
{
	SoundComponent::init();

	loadSoundBank(Config::getValue(LLConfigKeys::soundGameSoundPath));
	toggleBackgroundMusic();
}


void LLGameSound::update(const GameTime& gameTime)
{
	SoundComponent::update(gameTime);
	
	const SoundManager::AudioListener& listener = getManager().getListener();
	setEmitter((Vector3)listener.Position, (Vector3)listener.OrientFront, (Vector3)listener.OrientTop);

	LLGameLogic& gameLogic = getController();
	const LLGameLogic::ExplosionList& explosions = gameLogic.getExplosions();
	for(LLGameLogic::ExplosionList::const_iterator it = explosions.begin(); it != explosions.end(); ++it)
		playStatic3DSound("explosion", *it);
	gameLogic.clearExplosions();
}


void LLGameSound::toggleBackgroundMusic()
{
	backgroundPlay = !backgroundPlay;
	backgroundPlay ? playSound("Cry of the celts") : stopSound("Cry of the celts");
}