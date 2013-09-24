#include "LuckyLeprechauns.h"

#include "LLGraphicsManager.h"
#include "LLGuiManager.h"
#include "LLNetworkManager.h"
#include "LLPhysicsManager.h"
#include "LLGameComponent.h"
#include "LLGameLogic.h"
#include "LLSoundManager.h"

#include "Controllers.h"
#include "Views.h"
#include "Camera.h"

#undef min
#undef max

const char* LuckyLeprechauns::configPath = "Content/Config/LLConfig.txt";

LuckyLeprechauns::LuckyLeprechauns() : player(0), terrain(0), vegetation(0), pig(0), camera(0), gameLogic(0), gameOver(false)
{
	Config::addFile(configPath);

	terrain = addComponent<TerrainController>(this);
	vegetation = addComponent<VegetationController>(this);
	player = addComponent<PlayerController>(this);
	pig = addComponent<PigController>(this, false, false);
	trapManager = addComponent<TrapManager>(this);
	goldpot = addComponent<GoldpotController>(this);

	maxPlayerCount = Config::getValue<unsigned>(LLConfigKeys::gameMaxPlayers);

	unsigned remotePlayerCount = maxPlayerCount - 1;
	unsigned mushroomCount = Config::getValue<unsigned>(LLConfigKeys::gameMushroomCount);
	unsigned itemCount = Config::getValue<unsigned>(LLConfigKeys::gameMaxItemCount);

	for (unsigned i = 0; i < remotePlayerCount; ++i)
		remotePlayerPool.push_back(addComponent<PlayerController>(this, false, false));

	for (unsigned i = 0; i < mushroomCount; ++i)
		mushrooms.push_back(addComponent<MushroomController>(this, false, false));

	for (unsigned i = 0; i < itemCount; ++i)
		items.push_back(addComponent<ItemController>(this, false, false));

	gameLogic = addComponent<LLGameLogic>(this);
	
	camera = addCamera<LeprechaunCamera>();
	camera->setLeprechaun(&player->getLeprechaun());
	
	setGuiManager<LLGuiManager>(this);
	setGraphicsManager<LLGraphicsManager>(this);
	setNetworkManager<LLNetworkManager>(this);
	setPhysicsManager<LLPhysicsManager>(this);
	setSoundManager<LLSoundManager>(this);
}


void LuckyLeprechauns::init()
{		
	Game::init();
}


void LuckyLeprechauns::update(const GameTime& gameTime)
{
	if (player)
	{
		LeprechaunController& leprechaun = player->getLeprechaun();

		// Cheats
		if (Keyboard::isKeyPress('4'))
			leprechaun.collectItem(ItemType::invisibility);
		else if (Keyboard::isKeyPress('5'))
			leprechaun.collectItem(ItemType::explosionTrap);
		else if (Keyboard::isKeyPress('6'))
			leprechaun.collectItem(ItemType::pigRobbery);
		else if (Keyboard::isKeyPress('7'))
			leprechaun.collectItem(ItemType::attackDouble);
		else if (Keyboard::isKeyPress('8'))
			leprechaun.collectItem(ItemType::invulnerability);
		else if (Keyboard::isKeyPress('9'))
			leprechaun.collectItem(ItemType::showInvisibility);
		else if (Keyboard::isKeyPress('0'))
			leprechaun.collectItem(ItemType::speedBoost);
		else if (Keyboard::isKeyPress('I'))
			leprechaun.setInvulnerable(!leprechaun.hasInvulnerability());
	}

	if (Keyboard::isKeyPress(VK_TAB))
		toggleWireframe();
	else if (Keyboard::isKeyPress('M'))
		toggleCursor();
	else if (Keyboard::isKeyPress('C'))
		toggleCamera(true);
	else if (Keyboard::isKeyPress('H'))
		getGuiManager().setVisible(!getGuiManager().isVisible());
	else if (Keyboard::isKeyPress('L'))
		getCamera().toggleLock();
	else if (Keyboard::isKeyPress('R'))
		getPlayer().getLeprechaun().setPosition(getCamera().getPosition());
	else if (Keyboard::isKeyPress('B'))
		((LLSoundManager&)getSoundManager()).toggleBackgroundMusic();

	if (player)
		player->BasicComponent<Game>::setEnabled(&getCamera() == camera);

	Game::update(gameOver ? gameOverTime : gameTime);
}


void LuckyLeprechauns::draw(const GameTime& gameTime)
{
	Game::draw(gameOver ? gameOverTime : gameTime);
}


void LuckyLeprechauns::addPlayer(unsigned char playerId, const Vector3& playerColor, const Vector3& rainbowPosition)
{
	Game::addPlayer(playerId);

	PlayerController* remotePlayer = (PlayerController*)remotePlayers.find(playerId)->second;

	remotePlayer->setPlayerId(playerId);
	remotePlayer->setPlayerColor(playerColor);

	remotePlayer->getRainbow().setEnabled(true);
	remotePlayer->getRainbow().setPosition(rainbowPosition);
}


void LuckyLeprechauns::onServerWelcome(unsigned char playerId, const Vector3& playerColor, const Vector3& leprechaunPosition, const Vector3& rainbowPosition)
{
	player->setPlayerId(playerId);
	player->setPlayerColor(playerColor);

	player->getLeprechaun().setPosition(leprechaunPosition);

	player->getRainbow().setEnabled(true);
	player->getRainbow().setPosition(rainbowPosition);

	pig->setEnabled(true);

	for (MushroomCollection::iterator it = mushrooms.begin(); it != mushrooms.end(); ++it)
		(*it)->setEnabled(true);
}


void LuckyLeprechauns::onGameOver(unsigned char winner)
{
	if (winner != player->getPlayerId())
		camera->setLeprechaun(&((PlayerController*)remotePlayers[winner])->getLeprechaun());

	player->BasicComponent::setEnabled(false);
	camera->setViewOnly(true);
	
	gameOver = true;

	gameOverTime.total = getGameTime().total;
	gameOverTime.elapsed = 0.0f;

	((LLGuiManager&)getGuiManager()).setWinner(winner);
}


void LuckyLeprechauns::onCameraUpdated()
{
	if (&getCamera() == camera && terrain)
	{
		Vector3 cameraPosition = getCamera().getPosition();
		Vector3 terrainPosition = terrain->getHeight(cameraPosition);

		terrainPosition.y = std::max(terrainPosition.y + 1.0f, 1.0f);

		if (cameraPosition.y < terrainPosition.y)
			getCamera().setPosition(terrainPosition);
	}

	if (pig && pig->getCaptor() != 0)
		pig->setRotation(Quaternion::createFromAxisAngle(Vector3::up, D3DXToRadian(90)) * Quaternion::createFromAxisAngle(Vector3::forward, D3DXToRadian(180)) * pig->getCaptor()->getRotation());
}


void LuckyLeprechauns::toggleWireframe()
{
	GraphicsDevice device = getGraphicsDevice();

	unsigned long fillMode = device.getRenderState(D3DRS_FILLMODE) == D3DFILL_SOLID ? D3DFILL_WIREFRAME : D3DFILL_SOLID;

	device.setRenderState(D3DRS_FILLMODE, fillMode);
}


LeprechaunController* LuckyLeprechauns::getLeprechaun(unsigned playerId)
{
	if (player->getPlayerId() == playerId)
		return &player->getLeprechaun();

	ComponentMap::iterator it = remotePlayers.find(playerId);

	if (it == remotePlayers.end())
		return 0;

	return &((PlayerController*)it->second)->getLeprechaun();
}
