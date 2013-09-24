#include "LeprechaunController.h"
#include "LeprechaunCamera.h"


LeprechaunCamera::LeprechaunCamera() : viewOnly(false)
{
}


void LeprechaunCamera::setLeprechaun(LeprechaunController* leprechaun)
{
	this->leprechaun = leprechaun;
}


void LeprechaunCamera::init()
{
	distance = Config::getValue<float>(LLConfigKeys::cameraDistance);
	offset = Config::getValue<Vector3>(LLConfigKeys::cameraOffset);

	Camera::init();
}


void LeprechaunCamera::update(const GameTime& gameTime)
{
	Camera::update(gameTime);
	Camera::setPosition(getTarget() - getForward() * distance);

	if (!viewOnly)
		leprechaun->setRotation(Quaternion::createFromAxisAngle(Vector3::up, rotationX));
}


Vector3 LeprechaunCamera::getTarget() const
{
	return leprechaun->getPosition() + offset;
}


void LeprechaunCamera::setPosition(const Vector3& position)
{
	Camera::setPosition(position);
	lookAt(getTarget());

	leprechaun->setRotation(Quaternion::createFromAxisAngle(Vector3::up, rotationX));
}