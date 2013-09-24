#pragma once

#include "DrawableNetwork.h"
#include "CaneController.h"

class CaneNetwork : public DrawableNetwork<CaneController>
{
public:
	CaneNetwork(LLNetworkManager* manager, CaneController* controller);

	void update(const GameTime& gameTime);
	void receive(const NetworkPacketCane& packet);
};