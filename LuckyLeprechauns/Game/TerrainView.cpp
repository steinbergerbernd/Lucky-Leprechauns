#include "TerrainView.h"

#undef min
#undef max

TerrainView::TerrainView(LLGraphicsManager* manager, TerrainController* controller) 
	: LLControlledGraphicsComponent(manager, controller)
{
	water = manager->addComponent<WaterView>(manager, this);
}


void TerrainView::init()
{
	GraphicsDevice device = getManager().getDevice();

	const TerrainController& controller = getController();

	width = controller.getWidth();
	length = controller.getLength();

	patchSize = Config::getValue<unsigned>(LLConfigKeys::terrainPatchSize);

	detailLevels = (unsigned)MathHelper::log2(patchSize) + 1U;

	unsigned numBlocks = (width / patchSize) * (length / patchSize);

	visibleBlocks = BlockIndexCollection(detailLevels);
	instances = TerrainInstanceCollection(detailLevels);
	numVisibleBlocks = UnsignedCollection(detailLevels);
	numInstances = UnsignedCollection(detailLevels);

	for (unsigned i = 0; i < detailLevels; ++i)
	{
		visibleBlocks[i].resize(numBlocks);
		instances[i].resize(numBlocks);
	}

	unsigned patchStripeSize = Config::getValue<unsigned>(LLConfigKeys::terrainPatchStripeSize);

	tessellator.init(device, patchSize, numBlocks, patchStripeSize);

	blocks = TerrainBlockCollection(numBlocks);

	for (unsigned row = 0; row < length / patchSize; ++row)
		for (unsigned col = 0; col < width / patchSize; ++col)
			blocks[getBlockIndex(row, col)].init(*this, TerrainPoint(row * patchSize, col * patchSize));

	effect = Effect::createFromFile<TerrainEffect>(device, Config::getValue(LLConfigKeys::terrainEffectPath));

	effect.setNormalmap(controller.getNormalmap());
	effect.setHeightmap(controller.getHeightmap());

	effect.setTerrainWidth(width + 1);
	effect.setTerrainLength(length + 1);
	
	effect.setBumpiness(controller.getBumpiness());
	effect.setTerrainOffset(controller.getOffset());
	effect.setMinLevelOfDetail(detailLevels - 1);

	Texture sand = Texture::createFromFile(device, "Content/Textures/Terrain/Sand.png");
	Texture grass = Texture::createFromFile(device, "Content/Textures/Terrain/Grass.png");
	Texture stone = Texture::createFromFile(device, "Content/Textures/Terrain/Stone.png");
	Texture rock = Texture::createFromFile(device, "Content/Textures/Terrain/Rock.png");

	Texture lightmap = Texture::createFromFile(device, "Content/Textures/Terrain/Lightmap.png");

	effect.setTexture("Lightmap", lightmap);

	effect.setMaterial(Material(Vector4::one, Vector4::one, Vector3(0.5f), 20.0f));

	effect.setTexture("Texture0", sand);
	effect.setTexture("Texture1", grass);
	effect.setTexture("Texture2", stone);
	effect.setTexture("TextureCliff", rock);

	effect.setVector("TextureRange", Vector4(0.03f, 0.0f, 0.2f, 0.07f));
	effect.setVector("CliffRange", Vector2(0.65f, 0.85f));

	effect.setVector("TextureResolution", Vector2(32.0f, 32.0f));
}


void TerrainView::update(const GameTime& gameTime)
{
	for (unsigned i = 0; i < detailLevels; ++i)
		numVisibleBlocks[i] = 0;

	update(0, 0, length / patchSize, width / patchSize, false);
}


