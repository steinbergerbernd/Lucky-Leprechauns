#include "CaneView.h"


CaneView::CaneView(LLGraphicsManager* manager, CaneController* controller) : LLControlledGraphicsComponent(manager, controller)
{
}


void CaneView::init()
{
	model = FBXModelLoader::getModelClone("Cane");
}


void CaneView::draw(const GameTime& gameTime)
{
	const CaneController& controller = getController();

	if (!controller.isEnabled())
		return;

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld());
}