#include "PlayerNetwork.h"

PlayerNetwork::PlayerNetwork(LLNetworkManager* manager, PlayerController* controller) : LLControlledNetworkComponent(manager, controller)
{
	leprechaun = manager->addComponent<LeprechaunNetwork>(manager, &controller->getLeprechaun());
}


void PlayerNetwork::setEnabled(bool enabled)
{
	leprechaun->setEnabled(enabled);

	LLControlledNetworkComponent::setEnabled(enabled);
}


void PlayerNetwork::setLocal(bool local)
{
	leprechaun->setLocal(local);

	LLControlledNetworkComponent::setLocal(local);
}