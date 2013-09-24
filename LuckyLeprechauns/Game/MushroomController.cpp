#include "MushroomController.h"


MushroomController::MushroomController(LuckyLeprechauns* game) : LLDrawableGameComponent(game), owner(0), overtaken(false), level(0)
{
	stone = game->addComponent<StoneController>(game, this, false);
}


void MushroomController::update(const GameTime& gameTime)
{
	overtaken = false;
}


void MushroomController::setOwner(unsigned char playerId)
{
	LeprechaunController* newOwner = ((LuckyLeprechauns&)getManager()).getLeprechaun(playerId);
	if (newOwner != owner)
	{
		if (owner != 0)
			owner->removeMushroom();
		owner = newOwner;
		if (owner != 0)
		{
			owner->addMushroom();
			overtaken = true;
		}
	}
}


void MushroomController::setLevel(unsigned level)
{
	if(this->level != level)
	{
		this->level = level;
		if (owner != 0)
			overtaken = true;
	}
}