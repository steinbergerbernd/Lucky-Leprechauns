#pragma once

#include "GameComponent.h"
#include "LuckyLeprechauns.h"

class LLGameComponent : public GameComponent
{
public:
	LLGameComponent(LuckyLeprechauns* game);
	virtual ~LLGameComponent() = 0;
};