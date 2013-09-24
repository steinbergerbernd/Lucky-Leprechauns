#include "LLGraphicsManager.h"
#include "Views.h"

#undef max
#undef min

LLGraphicsManager::LLGraphicsManager(LuckyLeprechauns* game) 
	: GraphicsManager(game), skybox(0), terrain(0), vegetation(0), player(0), pig(0)
{
	skybox = BasicManager::addComponent<SkyboxView>(this);
	terrain = addComponent<TerrainView>(this, game->terrain);
	vegetation = addComponent<VegetationView>(this, game->vegetation);
	player = addComponent<PlayerView>(this, game->player);
	pig = addComponent<PigView>(this, game->pig);
	items = addComponent<ItemView>(this, &game->items);
	traps = addComponent<TrapView>(this, game->trapManager);
	goldpot = addComponent<GoldpotView>(this, game->goldpot);

	for (ComponentCollection::iterator it = game->remotePlayerPool.begin(); it != game->remotePlayerPool.end(); ++it)
	{
		PlayerView* player = addComponent<PlayerView>(this, (PlayerController*)*it, false, false);

		remotePlayerPool.push_back(player);

		refractionComponents.push_back(&player->getLeprechaun());
		refractionComponents.push_back(&player->getLeprechaun().getHat());
	}

	for (LuckyLeprechauns::MushroomCollection::iterator it = game->mushrooms.begin(); it != game->mushrooms.end(); ++it)
		mushrooms.push_back(addComponent<MushroomView>(this, *it));

	refractionComponents.push_back(terrain);
	refractionComponents.push_back(pig);
	refractionComponents.push_back(&player->getLeprechaun());
	refractionComponents.push_back(&player->getLeprechaun().getHat());

	shadowComponents.push_back(&player->getLeprechaun());
}


void LLGraphicsManager::init()
{
	FBXModelLoader::init(Config::getValue(LLConfigKeys::modelLoaderConfigPath), getDevice());

	GraphicsManager::init();

	Camera& camera = getCamera();

	nearPlaneDistance = camera.getNearPlaneDistance();
	farPlaneDistance = camera.getFarPlaneDistance();

	Light light;

	light.Direction = Config::getValue<Vector3>(LLConfigKeys::lightDirection);
	light.Ambient = Config::getValue<Vector3>(LLConfigKeys::lightAmbient);
	light.Diffuse = Config::getValue<Vector3>(LLConfigKeys::lightDiffuse);
	light.Specular = Config::getValue<Vector3>(LLConfigKeys::lightSpecular);

	setLight(light);

	setShadowMapEnabled(true);

	getDevice().setRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
}


void LLGraphicsManager::release()
{
	FBXModelLoader::release();
}


void LLGraphicsManager::update(const GameTime& gameTime)
{
	GraphicsManager::update(gameTime);

	if (!terrain)
		return;

	Camera& camera = getCamera();
	const TerrainController& terrainController = terrain->getController();

	float height = camera.getPosition().y - (1.0f + terrainController.getOffset()) * 2.0f * terrainController.getBumpiness();
	float distance = camera.getPosition().length() + std::max(terrainController.getWidth(), terrainController.getLength());

	camera.setNearPlaneDistance(std::max(height * 0.5f, nearPlaneDistance));
	camera.setFarPlaneDistance(std::max(distance, farPlaneDistance));
}


