#pragma once
#include "PhysicsManager.h"
#include "LeprechaunController.h"
#include "HatController.h"
#include "CaneController.h"
#include "TerrainController.h"

class LeprechaunPhysics;

class LLPhysicsManager : public PhysicsManager
{
public:
	LLPhysicsManager(LuckyLeprechauns* game);

	virtual void init();

	const TerrainController& getTerrainController() const;

private:
	LeprechaunPhysics* leprechaun;
};

