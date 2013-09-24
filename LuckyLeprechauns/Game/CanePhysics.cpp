#include "CanePhysics.h"
#include "LLPhysicsManager.h"

#undef max

CanePhysics::CanePhysics(LLPhysicsManager* manager, CaneController* controller) : PhysicsComponent(manager, controller)
{
}


CanePhysics::~CanePhysics(void)
{
}


void CanePhysics::update(const GameTime& gameTime)
{
	CaneController& cane = getController();
	const TerrainController& terrain = ((LLPhysicsManager&)getManager()).getTerrainController();

	if (!cane.isEnabled() || !isLocal())
		return;

	PhysicsComponent::update(gameTime);

	Vector3 canePos = getController().getPosition();
	Vector3 terrainPos = terrain.getHeight(canePos);

	if (cane.getLeprechaun().isBeginCane())
	{
		heightOffset = canePos.y - terrainPos.y;
		cane.setVelocity(cane.getLeprechaun().getForward() * cane.getThrowPower());
	}

	canePos.y = std::max(terrainPos.y, 0.0f) + heightOffset;
	cane.setPosition(canePos);
}