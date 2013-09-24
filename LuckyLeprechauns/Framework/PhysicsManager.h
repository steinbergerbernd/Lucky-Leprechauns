#pragma once
#include "BasicManager.h"
#include "GameComponent.h"
#include "STL.h"

class PhysicsManager : public GameComponent, public BasicManager
{
public:
	PhysicsManager(Game* game);
	virtual ~PhysicsManager(void);

	virtual void init();
	virtual void update(const GameTime& gameTime);
	virtual void release();

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

	virtual const Vector3& getGravity() const;
protected:
	Vector3 gravity;
};