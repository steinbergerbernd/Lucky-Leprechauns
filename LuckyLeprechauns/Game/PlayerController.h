#pragma once

#include "LLGameComponent.h"
#include "LeprechaunController.h"
#include "RainbowController.h"

class PlayerController : public LLGameComponent
{
public:

	PlayerController(LuckyLeprechauns* game);

	void init();
	void update(const GameTime& gameTime);

	LeprechaunController& getLeprechaun() { return *leprechaun; }
	const LeprechaunController& getLeprechaun() const { return *leprechaun; }

	RainbowController& getRainbow() { return *rainbow; }
	const RainbowController& getRainbow() const { return *rainbow; }

	unsigned getPlayerId() const { return playerId; }
	void setPlayerId(unsigned playerId) { this->playerId = playerId; }

	const Vector3& getPlayerColor() const { return playerColor; }
	void setPlayerColor(const Vector3& playerColor) { this->playerColor = playerColor; }

	void setEnabled(bool enabled);
	void setLocal(bool local);

private:
	LeprechaunController* leprechaun;
	RainbowController* rainbow;

	unsigned playerId;
	Vector3 playerColor;

	char buttonMoveForward;
	char buttonMoveBackward;
	char buttonMoveLeft;
	char buttonMoveRight;

	char buttonItemSlot1;
	char buttonItemSlot2;
	char buttonItemSlot3;

	char buttonJump;
	char buttonDodgeLeft;
	char buttonDodgeRight;

	float clickStart;

	float hatMaxChargingTime;
};