#include "GoldpotView.h"


GoldpotView::GoldpotView(LLGraphicsManager* manager, GoldpotController* controller) : LLControlledGraphicsComponent(manager, controller)
{
}


void GoldpotView::init()
{
	model = FBXModelLoader::getModelClone("Goldpot");
}


void GoldpotView::draw(const GameTime& gameTime)
{
	FBXRenderer::draw(model, gameTime, getManager(), getController().getWorld());
}