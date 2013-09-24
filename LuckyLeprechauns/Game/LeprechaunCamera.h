#pragma once

#include "Camera.h"

class LeprechaunController;

class LeprechaunCamera : public Camera
{
public:
	LeprechaunCamera();

	void init();
	void update(const GameTime& gameTime);

	void setPosition(const Vector3& position);
	void setLeprechaun(LeprechaunController* leprechaun);

	void setViewOnly(bool viewOnly) { this->viewOnly = viewOnly; }

private:
	Vector3 getTarget() const;

	LeprechaunController* leprechaun;

	Vector3 offset;
	float distance;

	bool viewOnly;
};