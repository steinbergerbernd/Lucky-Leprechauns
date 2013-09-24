#pragma once

#include "LLDrawableGameComponent.h"

class LeprechaunController;

class CaneController : public LLDrawableGameComponent
{
public:
	CaneController(LuckyLeprechauns* game, LeprechaunController* leprechaun);

	virtual void init();
	virtual void update(const GameTime& gameTime);

	void setEnabled(bool enabled);

	const LeprechaunController& getLeprechaun() const { return *leprechaun; }
	LeprechaunController& getLeprechaun() { return *leprechaun; }

	const Vector3& getStartOffset() const { return startOffset; }
	const Quaternion& getStartRotation() const { return startRotation; }

	float getThrowPower() const { return throwPower; }

	unsigned char getPlayerId() const;

private:
	LeprechaunController* leprechaun;

	float defaultScale;
	float flyScale;

	float currentCaneTime;
	float caneTime;
	float caneRotation;
	float growTime;
	float throwPower;
	float attackDoubleFactor;

	Vector3 startOffset;
	Quaternion startRotation;
};

