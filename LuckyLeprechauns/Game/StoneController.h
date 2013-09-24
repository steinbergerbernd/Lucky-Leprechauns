#pragma once

#include "LLDrawableGameComponent.h"

class MushroomController;

class StoneController : public LLDrawableGameComponent
{
public:
	StoneController(LuckyLeprechauns* game, MushroomController* mushroom);

	const MushroomController& getMushroom() const { return *mushroom; }

private:
	MushroomController* mushroom;
};