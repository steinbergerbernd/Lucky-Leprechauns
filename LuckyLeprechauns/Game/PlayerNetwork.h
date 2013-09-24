#pragma once

#include "LLControlledNetworkComponent.h"
#include "PlayerController.h"

#include "LeprechaunNetwork.h"

class PlayerNetwork : public LLControlledNetworkComponent<PlayerController>
{
public:
	PlayerNetwork::PlayerNetwork(LLNetworkManager* manager, PlayerController* controller);

	LeprechaunNetwork& getLeprechaun() { return *leprechaun; }

	void setEnabled(bool enabled);
	void setLocal(bool local);

private:
	LeprechaunNetwork* leprechaun;
};