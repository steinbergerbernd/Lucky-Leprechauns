#pragma once

#include "GraphicsComponent.h"
#include "LLGraphicsManager.h"

class LLGraphicsComponent : public GraphicsComponent
{
public:
	LLGraphicsComponent(GraphicsManager* graphicsManager);
	virtual ~LLGraphicsComponent() = 0;
};