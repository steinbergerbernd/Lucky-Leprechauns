#pragma once

#include "NetworkPacketId.h"

class NetworkPacketRemoveItem : public NetworkPacketId
{
public:
	NetworkPacketRemoveItem(const NetworkPacket& packet) : NetworkPacketId(packet) {}
};

