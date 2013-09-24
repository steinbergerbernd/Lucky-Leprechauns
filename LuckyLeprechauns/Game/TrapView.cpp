#include "TrapView.h"


TrapView::TrapView(LLGraphicsManager* manager, const TrapManager* trapManager) : LLGraphicsComponent(manager), trapManager(trapManager)
{
}


void TrapView::init()
{
	model = FBXModelLoader::getModelClone("ExplosionTrap");
	model.setKeyColor(Config::getValue<Vector3>(LLConfigKeys::explosionTrapKeyColor));

	enemyColor = Config::getValue<Vector3>(LLConfigKeys::explosionTrapEnemyColor);

	instances.resize(1024);
}


void TrapView::draw(const GameTime& gameTime)
{
	const TrapManager::TrapMap& localTraps = trapManager->getLocalTraps();

	unsigned count = 0;

	for (TrapManager::TrapMap::const_iterator it = localTraps.begin(); it != localTraps.end(); ++it)
		if (it->second->isEnabled())
			instances[count++] = it->second->getWorld();

	if (count > 0)
		FBXRenderer::draw(model, gameTime, getManager(), Matrix::identity, instances, count);

	count = 0;
	
	const TrapManager::RemoteTrapMap& remoteTraps = trapManager->getRemoteTraps();

	for (TrapManager::RemoteTrapMap::const_iterator it = remoteTraps.begin(); it != remoteTraps.end(); ++it)
		for(TrapManager::TrapMap::const_iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
		{
			TrapController& trap = *innerIt->second;

			if (trap.isEnabled() && trap.isDetected())
				instances[count++] = (trap.getWorld());
		}

	if (count > 0)
	{
		model.setKeyColorEnabled(true);
		model.setKeyColorReplace(enemyColor);

		FBXRenderer::draw(model, gameTime, getManager(), Matrix::identity, instances, count);
		
		model.setKeyColorEnabled(false);
	}
}