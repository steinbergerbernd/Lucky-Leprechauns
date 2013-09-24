#pragma once

#include "NetworkPacketDrawable.h"

struct NetworkPacketCane : public NetworkPacketDrawable
{
public:
	NetworkPacketCane(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
	NetworkPacketCane(unsigned char id) : NetworkPacketDrawable(NetworkPacketTypes::cane, id) {}
};