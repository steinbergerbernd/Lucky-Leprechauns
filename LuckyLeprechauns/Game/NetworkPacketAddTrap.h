#pragma once
#include "NetworkPacketDrawable.h"

class NetworkPacketAddTrap : public NetworkPacketDrawable
{
public:
	NetworkPacketAddTrap(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
};

