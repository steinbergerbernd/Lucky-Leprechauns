#pragma once

#include "BasicComponent.h"
#include "Game.h"

class GameComponent : public BasicComponent<Game>
{
public:
	GameComponent(Game* game);
	virtual ~GameComponent() = 0;

	virtual const GraphicsDevice& getGraphicsDevice() const;
	virtual const Camera& getCamera() const;
};