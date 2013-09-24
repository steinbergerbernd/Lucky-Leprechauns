#pragma once

#include "LLDrawableGameComponent.h"

#include "StoneController.h"
#include "LeprechaunController.h"

class MushroomController : public LLDrawableGameComponent
{
public:
	MushroomController(LuckyLeprechauns* game);

	void update(const GameTime& gameTime);

	bool isMoving() const { return velocity.lengthSquared() > 0; }
	bool isAttacking() const { return attacking; }

	void setAttacking(bool attacking) { this->attacking = attacking; }

	StoneController& getStone() { return *stone; }
	const StoneController& getStone() const { return *stone; }

	const LeprechaunController* getOwner() const { return owner; }
	LeprechaunController* getOwner() { return owner; }
	void setOwner(unsigned char playerId);
	void clearOwner() { owner = 0; }
	bool isOwned() const { return owner != 0; }

	float getRange() const { return range; }
	void setRange(float range) { this->range = range; }

	unsigned getLevel() const { return level; }
	void setLevel(unsigned level);

	bool isOvertaken() const { return overtaken; }

private:
	bool attacking;
	float range;
	bool overtaken;
	unsigned level;

	StoneController* stone;
	LeprechaunController* owner;
};