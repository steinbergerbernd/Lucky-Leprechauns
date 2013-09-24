#pragma once

#include "NetworkPacketDrawable.h"

class NetworkPacketAddItem : public NetworkPacketDrawable
{
public:
	NetworkPacketAddItem(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
};