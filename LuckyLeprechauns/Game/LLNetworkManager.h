#pragma once

#include "NetworkManager.h"
#include "LuckyLeprechauns.h"

#include "NetworkTypes.h"
#include "NetworkConstants.h"

class PigNetwork;
class MushroomNetwork;
class ItemNetwork;
class PlayerNetwork;
class LeprechaunNetwork;
class TrapNetwork;

class LLNetworkManager : public NetworkManager
{
	typedef List<PlayerNetwork*> PlayerCollection;
	typedef Map<unsigned char, PlayerNetwork*> PlayerMap;

public:
	typedef std::vector<MushroomNetwork*> MushroomCollection;
	typedef std::vector<ItemNetwork*> ItemCollection;

	LLNetworkManager(LuckyLeprechauns* game);

	void init();

protected:
	void process(const NetworkPacket& packet);
	void onDisconnect();

private:
	PlayerNetwork* player;
	PigNetwork* pig;

	TrapNetwork* traps;

	MushroomCollection mushrooms;
	ItemCollection items;
};
