#include "HatPhysics.h"
#include "LLPhysicsManager.h"

HatPhysics::HatPhysics(LLPhysicsManager* manager, HatController* controller) : PhysicsComponent(manager, controller)
{
}


void HatPhysics::update(const GameTime& gameTime)
{
	HatController& hat = getController();
	const TerrainController& terrain = ((LLPhysicsManager&)getManager()).getTerrainController();

	if (!hat.isEnabled() || !hat.isLocal() || hat.isOnGround())
		return;

	PhysicsComponent::update(gameTime);

	Vector3 hatPos = hat.getPosition();
	Vector3 terrainPos = terrain.getHeight(hatPos);

	if (isGravityEnabled() && terrainPos.y >= hatPos.y)
	{
		setGravityEnabled(false);
		hat.setOnGround(true);
		hat.setVelocity(0);
 		hat.setPosition(terrainPos);
	}

	if (hat.getLeprechaun().isBeginHat())
	{
		setGravityEnabled(true);

		float power = MathHelper::lerp(hat.getThrowMinPower(), hat.getThrowMaxPower(), hat.getLeprechaun().getHatCharging());

		hat.setVelocity(Vector3::normalize(2.0f * hat.getForward() + Vector3::up) * power);
	}
}