#include "PigController.h"

PigController::PigController(LuckyLeprechauns* game) : LLDrawableGameComponent(game), moving(false), captor(0), startedMoving(false),
	stoppedMoving(false), lost(false), captured(false)
{
}


void PigController::update(const GameTime& gameTime)
{
	captured = lost = startedMoving = stoppedMoving = false;
}


void PigController::setCaptor(unsigned char playerId)
{
	if (captor== 0 || captor->getPlayerId() != playerId)
	{
		captor = ((LuckyLeprechauns&)getManager()).getLeprechaun(playerId);
		captured = true;
		stoppedMoving = true;
	}
}


void PigController::clearCaptor()
{
	if(captor != 0)
	{
		captor = 0;
		lost = true;
	}
}


void PigController::setMoving(bool moving)
{
	if (this->moving != moving)
		if (moving)
			startedMoving = true;
		else
			stoppedMoving = true;

	this->moving = moving;
}