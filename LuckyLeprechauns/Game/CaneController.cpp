#include "CaneController.h"
#include "LeprechaunController.h"

CaneController::CaneController(LuckyLeprechauns* game, LeprechaunController* leprechaun) 
	: LLDrawableGameComponent(game), currentCaneTime(0), throwPower(0), leprechaun(leprechaun)
{
}


void CaneController::init()
{
	defaultScale = Config::getValue<float>(LLConfigKeys::leprechaunScale);
	flyScale = Config::getValue<float>(LLConfigKeys::caneFlyScale) * defaultScale;
	attackDoubleFactor = Config::getValue<float>(LLConfigKeys::caneAttackDoubleFactor);

	growTime = Config::getValue<float>(LLConfigKeys::caneGrowTime);

	Vector3 rotation = Config::getValue<Vector3>(LLConfigKeys::hatStartRotation);
	Vector3 offset = Config::getValue<Vector3>(LLConfigKeys::hatStartOffset);

	startOffset =  offset * defaultScale;
	startRotation = Quaternion::createFromMatrix(Matrix::createRotation(rotation));

	caneTime = Config::getValue<float>(LLConfigKeys::caneTime);
	caneRotation = Config::getValue<float>(LLConfigKeys::caneDegreesPerSecond) * D3DX_PI / 180;

	throwPower = Config::getValue<float>(LLConfigKeys::caneThrowPower);

	setScale(defaultScale);
}


void CaneController::update(const GameTime& gameTime)
{
	if (!isLocal())
		return;

	if ((currentCaneTime += gameTime.elapsed) >= caneTime)
		getLeprechaun().onCaneEnd();

	float targetScale = flyScale * (getLeprechaun().hasAttackDouble() ? attackDoubleFactor : 1.0f);
	setScale(MathHelper::lerp(defaultScale, targetScale, MathHelper::saturate(currentCaneTime / growTime)));
	setRotation(Quaternion::createFromAxisAngle(Vector3::up, caneRotation * gameTime.total));
}


void CaneController::setEnabled(bool enabled)
{
	if (enabled == isEnabled())
		return;

	currentCaneTime = 0.0f;
	setScale(defaultScale);

	LLDrawableGameComponent::setEnabled(enabled);
}


unsigned char CaneController::getPlayerId() const
{
	return leprechaun->getPlayerId();
}