#include "WaterView.h"
#include "TerrainView.h"

WaterView::WaterView(LLGraphicsManager* manager, TerrainView* terrain) : LLGraphicsComponent(manager), terrain(terrain)
{
}

void WaterView::init()
{
	GraphicsDevice device = getManager().getDevice();

	const TerrainController& controller = terrain->getController();

	unsigned terrainWidth = controller.getWidth();
	unsigned terrainLength = controller.getLength();

	unsigned waterWidth = Config::getValue<unsigned>(LLConfigKeys::terrainWaterWidth);
	unsigned waterLength = Config::getValue<unsigned>(LLConfigKeys::terrainWaterLength);

	Vector3 waterOffset = controller.getPosition() + Vector3(((int)terrainWidth - (int)waterWidth) / 2.0f, 0, ((int)terrainLength - (int)waterLength) / 2.0f);

	reflectionPlane = Plane::createFromPointNormal(Vector3::zero, Vector3::up);
	refractionPlane = Plane::createFromPointNormal(Vector3::zero, Vector3::down);

	effect = Effect::createFromFile<TerrainWaterEffect>(device, Config::getValue(LLConfigKeys::terrainWaterEffectPath));

	Texture wavemap = Texture::createFromFile(device, Config::getValue(LLConfigKeys::terrainWavemapPath));

	float waveLength = Config::getValue<float>(LLConfigKeys::terrainWaveLength);
	float waveHeight = Config::getValue<float>(LLConfigKeys::terrainWaveHeight);
	float waveSpeed = Config::getValue<float>(LLConfigKeys::terrainWaveSpeed);

	effect.setInt("WaterWidth", waterWidth);
	effect.setInt("WaterLength", waterLength);
	effect.setVector("WaterOffset", waterOffset);

	effect.setInt("PatchSize", Config::getValue<unsigned>(LLConfigKeys::terrainPatchSize));

	effect.setWavemap(wavemap);
	effect.setWaveLength(Vector2(waveLength / waterWidth, waveLength / waterLength));
	effect.setWaveHeight(Vector2(waveHeight, waveHeight));
	effect.setWaveSpeed(Vector2(waveSpeed / waterWidth, waveSpeed / waterLength));
}


void WaterView::draw(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();
	GraphicsDevice device = manager.getDevice();

	Surface renderTarget = device.getRenderTarget();

	if (renderTarget == reflectionSurface || renderTarget == refractionSurface || manager.isRenderShadowMap())
		return;

	effect.setWorldViewProjection(manager.getWorldViewProjection());

	effect.setReflectionmap(reflectionmap);
	effect.setRefractionmap(refractionmap);

	effect.setTime(gameTime.total);
	effect.setCameraPosition(manager.getCamera().getPosition());

	effect.beginSinglePass();

	terrain->getTessellator().drawPatch(device);

	effect.endSinglePass();
}


void WaterView::onLostDevice()
{
	reflectionSurface.release();
	refractionSurface.release();

	reflectionmap.release();
	refractionmap.release();

	zBuffer.release();

	effect.onLostDevice();
}


void WaterView::onResetDevice(const GraphicsDevice& device)
{
	Viewport viewport = device.getViewport();

	reflectionmap = Texture::create(device, viewport.Width, viewport.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT);
	refractionmap = Texture::create(device, viewport.Width, viewport.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT);
	
	reflectionSurface = reflectionmap.getSurfaceLevel();
	refractionSurface = refractionmap.getSurfaceLevel();

	zBuffer = device.createDepthStencilSurface(viewport.Width, viewport.Height, D3DFMT_D24X8);

	effect.onResetDevice();
}