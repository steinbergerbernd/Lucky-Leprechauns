#pragma once

#include "NetworkPacketId.h"

#define ADD_PLAYER_PLAYER_COLOR 0
#define ADD_PLAYER_RAINBOW_POSITION 12

class NetworkPacketAddPlayer : public NetworkPacketId
{
public:
	NetworkPacketAddPlayer(const NetworkPacket& packet) : NetworkPacketId(packet) {}

	const Vector3& getPlayerColor() const { return get<Vector3>(NetworkPacketId::getOffset() + ADD_PLAYER_PLAYER_COLOR); }
	const Vector3& getRainbowPosition() const { return get<Vector3>(NetworkPacketId::getOffset() + ADD_PLAYER_RAINBOW_POSITION); }
};