#include "GameComponent.h"

GameComponent::GameComponent(Game* game) : BasicComponent(game)
{
}


GameComponent::~GameComponent()
{
}


const GraphicsDevice& GameComponent::getGraphicsDevice() const
{
	return getManager().getGraphicsDevice();
}

const Camera& GameComponent::getCamera() const
{
	return getManager().getCamera();
}