#include "LLSoundManager.h"
#include "LuckyLeprechauns.h"
#include "PlayerController.h"
#include "LeprechaunSound.h"
#include "LLGameSound.h"
#include "PigSound.h"
#include "MushroomSound.h"

LLSoundManager::LLSoundManager(LuckyLeprechauns* game) : SoundManager(game)
{
	gameSound = addComponent<LLGameSound>(this, &game->getGameLogic());
	leprechaun = addComponent<LeprechaunSound>(this, &game->getPlayer().getLeprechaun());
	pig = addComponent<PigSound>(this, &game->getPig());

	for (ComponentCollection::iterator it = game->remotePlayerPool.begin(); it != game->remotePlayerPool.end(); ++it)
		remotePlayerPool.push_back(addComponent<LeprechaunSound>(this, &((PlayerController*)(*it))->getLeprechaun(), false, false));

	for (LuckyLeprechauns::MushroomCollection::const_iterator it = game->mushrooms.begin(); it != game->mushrooms.end(); ++it)
		mushrooms.push_back(addComponent<MushroomSound>(this, *it));
}


void LLSoundManager::init()
{
	loadConfig(Config::getValue(LLConfigKeys::soundConfigPath));
	loadWaveBank(Config::getValue(LLConfigKeys::soundWaveBankPath));

	SoundManager::init();
}


void LLSoundManager::update(const GameTime& gameTime)
{
	//Camera& camera = getManager().getCamera();
	//setListener(camera.getPosition(), camera.getForward(), camera.getUp());

	LeprechaunController& leprechaun = ((LuckyLeprechauns&)getManager()).getPlayer().getLeprechaun();
	setListener(leprechaun.getPosition(), leprechaun.getForward(), Vector3::up);

	SoundManager::update(gameTime);
}


void LLSoundManager::toggleBackgroundMusic()
{
	gameSound->toggleBackgroundMusic();
}