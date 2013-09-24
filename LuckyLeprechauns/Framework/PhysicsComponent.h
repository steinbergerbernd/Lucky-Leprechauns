#pragma once

#include "ControlledComponent.h"
#include "BasicComponent.h"
#include "STL.h"

class PhysicsManager;

template <class T>
class PhysicsComponent : public BasicComponent<PhysicsManager>, public ControlledComponent<T>
{
public:
	PhysicsComponent(PhysicsManager* manager, T* controller);
	virtual ~PhysicsComponent(void) = 0;

	virtual void update(const GameTime& gameTime);

	bool isGravityEnabled() const { return gravityEnabled; }
	void setGravityEnabled(bool gravityEnabled) { this->gravityEnabled = gravityEnabled; }

private:
	bool gravityEnabled;
};

template <class T>
PhysicsComponent<T>::PhysicsComponent(PhysicsManager* manager, T* controller) 
	: BasicComponent(manager), ControlledComponent(controller), gravityEnabled(false)
{
}

template <class T>
PhysicsComponent<T>::~PhysicsComponent()
{
}


template <class T>
void PhysicsComponent<T>::update(const GameTime& gameTime)
{
	T& controller = getController();

	if (gravityEnabled)
		controller.setVelocity(controller.getVelocity() + getManager().getGravity() * gameTime.elapsed);

	controller.setPosition(controller.getPosition() + controller.getVelocity() * gameTime.elapsed);
}