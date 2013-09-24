#pragma once

#include "NetworkPacketDrawable.h"

class NetworkPacketStone :  public NetworkPacketDrawable
{
public:
	NetworkPacketStone(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
};