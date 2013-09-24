#include "HatNetwork.h"


HatNetwork::HatNetwork(LLNetworkManager* manager, HatController* controller) : DrawableNetwork(manager, controller)
{
}


void HatNetwork::update(const GameTime& gameTime)
{
	HatController& hat = getController();

	if (!isLocal() || !hat.isEnabled())
		return;

	NetworkPacketHat packet(hat.getPlayerId());

	DrawableNetwork::set(packet);

	getManager().send(packet);
}


void HatNetwork::receive(const NetworkPacketHat& packet)
{
	DrawableNetwork::receive(packet);
}