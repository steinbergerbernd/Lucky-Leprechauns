#pragma once

#include "NetworkPacketDrawable.h"

class NetworkPacketMushroom : public NetworkPacketDrawable
{
public:
	NetworkPacketMushroom(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
};