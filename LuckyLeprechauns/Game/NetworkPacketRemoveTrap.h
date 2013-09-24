#pragma once
#include "NetworkPacketId.h"

class NetworkPacketRemoveTrap : public NetworkPacketId
{
public:
	NetworkPacketRemoveTrap(const NetworkPacket& packet) : NetworkPacketId(packet) {}
};

