#include "LLPhysicsManager.h"
#include "LeprechaunPhysics.h"
#include "HatPhysics.h"
#include "CanePhysics.h"
#include "PlayerController.h"
#include "PigController.h"

LLPhysicsManager::LLPhysicsManager(LuckyLeprechauns* game) : PhysicsManager(game)
{
	leprechaun = addComponent<LeprechaunPhysics>(this, &game->player->getLeprechaun());

	for (ComponentCollection::iterator it = game->remotePlayerPool.begin(); it != game->remotePlayerPool.end(); ++it)
		remotePlayerPool.push_back(addComponent<LeprechaunPhysics>(this, &((PlayerController*)(*it))->getLeprechaun(), false, false));
}


void LLPhysicsManager::init()
{
	PhysicsManager::init();

	gravity = Vector3(0, -Config::getValue<float>(LLConfigKeys::physicsGravity), 0);
}


const TerrainController& LLPhysicsManager::getTerrainController() const
{
	return ((LuckyLeprechauns&)getManager()).getTerrain();
}