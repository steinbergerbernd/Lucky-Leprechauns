#pragma once

#include "Game.h"

#include "LLConstants.h"
#include "LeprechaunCamera.h"

class TerrainController;
class LeprechaunController;
class VegetationController;
class PlayerController;
class PigController;
class MushroomController;
class ItemController;
class GoldpotController;
class TrapManager;
class LLGameLogic;

class LuckyLeprechauns : public Game
{
	friend class LLGraphicsManager;
	friend class LLNetworkManager;
	friend class LLGuiManager;
	friend class LLPhysicsManager;
	friend class LLGameLogic;
	friend class LLSoundManager;

public:
	typedef std::list<MushroomController*> MushroomCollection;
	typedef std::list<ItemController*> ItemCollection;

	LuckyLeprechauns();

	void init();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

	void addPlayer(unsigned char playerId, const Vector3& playerColor, const Vector3& rainbowPosition);
	void onServerWelcome(unsigned char playerId, const Vector3& playerColor, const Vector3& leprechaunPosition, const Vector3& rainbowPosition);
	void onGameOver(unsigned char winner);

	LLGameLogic& getGameLogic() const { return *gameLogic; }

	TerrainController& getTerrain() { return *terrain; }
	PigController& getPig() { return *pig; }
	PlayerController& getPlayer() { return *player; }
	TrapManager& getTrapManager() { return *trapManager; }

	const MushroomCollection& getMushrooms() const { return mushrooms; }
	const ItemCollection& getItems() const { return items; }

	LeprechaunController* getLeprechaun(unsigned playerId);
protected:
	void onCameraUpdated();
	
private:
	void toggleWireframe();

	LLGameLogic* gameLogic;

	TerrainController* terrain;
	LeprechaunCamera* camera;
	VegetationController* vegetation;
	PlayerController* player;
	PigController* pig;
	TrapManager* trapManager;
	GoldpotController* goldpot;

	MushroomCollection mushrooms;
	ItemCollection items;

	static const char* configPath;

	unsigned buttonToggleWireframe;

	std::vector<Vector3> playerColors;

	unsigned maxPlayerCount;

	bool gameOver;
	GameTime gameOverTime;
};
