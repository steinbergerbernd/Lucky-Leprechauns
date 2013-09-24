#pragma once

#include "GameComponent.h"
#include "BasicManager.h"

#include "Game.h"
#include "Input.h"

class InputManager : public GameComponent, public BasicManager
{
public:
	InputManager(Game* game);

	virtual void init();
	virtual void release();
	virtual void update(const GameTime& gameTime);

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

private:
	unsigned buttonShowCursor;

	bool resetDevice;
};