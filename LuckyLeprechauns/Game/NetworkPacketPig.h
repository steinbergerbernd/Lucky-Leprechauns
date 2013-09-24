#pragma once

#include "NetworkPacketDrawable.h"

class NetworkPacketPig : public NetworkPacketDrawable
{
public:
	NetworkPacketPig(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
};

