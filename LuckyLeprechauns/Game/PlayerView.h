#pragma once

#include "LLControlledGraphicsComponent.h"
#include "PlayerController.h"

#include "LeprechaunView.h"
#include "RainbowView.h"

class PlayerView : public LLControlledGraphicsComponent<PlayerController>
{
public:
	PlayerView(LLGraphicsManager* manager, PlayerController* controller);

	void setEnabled(bool enabled);
	void setLocal(bool local);

	LeprechaunView& getLeprechaun() { return *leprechaun; }

private:
	LeprechaunView* leprechaun;
	RainbowView* rainbow;
};