void TerrainView::update(unsigned row, unsigned col, unsigned numRows, unsigned numCols, bool frustumCulling)
{
	Camera& camera = getManager().getCamera();

	if (frustumCulling)
	{		
		switch (camera.getViewFrustum().contains(getBoundingBox(row, col, numRows, numCols)))
		{
			case ContainmentType::disjoint: return;
			case ContainmentType::contains: frustumCulling = false;
		}
	}

	if (numRows > 1 && numCols > 1)
	{
		for (unsigned r = 0; r < 2; ++r)
			for (unsigned c = 0; c < 2; ++c)
				update(row + r * numRows / 2, col + c * numCols / 2, numRows / 2, numCols / 2, frustumCulling);

		return;
	}

	for (unsigned r = 0; r < numRows; ++r)
		for (unsigned c = 0; c < numCols; ++c)
		{
			unsigned blockIndex = getBlockIndex(row + r, col + c);

			TerrainBlock& block = blocks[blockIndex];

			block.update(*this, camera.getPosition());

			unsigned lod = block.getLevelOfDetail();

			visibleBlocks[lod][numVisibleBlocks[lod]++] = blockIndex;
		}
}


void TerrainView::draw(const GameTime& gameTime)
{
	GraphicsManager& manager = getManager();
	const TerrainController& controller = getController();

	if (manager.isRenderShadowMap())
		return;

	GraphicsDevice device = manager.getDevice();
	const Plane& clipPlane = manager.getClipPlane();

	// filter out blocks that don't fit with clip plane
	for (unsigned lod = 0; lod < detailLevels; ++lod)
	{
		numInstances[lod] = 0;

		for (unsigned i = 0; i < numVisibleBlocks[lod]; ++i)
		{
			const TerrainBlock& block = blocks[visibleBlocks[lod][i]];

			if ((clipPlane.b <= 0.0f && block.isUnderWater()) || (clipPlane.b >= 0.0f && block.isOverWater()))
			{
				TerrainInstance& instance = instances[lod][numInstances[lod]++];

				instance.blending = block.getBlending();
				instance.offset = block.getOffset();
			}
		}
	}

	//effect.setTechnique(manager.isRenderShadowMap() ? "ShadowMap" : "LightMap");
	
	effect.setClipPlane(clipPlane);
	effect.setCamera(manager.getCamera());
	effect.setLight(manager.getLight());

	effect.setTexture("Shadowmap", manager.getShadowMap());
	effect.setMatrix("LightViewProjection", manager.getLightViewProjection());
	effect.setBool("ShadowmapEnabled", manager.isShadowMapEnabled());

	effect.setWorld(controller.getWorld() * manager.getWorld());
	effect.setMatrix("ViewProjection", manager.getViewProjection());

	effect.beginSinglePass();

	for (unsigned lod = 0; lod < detailLevels; ++lod)
		if (numInstances[lod])
		{
			effect.setLevelOfDetail(lod);
			effect.commitChanges();

			tessellator.drawPatches(device, lod, instances[lod], numInstances[lod]);
			tessellator.drawSkirts(device, lod, instances[lod], numInstances[lod]);
		}

	effect.endSinglePass();
}


void TerrainView::onResetDevice(const GraphicsDevice& device)
{
	effect.onResetDevice();
	tessellator.onResetDevice(device);
}


void TerrainView::onLostDevice()
{
	effect.onLostDevice();
	tessellator.onLostDevice();
}


unsigned TerrainView::getBlockIndex(const TerrainPoint& p) const
{
	return getBlockIndex(p.row, p.col);
}


unsigned TerrainView::getBlockIndex(unsigned row, unsigned col) const
{
	return row * (width / patchSize) + col;
}


BoundingBox TerrainView::getBoundingBox(unsigned row, unsigned col, unsigned numRows, unsigned numCols) const
{
	const TerrainController& controller = getController();

	float bumpiness = controller.getBumpiness();
	float offset = controller.getOffset();
	const Vector3& position = controller.getPosition();

	float minX = position.x + col * patchSize;
	float minZ = position.z + row * patchSize;

	Vector3 min(minX, (1 + offset) * -bumpiness, minZ);
	Vector3 max(minX + numCols * patchSize, (1 + offset) * bumpiness, minZ + numRows * patchSize);

	return BoundingBox(min, max);
}
