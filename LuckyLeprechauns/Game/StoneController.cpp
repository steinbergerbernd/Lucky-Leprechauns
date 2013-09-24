#include "StoneController.h"
#include "MushroomController.h"

StoneController::StoneController(LuckyLeprechauns* game, MushroomController* mushroom) 
	: LLDrawableGameComponent(game), mushroom(mushroom)
{
}