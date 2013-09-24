#include "VegetationView.h"


VegetationView::VegetationView(GraphicsManager* graphicsManager, VegetationController* controller) 
	: LLControlledGraphicsComponent(graphicsManager, controller)
{
}


void VegetationView::init()
{
	VegetationController::Vegetation vegetation = getController().getVegetation();

	for (VegetationController::Vegetation::iterator it = vegetation.begin(); it != vegetation.end(); ++it)
	{
		VegetationObject object;

		object.model = FBXModel::createFromFile(getManager().getDevice(), it->modelPath, it->transforms.size());
		object.model.setAlphaCutoff(it->alphaCutoff);
		object.transforms = it->transforms;

		this->vegetation.push_back(object);
	}
}


void VegetationView::update(const GameTime& gameTime)
{
}


void VegetationView::draw(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();
	const VegetationController& controller = getController();

	GraphicsDevice device = manager.getDevice();

	unsigned long prevCullMode = device.getRenderState(D3DRS_CULLMODE);

	device.setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Matrix world = controller.getWorld() * manager.getWorld();
	const Matrix& view = manager.getView();
	const Matrix& projection = manager.getProjection();

	for (Vegetation::iterator it = vegetation.begin(); it != vegetation.end(); ++it)
	{
		FBXModel& model = it->model;

		model.setClipPlane(manager.getClipPlane());
		model.setCamera(manager.getCamera());
		model.setLight(manager.getLight());
	
		model.draw(device, gameTime, world, view, projection, it->transforms, it->transforms.size(), manager.isRenderShadowMap());
	}

	device.setRenderState(D3DRS_CULLMODE, prevCullMode);
}


void VegetationView::onResetDevice(const GraphicsDevice& device)
{
	for (Vegetation::iterator it = vegetation.begin(); it != vegetation.end(); ++it)
		it->model.onResetDevice(device);
}


void VegetationView::onLostDevice()
{
	for (Vegetation::iterator it = vegetation.begin(); it != vegetation.end(); ++it)
		it->model.onLostDevice();
}