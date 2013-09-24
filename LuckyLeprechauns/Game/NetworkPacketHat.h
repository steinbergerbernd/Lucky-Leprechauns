#pragma once

#include "NetworkPacketDrawable.h"

struct NetworkPacketHat : public NetworkPacketDrawable
{
public:
	NetworkPacketHat(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
	NetworkPacketHat(unsigned char id) : NetworkPacketDrawable(NetworkPacketTypes::hat, id) {}
};