#pragma once

#include "NetworkPacket.h"

#define NETWORK_PACKET_ID_SIZE 1

#define ID 0

struct NetworkPacketId : public NetworkPacket
{
public:
	NetworkPacketId(const NetworkPacket& packet) : NetworkPacket(packet) {}

	NetworkPacketId(unsigned char type, unsigned char id, unsigned char additionalDataSize = 0) 
		: NetworkPacket(type, NETWORK_PACKET_ID_SIZE + additionalDataSize)
	{
		setId(id);
	}

	static unsigned char getOffset() { return NETWORK_PACKET_ID_SIZE; }

	unsigned char getId() const { return data[ID]; }

	void setId(unsigned char id) { data[ID] = id; }
};