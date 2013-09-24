#pragma once

#include "LLDrawableGameComponent.h"

#include "HatController.h"
#include "CaneController.h"

#include "LeprechaunAttack.h"
#include "LeprechaunFlag.h"
#include "ItemType.h"
#include "TimerType.h"

class PlayerController;

class LeprechaunController : public LLDrawableGameComponent
{
public:
	typedef std::bitset<LeprechaunFlag::count> Flags;
	typedef std::vector<unsigned> ItemSlots;

	LeprechaunController(LuckyLeprechauns* game, PlayerController* player);

	void init();
	void update(const GameTime& gameTime);

	void onLanding();

	void onKick(const Vector3& flyVelocity);
	void onStoneHit();
	void onCaneHit(float caneHitTime);
	void onCapturePig();
	void onDropPig();

	void onCaneEnd();
	void onHatEnd();

	void kick();
	void throwCane();
	void throwHat(float chargingTime = 0.0f);
	void throwPig();

	void jump();
	void dodgeLeft();
	void dodgeRight();

	bool isBeginKickAnimation() const { return getBeginFlag(LeprechaunFlag::kickAnimation); }
	bool isBeginHatAnimation() const { return getBeginFlag(LeprechaunFlag::hatAnimation); }
	bool isBeginCaneAnimation() const { return getBeginFlag(LeprechaunFlag::caneAnimation); }
	bool isBeginThrowAnimation() const { return getBeginFlag(LeprechaunFlag::throwPigAnimation); }

	bool isBeginKick() const { return getBeginFlag(LeprechaunFlag::kick); }
	bool isBeginCane() const { return getBeginFlag(LeprechaunFlag::cane); }
	bool isBeginHat() const { return getBeginFlag(LeprechaunFlag::hat); }

	bool isBeginStoneHit() const {return getBeginFlag(LeprechaunFlag::stoneHit); }
	bool isBeginCaneHit() const { return getBeginFlag(LeprechaunFlag::caneHit); }
	bool isBeginFly() const { return getBeginFlag(LeprechaunFlag::flying); }
	bool isBeginJumping() const { return getBeginFlag(LeprechaunFlag::jumping); }
	bool isBeginDodgingLeft() const { return getBeginFlag(LeprechaunFlag::dodgingLeft); }
	bool isBeginDodgingRight() const { return getBeginFlag(LeprechaunFlag::dodgingRight); }

	bool isCaneHit() const { return getFlag(LeprechaunFlag::caneHit); }
	bool isJumping() const { return getFlag(LeprechaunFlag::jumping); }
	bool isMoving() const { return getFlag(LeprechaunFlag::moving); }
	bool isFlying() const { return getFlag(LeprechaunFlag::flying); }
	bool isDodgingLeft() const { return getFlag(LeprechaunFlag::dodgingLeft); }
	bool isDodgingRight() const { return getFlag(LeprechaunFlag::dodgingRight); }
	bool isCarryingPig() const { return getFlag(LeprechaunFlag::carryingPig); }
	bool isThrowingPig() const { return getFlag(LeprechaunFlag::throwPig); }
	bool isStoneHit() const { return getFlag(LeprechaunFlag::stoneHit); }

	bool isDetected() const { return detected; }
	void setDetected(bool detected) { this->detected = detected; }

	bool hasSpeedBoost() const { return getFlag(LeprechaunFlag::speedBoost); }
	bool hasInvisibility() const { return getFlag(LeprechaunFlag::invisibility); }
	bool hasInvulnerability() const { return getFlag(LeprechaunFlag::invulnerability); }
	bool hasShowInvisibility() const { return getFlag(LeprechaunFlag::showInvisibility); }
	bool hasAttackDouble() const { return getFlag(LeprechaunFlag::attackDouble); }
	bool hasUsedExplosionTrap() const { return getBeginFlag(LeprechaunFlag::explosionTrap); }
	bool hasUsedItem() const { return usedItem; }
	bool hasCollectedItem() const { return collectedItem; }

	void setInvulnerable(bool invulnerable) { setFlag(LeprechaunFlag::invulnerability, invulnerable); }
	void setInvisible(bool invisible) { setFlag(LeprechaunFlag::invisibility, invisible); }

	float getJumpPower() const { return jumpPower; }
	float getDodgePower() const { return dodgePower; }

	float getMovementPower() const;

	const Flags& getBeginFlags() const { return beginFlags; }
	void setBeginFlags(const Flags& beginFlags);

	const Flags& getFlags() const { return flags; }
	void setFlags(const Flags& flags) { this->flags = flags; }

	HatController& getHat() { return *hat; }
	CaneController& getCane() { return *cane; }

	const HatController& getHat() const { return *hat; }
	const CaneController& getCane() const { return *cane; }

	unsigned getAttack() const { return activeAttack; }
	void setAttack(unsigned activeAttack) { this->activeAttack = activeAttack; }

	const Vector3& getMovingDirection() const { return movingDirection; }
	void setMoving(const Vector3& movingDirection);

	const Vector3& getFlyVelocity() const { return flyVelocity; }
	
	float getHatCharging() const { return hatCharging; }
	float getStoneHitFactor() const { return stoneHitFactor; }

	const ItemSlots& getItems() const { return items; }
	void collectItem(unsigned itemType);
	void useItem(unsigned slot);

	void setLocal(bool local);

	const Vector3& getPlayerColor() const;
	unsigned char getPlayerId() const;
	const PlayerController& getPlayer() const { return *player; }
	PlayerController& getPlayer() { return *player; }

	void addMushroom();
	void removeMushroom();
	unsigned getMushroomCount() const { return mushroomCount; }
private:
	void setFlag(unsigned flag, bool value) { flags[flag] = value; }
	bool getFlag(unsigned flag) const { return flags[flag]; }

	void setBeginFlag(unsigned flag, bool value);
	bool getBeginFlag(unsigned flag) const { return beginFlags[flag]; }

	unsigned mushroomCount;

	unsigned activeAttack;

	float hatCharging;

	float timers[TimerType::count];
	float configTimers[TimerType::count];
	bool timerFinished[TimerType::count];

	float attackDoubleFactor;
	float speedBoostFactor;
	float movementPower;
	float jumpPower;
	float dodgePower;
	float stoneHitFactor;

	bool detected;

	bool usedItem;
	bool collectedItem;
	
	Vector3 movingDirection;
	Vector3 flyVelocity;

	Flags flags;
	Flags beginFlags;

	ItemSlots items;

	PlayerController* player;
	HatController* hat;
	CaneController* cane;
};
