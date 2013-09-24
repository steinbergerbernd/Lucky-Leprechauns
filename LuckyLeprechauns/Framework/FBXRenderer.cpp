#include "FBXRenderer.h"

#include "GraphicsComponent.h"
#include "GraphicsManager.h"

unsigned long FBXRenderer::prevCullMode;

void FBXRenderer::draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, bool transparency, unsigned long blendFactor, bool cullNone, bool disableLighting)
{
	prepare(model, manager, transparency, blendFactor, cullNone, disableLighting);
	model.draw(manager.getDevice(), gameTime, world * manager.getWorld(), manager.getView(), manager.getProjection(), manager.isRenderShadowMap());
	cleanup(manager, transparency, cullNone);
}


void FBXRenderer::draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, FBXEffect& effect)
{
	prepare(model, manager);
	model.draw(manager.getDevice(), gameTime, world * manager.getWorld(), manager.getView(), manager.getProjection(), effect, manager.isRenderShadowMap());
	cleanup(manager);
}


void FBXRenderer::draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, const std::vector<Matrix>& instanceTransforms, unsigned numInstances)
{
	prepare(model, manager);
	model.draw(manager.getDevice(), gameTime, world * manager.getWorld(), manager.getView(), manager.getProjection(), instanceTransforms, numInstances, manager.isRenderShadowMap());
	cleanup(manager);
}


void FBXRenderer::prepare(FBXModel& model, const GraphicsManager& manager, bool transparency, unsigned long blendFactor, bool cullNone, bool disableLighting)
{
	GraphicsDevice device = manager.getDevice();

	model.setCamera(manager.getCamera());
	model.setClipPlane(manager.getClipPlane());
	model.setLight(manager.getLight());

	if (transparency)
	{
		device.setRenderState(D3DRS_ZWRITEENABLE, false);
		device.setRenderState(D3DRS_ALPHABLENDENABLE, true);
		device.setRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		device.setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
		device.setRenderState(D3DRS_BLENDFACTOR, blendFactor);
	}

	if (cullNone)
	{
		prevCullMode = device.getRenderState(D3DRS_CULLMODE);
		device.setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	if (disableLighting)
		model.setLight(Light(Vector3::zero, manager.getLight().Diffuse, Vector3::zero, Vector3::zero));
}


void FBXRenderer::cleanup(const GraphicsManager& manager, bool transparency, bool cullNone)
{
	GraphicsDevice device = manager.getDevice();

	if (transparency)
	{
		device.setRenderState(D3DRS_ZWRITEENABLE, true);
		device.setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	if (cullNone)
		device.setRenderState(D3DRS_CULLMODE, prevCullMode);
}