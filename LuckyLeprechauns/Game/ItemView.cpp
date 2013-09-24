#include "ItemView.h"


ItemView::ItemView(LLGraphicsManager* manager, const ItemCollection* items) : LLGraphicsComponent(manager), items(*items)
{
}


void ItemView::init()
{
	model = FBXModelLoader::getModelClone("Item");

	instances.resize(1024);
}


void ItemView::draw(const GameTime& gameTime)
{
	unsigned count = 0;

	for (ItemCollection::iterator it = items.begin(); it != items.end(); ++it)
		if ((*it)->isEnabled())
			instances[count++] = (*it)->getWorld();

	if (count > 0)
	{
		model.setAnimation(AnimationKeys::itemIdle);
		FBXRenderer::draw(model, gameTime, getManager(), Matrix::identity, instances, count);
	}
}