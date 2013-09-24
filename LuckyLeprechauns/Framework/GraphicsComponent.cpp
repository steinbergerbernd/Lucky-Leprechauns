#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent(GraphicsManager* graphicsManager) : BasicComponent(graphicsManager), transparency(false)
{
}


GraphicsComponent::~GraphicsComponent()
{
}


void GraphicsComponent::draw(const GameTime& gameTime)
{
}