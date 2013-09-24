#pragma once

#include "NetworkPacketId.h"

class NetworkPacketCollectItem : public NetworkPacketId
{
public:
	NetworkPacketCollectItem(const NetworkPacket& packet) : NetworkPacketId(packet) {}

	unsigned getItemType() const { return data[NetworkPacketId::getOffset()]; }
};