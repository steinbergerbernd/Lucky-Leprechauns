#include "TerrainTessellator.h"


const D3DFORMAT TerrainTessellator::indexFormat = D3DFMT_INDEX32;


void TerrainTessellator::init(const GraphicsDevice& device, unsigned size, unsigned numPatches, unsigned stripeSize)
{
	this->size = size;
	this->stripeSize = stripeSize;
	this->numPatches = numPatches;

	numVertices = (size + 3) * (size + 3);

	unsigned lod = (unsigned)MathHelper::log2(size) + 1;

	patchIndexBuffers = IndexBufferCollection(lod);
	skirtIndexBuffers = IndexBufferCollection(lod);

	for (unsigned i = 0; i < lod; ++i)
	{
		patchIndexBuffers[i] = createPatchIndexBuffer(device, i);
		skirtIndexBuffers[i] = createSkirtIndexBuffer(device, i);
	}

	VertexCollection vertices(numVertices);

	for (int row = -1; row <= (int)size + 1; ++row)
		for (int col = -1; col <= (int)size + 1; ++col)
		{
			int r = MathHelper::clamp(row, 0, (int)size);
			int c = MathHelper::clamp(col, 0, (int)size);

			float y = (r == row && c == col) ? 0.0f : -1.0f;

			vertices[getIndex(row, col)].position = Vector3((float)c, y, (float)r);
		}

	vertexBuffer = device.createVertexBuffer(sizeof(TerrainVertex) * numVertices, TerrainVertex::fvf, D3DUSAGE_WRITEONLY);

	vertexBuffer.setData(vertices);

	instanceVertexDeclaration = device.createVertexDeclaration(TerrainGeometry::vertexElements);
}


void TerrainTessellator::drawPatches(const GraphicsDevice& device, unsigned levelOfDetail, const std::vector<TerrainInstance>& instances, unsigned numInstances) const
{
	int lodSize = size / MathHelper::pow2(levelOfDetail);

	draw(device, levelOfDetail, lodSize * lodSize * 2, patchIndexBuffers, instances, numInstances);
}


void TerrainTessellator::drawSkirts(const GraphicsDevice& device, unsigned levelOfDetail, const std::vector<TerrainInstance>& instances, unsigned numInstances) const
{
	int lodSize = size / MathHelper::pow2(levelOfDetail);

	draw(device, levelOfDetail, lodSize * 8, skirtIndexBuffers, instances, numInstances);
}


void TerrainTessellator::drawPatch(const GraphicsDevice& device, unsigned levelOfDetail) const
{
	int lodSize = size / MathHelper::pow2(levelOfDetail);

	draw(device, levelOfDetail, lodSize * lodSize * 2, patchIndexBuffers);
}


void TerrainTessellator::drawSkirt(const GraphicsDevice& device, unsigned levelOfDetail) const
{
	int lodSize = size / MathHelper::pow2(levelOfDetail);

	draw(device, levelOfDetail, lodSize * 8, skirtIndexBuffers);
}


void TerrainTessellator::draw(const GraphicsDevice& device, unsigned levelOfDetail, unsigned primitiveCount, const IndexBufferCollection& indexBuffers, const std::vector<TerrainInstance>& instances, unsigned numInstances) const
{
	instanceVertexBuffer.setData(instances, D3DLOCK_DISCARD);

	device.setStreamSource(vertexBuffer, sizeof(TerrainVertex));
	device.setStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | numInstances);

	device.setStreamSource(instanceVertexBuffer, sizeof(TerrainInstance), 1);
	device.setStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	device.setVertexDeclaration(instanceVertexDeclaration);

	device.setIndices(indexBuffers[levelOfDetail]);

	device.drawIndexedPrimitive(D3DPT_TRIANGLELIST, primitiveCount, numVertices);
}

	
void TerrainTessellator::draw(const GraphicsDevice& device, unsigned levelOfDetail, unsigned primitiveCount, const IndexBufferCollection& indexBuffers) const
{
	device.setStreamSource(vertexBuffer, sizeof(TerrainVertex));
	device.setIndices(indexBuffers[levelOfDetail]);

	device.setFVF(TerrainVertex::fvf);

	device.drawIndexedPrimitive(D3DPT_TRIANGLELIST, primitiveCount, numVertices);
}


TerrainTessellator::TerrainIndex TerrainTessellator::getIndex(int row, int col)
{
	return (row + 1) * (size + 3) + (col + 1);
}


IndexBuffer TerrainTessellator::createSkirtIndexBuffer(const GraphicsDevice& device, unsigned levelOfDetail)
{
	// distance between two neighbor vertices
	int delta = MathHelper::pow2(levelOfDetail);

	// size of grid in current lod (e.g. lod 0: 64, lod 1: 32)
	int lodSize = size / delta;

	// main indices + skirt indices
	int numIndices = lodSize * 4 * 6;

	IndexCollection indices(numIndices);

	int count = 0;

	int interval = min(stripeSize * delta, size);

	// init skirt indices
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			for (unsigned row = 0; row < (i ? size : 1); row += delta)
				for (unsigned col = 0; col < (i ? 1 : size); col += delta)
				{
					int r = i ? row : -1 + j * (size + 1);
					int c = i ? -1 + j * (size + 1) : col;

					int deltaCol = i ? 1 : delta;
					int deltaRow = i ? delta : 1;

					initQuadIndices(r, c, deltaRow, deltaCol, &indices, &count);
				}

	IndexBuffer indexBuffer = device.createIndexBuffer(sizeof(TerrainIndex) * numIndices, indexFormat, D3DUSAGE_WRITEONLY);

	indexBuffer.setData(indices);

	return indexBuffer;
}


IndexBuffer TerrainTessellator::createPatchIndexBuffer(const GraphicsDevice& device, unsigned levelOfDetail)
{
	// distance between two neighbor vertices
	int delta = MathHelper::pow2(levelOfDetail);

	// size of grid in current lod (e.g. lod 0: 64, lod 1: 32)
	int lodSize = size / delta;

	// main indices + skirt indices
	int numIndices = lodSize * lodSize * 6;

	IndexCollection indices(numIndices);

	int count = 0;

	unsigned interval = min(stripeSize * delta, size);

	// init main indices
	for (unsigned i = 0; i < size / interval; ++i)
		for (unsigned row = 0; row < size; row += delta)
			for (unsigned col = i * interval; col < (i+1) * interval; col += delta)
				initQuadIndices(row, col, delta, delta, &indices, &count);

	IndexBuffer indexBuffer = device.createIndexBuffer(sizeof(TerrainIndex) * numIndices, indexFormat, D3DUSAGE_WRITEONLY);

	indexBuffer.setData(indices);

	return indexBuffer;
}


void TerrainTessellator::initQuadIndices(int row, int col, int deltaRow, int deltaCol, IndexCollection* indices, int* count)
{
	int& c = *count;
	IndexCollection& ind = *indices;

	ind[c++] = getIndex(row, col);
	ind[c++] = getIndex(row + deltaRow, col);
	ind[c++] = getIndex(row + deltaRow, col + deltaCol);

	ind[c++] = getIndex(row + deltaRow, col + deltaCol);
	ind[c++] = getIndex(row, col + deltaCol);
	ind[c++] = getIndex(row, col);
}


void TerrainTessellator::onResetDevice(const GraphicsDevice& device)
{
	instanceVertexBuffer = device.createVertexBuffer(sizeof(TerrainInstance) * numPatches, 0, D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
}


void TerrainTessellator::onLostDevice()
{
	instanceVertexBuffer.release();
}