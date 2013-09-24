#pragma once
#include "PhysicsComponent.h"
#include "CaneController.h"

class LLPhysicsManager;

class CanePhysics : public PhysicsComponent<CaneController>
{
public:
	CanePhysics(LLPhysicsManager* manager, CaneController* controller);
	virtual ~CanePhysics(void);

	virtual void update(const GameTime& gameTime);
private:
	bool caneImpulseSet;

	float heightOffset;
};

