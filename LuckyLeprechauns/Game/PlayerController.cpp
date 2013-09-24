#include "PlayerController.h"

PlayerController::PlayerController(LuckyLeprechauns* game) : LLGameComponent(game), playerId(0)
{
	leprechaun = game->addComponent<LeprechaunController>(game, this);
	rainbow = game->addComponent<RainbowController>(game, this);
}


void PlayerController::init()
{
	buttonMoveForward = Button::get(Config::getValue(ConfigKeys::buttonMoveForward));
	buttonMoveBackward = Button::get(Config::getValue(ConfigKeys::buttonMoveBackward));
	buttonMoveLeft = Button::get(Config::getValue(ConfigKeys::buttonMoveLeft));
	buttonMoveRight = Button::get(Config::getValue(ConfigKeys::buttonMoveRight));

	buttonItemSlot1 = Button::get(Config::getValue(LLConfigKeys::buttonItemSlot1));
	buttonItemSlot2 = Button::get(Config::getValue(LLConfigKeys::buttonItemSlot2));
	buttonItemSlot3 = Button::get(Config::getValue(LLConfigKeys::buttonItemSlot3));

	buttonJump = Button::get(Config::getValue(LLConfigKeys::buttonJump));
	buttonDodgeLeft = Button::get(Config::getValue(LLConfigKeys::buttonDodgeLeft));
	buttonDodgeRight = Button::get(Config::getValue(LLConfigKeys::buttonDodgeRight));

	hatMaxChargingTime = Config::getValue<float>(LLConfigKeys::hatMaxChargingTime);
}


void PlayerController::update(const GameTime& gameTime)
{
	if (!isLocal())
		return;

	Vector3 forward = leprechaun->getForward();
	Vector3 right = leprechaun->getRight();

	Vector3 direction;

	if (Keyboard::isKeyDown(buttonMoveForward))
		direction += forward;

	if (Keyboard::isKeyDown(buttonMoveBackward))
		direction -= forward;

	if (Keyboard::isKeyDown(buttonMoveRight))
		direction += right;

	if (Keyboard::isKeyDown(buttonMoveLeft))
		direction -= right;

	leprechaun->setMoving(Vector3::normalize(direction));

	if (Keyboard::isKeyPress(buttonJump))
		leprechaun->jump();
	else if (Keyboard::isKeyPress(buttonDodgeLeft))
		leprechaun->dodgeLeft();
	else if (Keyboard::isKeyPress(buttonDodgeRight))
		leprechaun->dodgeRight();

	if (Keyboard::isKeyPress(buttonItemSlot1))
		leprechaun->useItem(0);
	else if (Keyboard::isKeyPress(buttonItemSlot2))
		leprechaun->useItem(1);
	else if (Keyboard::isKeyPress(buttonItemSlot3))
		leprechaun->useItem(2);

	int mouseWheelDelta = Mouse::getMouseWheelDelta();

	if (mouseWheelDelta)
	{
		int attack = leprechaun->getAttack() + ((mouseWheelDelta > 0) ? 1 : -1);
		leprechaun->setAttack((attack + LeprechaunAttack::count) % LeprechaunAttack::count);
	}

	if (Mouse::isLeftButtonClick())
	{
		if (leprechaun->isCarryingPig())
			leprechaun->throwPig();
		else
		{
			switch (leprechaun->getAttack())
			{
				case LeprechaunAttack::kick: leprechaun->kick(); break;
				case LeprechaunAttack::cane: leprechaun->throwCane(); break;
				case LeprechaunAttack::hat: clickStart = gameTime.total; break;
			}
		}
	}

	if (Mouse::isLeftButtonClickEnd() && clickStart != 0)
	{
		switch (leprechaun->getAttack())
		{
 			case LeprechaunAttack::hat: leprechaun->throwHat((gameTime.total - clickStart) / hatMaxChargingTime); clickStart = 0; break;
		}
	}
}


void PlayerController::setEnabled(bool enabled)
{
	leprechaun->setEnabled(enabled);
	LLGameComponent::setEnabled(enabled);
}


void PlayerController::setLocal(bool local)
{
	leprechaun->setLocal(local);
	LLGameComponent::setLocal(local);
}