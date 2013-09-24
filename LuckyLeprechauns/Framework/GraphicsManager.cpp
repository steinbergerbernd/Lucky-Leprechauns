#include "GraphicsManager.h"
#include "GraphicsComponent.h"

#undef max
#undef min


GraphicsManager::GraphicsManager(Game* game) : BasicComponent(game), shadowMapEnabled(false), renderShadowMap(false)
{
}


GraphicsManager::~GraphicsManager()
{
}


void GraphicsManager::init()
{
	BasicManager::init();
}


void GraphicsManager::release()
{
	BasicManager::release();
}


void GraphicsManager::update(const GameTime& gameTime)
{
	Camera& camera = getCamera();

	camera.setAspectRatio(device.getViewport().getAspectRatio());

	setView(camera.getView());
	setProjection(camera.getProjection());

	BasicManager::update(gameTime);
}


void GraphicsManager::draw(const GameTime& gameTime)
{
	draw(gameTime, components);
}


void GraphicsManager::draw(const GameTime& gameTime, ComponentCollection& components)
{
	device.setWorld(getWorld());
	device.setView(getView());
	device.setProjection(getProjection());

	device.clear();
	device.beginScene();

	for (unsigned pass = 0; pass < 2; ++pass)
	{
		bool renderOpaque = pass == 0;

		for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		{
			GraphicsComponent& component = (GraphicsComponent&)**it;

			if (!component.isEnabled() || component.hasTransparency() == renderOpaque)
				continue;

			//StateBlock state = device.createStateBlock();

			component.draw(gameTime);
		
			//state.apply();
		}
	}

	device.endScene();
}