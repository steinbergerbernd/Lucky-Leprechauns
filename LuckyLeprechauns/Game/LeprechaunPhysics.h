#pragma once

#include "PhysicsComponent.h"
#include "LeprechaunController.h"

#include "HatPhysics.h"
#include "CanePhysics.h"

class LLPhysicsManager;

class LeprechaunPhysics : public PhysicsComponent<LeprechaunController>
{
public:
	LeprechaunPhysics(LLPhysicsManager* manager, LeprechaunController* controller);

	void init();
	void update(const GameTime& gameTime);

	void setEnabled(bool enabled);
	void setLocal(bool local);

private:
	HatPhysics* hat;
	CanePhysics* cane;

	float slidingGradient;
	float maxSlidingForce;

	Vector3 lastSafePosition;
	Vector3 slidingVelocity;

	bool wasInWater;
};

