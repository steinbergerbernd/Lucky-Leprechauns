#include "MushroomNetwork.h"

MushroomNetwork::MushroomNetwork(LLNetworkManager* manager, MushroomController* controller)
	: DrawableNetwork(manager, controller)
{
	stone = manager->addComponent<StoneNetwork>(manager, &controller->getStone());
}


void MushroomNetwork::receive(const NetworkPacketMushroom& packet)
{
	MushroomController& controller = getController();

	DrawableNetwork::receive(packet);

	unsigned index = NetworkPacketMushroom::getOffset();

	controller.setRange(packet.get<float>(index, &index));
	controller.setAttacking(packet.get<bool>(index, &index));
	bool hasOwner = packet.get<bool>(index, &index);
	controller.setLevel(packet.get<unsigned>(index, &index));

	if (hasOwner)
		controller.setOwner(packet.get<unsigned char>(index, &index));
	else
		controller.clearOwner();
}