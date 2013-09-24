#include "CaneNetwork.h"


CaneNetwork::CaneNetwork(LLNetworkManager* manager, CaneController* controller) : DrawableNetwork(manager, controller)
{
}


void CaneNetwork::update(const GameTime& gameTime)
{
	CaneController& cane = getController();

	if (!isLocal() || !cane.isEnabled())
		return;

	NetworkPacketCane packet(cane.getPlayerId());

	DrawableNetwork::set(packet);

	getManager().send(packet);
}


void CaneNetwork::receive(const NetworkPacketCane& packet)
{
	DrawableNetwork::receive(packet);
}