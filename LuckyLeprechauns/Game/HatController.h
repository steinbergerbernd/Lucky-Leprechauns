#pragma once

#include "LLDrawableGameComponent.h"

class LeprechaunController;

class HatController : public LLDrawableGameComponent
{
public:
	HatController(LuckyLeprechauns* game, LeprechaunController* leprechaun);

	void init();
	void update(const GameTime& gameTime);

	bool isOnGround() const { return onGround; };
	void setOnGround(bool onGround) { this->onGround = onGround; }

	void setEnabled(bool enabled);

	const Vector3& getStartOffset() const { return startOffset; }
	const Quaternion& getStartRotation() const { return startRotation; }

	const LeprechaunController& getLeprechaun() const { return *leprechaun; }
	LeprechaunController& getLeprechaun() { return *leprechaun; }
	void setLeprechaun(LeprechaunController* leprechaun) { this->leprechaun = leprechaun; }

	float getThrowMinPower() const { return throwMinPower; }
	float getThrowMaxPower() const { return throwMaxPower; }

	unsigned char getPlayerId() const;

	bool isLanded() const { return onGround && currentHatTime == 0; }
private:
	LeprechaunController* leprechaun;

	float defaultScale;
	float bigScale;
	
	Vector3 startOffset;
	Quaternion startRotation;

	bool onGround;

	float hatTime;
	float currentHatTime;
	float growTime;
	float hatRotation;
	float throwMinPower;
	float throwMaxPower;
	float attackDoubleFactor;
};

