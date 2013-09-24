#pragma once

#include "LLControlledGraphicsComponent.h"

#include "TerrainController.h"
#include "TerrainEffect.h"
#include "TerrainTessellator.h"
#include "TerrainBlock.h"

#include "WaterView.h"

class TerrainView : public LLControlledGraphicsComponent<TerrainController>
{
	typedef std::vector<unsigned> UnsignedCollection;
	typedef std::vector<std::vector<unsigned>> BlockIndexCollection;
	typedef std::vector<std::vector<TerrainInstance>> TerrainInstanceCollection;
	typedef std::vector<TerrainBlock> TerrainBlockCollection;
	typedef std::pair<unsigned, unsigned> UnsignedPair;
	typedef std::vector<UnsignedPair> UnsignedPairCollection;

public:
	TerrainView(LLGraphicsManager* manager, TerrainController* controller);

	void init();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

	unsigned getPatchSize() const { return patchSize; }
	unsigned getDetailLevels() const { return detailLevels; }

	void onResetDevice(const GraphicsDevice& device);
	void onLostDevice();

	const WaterView& getWater() const { return *water; }
	const TerrainTessellator& getTessellator() const { return tessellator; }

private:
	void update(unsigned row, unsigned col, unsigned numRows, unsigned numCols, bool frustumCulling = true);

	unsigned getBlockIndex(const TerrainPoint& p) const;
	unsigned getBlockIndex(unsigned row, unsigned col) const;
	BoundingBox getBoundingBox(unsigned row, unsigned col, unsigned numRows, unsigned numCols) const;
	UnsignedPairCollection getDrawOrder(unsigned rowMin, unsigned colMin, unsigned rowMax, unsigned colMax) const;

	unsigned width;
	unsigned length;

	unsigned patchSize;
	unsigned detailLevels;

	WaterView* water;

	TerrainEffect effect;
	TerrainTessellator tessellator;

	TerrainBlockCollection blocks;
	UnsignedCollection numVisibleBlocks;
	BlockIndexCollection visibleBlocks;
	TerrainInstanceCollection instances;
	UnsignedCollection numInstances;
};