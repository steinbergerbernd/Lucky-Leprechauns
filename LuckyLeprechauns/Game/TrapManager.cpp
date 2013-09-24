#include "TrapManager.h"

#include "PlayerController.h"
#include "TerrainController.h"

TrapManager::TrapManager(LuckyLeprechauns* game) : LLGameComponent(game), trapId(0)
{
}


TrapManager::~TrapManager()
{
	for (TrapMap::iterator it = localTraps.begin(); it != localTraps.end(); ++it)
		delete it->second;

	for (RemoteTrapMap::iterator it = remoteTraps.begin(); it != remoteTraps.end(); ++it)
		for(TrapMap::iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
			delete innerIt->second;
}


void TrapManager::init()
{
	trapScale = Config::getValue<float>(LLConfigKeys::explosionTrapScale);
}


TrapController* TrapManager::createTrap()
{
	return new TrapController(&(LuckyLeprechauns&)getManager());
}


void TrapManager::addTrap(const Vector3& position)
{
	LuckyLeprechauns& game = (LuckyLeprechauns&)getManager();
	TrapController* trap = new TrapController(&game);
	trap->setPosition(position);
	trap->setScale(Vector3(trapScale));

	localTraps[trapId++] = trap;
}


void TrapManager::addTrap(TrapController* trap, unsigned playerId, unsigned trapId)
{
	remoteTraps[playerId][trapId] = trap;
}


void TrapManager::removeTrap(unsigned playerId, unsigned trapId)
{
	LuckyLeprechauns& game = (LuckyLeprechauns&)getManager();

	if (game.getPlayer().getPlayerId() == playerId)
	{
		explosions.push_back(localTraps[trapId]->getPosition());
		localTraps.erase(trapId);
	}
	else
	{
		explosions.push_back(remoteTraps[playerId][trapId]->getPosition());
		remoteTraps[playerId].erase(trapId);
	}
}


void TrapManager::clearExplosions()
{
	explosions.clear();
}