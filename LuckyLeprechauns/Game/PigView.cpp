#include "PigView.h"
#include "PlayerController.h"
#include "TerrainController.h"


PigView::PigView(LLGraphicsManager* manager, PigController* controller) : LLControlledGraphicsComponent(manager, controller)
{
}


void PigView::init()
{
	model = FBXModelLoader::getModelClone("Pig");
}


void PigView::draw(const GameTime& gameTime)
{
	const PigController& controller = getController();

	if (!controller.isEnabled() || controller.getCaptor() != 0)
		return;

	const Vector3& position = controller.getPosition();

	if (position.y > 0.0f && getManager().getClipPlane().b < 0.0f)
		return;

	model.setAnimation(controller.isMoving() ? AnimationKeys::pigRun : AnimationKeys::pigIdle);
	
	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld());
}