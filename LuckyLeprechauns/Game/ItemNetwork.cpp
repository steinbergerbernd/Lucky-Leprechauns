#include "ItemNetwork.h"

ItemNetwork::ItemNetwork(LLNetworkManager* manager, ItemController* controller)  : DrawableNetwork(manager, controller)
{
}


void ItemNetwork::receive(const NetworkPacketAddItem& packet)
{
	DrawableNetwork::receive(packet);

	getController().setEnabled(true);
}


void ItemNetwork::receive(const NetworkPacketRemoveItem& packet)
{
	getController().setEnabled(false);
}


void ItemNetwork::receive(const NetworkPacketCollectItem& packet)
{
	getController().collectItem(packet.getItemType());
}