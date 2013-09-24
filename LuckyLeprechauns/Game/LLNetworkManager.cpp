#include "LLNetworkManager.h"

#include "PlayerNetwork.h"

#include "PigNetwork.h"
#include "MushroomNetwork.h"
#include "ItemNetwork.h"
#include "TrapNetwork.h"

LLNetworkManager::LLNetworkManager(LuckyLeprechauns* game) : NetworkManager(game)
{
	player = addComponent<PlayerNetwork>(this, game->player, false);

	pig = addComponent<PigNetwork>(this, game->pig, true, false);
	traps = addComponent<TrapNetwork>(this, game->trapManager, true, false);

	for (ComponentCollection::iterator it = game->remotePlayerPool.begin(); it != game->remotePlayerPool.end(); ++it)
		remotePlayerPool.push_back(addComponent<PlayerNetwork>(this, (PlayerController*)*it, true, false));

	for (LuckyLeprechauns::MushroomCollection::iterator it = game->mushrooms.begin(); it != game->mushrooms.end(); ++it)
		mushrooms.push_back(addComponent<MushroomNetwork>(this, *it, true, false));

	for (LuckyLeprechauns::ItemCollection::iterator it = game->items.begin(); it != game->items.end(); ++it)
		items.push_back(addComponent<ItemNetwork>(this, *it, true, false));
}


void LLNetworkManager::init()
{
	NetworkManager::init();

	const std::string& server = Config::getValue(LLConfigKeys::networkServer);
	const std::string& port = Config::getValue(LLConfigKeys::networkPort);

	NetworkManager::start(server, port);
}


void LLNetworkManager::process(const NetworkPacket& packet)
{
	LuckyLeprechauns& manager = (LuckyLeprechauns&)getManager();

	switch (packet.type)
	{
		case NetworkPacketTypes::welcome:
			{
				NetworkPacketWelcome welcomePacket = (NetworkPacketWelcome)packet;
				manager.onServerWelcome(welcomePacket.getId(), welcomePacket.getPlayerColor(), welcomePacket.getLeprechaunPosition(), welcomePacket.getRainbowPosition());
				player->setEnabled(true);
			}
			break;

		case NetworkPacketTypes::addPlayer:
			{
				NetworkPacketAddPlayer addPlayerPacket = (NetworkPacketAddPlayer)packet;
				manager.addPlayer(addPlayerPacket.getId(), addPlayerPacket.getPlayerColor(), addPlayerPacket.getRainbowPosition());
			}
			break;

		case NetworkPacketTypes::removePlayer:
			{
				manager.removePlayer(((NetworkPacketRemovePlayer)packet).getId());
			}
			break;

		case NetworkPacketTypes::leprechaun:
			{
				((PlayerNetwork*)remotePlayers[((NetworkPacketId)packet).getId()])->getLeprechaun().receive(packet);
			}
			break;

		case NetworkPacketTypes::cane:
			{
				(((PlayerNetwork*)remotePlayers[((NetworkPacketId)packet).getId()]))->getLeprechaun().getCane().receive(packet);
			}
			break;

		case NetworkPacketTypes::hat:
			{
				(((PlayerNetwork*)remotePlayers[((NetworkPacketId)packet).getId()]))->getLeprechaun().getHat().receive(packet);
			}
			break;

		case NetworkPacketTypes::kick:
			{
				player->getLeprechaun().onKick(packet);
			}
			break;

		case NetworkPacketTypes::caneHit:
			{
				player->getLeprechaun().onCaneHit(packet);
			}
			break;

		case NetworkPacketTypes::pig:
			{
				pig->receive(packet);
			}
			break;

		case NetworkPacketTypes::mushroom:
			{
				mushrooms[((NetworkPacketMushroom)packet).getId()]->receive(packet);
			}
			break;

		case NetworkPacketTypes::stone:
			{
				mushrooms[((NetworkPacketStone)packet).getId()]->getStone().receive(packet);
			}
			break;

		case NetworkPacketTypes::stoneHit:
			{
				player->getLeprechaun().onStoneHit(packet);
			}
			break;

		case NetworkPacketTypes::capturePig:
			{
				player->getLeprechaun().onCapturePig();
			}
			break;

		case NetworkPacketTypes::addItem:
			{
				items[((NetworkPacketId)packet).getId()]->receive((NetworkPacketAddItem)packet);
			}
			break;

		case NetworkPacketTypes::removeItem:
			{
				items[((NetworkPacketId)packet).getId()]->receive((NetworkPacketRemoveItem)packet);
			}
			break;

		case NetworkPacketTypes::collectItem:
			{
				items[((NetworkPacketId)packet).getId()]->receive((NetworkPacketCollectItem)packet);
			}
			break;

		case NetworkPacketTypes::dropPig:
			{
				player->getLeprechaun().onDropPig();
			}
			break;

		case NetworkPacketTypes::addTrap:
			{
				traps->receive((NetworkPacketAddTrap)packet);
			}
			break;

		case NetworkPacketTypes::removeTrap:
			{
				traps->receive((NetworkPacketRemoveTrap)packet);
			}
			break;

		case NetworkPacketTypes::gameOver:
			{
				manager.onGameOver(((NetworkPacketId)packet).getId());
			}
			break;

		default: throw Exception(LLExceptions::invalidNetworkPacketType);
	}
}


void LLNetworkManager::onDisconnect()
{
	LuckyLeprechauns& game = (LuckyLeprechauns&)getManager();

	ComponentMap remotePlayersCopy(game.remotePlayers);

	for (ComponentMap::iterator it = remotePlayersCopy.begin(); it != remotePlayersCopy.end(); ++it)
		game.removePlayer(((PlayerController*)it->second)->getPlayerId());

	player->setEnabled(false);
}
