#include "LLGameLogic.h"
#include "RainbowController.h"

#undef max
#undef min

LLGameLogic::LLGameLogic(LuckyLeprechauns* game) : LLGameComponent(game), rainbowGrowthPerMinute(0)
{
}


void LLGameLogic::init()
{
	rainbowGrowthPerMinute = Config::getValue<float>(LLConfigKeys::rainbowGrowthPerMinute);
}


void LLGameLogic::update(const GameTime& gameTime)
{
	LuckyLeprechauns& game = (LuckyLeprechauns&)getManager();

	TrapManager& trapManager = game.getTrapManager();
	LeprechaunController& localLeprechaun = game.getPlayer().getLeprechaun();
	
	detectRemoteLeprechauns(game.remotePlayers, localLeprechaun, game.mushrooms);
	detectRemoteTraps(trapManager.getRemoteTraps(), localLeprechaun, game.mushrooms);

	if (localLeprechaun.hasUsedExplosionTrap())
		trapManager.addTrap(localLeprechaun.getPosition());

	updateRainbowProgress(gameTime, game.mushrooms);
}


void LLGameLogic::updateRainbowProgress(const GameTime& gameTime, const MushroomCollection& mushrooms)
{
	for (LuckyLeprechauns::MushroomCollection::const_iterator it = mushrooms.begin(); it != mushrooms.end(); ++it)
	{
		LeprechaunController* owner = (*it)->getOwner();

		if (owner != 0)
		{
			RainbowController& rainbow = owner->getPlayer().getRainbow();
			float progress = rainbow.getProgress();

			if (progress < 100.0f)
			{
				progress += (rainbowGrowthPerMinute / 60.0f) * (*it)->getLevel() * gameTime.elapsed;

				progress = std::min(progress, 100.0f);

				rainbow.setProgress(progress);
			}
		}
	}
}


void LLGameLogic::detectRemoteLeprechauns(ComponentMap& remotePlayers, const LeprechaunController& localLeprechaun, const MushroomCollection& mushrooms)
{
	for (LuckyLeprechauns::ComponentMap::iterator it = remotePlayers.begin(); it != remotePlayers.end(); ++it)
	{
		LeprechaunController& remoteLeprechaun = ((PlayerController*)it->second)->getLeprechaun();

		bool detected = localLeprechaun.hasShowInvisibility();

		for (LuckyLeprechauns::MushroomCollection::const_iterator it = mushrooms.begin(); it != mushrooms.end() && !detected; ++it)
		{
			const MushroomController& mushroom = **it;

			if (mushroom.getOwner() == &localLeprechaun && (mushroom.getPosition() - remoteLeprechaun.getPosition()).lengthSquared() < std::pow(mushroom.getRange(), 2))
				detected = true;
		}

		remoteLeprechaun.setDetected(detected);
	}
}


void LLGameLogic::detectRemoteTraps(RemoteTrapMap& remoteTraps, const LeprechaunController& localLeprechaun, const MushroomCollection& mushrooms)
{
	for (RemoteTrapMap::iterator it = remoteTraps.begin(); it != remoteTraps.end(); ++it)
		for (TrapMap::iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
		{
			TrapController& trap = *innerIt->second;

			bool detected = localLeprechaun.hasShowInvisibility();

			for (MushroomCollection::const_iterator it = mushrooms.begin(); it != mushrooms.end() && !detected; ++it)
			{
				const MushroomController& mushroom = **it;

				if (mushroom.getOwner() == &localLeprechaun && (mushroom.getPosition() - trap.getPosition()).lengthSquared() < std::pow(mushroom.getRange(), 2))
					detected = true;
			}

			trap.setDetected(detected);
		}
}


void LLGameLogic::clearExplosions()
{
	((LuckyLeprechauns&)getManager()).getTrapManager().clearExplosions();
}