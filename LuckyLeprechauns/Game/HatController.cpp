#include "HatController.h"
#include "LeprechaunController.h"


HatController::HatController(LuckyLeprechauns* game, LeprechaunController* leprechaun) 
	: LLDrawableGameComponent(game), currentHatTime(0), onGround(false), leprechaun(leprechaun)
{
}


void HatController::init()
{
	defaultScale = Config::getValue<float>(LLConfigKeys::leprechaunScale);
	bigScale = Config::getValue<float>(LLConfigKeys::hatBigScale) * defaultScale;
	attackDoubleFactor = Config::getValue<float>(LLConfigKeys::hatAttackDoubleFactor);

	hatTime = Config::getValue<float>(LLConfigKeys::hatTime);
	growTime = Config::getValue<float>(LLConfigKeys::hatGrowTime);

	Vector3 rotation = Config::getValue<Vector3>(LLConfigKeys::hatStartRotation);
	Vector3 offset = Config::getValue<Vector3>(LLConfigKeys::hatStartOffset);

	startOffset =  offset * defaultScale;
	startRotation = Quaternion::createFromMatrix(Matrix::createRotation(rotation));

	hatRotation = Config::getValue<float>(LLConfigKeys::hatDegreesPerSecond) * D3DX_PI / 180;

	throwMinPower = Config::getValue<float>(LLConfigKeys::hatThrowMinPower);
	throwMaxPower = Config::getValue<float>(LLConfigKeys::hatThrowMaxPower);
	
	setScale(defaultScale);
}


void HatController::update(const GameTime& gameTime)
{
	if (!isLocal())
		return;

	if (!onGround)
		setRotation(Quaternion::createFromAxisAngle(Vector3::up, hatRotation * gameTime.total));
	else
	{
		currentHatTime += gameTime.elapsed;

		if (currentHatTime >= hatTime)
			getLeprechaun().onHatEnd();

		float targetScale = bigScale * (getLeprechaun().hasAttackDouble() ? attackDoubleFactor : 1.0f);
		setScale(MathHelper::lerp(defaultScale, targetScale, MathHelper::saturate(currentHatTime / growTime)));
	}
}


void HatController::setEnabled(bool enabled)
{
	if (enabled == isEnabled())
		return;

	onGround = false;
	currentHatTime = 0.0f;
	setScale(defaultScale);

	LLDrawableGameComponent::setEnabled(enabled);
}


unsigned char HatController::getPlayerId() const
{
	return leprechaun->getPlayerId();
}