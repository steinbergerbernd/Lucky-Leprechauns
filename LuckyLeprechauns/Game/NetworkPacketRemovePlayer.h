#pragma once

#include "NetworkPacketId.h"

class NetworkPacketRemovePlayer : public NetworkPacketId
{
public:
	NetworkPacketRemovePlayer(const NetworkPacket& packet) : NetworkPacketId(packet) {}
};