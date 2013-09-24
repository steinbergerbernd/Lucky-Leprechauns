#pragma once
#include "DrawableNetwork.h"
#include "TrapManager.h"

class TrapNetwork : public DrawableNetwork<TrapManager>
{
public:
	TrapNetwork(LLNetworkManager* manager, TrapManager* trapManager);

	void receive(const NetworkPacketAddTrap& packet);
	void receive(const NetworkPacketRemoveTrap& packet);
};

