#include "BubbleView.h"

BubbleView::BubbleView(LLGraphicsManager* manager, LeprechaunController* controller) : LLControlledGraphicsComponent(manager, controller)
{
	setTransparency(true);
}


void BubbleView::init()
{
	model = FBXModelLoader::getModelClone("Bubble");
}


void BubbleView::draw(const GameTime& gameTime)
{
	const LeprechaunController& controller = getController();

	if (!controller.isEnabled() || !controller.hasInvulnerability() || (!controller.isLocal() && controller.hasInvisibility() && !controller.isDetected()))
		return;

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld(), true, 0x555555, false);
}