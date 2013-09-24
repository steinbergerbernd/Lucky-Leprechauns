#pragma once

#include "GameComponent.h"

class DrawableGameComponent : public GameComponent
{
public:
	DrawableGameComponent(Game* game);
	virtual ~DrawableGameComponent() = 0;

	virtual Matrix getWorld() const;

	const Vector3& getPosition() const;
	const Vector3& getScale() const;
	const Quaternion& getRotation() const;
	const Vector3& getVelocity() const;

	void setPosition(const Vector3& position);
	void setScale(const Vector3& scale);
	void setRotation(const Quaternion& rotation);
	void setVelocity(const Vector3& velocity);

	const Vector3 getForward() const;
	const Vector3 getRight() const;
protected:
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;
	Vector3 velocity;
};