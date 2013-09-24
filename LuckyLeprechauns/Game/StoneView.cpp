#include "StoneView.h"
#include "MushroomController.h"

StoneView::StoneView(LLGraphicsManager* manager, const StoneController* controller) : LLControlledGraphicsComponent(manager, controller)
{
}


void StoneView::init()
{
	model = FBXModelLoader::getModelClone("Stone");
}


void StoneView::draw(const GameTime& gameTime)
{
	const StoneController& controller = getController();

	if (!controller.isEnabled())
		return;

	FBXModel::MeshMap& meshes = model.getMeshes();

	const MushroomController& mushroom = controller.getMushroom();

	meshes["Stone"].color = (mushroom.isOwned()) ? mushroom.getOwner()->getPlayerColor() : Vector3::one;

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld());
}