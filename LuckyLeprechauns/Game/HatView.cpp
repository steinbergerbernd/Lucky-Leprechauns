#include "HatView.h"
#include "LeprechaunController.h"


HatView::HatView(LLGraphicsManager* manager, HatController* controller) : LLControlledGraphicsComponent(manager, controller)
{
	setTransparency(true);
}


void HatView::init()
{
	model = FBXModelLoader::getModelClone("Hat");
	model.setKeyColorEnabled(true);
	model.setKeyColor(Config::getValue<Vector3>(LLConfigKeys::leprechaunKeyColor));
}


void HatView::draw(const GameTime& gameTime)
{
	const HatController& controller = getController();

	if (!controller.isEnabled())
		return;

	const Vector3& position = controller.getPosition();

	if (position.y > 0.0f && getManager().getClipPlane().b < 0.0f)
		return;

	model.setKeyColorReplace(controller.getLeprechaun().getPlayerColor());

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld(), true, 0x888888, true, true);
}