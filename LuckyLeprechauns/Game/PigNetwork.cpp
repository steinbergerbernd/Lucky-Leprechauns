#include "PigNetwork.h"


PigNetwork::PigNetwork(LLNetworkManager* manager, PigController* controller) : DrawableNetwork(manager, controller)
{
}


void PigNetwork::receive(const NetworkPacketPig& packet)
{
	PigController& controller = getController();

	DrawableNetwork::receive(packet);

	unsigned index = NetworkPacketPig::getOffset();

	bool gravityEnabled = packet.get<bool>(index, &index);
	bool hasCaptor = packet.get<bool>(index, &index);

	if (hasCaptor)
		controller.setCaptor(packet.get<unsigned char>(index, &index));
	else
		controller.clearCaptor();

	controller.setMoving(controller.getVelocity().lengthSquared() > 0 && !gravityEnabled);
}