#pragma once
#include "DrawableNetwork.h"
#include "PigController.h"
#include "NetworkPacketPig.h"

class PigNetwork : public DrawableNetwork<PigController>
{
public:
	PigNetwork(LLNetworkManager* manager, PigController* controller);

	void receive(const NetworkPacketPig& packet);
};

