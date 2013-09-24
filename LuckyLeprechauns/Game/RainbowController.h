#pragma once

#include "LLDrawableGameComponent.h"

class PlayerController;

class RainbowController : public LLDrawableGameComponent
{
public:
	RainbowController(LuckyLeprechauns* game, PlayerController* player);

	void init();

	float getProgress() const { return progress; }
	unsigned getProgressInt() const { return (unsigned)std::floor(progress + 0.5f); }
	void setProgress(float progress) { this->progress = progress; }

	unsigned char getPlayerId() const;

	void setPosition(const Vector3& position);
	const Vector3& getPlayerColor() const;

private:
	PlayerController* player;

	float progress;

	Vector3 goldpotPosition;
	float goldpotScale;
	float goldpotRadius;
};