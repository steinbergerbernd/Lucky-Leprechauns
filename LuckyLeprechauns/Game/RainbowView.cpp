#include "RainbowView.h"

RainbowView::RainbowView(LLGraphicsManager* manager, RainbowController* controller) : LLControlledGraphicsComponent(manager, controller)
{
}


void RainbowView::init()
{
	model = FBXModelLoader::getModelClone("Rainbow");
	effect = Effect::createFromFile<FBXEffect>(getManager().getDevice(), Config::getValue(LLConfigKeys::rainbowEffectPath));
}


void RainbowView::draw(const GameTime& gameTime)
{
	if (getManager().isRenderShadowMap())
		return;

	const RainbowController& controller = getController();

	if (!controller.isEnabled())
		return;

	model.setKeyColor(Vector3(0, 0, 1));
	model.setKeyColorEnabled(true);
	model.setKeyColorReplace(controller.getPlayerColor());

	effect.setFloat("Progress", controller.getProgress() / 100.0f);

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld(), effect);
}


void RainbowView::onLostDevice()
{
	effect.onLostDevice();
	LLControlledGraphicsComponent::onLostDevice();
}


void RainbowView::onResetDevice(const GraphicsDevice& device)
{
	effect.onResetDevice();
	LLControlledGraphicsComponent::onResetDevice(device);
}