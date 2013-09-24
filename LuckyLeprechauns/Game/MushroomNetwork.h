#pragma once

#include "DrawableNetwork.h"
#include "MushroomController.h"

#include "StoneNetwork.h"

class MushroomNetwork : public DrawableNetwork<MushroomController>
{
public:
	MushroomNetwork(LLNetworkManager* manager, MushroomController* controller);

	void receive(const NetworkPacketMushroom& packet);

	StoneNetwork& getStone() { return *stone; }

private:
	StoneNetwork* stone;
};