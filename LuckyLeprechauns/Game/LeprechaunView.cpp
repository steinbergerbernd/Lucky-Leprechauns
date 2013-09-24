#include "LeprechaunView.h"


LeprechaunView::LeprechaunView(LLGraphicsManager* manager, LeprechaunController* controller)
	: LLControlledGraphicsComponent(manager, controller)
{
	hat = manager->addComponent<HatView>(manager, &controller->getHat());
	cane = manager->addComponent<CaneView>(manager, &controller->getCane());
	bubble = manager->addComponent<BubbleView>(manager, controller);
	eye = manager->addComponent<EyeView>(manager, controller);
}


void LeprechaunView::init()
{
	model = FBXModelLoader::getModelClone("Leprechaun");

	model.setKeyColorEnabled(true);
	model.setKeyColor(Config::getValue<Vector3>(LLConfigKeys::leprechaunKeyColor));
}


void LeprechaunView::update(const GameTime& gameTime)
{
	setTransparency(getController().hasInvisibility());
}


void LeprechaunView::draw(const GameTime& gameTime)
{
	const LeprechaunController& controller = getController();

	if (!controller.isEnabled())
		return;

	GraphicsManager& manager = getManager();
	const GraphicsDevice& device = manager.getDevice();
	const Vector3& position = controller.getPosition();

	if (position.y > 0.0f && manager.getClipPlane().b < 0.0f)
		return;

	if (!controller.isLocal() && controller.hasInvisibility() && !controller.isDetected())
		return;

	bool moving = controller.isMoving();

	std::string animation = getCurrentAnimation();

	if (!animation.empty())
		model.setAnimation(animation);

	bool force = (model.getAnimationName() == AnimationKeys::leprechaunFly && !controller.isFlying()) ||
		(model.getAnimationName() == AnimationKeys::leprechaunCaneHit && !controller.isCaneHit());
	
	model.setAnimation(moving ? AnimationKeys::leprechaunRun : AnimationKeys::leprechaunIdle, force);

	FBXAnimation& fbxAnimation = model.getAnimation(model.getAnimationName());

	fbxAnimation.currentFps = fbxAnimation.originalFps;

	if (controller.isStoneHit())
		fbxAnimation.currentFps = (unsigned)(fbxAnimation.currentFps / controller.getStoneHitFactor());

	FBXModel::MeshMap& meshes = model.getMeshes();

	meshes["Cane"].visible = !controller.getCane().isEnabled();
	meshes["Hat"].visible = !controller.getHat().isEnabled();
	meshes["Pig"].visible = controller.isCarryingPig();

	Vector3 replaceColor = controller.getPlayerColor();

	if (controller.hasAttackDouble())
		replaceColor = Vector3::lerp(replaceColor, Vector3(1, 1, 0), (std::sin(gameTime.total * 5.0f) + 1.0f) / 2.0f);

	model.setKeyColorReplace(replaceColor);

	bool transparency = controller.hasInvisibility();

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld(), transparency, 0x333333, false, transparency);
}


std::string LeprechaunView::getCurrentAnimation() const
{
	const LeprechaunController& leprechaun = getController();

	bool moving = leprechaun.isMoving();

	if (leprechaun.isFlying())
		return AnimationKeys::leprechaunFly;

	if (leprechaun.isCaneHit())
		return AnimationKeys::leprechaunCaneHit;
	
	if (leprechaun.isBeginKickAnimation())
		return (moving ? AnimationKeys::leprechaunKickRunning : AnimationKeys::leprechaunKickStanding);
	
	if (leprechaun.isBeginCaneAnimation())
		return (moving ? AnimationKeys::leprechaunCaneRunning : AnimationKeys::leprechaunCaneStanding);
	
	if (leprechaun.isBeginHatAnimation())
		return (moving ? AnimationKeys::leprechaunHatRunning : AnimationKeys::leprechaunHatStanding);

	if (leprechaun.isBeginThrowAnimation())
		return (moving ? AnimationKeys::leprechaunPigThrowRunning : AnimationKeys::leprechaunPigThrowStanding);
	
	if (leprechaun.isBeginJumping())
		return (AnimationKeys::leprechaunJump);
	
	if (leprechaun.isBeginDodgingLeft())
		return (AnimationKeys::leprechaunDodgeLeft);
	
	if (leprechaun.isBeginDodgingRight())
		return (AnimationKeys::leprechaunDodgeRight);
	
	if (leprechaun.isCarryingPig())
		return (moving ? AnimationKeys::leprechaunPigCarryRunning : AnimationKeys::leprechaunPigCarryStanding);

	return std::string();
}
