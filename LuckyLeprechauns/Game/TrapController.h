#pragma once
#include "LLDrawableGameComponent.h"
#include "LuckyLeprechauns.h"

class TrapController : public LLDrawableGameComponent
{
public:
	TrapController(LuckyLeprechauns* game);

	bool isDetected() const { return detected; }
	void setDetected(bool detected) { this->detected = detected; }

private:
	bool detected;
};

