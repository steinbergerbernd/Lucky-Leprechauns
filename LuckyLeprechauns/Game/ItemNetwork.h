#pragma once
#include "DrawableNetwork.h"
#include "ItemController.h"
#include "NetworkTypes.h"

class ItemNetwork : public DrawableNetwork<ItemController>
{
public:
	ItemNetwork(LLNetworkManager* manager, ItemController* controller);
	
	void receive(const NetworkPacketAddItem& packet);
	void receive(const NetworkPacketRemoveItem& packet);
	void receive(const NetworkPacketCollectItem& packet);
};

