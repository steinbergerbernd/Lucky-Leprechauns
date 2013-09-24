#pragma once
#include "PhysicsComponent.h"
#include "HatController.h"

class LLPhysicsManager;

class HatPhysics : public PhysicsComponent<HatController>
{
public:
	HatPhysics(LLPhysicsManager* manager, HatController* controller);

	virtual void update(const GameTime& gameTime);

private:
	float maxChargingTime;
};

