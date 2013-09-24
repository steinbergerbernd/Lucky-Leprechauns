#pragma once

#include "BasicManager.h"

class Camera;
class GameComponent;
class GraphicsManager;
class InputManager;
class GuiManager;
class NetworkManager;
class PhysicsManager;
class SoundManager;

class Game : public BasicManager
{
	typedef std::vector<Camera*> CameraCollection;

public:
	Game();
	virtual ~Game(void) = 0;

	virtual void init();
	virtual void release();

	virtual void update(const GameTime& gameTime);
	virtual void draw(const GameTime& gameTime);

	virtual void onResetDevice(const GraphicsDevice& device);
	virtual void onLostDevice();

	static const GameTime& getGameTime() { return gameTime; }

	const GraphicsDevice& getGraphicsDevice() const;
	void setGraphicsDevice(const GraphicsDevice& device);

	virtual void addPlayer(unsigned char playerId);
	virtual void removePlayer(unsigned char playerId);

	Camera& getCamera() { return *camera; }
	const Camera& getCamera() const { return *camera; }

	GuiManager& getGuiManager() { return *guiManager; }
	const GuiManager& getGuiManager() const { return *guiManager; }

	GraphicsManager& getGraphicsManager() { return *graphicsManager; }
	const GraphicsManager& getGraphicsManager() const { return *graphicsManager; }

	NetworkManager& getNetworkManager() { return *networkManager; }
	const NetworkManager& getNetworkManager() const { return *networkManager; }

	PhysicsManager& getPhysicsManager() { return *physicsManager; }
	const PhysicsManager& getPhysicsManager() const { return *physicsManager; }

	SoundManager& getSoundManager() { return *soundManager; }
	const SoundManager& getSoundManager() const { return *soundManager; }

	template <class T>
	T* addCamera(bool activate = true);

	void toggleCamera(bool copySettings = false);
	void toggleCursor();

	template <class T, class U>
	void setGuiManager(U* owner);

	template <class T, class U>
	void setInputManager(U* owner);

	template <class T, class U>
	void setGraphicsManager(U* owner);

	template <class T, class U>
	void setNetworkManager(U* owner);

	template <class T, class U>
	void setPhysicsManager(U* owner);

	template <class T, class U>
	void setSoundManager(U* owner);
protected:
	virtual void onCameraUpdated();
	
private:
	CameraCollection cameras;

	Camera* camera;
	InputManager* inputManager;
	GraphicsManager* graphicsManager;
	GuiManager* guiManager;
	NetworkManager* networkManager;
	PhysicsManager* physicsManager;
	SoundManager* soundManager;

	static GameTime gameTime;

	template <class T>
	void setPointer(T** destination, T* source);

	static const char* configPath;

	unsigned currentCamera;
};


template <class T>
void Game::setPointer(T** destination, T* source)
{
	if (*destination != source)
		delete *destination;

	*destination = source;
}


template <class T>
T* Game::addCamera(bool activate)
{
	T* camera = new T();

	cameras.push_back(camera);

	if (activate)
	{
		currentCamera = cameras.size() - 1;
		this->camera = camera;
	}

	return camera;
}


template <class T, class U>
void Game::setGraphicsManager(U* owner)
{
	setPointer(&this->graphicsManager, (GraphicsManager*)new T(owner));
}


template <class T, class U>
void Game::setInputManager(U* owner)
{
	setPointer(&this->inputManager, (InputManager*)new T(owner));
}


template <class T, class U>
void Game::setGuiManager(U* owner)
{
	setPointer(&this->guiManager, (GuiManager*)new T(owner));
}


template <class T, class U>
void Game::setNetworkManager(U* owner)
{
	setPointer(&this->networkManager, (NetworkManager*)new T(owner));
}


template <class T, class U>
void Game::setPhysicsManager(U* owner)
{
	setPointer(&this->physicsManager, (PhysicsManager*)new T(owner));
}


template <class T, class U>
void Game::setSoundManager(U* owner)
{
	setPointer(&this->soundManager, (SoundManager*)new T(owner));
}