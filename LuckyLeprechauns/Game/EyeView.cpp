#include "EyeView.h"

EyeView::EyeView(LLGraphicsManager* manager, LeprechaunController* controller) 
	: LLControlledGraphicsComponent(manager, controller), specialLight(false)
{
}


void EyeView::init()
{
	model = FBXModelLoader::getModelClone("Eye");
}


void EyeView::update(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();
	const LeprechaunController& controller = getController();

	if (!controller.isLocal())
		return;

	if (!specialLight && controller.hasShowInvisibility())
	{
		lightCopy = manager.getLight();
		manager.setLight(Light(lightCopy.Direction, lightCopy.Ambient, Vector4(1, 0, 1), lightCopy.Specular));
		specialLight = true;
	}
	else if (specialLight && !controller.hasShowInvisibility())
	{
		manager.setLight(lightCopy);
		specialLight = false;
	}
}


void EyeView::draw(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();
	const LeprechaunController& controller = getController();

	if (!controller.isEnabled() || !controller.hasShowInvisibility() || (!controller.isLocal() && controller.hasInvisibility() && !controller.isDetected()))
		return;

	Matrix world = Matrix::createFromAxisAngle(Vector3::up, gameTime.total) * controller.getWorld();

	FBXRenderer::draw(model, gameTime, getManager(), world, false, 0, true, true);
}