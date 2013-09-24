#include "InputManager.h"


InputManager::InputManager(Game* game) : GameComponent(game), resetDevice(false)
{
}


void InputManager::init()
{
	Mouse::init();
	Keyboard::init();

	BasicManager::init();
}


void InputManager::release()
{
	BasicManager::release();
}


void InputManager::update(const GameTime& gameTime)
{
	if (resetDevice)
	{
		resetDevice = false;

		if (!Mouse::isVisible())
			Mouse::setPosition(Window::getCenter());
	}

	Keyboard::update();
	Mouse::update();
}


void InputManager::onResetDevice(const GraphicsDevice& device)
{
	BasicManager::onResetDevice(device);

	resetDevice = true;
}


void InputManager::onLostDevice()
{
	BasicManager::onLostDevice();
}