#include "SkyboxView.h"

SkyboxView::SkyboxView(GraphicsManager* graphicsManager) : LLGraphicsComponent(graphicsManager)
{
}


void SkyboxView::init()
{
	model = FBXModelLoader::getModelClone("Skybox");
}


void SkyboxView::draw(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();

	if (manager.isRenderShadowMap())
		return;

	Camera& camera = manager.getCamera();
	GraphicsDevice device = manager.getDevice();
	
	Vector3 translation, scale;
	Quaternion rotation;

	manager.getView().decompose(&scale, &rotation, &translation);

	device.setView(Matrix::createScale(scale) * Matrix::createFromQuaternion(rotation));
	device.setProjection(camera.getProjection(0.1f, 100.0f));

	device.setRenderState(D3DRS_AMBIENT, (D3DXCOLOR)manager.getLight().Diffuse);
	device.setRenderState(D3DRS_ZWRITEENABLE, false);

	device.setSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	device.setSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	const FBXMesh& mesh = model.getMeshes().begin()->second;

	device.setMaterial(mesh.materials[0].material);
	device.setTexture(mesh.materials[0].texture);

	mesh.mesh.drawSubset(0);

	device.setRenderState(D3DRS_ZWRITEENABLE, true);
}