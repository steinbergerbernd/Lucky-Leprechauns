#pragma once

#include "NetworkPacketId.h"

#define PLAYER_COLOR 0
#define LEPRECHAUN_POSITION 12
#define RAINBOW_POSITION 24

class NetworkPacketWelcome : public NetworkPacketId
{
public:
	NetworkPacketWelcome(const NetworkPacket& packet) : NetworkPacketId(packet) {}

	const Vector3& getLeprechaunPosition() const { return get<Vector3>(NetworkPacketId::getOffset() + LEPRECHAUN_POSITION); }
	const Vector3& getPlayerColor() const { return get<Vector3>(NetworkPacketId::getOffset() + PLAYER_COLOR); }
	const Vector3& getRainbowPosition() const { return get<Vector3>(NetworkPacketId::getOffset() + RAINBOW_POSITION); }
};