#pragma once

#include "NetworkPacketDrawable.h"
#include "NetworkPacketTypes.h"

class NetworkPacketLeprechaun : public NetworkPacketDrawable
{
public:
	NetworkPacketLeprechaun(const NetworkPacket& packet) : NetworkPacketDrawable(packet) {}
	NetworkPacketLeprechaun(unsigned char id) : NetworkPacketDrawable(NetworkPacketTypes::leprechaun, id) {}
};