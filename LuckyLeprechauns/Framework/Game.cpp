#include "Game.h"

#include "GameComponent.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "GuiManager.h"
#include "NetworkManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "Camera.h"

const char* Game::configPath = "Content/Config/Config.txt";

GameTime Game::gameTime;

Game::Game() : currentCamera(0)
{
	graphicsManager = new GraphicsManager(this);
	inputManager = new InputManager(this);
	guiManager = new GuiManager(this);
	networkManager = new NetworkManager(this);
	physicsManager = new PhysicsManager(this);
	soundManager = new SoundManager(this);

	addCamera<Camera>();
}


Game::~Game(void)
{	
	for (CameraCollection::iterator it = cameras.begin(); it != cameras.end(); ++it)
		delete *it;

	delete inputManager;
	delete graphicsManager;
	delete networkManager;
	delete guiManager;
	delete physicsManager;
	delete soundManager;
}


void Game::init()
{
	Config::addFile(configPath);
	Logging::init();

	inputManager->init();

	for (CameraCollection::iterator it = cameras.begin(); it != cameras.end(); ++it)
		(*it)->init();

	BasicManager::init();

	soundManager->init();
	physicsManager->init();
	graphicsManager->init();
	guiManager->init();
	networkManager->init();
}


void Game::release()
{
	guiManager->release();
	graphicsManager->release();
	physicsManager->release();
	soundManager->release();
	networkManager->release();

	BasicManager::release();

	for (CameraCollection::iterator it = cameras.begin(); it != cameras.end(); ++it)
		(*it)->release();
	
	inputManager->release();
}


void Game::update(const GameTime& gameTime)
{
	this->gameTime = gameTime;

	inputManager->update(gameTime);

	BasicManager::update(gameTime);

	networkManager->update(gameTime);
	physicsManager->update(gameTime);
	soundManager->update(gameTime);

	getCamera().update(gameTime);
	onCameraUpdated();

	graphicsManager->update(gameTime);
	guiManager->update(gameTime);
}


void Game::onCameraUpdated()
{
}


void Game::draw(const GameTime& gameTime)
{
	graphicsManager->draw(gameTime);
	guiManager->draw(gameTime);
}


void Game::addPlayer(unsigned char playerId)
{
	BasicManager::addPlayer(playerId);

	networkManager->addPlayer(playerId);
	graphicsManager->addPlayer(playerId);
	physicsManager->addPlayer(playerId);
	soundManager->addPlayer(playerId);
}


void Game::removePlayer(unsigned char playerId)
{
	BasicManager::removePlayer(playerId);

	networkManager->removePlayer(playerId);
	graphicsManager->removePlayer(playerId);
	physicsManager->removePlayer(playerId);
	soundManager->removePlayer(playerId);
}


const GraphicsDevice& Game::getGraphicsDevice() const
{
	return graphicsManager->getDevice();
}


void Game::setGraphicsDevice(const GraphicsDevice& device)
{
	graphicsManager->setDevice(device);
}


void Game::toggleCamera(bool copySettings)
{
	Camera* prevCamera = camera;

	currentCamera = (currentCamera + 1) % cameras.size();
	camera = cameras[currentCamera]; 

	if (copySettings)
	{
		camera->setPosition(prevCamera->getPosition());
		camera->look(prevCamera->getForward());
	}
}


void Game::toggleCursor()
{
	Mouse::setVisible(!Mouse::isVisible());
	Mouse::setPosition(Window::getCenter());
}


void Game::onResetDevice(const GraphicsDevice& device)
{
	inputManager->onResetDevice(device);
	graphicsManager->onResetDevice(device);
	guiManager->onResetDevice(device);
	physicsManager->onResetDevice(device);
	soundManager->onResetDevice(device);
}


void Game::onLostDevice()
{
	inputManager->onLostDevice();
	graphicsManager->onLostDevice();
	guiManager->onLostDevice();
	physicsManager->onLostDevice();
	soundManager->onLostDevice();
}
