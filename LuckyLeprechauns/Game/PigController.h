#pragma once

#include "LLDrawableGameComponent.h"
#include "LeprechaunController.h"

class PigController : public LLDrawableGameComponent
{
public:
	PigController(LuckyLeprechauns* game);

	void update(const GameTime& gameTime);

	void setMoving(bool moving);
	bool isMoving() const { return moving; }

	void setCaptor(unsigned char playerId);
	void clearCaptor();
	const LeprechaunController* getCaptor() const { return captor; }

	bool isCaptured() const { return captured; }
	bool isLost() const { return lost; }
	bool hasStartedMoving() const { return startedMoving; }
	bool hasStoppedMoving() const { return stoppedMoving; }

private:
	bool startedMoving;
	bool stoppedMoving;
	bool moving;
	bool captured;
	bool lost;

	const LeprechaunController* captor;
};

