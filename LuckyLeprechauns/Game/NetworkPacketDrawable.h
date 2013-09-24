#pragma once

#include "NetworkPacketId.h"

#include "Vector3.h"
#include "Quaternion.h"

#define POSITION 0
#define ROTATION 12
#define SCALE 28
#define VELOCITY 40

#define NETWORK_PACKET_DRAWABLE_SIZE 52

struct NetworkPacketDrawable : NetworkPacketId
{
public:
	NetworkPacketDrawable(const NetworkPacket& packet) : NetworkPacketId(packet) {}

	NetworkPacketDrawable(unsigned char type, unsigned char id, unsigned char additionalDataSize = 0)
		: NetworkPacketId(type, id, NETWORK_PACKET_DRAWABLE_SIZE + additionalDataSize) {}

	static unsigned char getOffset() { return NetworkPacketId::getOffset() + NETWORK_PACKET_DRAWABLE_SIZE; }

	void setPosition(const Vector3& position) { set<Vector3>(position, NetworkPacketId::getOffset() + POSITION); }
	void setRotation(const Quaternion& rotation) { set<Quaternion>(rotation, NetworkPacketId::getOffset() + ROTATION); }
	void setScale(const Vector3& scale) { set<Vector3>(scale, NetworkPacketId::getOffset() + SCALE); }
	void setVelocity(const Vector3& velocity) { set<Vector3>(velocity, NetworkPacketId::getOffset() + VELOCITY); }

	const Vector3& getPosition() const { return get<Vector3>(NetworkPacketId::getOffset() + POSITION); }
	const Quaternion& getRotation() const { return get<Quaternion>(NetworkPacketId::getOffset() + ROTATION); }
	const Vector3& getScale() const { return get<Vector3>(NetworkPacketId::getOffset() + SCALE); }
	const Vector3& getVelocity() const { return get<Vector3>(NetworkPacketId::getOffset() + VELOCITY); }
};