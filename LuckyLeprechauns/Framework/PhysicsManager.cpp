#include "PhysicsManager.h"


PhysicsManager::PhysicsManager(Game* game) : GameComponent(game), gravity(0, -9.81f, 0)
{
}


PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::init()
{
	BasicManager::init();
}

void PhysicsManager::update(const GameTime& gameTime)
{
	BasicManager::update(gameTime);
}

void PhysicsManager::release()
{
	BasicManager::release();
}

void PhysicsManager::onLostDevice()
{
	BasicManager::onLostDevice();
}

void PhysicsManager::onResetDevice(const GraphicsDevice& device)
{
	BasicManager::onResetDevice(device);
}

const Vector3& PhysicsManager::getGravity() const
{
	return gravity;
}