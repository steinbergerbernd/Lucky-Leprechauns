#pragma once
#include "SoundManager.h"

class LuckyLeprechauns;
class LeprechaunSound;
class PigSound;
class MushroomSound;
class LLGameSound;

class LLSoundManager : public SoundManager
{
	typedef std::list<MushroomSound*> MushroomList;
public:
	LLSoundManager(LuckyLeprechauns* game);

	virtual void init();
	virtual void update(const GameTime& gameTime);

	void toggleBackgroundMusic();
private:
	LeprechaunSound* leprechaun;
	LLGameSound* gameSound;
	PigSound* pig;
	
	MushroomList mushrooms;
};