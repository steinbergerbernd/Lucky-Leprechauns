#pragma once

#include "DrawableNetwork.h"
#include "StoneController.h"

class StoneNetwork : public DrawableNetwork<StoneController>
{
public:
	StoneNetwork(LLNetworkManager* manager, StoneController* controller);

	void receive(const NetworkPacketStone& packet);
};