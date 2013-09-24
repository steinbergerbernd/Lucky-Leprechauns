#pragma once

#include "GraphicsDevice.h"
#include "GraphicsTypes.h"
#include "MathHelper.h"
#include "STL.h"

#include "TerrainInstance.h"
#include "TerrainGeometry.h"

class TerrainTessellator
{
	typedef unsigned TerrainIndex;
	typedef VertexPosition TerrainVertex;

	typedef std::vector<IndexBuffer> IndexBufferCollection;
	typedef std::vector<TerrainVertex> VertexCollection;
	typedef std::vector<TerrainIndex> IndexCollection;

public:
	void init(const GraphicsDevice& device, unsigned size, unsigned numPatches, unsigned stripeSize);

	void drawPatches(const GraphicsDevice& device, unsigned levelOfDetail, const std::vector<TerrainInstance>& instances, unsigned numInstances) const;
	void drawSkirts(const GraphicsDevice& device, unsigned levelOfDetail, const std::vector<TerrainInstance>& instances, unsigned numInstances) const;

	void drawPatch(const GraphicsDevice& device, unsigned levelOfDetail = 0) const;
	void drawSkirt(const GraphicsDevice& device, unsigned levelOfDetail = 0) const;

	void onResetDevice(const GraphicsDevice& device);
	void onLostDevice();

private:
	void draw(const GraphicsDevice& device, unsigned levelOfDetail, unsigned primitiveCount, const IndexBufferCollection& indexBuffers, const std::vector<TerrainInstance>& instances, unsigned numInstances) const;
	void draw(const GraphicsDevice& device, unsigned levelOfDetail, unsigned primitiveCount, const IndexBufferCollection& indexBuffers) const;

	IndexBuffer createPatchIndexBuffer(const GraphicsDevice& device, unsigned levelOfDetail);
	IndexBuffer createSkirtIndexBuffer(const GraphicsDevice& device, unsigned levelOfDetail);

	void initQuadIndices(int row, int col, int deltaRow, int deltaCol, IndexCollection* indices, int* count);

	TerrainIndex getIndex(int x, int z);

	VertexBuffer vertexBuffer;
	VertexBuffer instanceVertexBuffer;

	unsigned size;
	unsigned stripeSize;
	unsigned numVertices;
	unsigned numPatches;

	IndexBufferCollection patchIndexBuffers;
	IndexBufferCollection skirtIndexBuffers;

	static const D3DFORMAT indexFormat;

	VertexDeclaration instanceVertexDeclaration;
};