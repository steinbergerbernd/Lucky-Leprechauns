#pragma once

#include "LLGameComponent.h"

#include "TrapManager.h"
#include "PlayerController.h"
#include "MushroomController.h"

class LLGameLogic : public LLGameComponent
{
	typedef LuckyLeprechauns::ComponentMap ComponentMap;
	typedef LuckyLeprechauns::MushroomCollection MushroomCollection;

	typedef TrapManager::RemoteTrapMap RemoteTrapMap;
	typedef TrapManager::TrapMap TrapMap;
public:
	typedef TrapManager::ExplosionList ExplosionList;

	LLGameLogic(LuckyLeprechauns* game);

	void init();
	void update(const GameTime& gameTime);

	const ExplosionList& getExplosions() const { return ((LuckyLeprechauns&)getManager()).getTrapManager().getExplosions(); }
	void clearExplosions();

private:
	float rainbowGrowthPerMinute;

	void detectRemoteLeprechauns(ComponentMap& remotePlayers, const LeprechaunController& localLeprechaun, const MushroomCollection& mushrooms);
	void detectRemoteTraps(RemoteTrapMap& remoteTraps, const LeprechaunController& localLeprechaun, const MushroomCollection& mushrooms);

	void updateRainbowProgress(const GameTime& gameTime, const MushroomCollection& mushrooms);
};

