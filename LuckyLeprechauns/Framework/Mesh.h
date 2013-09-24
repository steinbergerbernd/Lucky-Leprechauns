#pragma once

#include <d3dx9.h>

#include "Validator.h"
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class GraphicsDevice;

class Mesh : public Resource<ID3DXMesh>
{
public:
	typedef std::vector<unsigned long> Adjacency;
	typedef std::vector<D3DXATTRIBUTERANGE> AttributeTable;

	static Mesh create(const GraphicsDevice& device, unsigned long numFaces, unsigned long numVertices, const D3DVERTEXELEMENT9* vertexElements, unsigned long options = 0);

	VertexBuffer getVertexBuffer() const;
	IndexBuffer getIndexBuffer() const;
	AttributeTable getAttributeTable(unsigned long size) const;

	void drawSubset(unsigned long index) const;

	unsigned long* lockAttributeBuffer(unsigned long flag = 0) const;
	void unlockAttributeBuffer() const;

	Adjacency generateAdjacency(float epsilon = 0) const;
	void clean(D3DXCLEANTYPE cleanType, Adjacency& adjacency);
	void optimizeInplace(unsigned long flags, Adjacency& adjacency);
	void computeNormals(const Adjacency& adjacency);
};