#include "LeprechaunPhysics.h"
#include "LLPhysicsManager.h"

#undef max
#undef min


LeprechaunPhysics::LeprechaunPhysics(LLPhysicsManager* manager, LeprechaunController* controller) 
	: PhysicsComponent(manager, controller), wasInWater(false)
{
	hat = manager->addComponent<HatPhysics>(manager, &controller->getHat());
	cane = manager->addComponent<CanePhysics>(manager, &controller->getCane());
}


void LeprechaunPhysics::init()
{
	slidingGradient = Config::getValue<float>(LLConfigKeys::gameSlidingGradient);
	maxSlidingForce = Config::getValue<float>(LLConfigKeys::gameMaxSlidingForce);
}


void LeprechaunPhysics::update(const GameTime& gameTime)
{
	LeprechaunController& leprechaun = getController();

	if (!leprechaun.isEnabled())
		return;

	PhysicsComponent::update(gameTime);

	if (!leprechaun.isLocal())
		return;

	LLPhysicsManager& manager = (LLPhysicsManager&)getManager();
	const TerrainController& terrain = manager.getTerrainController();

	const Vector3& leprechaunPosition = leprechaun.getPosition();
	const Vector3& leprechaunVelocity = leprechaun.getVelocity();

	Vector3 terrainPosition = terrain.getHeight(leprechaunPosition);
	Vector3 terrainNormal = terrain.getNormal(leprechaunPosition);

	// if gravity is disabled -> set leprechaun on ground and clear vertical velocity
	if (!isGravityEnabled())
		leprechaun.setPosition(terrainPosition);
	// else if leprechaun is below ground -> set him on ground, clear velocity and disable gravity
	else if (leprechaunPosition.y < terrainPosition.y)
	{
		leprechaun.setPosition(terrainPosition);
		leprechaun.setVelocity(0);
		leprechaun.onLanding();

		setGravityEnabled(false);
	}

	// if leprechaun is below water -> kick leprechaun out of water
	if (leprechaunPosition.y < 0.0f && leprechaunPosition.y <= terrainPosition.y && terrainNormal.y > slidingGradient)
	{
		Vector3 v = lastSafePosition - leprechaunPosition;
		Vector3 direction = Vector3::normalize(Vector3::normalize(Vector3(v.x, 0, v.z)) + Vector3::up * 2.0f);
		leprechaun.onKick(direction * 70.0f);
		wasInWater = true;
	}
	else if (!isGravityEnabled() && wasInWater && terrainNormal.y > slidingGradient)
	{
		wasInWater = false;

		if (!leprechaun.hasInvulnerability())
			leprechaun.onCaneHit(3.0f);
	}

	if (leprechaun.isBeginFly())
		leprechaun.setVelocity(leprechaun.getFlyVelocity());
	else if (leprechaun.isBeginJumping())
		leprechaun.setVelocity(leprechaunVelocity + Vector3::up * leprechaun.getJumpPower());
	else if (leprechaun.isBeginDodgingLeft())
		leprechaun.setVelocity((-leprechaun.getRight() + Vector3::up) * leprechaun.getDodgePower());
	else if (leprechaun.isBeginDodgingRight())
		leprechaun.setVelocity((leprechaun.getRight() + Vector3::up) * leprechaun.getDodgePower());

	if (leprechaun.isJumping() || leprechaun.isFlying() || leprechaun.isDodgingLeft() || leprechaun.isDodgingRight())
		setGravityEnabled(true);

	// if leprechaun is on slope -> make him slide
	if (!isGravityEnabled() && terrainNormal.y < slidingGradient)
		slidingVelocity += terrainNormal * maxSlidingForce * gameTime.elapsed;
	else
		slidingVelocity = Vector3::zero;

	if (!isGravityEnabled())
	{
		Vector3 movement = leprechaun.getMovingDirection() * leprechaun.getMovementPower() + slidingVelocity;
		leprechaun.setVelocity(Vector3(movement.x, 0, movement.z));

		lastSafePosition = leprechaunPosition;
	}
}


void LeprechaunPhysics::setLocal(bool local)
{
	hat->setLocal(local);
	cane->setLocal(local);
	PhysicsComponent::setLocal(local);
}


void LeprechaunPhysics::setEnabled(bool enabled)
{
	hat->setEnabled(enabled);
	cane->setEnabled(enabled);
	PhysicsComponent::setEnabled(enabled);
}