#include "StoneNetwork.h"

StoneNetwork::StoneNetwork(LLNetworkManager* manager, StoneController* controller) : DrawableNetwork(manager, controller)
{
}


void StoneNetwork::receive(const NetworkPacketStone& packet)
{
	DrawableNetwork::receive(packet);

	getController().setEnabled(packet.get<bool>(NetworkPacketStone::getOffset()));
}