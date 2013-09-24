#pragma once

#include "DrawableGameComponent.h"
#include "LLGameComponent.h"

class LLDrawableGameComponent : public DrawableGameComponent
{
public:
	LLDrawableGameComponent(LuckyLeprechauns* game);
	virtual ~LLDrawableGameComponent() = 0;
};