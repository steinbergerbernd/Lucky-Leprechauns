#pragma once

#include "DrawableNetwork.h"
#include "HatController.h"

class HatNetwork : public DrawableNetwork<HatController>
{
public:
	HatNetwork(LLNetworkManager* manager, HatController* controller);
	
	void update(const GameTime& gameTime);
	void receive(const NetworkPacketHat& packet);
};