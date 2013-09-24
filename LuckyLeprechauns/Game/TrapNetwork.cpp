#include "TrapNetwork.h"
#include "TrapController.h"

TrapNetwork::TrapNetwork(LLNetworkManager* manager, TrapManager* trapManager) : DrawableNetwork(manager, trapManager)
{
}


void TrapNetwork::receive(const NetworkPacketAddTrap& packet)
{
	TrapController* trapController = getController().createTrap();
	
	trapController->setPosition(packet.getPosition());
	trapController->setRotation(packet.getRotation());
	trapController->setScale(packet.getScale());
	trapController->setVelocity(packet.getVelocity());
	
	unsigned char playerId = packet.getId();
	unsigned trapId = packet.get<unsigned>(NetworkPacketAddTrap::getOffset());

	getController().addTrap(trapController, playerId, trapId);
}


void TrapNetwork::receive(const NetworkPacketRemoveTrap& packet)
{
	unsigned char playerId = packet.getId();
	unsigned trapId = packet.get<unsigned>(NetworkPacketRemoveTrap::getOffset());
	getController().removeTrap(playerId, trapId);
}