void LLGraphicsManager::draw(const GameTime& gameTime)
{
	LuckyLeprechauns& game = ((LuckyLeprechauns&)getManager());
	GraphicsDevice device = getDevice();

	// Render Shadow Map
	Vector3 leprechaunPosition = game.getPlayer().getLeprechaun().getPosition();
	Vector3 lightPosition =  leprechaunPosition - Vector3(getLight().Direction) * 5.0f;
	
	setLightView(Matrix::createLookAt(lightPosition, leprechaunPosition, Vector3::up));
	setLightProjection(Matrix::createOrthographic(8, 8, 1, 200));

	device.setRenderTarget(shadowMapSurface);
	device.setDepthStencilSurface(shadowMapZBuffer);

	setRenderShadowMap(true);

	GraphicsManager::draw(gameTime, shadowComponents);

	setRenderShadowMap(false);

	device.setRenderTarget(backBuffer);
	device.setDepthStencilSurface(zBuffer);


	if (terrain)
	{
		// Render Water Refletion & Refraction to Texture
		const WaterView& water = terrain->getWater();

		const Plane& refractionPlane = water.getRefractionPlane();
		const Plane& reflectionPlane = water.getReflectionPlane();

		Matrix normalView = getView();
		Matrix reflectionView = Matrix::createReflection(reflectionPlane) * normalView;

		// Refraction
		set(normalView, refractionPlane, D3DCULL_CCW, water.getRefractionSurface(), water.getZBuffer());
		GraphicsManager::draw(gameTime, refractionComponents);
	
		// Reflection
		set(reflectionView, reflectionPlane, D3DCULL_CW, water.getReflectionSurface(), water.getZBuffer());
		GraphicsManager::draw(gameTime);

		set(normalView, reflectionPlane, D3DCULL_CCW, backBuffer, zBuffer);
	}

	// Render Scene to Screen
	GraphicsManager::draw(gameTime);
}


void LLGraphicsManager::drawLightMap(const GameTime& gameTime)
{
	GraphicsDevice device = getDevice();

	Texture lightmap = Texture::create(getDevice(), 4096, 4096, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT);

	device.setRenderTarget(lightmap.getSurfaceLevel());
	device.resource->SetDepthStencilSurface(shadowMapZBuffer);

	setView(Matrix::createLookAt(Vector3::up * 100.0f, Vector3::zero, Vector3::forward));
	setProjection(Matrix::createOrthographic(1024, 1024, 1, 200));

	GraphicsManager::draw(gameTime, ComponentCollection(1, terrain));

	D3DXSaveTextureToFileA("Lightmap.png", D3DXIFF_PNG, lightmap, 0);

	exit(0);
}


void LLGraphicsManager::drawShadowMap(const GameTime& gameTime)
{
	GraphicsDevice device = getDevice();

	Vector3 target = Vector3(0.0f, 0.0f, -100.0f);
	Vector3 lightPosition = -Vector3(getLight().Direction) * 200.0f + target;

	setLightView(Matrix::createLookAt(lightPosition, target, Vector3::up));
	setLightProjection(Matrix::createOrthographic(700, 250, 1, 600));

	device.setRenderTarget(shadowMapSurface);
	device.setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device.resource->SetDepthStencilSurface(shadowMapZBuffer);

	setRenderShadowMap(true);

	ComponentCollection components;

	components.push_back(terrain);
	components.push_back(vegetation);

	GraphicsManager::draw(gameTime, components);

	setRenderShadowMap(false);

	D3DXSaveTextureToFileA("Shadowmap.png", D3DXIFF_PNG, getShadowMap(), 0);

	device.setRenderTarget(backBuffer);
	device.resource->SetDepthStencilSurface(zBuffer);
}


void LLGraphicsManager::onLostDevice()
{
	shadowMapSurface.release();
	shadowMapZBuffer.release();

	getShadowMap().release();

	GraphicsManager::onLostDevice();
	FBXModelLoader::onLostDevice();

	backBuffer.release();
	zBuffer.release();
}


void LLGraphicsManager::onResetDevice(const GraphicsDevice& device)
{
	GraphicsManager::onResetDevice(device);
	FBXModelLoader::onResetDevice(device);

	backBuffer = device.getRenderTarget();
	device.resource->GetDepthStencilSurface(&zBuffer);

	Viewport viewport = device.getViewport();

	setShadowMap(Texture::create(device, 512, 512, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT));

	shadowMapZBuffer = device.createDepthStencilSurface(512, 512);
	shadowMapSurface = getShadowMap().getSurfaceLevel();
}


void LLGraphicsManager::set(const Matrix& view, const Plane& clipPlane, unsigned long cullmode, const Surface& renderTarget, const Surface& zBuffer)
{
	GraphicsDevice device = getDevice();

	device.setRenderState(D3DRS_CULLMODE, cullmode);
	device.setRenderTarget(renderTarget);
	device.setDepthStencilSurface(zBuffer);

	setView(view);
	setClipPlane(clipPlane);
}
