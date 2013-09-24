#include "GoldpotController.h"
#include "TerrainController.h"

GoldpotController::GoldpotController(LuckyLeprechauns* game) : LLDrawableGameComponent(game)
{
}


void GoldpotController::init()
{
	LLDrawableGameComponent::init();

	setScale(Config::getValue<float>(LLConfigKeys::goldpotScale));
	Vector3 pos = Config::getValue<Vector3>(LLConfigKeys::goldpotPosition);
	setPosition(((LuckyLeprechauns&)getManager()).getTerrain().getHeight(pos));
}